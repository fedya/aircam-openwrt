/*
 * Copyright (c) 2009  Felix Fietkau <nbd@openwrt.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * uncompress.c
 */

#include <linux/types.h>
#include <asm/byteorder.h>
#include "unlzma.h"

struct unlzma_ctx {
	const u8 *next_in;
	u8 *next_out;
	u8 *outbuf;

	/* reader state */
	u32 code;
	u32 range;
	u32 bound;

	/* writer state */
	u8 previous_byte;
	ssize_t pos;

	/* cstate */
	int state;
	u32 rep0, rep1, rep2, rep3;

	void *workspace;
} ctx;

static int inbs = 0;
static inline u8
rc_read(void)
{
#if 0
	if (unlikely(++inbs > 16 * 1024)) {
		putstr(".");
		inbs = 0;
	}
#endif
	return *(ctx.next_in++);
}


static inline void
rc_get_code(void)
{
	ctx.code = (ctx.code << 8) | rc_read();
}

static inline void
rc_normalize(void)
{
	if (ctx.range < (1 << RC_TOP_BITS)) {
		ctx.range <<= 8;
		rc_get_code();
	}
}

static inline int
rc_is_bit_0(u16 *p)
{
	rc_normalize();
	ctx.bound = *p * (ctx.range >> RC_MODEL_TOTAL_BITS);
	return ctx.code < ctx.bound;
}

static inline void
rc_update_bit_0(u16 *p)
{
	ctx.range = ctx.bound;
	*p += ((1 << RC_MODEL_TOTAL_BITS) - *p) >> RC_MOVE_BITS;
}

static inline void
rc_update_bit_1(u16 *p)
{
	ctx.range -= ctx.bound;
	ctx.code -= ctx.bound;
	*p -= *p >> RC_MOVE_BITS;
}

static inline bool
rc_get_bit(u16 *p, int *symbol)
{
	if (rc_is_bit_0(p)) {
		rc_update_bit_0(p);
		*symbol *= 2;
		return 0;
	} else {
		rc_update_bit_1(p);
		*symbol = *symbol * 2 + 1;
		return 1;
	}
}

static inline int
rc_direct_bit(void)
{
	rc_normalize();
	ctx.range >>= 1;
	if (ctx.code >= ctx.range) {
		ctx.code -= ctx.range;
		return 1;
	}
	return 0;
}

static inline void
rc_bit_tree_decode(u16 *p, int num_levels, int *symbol)
{
	int i = num_levels;

	*symbol = 1;
	while (i--)
		rc_get_bit(p + *symbol, symbol);
	*symbol -= 1 << num_levels;
}

static inline u8
peek_old_byte(u32 offs)
{
	u32 pos = ctx.pos - offs;
	return ctx.outbuf[pos];
}

static inline void
write_byte(u8 byte)
{
	ctx.previous_byte = byte;
	*(ctx.next_out++) = byte;
	ctx.pos++;
}


static inline void
copy_byte(u32 offs)
{
	write_byte(peek_old_byte(offs));
}

static inline void
copy_bytes(u32 rep0, int len)
{
	do {
		copy_byte(rep0);
		len--;
	} while (len != 0);
}

static inline void
process_bit0(u16 *p, int pos_state, u16 *prob,
             int lc, u32 literal_pos_mask)
{
	int mi = 1;
	rc_update_bit_0(prob);
	prob = (p + LZMA_LITERAL +
		(LZMA_LIT_SIZE
		 * (((ctx.pos & literal_pos_mask) << lc)
		    + (ctx.previous_byte >> (8 - lc))))
		);

	if (ctx.state >= LZMA_NUM_LIT_STATES) {
		int match_byte = peek_old_byte(ctx.rep0);
		do {
			u16 bit;
			u16 *prob_lit;

			match_byte <<= 1;
			bit = match_byte & 0x100;
			prob_lit = prob + 0x100 + bit + mi;
			if (rc_get_bit(prob_lit, &mi) != !!bit)
				break;
		} while (mi < 0x100);
	}
	while (mi < 0x100) {
		u16 *prob_lit = prob + mi;
		rc_get_bit(prob_lit, &mi);
	}
	write_byte(mi);
	if (ctx.state < 4)
		ctx.state = 0;
	else if (ctx.state < 10)
		ctx.state -= 3;
	else
		ctx.state -= 6;
}

static inline void
process_bit1(u16 *p, int pos_state, u16 *prob)
{
	int offset;
	u16 *prob_len;
	int num_bits;
	int len;

	rc_update_bit_1(prob);
	prob = p + LZMA_IS_REP + ctx.state;
	if (rc_is_bit_0(prob)) {
		rc_update_bit_0(prob);
		ctx.rep3 = ctx.rep2;
		ctx.rep2 = ctx.rep1;
		ctx.rep1 = ctx.rep0;
		ctx.state = ctx.state < LZMA_NUM_LIT_STATES ? 0 : 3;
		prob = p + LZMA_LEN_CODER;
	} else {
		rc_update_bit_1(prob);
		prob = p + LZMA_IS_REP_G0 + ctx.state;
		if (rc_is_bit_0(prob)) {
			rc_update_bit_0(prob);
			prob = (p + LZMA_IS_REP_0_LONG
				+ (ctx.state <<
				   LZMA_NUM_POS_BITS_MAX) +
				pos_state);
			if (rc_is_bit_0(prob)) {
				rc_update_bit_0(prob);

				ctx.state = ctx.state < LZMA_NUM_LIT_STATES ?
					9 : 11;
				copy_byte(ctx.rep0);
				return;
			} else {
				rc_update_bit_1(prob);
			}
		} else {
			u32 distance;

			rc_update_bit_1(prob);
			prob = p + LZMA_IS_REP_G1 + ctx.state;
			if (rc_is_bit_0(prob)) {
				rc_update_bit_0(prob);
				distance = ctx.rep1;
			} else {
				rc_update_bit_1(prob);
				prob = p + LZMA_IS_REP_G2 + ctx.state;
				if (rc_is_bit_0(prob)) {
					rc_update_bit_0(prob);
					distance = ctx.rep2;
				} else {
					rc_update_bit_1(prob);
					distance = ctx.rep3;
					ctx.rep3 = ctx.rep2;
				}
				ctx.rep2 = ctx.rep1;
			}
			ctx.rep1 = ctx.rep0;
			ctx.rep0 = distance;
		}
		ctx.state = ctx.state < LZMA_NUM_LIT_STATES ? 8 : 11;
		prob = p + LZMA_REP_LEN_CODER;
	}

	prob_len = prob + LZMA_LEN_CHOICE;
	if (rc_is_bit_0(prob_len)) {
		rc_update_bit_0(prob_len);
		prob_len = (prob + LZMA_LEN_LOW
			    + (pos_state <<
			       LZMA_LEN_NUM_LOW_BITS));
		offset = 0;
		num_bits = LZMA_LEN_NUM_LOW_BITS;
	} else {
		rc_update_bit_1(prob_len);
		prob_len = prob + LZMA_LEN_CHOICE_2;
		if (rc_is_bit_0(prob_len)) {
			rc_update_bit_0(prob_len);
			prob_len = (prob + LZMA_LEN_MID
				    + (pos_state <<
				       LZMA_LEN_NUM_MID_BITS));
			offset = 1 << LZMA_LEN_NUM_LOW_BITS;
			num_bits = LZMA_LEN_NUM_MID_BITS;
		} else {
			rc_update_bit_1(prob_len);
			prob_len = prob + LZMA_LEN_HIGH;
			offset = ((1 << LZMA_LEN_NUM_LOW_BITS)
				  + (1 << LZMA_LEN_NUM_MID_BITS));
			num_bits = LZMA_LEN_NUM_HIGH_BITS;
		}
	}

	rc_bit_tree_decode(prob_len, num_bits, &len);
	len += offset;

	if (ctx.state < 4) {
		int pos_slot;

		ctx.state += LZMA_NUM_LIT_STATES;
		prob =
			p + LZMA_POS_SLOT +
			((len <
			  LZMA_NUM_LEN_TO_POS_STATES ? len :
			  LZMA_NUM_LEN_TO_POS_STATES - 1)
			 << LZMA_NUM_POS_SLOT_BITS);
		rc_bit_tree_decode(prob,
				   LZMA_NUM_POS_SLOT_BITS,
				   &pos_slot);
		if (pos_slot >= LZMA_START_POS_MODEL_INDEX) {
			int i, mi;
			num_bits = (pos_slot >> 1) - 1;
			ctx.rep0 = 2 | (pos_slot & 1);
			if (pos_slot < LZMA_END_POS_MODEL_INDEX) {
				ctx.rep0 <<= num_bits;
				prob = p + LZMA_SPEC_POS +
					ctx.rep0 - pos_slot - 1;
			} else {
				num_bits -= LZMA_NUM_ALIGN_BITS;
				while (num_bits--)
					ctx.rep0 = (ctx.rep0 << 1) |
						rc_direct_bit();
				prob = p + LZMA_ALIGN;
				ctx.rep0 <<= LZMA_NUM_ALIGN_BITS;
				num_bits = LZMA_NUM_ALIGN_BITS;
			}
			i = 1;
			mi = 1;
			while (num_bits--) {
				if (rc_get_bit(prob + mi, &mi))
					ctx.rep0 |= i;
				i <<= 1;
			}
		} else
			ctx.rep0 = pos_slot;
		if (++(ctx.rep0) == 0)
			return;
	}

	len += LZMA_MATCH_MIN_LEN;

	copy_bytes(ctx.rep0, len);
}


static int
do_unlzma(void)
{
	u8 hdr_buf[sizeof(struct lzma_header)];
	struct lzma_header *header = (struct lzma_header *)hdr_buf;
	u32 pos_state_mask;
	u32 literal_pos_mask;
	int lc, pb, lp;
	int num_probs;
	int i, mi;
	u16 *p;

	for (i = 0; i < sizeof(struct lzma_header); i++) {
		hdr_buf[i] = rc_read();
	}

	ctx.pos = 0;
	ctx.state = 0;
	ctx.rep0 = ctx.rep1 = ctx.rep2 = ctx.rep3 = 1;

	ctx.previous_byte = 0;
	ctx.code = 0;
	ctx.range = 0xFFFFFFFF;

	if (header->pos >= (9 * 5 * 5))
		return -1;

	mi = 0;
	lc = header->pos;
	while (lc >= 9) {
		mi++;
		lc -= 9;
	}
	pb = 0;
	lp = mi;
	while (lp >= 5) {
		pb++;
		lp -= 5;
	}
	pos_state_mask = (1 << pb) - 1;
	literal_pos_mask = (1 << lp) - 1;

	p = (u16 *) ctx.workspace;
	if (!p)
		return -1;

	num_probs = LZMA_LITERAL + (LZMA_LIT_SIZE << (lc + lp));
	for (i = 0; i < num_probs; i++)
		p[i] = (1 << RC_MODEL_TOTAL_BITS) >> 1;

	for (i = 0; i < 5; i++)
		rc_get_code();

	while (1) {
		int pos_state =	ctx.pos & pos_state_mask;
		u16 *prob = p + LZMA_IS_MATCH +
			(ctx.state << LZMA_NUM_POS_BITS_MAX) + pos_state;
		if (rc_is_bit_0(prob))
			process_bit0(p, pos_state, prob,
				     lc, literal_pos_mask);
		else {
			process_bit1(p, pos_state, prob);
			if (ctx.rep0 == 0)
				break;
		}
	}

	return ctx.pos;
}


static int unlzma(unsigned char *dest, const unsigned char *src, unsigned char *workspace)
{
	memset(&ctx, 0, sizeof(ctx));
	ctx.outbuf = dest;
	ctx.next_in = src;
	ctx.next_out = dest;
	ctx.workspace = workspace;

	return do_unlzma();
}


