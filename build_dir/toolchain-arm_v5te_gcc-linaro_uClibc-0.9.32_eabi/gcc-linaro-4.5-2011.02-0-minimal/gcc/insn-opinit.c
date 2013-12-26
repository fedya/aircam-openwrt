/* Generated automatically by the program `genopinit'
from the machine description file `md'.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tm_p.h"
#include "flags.h"
#include "insn-config.h"
#include "recog.h"
#include "expr.h"
#include "optabs.h"
#include "reload.h"

void
init_all_optabs (void)
{
#ifdef FIXUNS_TRUNC_LIKE_FIX_TRUNC
  int i, j;
#endif

  if (HAVE_mulhisi3)
    optab_handler (smul_widen_optab, SImode)->insn_code = CODE_FOR_mulhisi3;
  if (HAVE_maddhisi4)
    optab_handler (smadd_widen_optab, SImode)->insn_code = CODE_FOR_maddhisi4;
  if (HAVE_one_cmpldi2)
    optab_handler (one_cmpl_optab, DImode)->insn_code = CODE_FOR_one_cmpldi2;
  if (HAVE_clzsi2)
    optab_handler (clz_optab, SImode)->insn_code = CODE_FOR_clzsi2;
  if (HAVE_muldi3)
    optab_handler (smul_optab, DImode)->insn_code = CODE_FOR_muldi3;
  if (HAVE_floatdisf2)
    convert_optab_handler (sfloat_optab, SFmode, DImode)->insn_code = CODE_FOR_floatdisf2;
  if (HAVE_floatdidf2)
    convert_optab_handler (sfloat_optab, DFmode, DImode)->insn_code = CODE_FOR_floatdidf2;
  if (HAVE_smulv4hi3_highpart)
    optab_handler (smul_highpart_optab, V4HImode)->insn_code = CODE_FOR_smulv4hi3_highpart;
  if (HAVE_umulv4hi3_highpart)
    optab_handler (umul_highpart_optab, V4HImode)->insn_code = CODE_FOR_umulv4hi3_highpart;
  if (HAVE_extendhfsf2)
    convert_optab_handler (sext_optab, SFmode, HFmode)->insn_code = CODE_FOR_extendhfsf2;
  if (HAVE_truncsfhf2)
    convert_optab_handler (trunc_optab, HFmode, SFmode)->insn_code = CODE_FOR_truncsfhf2;
  if (HAVE_fixuns_truncsfsi2)
    convert_optab_handler (ufixtrunc_optab, SImode, SFmode)->insn_code = CODE_FOR_fixuns_truncsfsi2;
  if (HAVE_fixuns_truncdfsi2)
    convert_optab_handler (ufixtrunc_optab, SImode, DFmode)->insn_code = CODE_FOR_fixuns_truncdfsi2;
  if (HAVE_floatunssisf2)
    convert_optab_handler (ufloat_optab, SFmode, SImode)->insn_code = CODE_FOR_floatunssisf2;
  if (HAVE_floatunssidf2)
    convert_optab_handler (ufloat_optab, DFmode, SImode)->insn_code = CODE_FOR_floatunssidf2;
  if (HAVE_divsi3)
    optab_handler (sdiv_optab, SImode)->insn_code = CODE_FOR_divsi3;
  if (HAVE_udivsi3)
    optab_handler (udiv_optab, SImode)->insn_code = CODE_FOR_udivsi3;
  if (HAVE_vec_extractv8qi)
    optab_handler (vec_extract_optab, V8QImode)->insn_code = CODE_FOR_vec_extractv8qi;
  if (HAVE_vec_extractv4hi)
    optab_handler (vec_extract_optab, V4HImode)->insn_code = CODE_FOR_vec_extractv4hi;
  if (HAVE_vec_extractv2si)
    optab_handler (vec_extract_optab, V2SImode)->insn_code = CODE_FOR_vec_extractv2si;
  if (HAVE_vec_extractv2sf)
    optab_handler (vec_extract_optab, V2SFmode)->insn_code = CODE_FOR_vec_extractv2sf;
  if (HAVE_vec_extractv16qi)
    optab_handler (vec_extract_optab, V16QImode)->insn_code = CODE_FOR_vec_extractv16qi;
  if (HAVE_vec_extractv8hi)
    optab_handler (vec_extract_optab, V8HImode)->insn_code = CODE_FOR_vec_extractv8hi;
  if (HAVE_vec_extractv4si)
    optab_handler (vec_extract_optab, V4SImode)->insn_code = CODE_FOR_vec_extractv4si;
  if (HAVE_vec_extractv4sf)
    optab_handler (vec_extract_optab, V4SFmode)->insn_code = CODE_FOR_vec_extractv4sf;
  if (HAVE_vec_extractv2di)
    optab_handler (vec_extract_optab, V2DImode)->insn_code = CODE_FOR_vec_extractv2di;
  if (HAVE_iorv8qi3)
    optab_handler (ior_optab, V8QImode)->insn_code = CODE_FOR_iorv8qi3;
  if (HAVE_iorv16qi3)
    optab_handler (ior_optab, V16QImode)->insn_code = CODE_FOR_iorv16qi3;
  if (HAVE_iorv4hi3)
    optab_handler (ior_optab, V4HImode)->insn_code = CODE_FOR_iorv4hi3;
  if (HAVE_iorv8hi3)
    optab_handler (ior_optab, V8HImode)->insn_code = CODE_FOR_iorv8hi3;
  if (HAVE_iorv2si3)
    optab_handler (ior_optab, V2SImode)->insn_code = CODE_FOR_iorv2si3;
  if (HAVE_iorv4si3)
    optab_handler (ior_optab, V4SImode)->insn_code = CODE_FOR_iorv4si3;
  if (HAVE_iorv2sf3)
    optab_handler (ior_optab, V2SFmode)->insn_code = CODE_FOR_iorv2sf3;
  if (HAVE_iorv4sf3)
    optab_handler (ior_optab, V4SFmode)->insn_code = CODE_FOR_iorv4sf3;
  if (HAVE_iorv2di3)
    optab_handler (ior_optab, V2DImode)->insn_code = CODE_FOR_iorv2di3;
  if (HAVE_andv8qi3)
    optab_handler (and_optab, V8QImode)->insn_code = CODE_FOR_andv8qi3;
  if (HAVE_andv16qi3)
    optab_handler (and_optab, V16QImode)->insn_code = CODE_FOR_andv16qi3;
  if (HAVE_andv4hi3)
    optab_handler (and_optab, V4HImode)->insn_code = CODE_FOR_andv4hi3;
  if (HAVE_andv8hi3)
    optab_handler (and_optab, V8HImode)->insn_code = CODE_FOR_andv8hi3;
  if (HAVE_andv2si3)
    optab_handler (and_optab, V2SImode)->insn_code = CODE_FOR_andv2si3;
  if (HAVE_andv4si3)
    optab_handler (and_optab, V4SImode)->insn_code = CODE_FOR_andv4si3;
  if (HAVE_andv2sf3)
    optab_handler (and_optab, V2SFmode)->insn_code = CODE_FOR_andv2sf3;
  if (HAVE_andv4sf3)
    optab_handler (and_optab, V4SFmode)->insn_code = CODE_FOR_andv4sf3;
  if (HAVE_andv2di3)
    optab_handler (and_optab, V2DImode)->insn_code = CODE_FOR_andv2di3;
  if (HAVE_xorv8qi3)
    optab_handler (xor_optab, V8QImode)->insn_code = CODE_FOR_xorv8qi3;
  if (HAVE_xorv16qi3)
    optab_handler (xor_optab, V16QImode)->insn_code = CODE_FOR_xorv16qi3;
  if (HAVE_xorv4hi3)
    optab_handler (xor_optab, V4HImode)->insn_code = CODE_FOR_xorv4hi3;
  if (HAVE_xorv8hi3)
    optab_handler (xor_optab, V8HImode)->insn_code = CODE_FOR_xorv8hi3;
  if (HAVE_xorv2si3)
    optab_handler (xor_optab, V2SImode)->insn_code = CODE_FOR_xorv2si3;
  if (HAVE_xorv4si3)
    optab_handler (xor_optab, V4SImode)->insn_code = CODE_FOR_xorv4si3;
  if (HAVE_xorv2sf3)
    optab_handler (xor_optab, V2SFmode)->insn_code = CODE_FOR_xorv2sf3;
  if (HAVE_xorv4sf3)
    optab_handler (xor_optab, V4SFmode)->insn_code = CODE_FOR_xorv4sf3;
  if (HAVE_xorv2di3)
    optab_handler (xor_optab, V2DImode)->insn_code = CODE_FOR_xorv2di3;
  if (HAVE_one_cmplv8qi2)
    optab_handler (one_cmpl_optab, V8QImode)->insn_code = CODE_FOR_one_cmplv8qi2;
  if (HAVE_one_cmplv16qi2)
    optab_handler (one_cmpl_optab, V16QImode)->insn_code = CODE_FOR_one_cmplv16qi2;
  if (HAVE_one_cmplv4hi2)
    optab_handler (one_cmpl_optab, V4HImode)->insn_code = CODE_FOR_one_cmplv4hi2;
  if (HAVE_one_cmplv8hi2)
    optab_handler (one_cmpl_optab, V8HImode)->insn_code = CODE_FOR_one_cmplv8hi2;
  if (HAVE_one_cmplv2si2)
    optab_handler (one_cmpl_optab, V2SImode)->insn_code = CODE_FOR_one_cmplv2si2;
  if (HAVE_one_cmplv4si2)
    optab_handler (one_cmpl_optab, V4SImode)->insn_code = CODE_FOR_one_cmplv4si2;
  if (HAVE_one_cmplv2sf2)
    optab_handler (one_cmpl_optab, V2SFmode)->insn_code = CODE_FOR_one_cmplv2sf2;
  if (HAVE_one_cmplv4sf2)
    optab_handler (one_cmpl_optab, V4SFmode)->insn_code = CODE_FOR_one_cmplv4sf2;
  if (HAVE_one_cmplv2di2)
    optab_handler (one_cmpl_optab, V2DImode)->insn_code = CODE_FOR_one_cmplv2di2;
  if (HAVE_absv8qi2)
    optab_handler (abs_optab, V8QImode)->insn_code = CODE_FOR_absv8qi2;
  if (HAVE_absv16qi2)
    optab_handler (abs_optab, V16QImode)->insn_code = CODE_FOR_absv16qi2;
  if (HAVE_absv4hi2)
    optab_handler (abs_optab, V4HImode)->insn_code = CODE_FOR_absv4hi2;
  if (HAVE_absv8hi2)
    optab_handler (abs_optab, V8HImode)->insn_code = CODE_FOR_absv8hi2;
  if (HAVE_absv2si2)
    optab_handler (abs_optab, V2SImode)->insn_code = CODE_FOR_absv2si2;
  if (HAVE_absv4si2)
    optab_handler (abs_optab, V4SImode)->insn_code = CODE_FOR_absv4si2;
  if (HAVE_absv2sf2)
    optab_handler (absv_optab, V2SFmode)->insn_code =
    optab_handler (abs_optab, V2SFmode)->insn_code = CODE_FOR_absv2sf2;
  if (HAVE_absv4sf2)
    optab_handler (absv_optab, V4SFmode)->insn_code =
    optab_handler (abs_optab, V4SFmode)->insn_code = CODE_FOR_absv4sf2;
  if (HAVE_negv8qi2)
    optab_handler (neg_optab, V8QImode)->insn_code = CODE_FOR_negv8qi2;
  if (HAVE_negv16qi2)
    optab_handler (neg_optab, V16QImode)->insn_code = CODE_FOR_negv16qi2;
  if (HAVE_negv4hi2)
    optab_handler (neg_optab, V4HImode)->insn_code = CODE_FOR_negv4hi2;
  if (HAVE_negv8hi2)
    optab_handler (neg_optab, V8HImode)->insn_code = CODE_FOR_negv8hi2;
  if (HAVE_negv2si2)
    optab_handler (neg_optab, V2SImode)->insn_code = CODE_FOR_negv2si2;
  if (HAVE_negv4si2)
    optab_handler (neg_optab, V4SImode)->insn_code = CODE_FOR_negv4si2;
  if (HAVE_negv2sf2)
    optab_handler (negv_optab, V2SFmode)->insn_code =
    optab_handler (neg_optab, V2SFmode)->insn_code = CODE_FOR_negv2sf2;
  if (HAVE_negv4sf2)
    optab_handler (negv_optab, V4SFmode)->insn_code =
    optab_handler (neg_optab, V4SFmode)->insn_code = CODE_FOR_negv4sf2;
  if (HAVE_vashlv8qi3)
    optab_handler (vashl_optab, V8QImode)->insn_code = CODE_FOR_vashlv8qi3;
  if (HAVE_vashlv16qi3)
    optab_handler (vashl_optab, V16QImode)->insn_code = CODE_FOR_vashlv16qi3;
  if (HAVE_vashlv4hi3)
    optab_handler (vashl_optab, V4HImode)->insn_code = CODE_FOR_vashlv4hi3;
  if (HAVE_vashlv8hi3)
    optab_handler (vashl_optab, V8HImode)->insn_code = CODE_FOR_vashlv8hi3;
  if (HAVE_vashlv2si3)
    optab_handler (vashl_optab, V2SImode)->insn_code = CODE_FOR_vashlv2si3;
  if (HAVE_vashlv4si3)
    optab_handler (vashl_optab, V4SImode)->insn_code = CODE_FOR_vashlv4si3;
  if (HAVE_widen_ssumv8qi3)
    optab_handler (ssum_widen_optab, V8QImode)->insn_code = CODE_FOR_widen_ssumv8qi3;
  if (HAVE_widen_ssumv4hi3)
    optab_handler (ssum_widen_optab, V4HImode)->insn_code = CODE_FOR_widen_ssumv4hi3;
  if (HAVE_widen_ssumv2si3)
    optab_handler (ssum_widen_optab, V2SImode)->insn_code = CODE_FOR_widen_ssumv2si3;
  if (HAVE_widen_usumv8qi3)
    optab_handler (usum_widen_optab, V8QImode)->insn_code = CODE_FOR_widen_usumv8qi3;
  if (HAVE_widen_usumv4hi3)
    optab_handler (usum_widen_optab, V4HImode)->insn_code = CODE_FOR_widen_usumv4hi3;
  if (HAVE_widen_usumv2si3)
    optab_handler (usum_widen_optab, V2SImode)->insn_code = CODE_FOR_widen_usumv2si3;
  if (HAVE_reduc_splus_v2di)
    optab_handler (reduc_splus_optab, V2DImode)->insn_code = CODE_FOR_reduc_splus_v2di;
  if (HAVE_clzv8qi2)
    optab_handler (clz_optab, V8QImode)->insn_code = CODE_FOR_clzv8qi2;
  if (HAVE_clzv16qi2)
    optab_handler (clz_optab, V16QImode)->insn_code = CODE_FOR_clzv16qi2;
  if (HAVE_clzv4hi2)
    optab_handler (clz_optab, V4HImode)->insn_code = CODE_FOR_clzv4hi2;
  if (HAVE_clzv8hi2)
    optab_handler (clz_optab, V8HImode)->insn_code = CODE_FOR_clzv8hi2;
  if (HAVE_clzv2si2)
    optab_handler (clz_optab, V2SImode)->insn_code = CODE_FOR_clzv2si2;
  if (HAVE_clzv4si2)
    optab_handler (clz_optab, V4SImode)->insn_code = CODE_FOR_clzv4si2;
  if (HAVE_popcountv8qi2)
    optab_handler (popcount_optab, V8QImode)->insn_code = CODE_FOR_popcountv8qi2;
  if (HAVE_popcountv16qi2)
    optab_handler (popcount_optab, V16QImode)->insn_code = CODE_FOR_popcountv16qi2;
  if (HAVE_adddi3)
    optab_handler (add_optab, DImode)->insn_code = CODE_FOR_adddi3;
  if (HAVE_addsi3)
    optab_handler (add_optab, SImode)->insn_code = CODE_FOR_addsi3;
  if (HAVE_addsf3)
    optab_handler (addv_optab, SFmode)->insn_code =
    optab_handler (add_optab, SFmode)->insn_code = CODE_FOR_addsf3;
  if (HAVE_adddf3)
    optab_handler (addv_optab, DFmode)->insn_code =
    optab_handler (add_optab, DFmode)->insn_code = CODE_FOR_adddf3;
  if (HAVE_subdi3)
    optab_handler (sub_optab, DImode)->insn_code = CODE_FOR_subdi3;
  if (HAVE_subsi3)
    optab_handler (sub_optab, SImode)->insn_code = CODE_FOR_subsi3;
  if (HAVE_subsf3)
    optab_handler (subv_optab, SFmode)->insn_code =
    optab_handler (sub_optab, SFmode)->insn_code = CODE_FOR_subsf3;
  if (HAVE_subdf3)
    optab_handler (subv_optab, DFmode)->insn_code =
    optab_handler (sub_optab, DFmode)->insn_code = CODE_FOR_subdf3;
  if (HAVE_mulsi3)
    optab_handler (smul_optab, SImode)->insn_code = CODE_FOR_mulsi3;
  if (HAVE_maddsidi4)
    optab_handler (smadd_widen_optab, DImode)->insn_code = CODE_FOR_maddsidi4;
  if (HAVE_mulsidi3)
    optab_handler (smul_widen_optab, DImode)->insn_code = CODE_FOR_mulsidi3;
  if (HAVE_umulsidi3)
    optab_handler (umul_widen_optab, DImode)->insn_code = CODE_FOR_umulsidi3;
  if (HAVE_umaddsidi4)
    optab_handler (umadd_widen_optab, DImode)->insn_code = CODE_FOR_umaddsidi4;
  if (HAVE_smulsi3_highpart)
    optab_handler (smul_highpart_optab, SImode)->insn_code = CODE_FOR_smulsi3_highpart;
  if (HAVE_umulsi3_highpart)
    optab_handler (umul_highpart_optab, SImode)->insn_code = CODE_FOR_umulsi3_highpart;
  if (HAVE_mulsf3)
    optab_handler (smulv_optab, SFmode)->insn_code =
    optab_handler (smul_optab, SFmode)->insn_code = CODE_FOR_mulsf3;
  if (HAVE_muldf3)
    optab_handler (smulv_optab, DFmode)->insn_code =
    optab_handler (smul_optab, DFmode)->insn_code = CODE_FOR_muldf3;
  if (HAVE_divsf3)
    optab_handler (sdiv_optab, SFmode)->insn_code = CODE_FOR_divsf3;
  if (HAVE_divdf3)
    optab_handler (sdiv_optab, DFmode)->insn_code = CODE_FOR_divdf3;
  if (HAVE_modsf3)
    optab_handler (smod_optab, SFmode)->insn_code = CODE_FOR_modsf3;
  if (HAVE_moddf3)
    optab_handler (smod_optab, DFmode)->insn_code = CODE_FOR_moddf3;
  if (HAVE_anddi3)
    optab_handler (and_optab, DImode)->insn_code = CODE_FOR_anddi3;
  if (HAVE_andsi3)
    optab_handler (and_optab, SImode)->insn_code = CODE_FOR_andsi3;
  if (HAVE_iordi3)
    optab_handler (ior_optab, DImode)->insn_code = CODE_FOR_iordi3;
  if (HAVE_iorsi3)
    optab_handler (ior_optab, SImode)->insn_code = CODE_FOR_iorsi3;
  if (HAVE_xordi3)
    optab_handler (xor_optab, DImode)->insn_code = CODE_FOR_xordi3;
  if (HAVE_xorsi3)
    optab_handler (xor_optab, SImode)->insn_code = CODE_FOR_xorsi3;
  if (HAVE_smaxsi3)
    optab_handler (smax_optab, SImode)->insn_code = CODE_FOR_smaxsi3;
  if (HAVE_sminsi3)
    optab_handler (smin_optab, SImode)->insn_code = CODE_FOR_sminsi3;
  if (HAVE_umaxsi3)
    optab_handler (umax_optab, SImode)->insn_code = CODE_FOR_umaxsi3;
  if (HAVE_uminsi3)
    optab_handler (umin_optab, SImode)->insn_code = CODE_FOR_uminsi3;
  if (HAVE_ashldi3)
    optab_handler (ashl_optab, DImode)->insn_code = CODE_FOR_ashldi3;
  if (HAVE_ashlsi3)
    optab_handler (ashl_optab, SImode)->insn_code = CODE_FOR_ashlsi3;
  if (HAVE_ashrdi3)
    optab_handler (ashr_optab, DImode)->insn_code = CODE_FOR_ashrdi3;
  if (HAVE_ashrsi3)
    optab_handler (ashr_optab, SImode)->insn_code = CODE_FOR_ashrsi3;
  if (HAVE_lshrdi3)
    optab_handler (lshr_optab, DImode)->insn_code = CODE_FOR_lshrdi3;
  if (HAVE_lshrsi3)
    optab_handler (lshr_optab, SImode)->insn_code = CODE_FOR_lshrsi3;
  if (HAVE_rotlsi3)
    optab_handler (rotl_optab, SImode)->insn_code = CODE_FOR_rotlsi3;
  if (HAVE_rotrsi3)
    optab_handler (rotr_optab, SImode)->insn_code = CODE_FOR_rotrsi3;
  if (HAVE_negdi2)
    optab_handler (neg_optab, DImode)->insn_code = CODE_FOR_negdi2;
  if (HAVE_negsi2)
    optab_handler (neg_optab, SImode)->insn_code = CODE_FOR_negsi2;
  if (HAVE_negsf2)
    optab_handler (negv_optab, SFmode)->insn_code =
    optab_handler (neg_optab, SFmode)->insn_code = CODE_FOR_negsf2;
  if (HAVE_negdf2)
    optab_handler (negv_optab, DFmode)->insn_code =
    optab_handler (neg_optab, DFmode)->insn_code = CODE_FOR_negdf2;
  if (HAVE_abssi2)
    optab_handler (abs_optab, SImode)->insn_code = CODE_FOR_abssi2;
  if (HAVE_abssf2)
    optab_handler (absv_optab, SFmode)->insn_code =
    optab_handler (abs_optab, SFmode)->insn_code = CODE_FOR_abssf2;
  if (HAVE_absdf2)
    optab_handler (absv_optab, DFmode)->insn_code =
    optab_handler (abs_optab, DFmode)->insn_code = CODE_FOR_absdf2;
  if (HAVE_sqrtsf2)
    optab_handler (sqrt_optab, SFmode)->insn_code = CODE_FOR_sqrtsf2;
  if (HAVE_sqrtdf2)
    optab_handler (sqrt_optab, DFmode)->insn_code = CODE_FOR_sqrtdf2;
  if (HAVE_one_cmplsi2)
    optab_handler (one_cmpl_optab, SImode)->insn_code = CODE_FOR_one_cmplsi2;
  if (HAVE_floatsihf2)
    convert_optab_handler (sfloat_optab, HFmode, SImode)->insn_code = CODE_FOR_floatsihf2;
  if (HAVE_floatdihf2)
    convert_optab_handler (sfloat_optab, HFmode, DImode)->insn_code = CODE_FOR_floatdihf2;
  if (HAVE_floatsisf2)
    convert_optab_handler (sfloat_optab, SFmode, SImode)->insn_code = CODE_FOR_floatsisf2;
  if (HAVE_floatsidf2)
    convert_optab_handler (sfloat_optab, DFmode, SImode)->insn_code = CODE_FOR_floatsidf2;
  if (HAVE_fix_trunchfsi2)
    convert_optab_handler (sfixtrunc_optab, SImode, HFmode)->insn_code = CODE_FOR_fix_trunchfsi2;
  if (HAVE_fix_trunchfdi2)
    convert_optab_handler (sfixtrunc_optab, DImode, HFmode)->insn_code = CODE_FOR_fix_trunchfdi2;
  if (HAVE_fix_truncsfsi2)
    convert_optab_handler (sfixtrunc_optab, SImode, SFmode)->insn_code = CODE_FOR_fix_truncsfsi2;
  if (HAVE_fix_truncdfsi2)
    convert_optab_handler (sfixtrunc_optab, SImode, DFmode)->insn_code = CODE_FOR_fix_truncdfsi2;
  if (HAVE_truncdfsf2)
    convert_optab_handler (trunc_optab, SFmode, DFmode)->insn_code = CODE_FOR_truncdfsf2;
  if (HAVE_truncdfhf2)
    convert_optab_handler (trunc_optab, HFmode, DFmode)->insn_code = CODE_FOR_truncdfhf2;
  if (HAVE_zero_extendsidi2)
    convert_optab_handler (zext_optab, DImode, SImode)->insn_code = CODE_FOR_zero_extendsidi2;
  if (HAVE_zero_extendqidi2)
    convert_optab_handler (zext_optab, DImode, QImode)->insn_code = CODE_FOR_zero_extendqidi2;
  if (HAVE_extendsidi2)
    convert_optab_handler (sext_optab, DImode, SImode)->insn_code = CODE_FOR_extendsidi2;
  if (HAVE_zero_extendhisi2)
    convert_optab_handler (zext_optab, SImode, HImode)->insn_code = CODE_FOR_zero_extendhisi2;
  if (HAVE_zero_extendqisi2)
    convert_optab_handler (zext_optab, SImode, QImode)->insn_code = CODE_FOR_zero_extendqisi2;
  if (HAVE_extendhisi2)
    convert_optab_handler (sext_optab, SImode, HImode)->insn_code = CODE_FOR_extendhisi2;
  if (HAVE_extendqihi2)
    convert_optab_handler (sext_optab, HImode, QImode)->insn_code = CODE_FOR_extendqihi2;
  if (HAVE_extendqisi2)
    convert_optab_handler (sext_optab, SImode, QImode)->insn_code = CODE_FOR_extendqisi2;
  if (HAVE_extendsfdf2)
    convert_optab_handler (sext_optab, DFmode, SFmode)->insn_code = CODE_FOR_extendsfdf2;
  if (HAVE_extendhfdf2)
    convert_optab_handler (sext_optab, DFmode, HFmode)->insn_code = CODE_FOR_extendhfdf2;
  if (HAVE_movdi)
    optab_handler (mov_optab, DImode)->insn_code = CODE_FOR_movdi;
  if (HAVE_movsi)
    optab_handler (mov_optab, SImode)->insn_code = CODE_FOR_movsi;
  if (HAVE_movhi)
    optab_handler (mov_optab, HImode)->insn_code = CODE_FOR_movhi;
  if (HAVE_reload_outhi)
    reload_out_optab[HImode] = CODE_FOR_reload_outhi;
  if (HAVE_reload_inhi)
    reload_in_optab[HImode] = CODE_FOR_reload_inhi;
  if (HAVE_movqi)
    optab_handler (mov_optab, QImode)->insn_code = CODE_FOR_movqi;
  if (HAVE_movhf)
    optab_handler (mov_optab, HFmode)->insn_code = CODE_FOR_movhf;
  if (HAVE_movsf)
    optab_handler (mov_optab, SFmode)->insn_code = CODE_FOR_movsf;
  if (HAVE_movdf)
    optab_handler (mov_optab, DFmode)->insn_code = CODE_FOR_movdf;
  if (HAVE_reload_outdf)
    reload_out_optab[DFmode] = CODE_FOR_reload_outdf;
  if (HAVE_movxf)
    optab_handler (mov_optab, XFmode)->insn_code = CODE_FOR_movxf;
  if (HAVE_movmemqi)
    movmem_optab[QImode] = CODE_FOR_movmemqi;
  if (HAVE_cbranchsi4)
    optab_handler (cbranch_optab, SImode)->insn_code = CODE_FOR_cbranchsi4;
  if (HAVE_cbranchqi4)
    optab_handler (cbranch_optab, QImode)->insn_code = CODE_FOR_cbranchqi4;
  if (HAVE_cbranchsf4)
    optab_handler (cbranch_optab, SFmode)->insn_code = CODE_FOR_cbranchsf4;
  if (HAVE_cbranchdf4)
    optab_handler (cbranch_optab, DFmode)->insn_code = CODE_FOR_cbranchdf4;
  if (HAVE_cbranchdi4)
    optab_handler (cbranch_optab, DImode)->insn_code = CODE_FOR_cbranchdi4;
  if (HAVE_cstoresi4)
    optab_handler (cstore_optab, SImode)->insn_code = CODE_FOR_cstoresi4;
  if (HAVE_cstoresf4)
    optab_handler (cstore_optab, SFmode)->insn_code = CODE_FOR_cstoresf4;
  if (HAVE_cstoredf4)
    optab_handler (cstore_optab, DFmode)->insn_code = CODE_FOR_cstoredf4;
  if (HAVE_cstoredi4)
    optab_handler (cstore_optab, DImode)->insn_code = CODE_FOR_cstoredi4;
  if (HAVE_movsicc)
    movcc_gen_code[SImode] = CODE_FOR_movsicc;
  if (HAVE_movsfcc)
    movcc_gen_code[SFmode] = CODE_FOR_movsfcc;
  if (HAVE_movdfcc)
    movcc_gen_code[DFmode] = CODE_FOR_movdfcc;
  if (HAVE_ctzsi2)
    optab_handler (ctz_optab, SImode)->insn_code = CODE_FOR_ctzsi2;
  if (HAVE_bswapsi2)
    optab_handler (bswap_optab, SImode)->insn_code = CODE_FOR_bswapsi2;
  if (HAVE_movv2di)
    optab_handler (mov_optab, V2DImode)->insn_code = CODE_FOR_movv2di;
  if (HAVE_movv2si)
    optab_handler (mov_optab, V2SImode)->insn_code = CODE_FOR_movv2si;
  if (HAVE_movv4hi)
    optab_handler (mov_optab, V4HImode)->insn_code = CODE_FOR_movv4hi;
  if (HAVE_movv8qi)
    optab_handler (mov_optab, V8QImode)->insn_code = CODE_FOR_movv8qi;
  if (HAVE_movv2sf)
    optab_handler (mov_optab, V2SFmode)->insn_code = CODE_FOR_movv2sf;
  if (HAVE_movv4si)
    optab_handler (mov_optab, V4SImode)->insn_code = CODE_FOR_movv4si;
  if (HAVE_movv8hi)
    optab_handler (mov_optab, V8HImode)->insn_code = CODE_FOR_movv8hi;
  if (HAVE_movv16qi)
    optab_handler (mov_optab, V16QImode)->insn_code = CODE_FOR_movv16qi;
  if (HAVE_movv4sf)
    optab_handler (mov_optab, V4SFmode)->insn_code = CODE_FOR_movv4sf;
  if (HAVE_addv2di3)
    optab_handler (add_optab, V2DImode)->insn_code = CODE_FOR_addv2di3;
  if (HAVE_addv2si3)
    optab_handler (add_optab, V2SImode)->insn_code = CODE_FOR_addv2si3;
  if (HAVE_addv4hi3)
    optab_handler (add_optab, V4HImode)->insn_code = CODE_FOR_addv4hi3;
  if (HAVE_addv8qi3)
    optab_handler (add_optab, V8QImode)->insn_code = CODE_FOR_addv8qi3;
  if (HAVE_addv2sf3)
    optab_handler (addv_optab, V2SFmode)->insn_code =
    optab_handler (add_optab, V2SFmode)->insn_code = CODE_FOR_addv2sf3;
  if (HAVE_addv4si3)
    optab_handler (add_optab, V4SImode)->insn_code = CODE_FOR_addv4si3;
  if (HAVE_addv8hi3)
    optab_handler (add_optab, V8HImode)->insn_code = CODE_FOR_addv8hi3;
  if (HAVE_addv16qi3)
    optab_handler (add_optab, V16QImode)->insn_code = CODE_FOR_addv16qi3;
  if (HAVE_addv4sf3)
    optab_handler (addv_optab, V4SFmode)->insn_code =
    optab_handler (add_optab, V4SFmode)->insn_code = CODE_FOR_addv4sf3;
  if (HAVE_subv2di3)
    optab_handler (sub_optab, V2DImode)->insn_code = CODE_FOR_subv2di3;
  if (HAVE_subv2si3)
    optab_handler (sub_optab, V2SImode)->insn_code = CODE_FOR_subv2si3;
  if (HAVE_subv4hi3)
    optab_handler (sub_optab, V4HImode)->insn_code = CODE_FOR_subv4hi3;
  if (HAVE_subv8qi3)
    optab_handler (sub_optab, V8QImode)->insn_code = CODE_FOR_subv8qi3;
  if (HAVE_subv2sf3)
    optab_handler (subv_optab, V2SFmode)->insn_code =
    optab_handler (sub_optab, V2SFmode)->insn_code = CODE_FOR_subv2sf3;
  if (HAVE_subv4si3)
    optab_handler (sub_optab, V4SImode)->insn_code = CODE_FOR_subv4si3;
  if (HAVE_subv8hi3)
    optab_handler (sub_optab, V8HImode)->insn_code = CODE_FOR_subv8hi3;
  if (HAVE_subv16qi3)
    optab_handler (sub_optab, V16QImode)->insn_code = CODE_FOR_subv16qi3;
  if (HAVE_subv4sf3)
    optab_handler (subv_optab, V4SFmode)->insn_code =
    optab_handler (sub_optab, V4SFmode)->insn_code = CODE_FOR_subv4sf3;
  if (HAVE_mulv2si3)
    optab_handler (smul_optab, V2SImode)->insn_code = CODE_FOR_mulv2si3;
  if (HAVE_mulv4hi3)
    optab_handler (smul_optab, V4HImode)->insn_code = CODE_FOR_mulv4hi3;
  if (HAVE_mulv8qi3)
    optab_handler (smul_optab, V8QImode)->insn_code = CODE_FOR_mulv8qi3;
  if (HAVE_mulv2sf3)
    optab_handler (smulv_optab, V2SFmode)->insn_code =
    optab_handler (smul_optab, V2SFmode)->insn_code = CODE_FOR_mulv2sf3;
  if (HAVE_mulv4si3)
    optab_handler (smul_optab, V4SImode)->insn_code = CODE_FOR_mulv4si3;
  if (HAVE_mulv8hi3)
    optab_handler (smul_optab, V8HImode)->insn_code = CODE_FOR_mulv8hi3;
  if (HAVE_mulv16qi3)
    optab_handler (smul_optab, V16QImode)->insn_code = CODE_FOR_mulv16qi3;
  if (HAVE_mulv4sf3)
    optab_handler (smulv_optab, V4SFmode)->insn_code =
    optab_handler (smul_optab, V4SFmode)->insn_code = CODE_FOR_mulv4sf3;
  if (HAVE_sminv2si3)
    optab_handler (smin_optab, V2SImode)->insn_code = CODE_FOR_sminv2si3;
  if (HAVE_sminv4hi3)
    optab_handler (smin_optab, V4HImode)->insn_code = CODE_FOR_sminv4hi3;
  if (HAVE_sminv8qi3)
    optab_handler (smin_optab, V8QImode)->insn_code = CODE_FOR_sminv8qi3;
  if (HAVE_sminv2sf3)
    optab_handler (smin_optab, V2SFmode)->insn_code = CODE_FOR_sminv2sf3;
  if (HAVE_sminv4si3)
    optab_handler (smin_optab, V4SImode)->insn_code = CODE_FOR_sminv4si3;
  if (HAVE_sminv8hi3)
    optab_handler (smin_optab, V8HImode)->insn_code = CODE_FOR_sminv8hi3;
  if (HAVE_sminv16qi3)
    optab_handler (smin_optab, V16QImode)->insn_code = CODE_FOR_sminv16qi3;
  if (HAVE_sminv4sf3)
    optab_handler (smin_optab, V4SFmode)->insn_code = CODE_FOR_sminv4sf3;
  if (HAVE_uminv2si3)
    optab_handler (umin_optab, V2SImode)->insn_code = CODE_FOR_uminv2si3;
  if (HAVE_uminv4hi3)
    optab_handler (umin_optab, V4HImode)->insn_code = CODE_FOR_uminv4hi3;
  if (HAVE_uminv8qi3)
    optab_handler (umin_optab, V8QImode)->insn_code = CODE_FOR_uminv8qi3;
  if (HAVE_uminv4si3)
    optab_handler (umin_optab, V4SImode)->insn_code = CODE_FOR_uminv4si3;
  if (HAVE_uminv8hi3)
    optab_handler (umin_optab, V8HImode)->insn_code = CODE_FOR_uminv8hi3;
  if (HAVE_uminv16qi3)
    optab_handler (umin_optab, V16QImode)->insn_code = CODE_FOR_uminv16qi3;
  if (HAVE_smaxv2si3)
    optab_handler (smax_optab, V2SImode)->insn_code = CODE_FOR_smaxv2si3;
  if (HAVE_smaxv4hi3)
    optab_handler (smax_optab, V4HImode)->insn_code = CODE_FOR_smaxv4hi3;
  if (HAVE_smaxv8qi3)
    optab_handler (smax_optab, V8QImode)->insn_code = CODE_FOR_smaxv8qi3;
  if (HAVE_smaxv2sf3)
    optab_handler (smax_optab, V2SFmode)->insn_code = CODE_FOR_smaxv2sf3;
  if (HAVE_smaxv4si3)
    optab_handler (smax_optab, V4SImode)->insn_code = CODE_FOR_smaxv4si3;
  if (HAVE_smaxv8hi3)
    optab_handler (smax_optab, V8HImode)->insn_code = CODE_FOR_smaxv8hi3;
  if (HAVE_smaxv16qi3)
    optab_handler (smax_optab, V16QImode)->insn_code = CODE_FOR_smaxv16qi3;
  if (HAVE_smaxv4sf3)
    optab_handler (smax_optab, V4SFmode)->insn_code = CODE_FOR_smaxv4sf3;
  if (HAVE_umaxv2si3)
    optab_handler (umax_optab, V2SImode)->insn_code = CODE_FOR_umaxv2si3;
  if (HAVE_umaxv4hi3)
    optab_handler (umax_optab, V4HImode)->insn_code = CODE_FOR_umaxv4hi3;
  if (HAVE_umaxv8qi3)
    optab_handler (umax_optab, V8QImode)->insn_code = CODE_FOR_umaxv8qi3;
  if (HAVE_umaxv4si3)
    optab_handler (umax_optab, V4SImode)->insn_code = CODE_FOR_umaxv4si3;
  if (HAVE_umaxv8hi3)
    optab_handler (umax_optab, V8HImode)->insn_code = CODE_FOR_umaxv8hi3;
  if (HAVE_umaxv16qi3)
    optab_handler (umax_optab, V16QImode)->insn_code = CODE_FOR_umaxv16qi3;
  if (HAVE_movti)
    optab_handler (mov_optab, TImode)->insn_code = CODE_FOR_movti;
  if (HAVE_movei)
    optab_handler (mov_optab, EImode)->insn_code = CODE_FOR_movei;
  if (HAVE_movoi)
    optab_handler (mov_optab, OImode)->insn_code = CODE_FOR_movoi;
  if (HAVE_movci)
    optab_handler (mov_optab, CImode)->insn_code = CODE_FOR_movci;
  if (HAVE_movxi)
    optab_handler (mov_optab, XImode)->insn_code = CODE_FOR_movxi;
  if (HAVE_movmisalignv8qi)
    optab_handler (movmisalign_optab, V8QImode)->insn_code = CODE_FOR_movmisalignv8qi;
  if (HAVE_movmisalignv16qi)
    optab_handler (movmisalign_optab, V16QImode)->insn_code = CODE_FOR_movmisalignv16qi;
  if (HAVE_movmisalignv4hi)
    optab_handler (movmisalign_optab, V4HImode)->insn_code = CODE_FOR_movmisalignv4hi;
  if (HAVE_movmisalignv8hi)
    optab_handler (movmisalign_optab, V8HImode)->insn_code = CODE_FOR_movmisalignv8hi;
  if (HAVE_movmisalignv2si)
    optab_handler (movmisalign_optab, V2SImode)->insn_code = CODE_FOR_movmisalignv2si;
  if (HAVE_movmisalignv4si)
    optab_handler (movmisalign_optab, V4SImode)->insn_code = CODE_FOR_movmisalignv4si;
  if (HAVE_movmisalignv2sf)
    optab_handler (movmisalign_optab, V2SFmode)->insn_code = CODE_FOR_movmisalignv2sf;
  if (HAVE_movmisalignv4sf)
    optab_handler (movmisalign_optab, V4SFmode)->insn_code = CODE_FOR_movmisalignv4sf;
  if (HAVE_movmisaligndi)
    optab_handler (movmisalign_optab, DImode)->insn_code = CODE_FOR_movmisaligndi;
  if (HAVE_movmisalignv2di)
    optab_handler (movmisalign_optab, V2DImode)->insn_code = CODE_FOR_movmisalignv2di;
  if (HAVE_vec_setv8qi)
    optab_handler (vec_set_optab, V8QImode)->insn_code = CODE_FOR_vec_setv8qi;
  if (HAVE_vec_setv16qi)
    optab_handler (vec_set_optab, V16QImode)->insn_code = CODE_FOR_vec_setv16qi;
  if (HAVE_vec_setv4hi)
    optab_handler (vec_set_optab, V4HImode)->insn_code = CODE_FOR_vec_setv4hi;
  if (HAVE_vec_setv8hi)
    optab_handler (vec_set_optab, V8HImode)->insn_code = CODE_FOR_vec_setv8hi;
  if (HAVE_vec_setv2si)
    optab_handler (vec_set_optab, V2SImode)->insn_code = CODE_FOR_vec_setv2si;
  if (HAVE_vec_setv4si)
    optab_handler (vec_set_optab, V4SImode)->insn_code = CODE_FOR_vec_setv4si;
  if (HAVE_vec_setv2sf)
    optab_handler (vec_set_optab, V2SFmode)->insn_code = CODE_FOR_vec_setv2sf;
  if (HAVE_vec_setv4sf)
    optab_handler (vec_set_optab, V4SFmode)->insn_code = CODE_FOR_vec_setv4sf;
  if (HAVE_vec_setv2di)
    optab_handler (vec_set_optab, V2DImode)->insn_code = CODE_FOR_vec_setv2di;
  if (HAVE_vec_initv8qi)
    optab_handler (vec_init_optab, V8QImode)->insn_code = CODE_FOR_vec_initv8qi;
  if (HAVE_vec_initv16qi)
    optab_handler (vec_init_optab, V16QImode)->insn_code = CODE_FOR_vec_initv16qi;
  if (HAVE_vec_initv4hi)
    optab_handler (vec_init_optab, V4HImode)->insn_code = CODE_FOR_vec_initv4hi;
  if (HAVE_vec_initv8hi)
    optab_handler (vec_init_optab, V8HImode)->insn_code = CODE_FOR_vec_initv8hi;
  if (HAVE_vec_initv2si)
    optab_handler (vec_init_optab, V2SImode)->insn_code = CODE_FOR_vec_initv2si;
  if (HAVE_vec_initv4si)
    optab_handler (vec_init_optab, V4SImode)->insn_code = CODE_FOR_vec_initv4si;
  if (HAVE_vec_initv2sf)
    optab_handler (vec_init_optab, V2SFmode)->insn_code = CODE_FOR_vec_initv2sf;
  if (HAVE_vec_initv4sf)
    optab_handler (vec_init_optab, V4SFmode)->insn_code = CODE_FOR_vec_initv4sf;
  if (HAVE_vec_initv2di)
    optab_handler (vec_init_optab, V2DImode)->insn_code = CODE_FOR_vec_initv2di;
  if (HAVE_vashrv8qi3)
    optab_handler (vashr_optab, V8QImode)->insn_code = CODE_FOR_vashrv8qi3;
  if (HAVE_vashrv16qi3)
    optab_handler (vashr_optab, V16QImode)->insn_code = CODE_FOR_vashrv16qi3;
  if (HAVE_vashrv4hi3)
    optab_handler (vashr_optab, V4HImode)->insn_code = CODE_FOR_vashrv4hi3;
  if (HAVE_vashrv8hi3)
    optab_handler (vashr_optab, V8HImode)->insn_code = CODE_FOR_vashrv8hi3;
  if (HAVE_vashrv2si3)
    optab_handler (vashr_optab, V2SImode)->insn_code = CODE_FOR_vashrv2si3;
  if (HAVE_vashrv4si3)
    optab_handler (vashr_optab, V4SImode)->insn_code = CODE_FOR_vashrv4si3;
  if (HAVE_vlshrv8qi3)
    optab_handler (vlshr_optab, V8QImode)->insn_code = CODE_FOR_vlshrv8qi3;
  if (HAVE_vlshrv16qi3)
    optab_handler (vlshr_optab, V16QImode)->insn_code = CODE_FOR_vlshrv16qi3;
  if (HAVE_vlshrv4hi3)
    optab_handler (vlshr_optab, V4HImode)->insn_code = CODE_FOR_vlshrv4hi3;
  if (HAVE_vlshrv8hi3)
    optab_handler (vlshr_optab, V8HImode)->insn_code = CODE_FOR_vlshrv8hi3;
  if (HAVE_vlshrv2si3)
    optab_handler (vlshr_optab, V2SImode)->insn_code = CODE_FOR_vlshrv2si3;
  if (HAVE_vlshrv4si3)
    optab_handler (vlshr_optab, V4SImode)->insn_code = CODE_FOR_vlshrv4si3;
  if (HAVE_vec_shr_v8qi)
    optab_handler (vec_shr_optab, V8QImode)->insn_code = CODE_FOR_vec_shr_v8qi;
  if (HAVE_vec_shr_v16qi)
    optab_handler (vec_shr_optab, V16QImode)->insn_code = CODE_FOR_vec_shr_v16qi;
  if (HAVE_vec_shr_v4hi)
    optab_handler (vec_shr_optab, V4HImode)->insn_code = CODE_FOR_vec_shr_v4hi;
  if (HAVE_vec_shr_v8hi)
    optab_handler (vec_shr_optab, V8HImode)->insn_code = CODE_FOR_vec_shr_v8hi;
  if (HAVE_vec_shr_v2si)
    optab_handler (vec_shr_optab, V2SImode)->insn_code = CODE_FOR_vec_shr_v2si;
  if (HAVE_vec_shr_v4si)
    optab_handler (vec_shr_optab, V4SImode)->insn_code = CODE_FOR_vec_shr_v4si;
  if (HAVE_vec_shr_v2sf)
    optab_handler (vec_shr_optab, V2SFmode)->insn_code = CODE_FOR_vec_shr_v2sf;
  if (HAVE_vec_shr_v4sf)
    optab_handler (vec_shr_optab, V4SFmode)->insn_code = CODE_FOR_vec_shr_v4sf;
  if (HAVE_vec_shr_v2di)
    optab_handler (vec_shr_optab, V2DImode)->insn_code = CODE_FOR_vec_shr_v2di;
  if (HAVE_vec_shl_v8qi)
    optab_handler (vec_shl_optab, V8QImode)->insn_code = CODE_FOR_vec_shl_v8qi;
  if (HAVE_vec_shl_v16qi)
    optab_handler (vec_shl_optab, V16QImode)->insn_code = CODE_FOR_vec_shl_v16qi;
  if (HAVE_vec_shl_v4hi)
    optab_handler (vec_shl_optab, V4HImode)->insn_code = CODE_FOR_vec_shl_v4hi;
  if (HAVE_vec_shl_v8hi)
    optab_handler (vec_shl_optab, V8HImode)->insn_code = CODE_FOR_vec_shl_v8hi;
  if (HAVE_vec_shl_v2si)
    optab_handler (vec_shl_optab, V2SImode)->insn_code = CODE_FOR_vec_shl_v2si;
  if (HAVE_vec_shl_v4si)
    optab_handler (vec_shl_optab, V4SImode)->insn_code = CODE_FOR_vec_shl_v4si;
  if (HAVE_vec_shl_v2sf)
    optab_handler (vec_shl_optab, V2SFmode)->insn_code = CODE_FOR_vec_shl_v2sf;
  if (HAVE_vec_shl_v4sf)
    optab_handler (vec_shl_optab, V4SFmode)->insn_code = CODE_FOR_vec_shl_v4sf;
  if (HAVE_vec_shl_v2di)
    optab_handler (vec_shl_optab, V2DImode)->insn_code = CODE_FOR_vec_shl_v2di;
  if (HAVE_reduc_splus_v8qi)
    optab_handler (reduc_splus_optab, V8QImode)->insn_code = CODE_FOR_reduc_splus_v8qi;
  if (HAVE_reduc_splus_v4hi)
    optab_handler (reduc_splus_optab, V4HImode)->insn_code = CODE_FOR_reduc_splus_v4hi;
  if (HAVE_reduc_splus_v2si)
    optab_handler (reduc_splus_optab, V2SImode)->insn_code = CODE_FOR_reduc_splus_v2si;
  if (HAVE_reduc_splus_v2sf)
    optab_handler (reduc_splus_optab, V2SFmode)->insn_code = CODE_FOR_reduc_splus_v2sf;
  if (HAVE_reduc_splus_v16qi)
    optab_handler (reduc_splus_optab, V16QImode)->insn_code = CODE_FOR_reduc_splus_v16qi;
  if (HAVE_reduc_splus_v8hi)
    optab_handler (reduc_splus_optab, V8HImode)->insn_code = CODE_FOR_reduc_splus_v8hi;
  if (HAVE_reduc_splus_v4si)
    optab_handler (reduc_splus_optab, V4SImode)->insn_code = CODE_FOR_reduc_splus_v4si;
  if (HAVE_reduc_splus_v4sf)
    optab_handler (reduc_splus_optab, V4SFmode)->insn_code = CODE_FOR_reduc_splus_v4sf;
  if (HAVE_reduc_uplus_v8qi)
    optab_handler (reduc_uplus_optab, V8QImode)->insn_code = CODE_FOR_reduc_uplus_v8qi;
  if (HAVE_reduc_uplus_v16qi)
    optab_handler (reduc_uplus_optab, V16QImode)->insn_code = CODE_FOR_reduc_uplus_v16qi;
  if (HAVE_reduc_uplus_v4hi)
    optab_handler (reduc_uplus_optab, V4HImode)->insn_code = CODE_FOR_reduc_uplus_v4hi;
  if (HAVE_reduc_uplus_v8hi)
    optab_handler (reduc_uplus_optab, V8HImode)->insn_code = CODE_FOR_reduc_uplus_v8hi;
  if (HAVE_reduc_uplus_v2si)
    optab_handler (reduc_uplus_optab, V2SImode)->insn_code = CODE_FOR_reduc_uplus_v2si;
  if (HAVE_reduc_uplus_v4si)
    optab_handler (reduc_uplus_optab, V4SImode)->insn_code = CODE_FOR_reduc_uplus_v4si;
  if (HAVE_reduc_uplus_v2di)
    optab_handler (reduc_uplus_optab, V2DImode)->insn_code = CODE_FOR_reduc_uplus_v2di;
  if (HAVE_reduc_smin_v8qi)
    optab_handler (reduc_smin_optab, V8QImode)->insn_code = CODE_FOR_reduc_smin_v8qi;
  if (HAVE_reduc_smin_v4hi)
    optab_handler (reduc_smin_optab, V4HImode)->insn_code = CODE_FOR_reduc_smin_v4hi;
  if (HAVE_reduc_smin_v2si)
    optab_handler (reduc_smin_optab, V2SImode)->insn_code = CODE_FOR_reduc_smin_v2si;
  if (HAVE_reduc_smin_v2sf)
    optab_handler (reduc_smin_optab, V2SFmode)->insn_code = CODE_FOR_reduc_smin_v2sf;
  if (HAVE_reduc_smin_v16qi)
    optab_handler (reduc_smin_optab, V16QImode)->insn_code = CODE_FOR_reduc_smin_v16qi;
  if (HAVE_reduc_smin_v8hi)
    optab_handler (reduc_smin_optab, V8HImode)->insn_code = CODE_FOR_reduc_smin_v8hi;
  if (HAVE_reduc_smin_v4si)
    optab_handler (reduc_smin_optab, V4SImode)->insn_code = CODE_FOR_reduc_smin_v4si;
  if (HAVE_reduc_smin_v4sf)
    optab_handler (reduc_smin_optab, V4SFmode)->insn_code = CODE_FOR_reduc_smin_v4sf;
  if (HAVE_reduc_smax_v8qi)
    optab_handler (reduc_smax_optab, V8QImode)->insn_code = CODE_FOR_reduc_smax_v8qi;
  if (HAVE_reduc_smax_v4hi)
    optab_handler (reduc_smax_optab, V4HImode)->insn_code = CODE_FOR_reduc_smax_v4hi;
  if (HAVE_reduc_smax_v2si)
    optab_handler (reduc_smax_optab, V2SImode)->insn_code = CODE_FOR_reduc_smax_v2si;
  if (HAVE_reduc_smax_v2sf)
    optab_handler (reduc_smax_optab, V2SFmode)->insn_code = CODE_FOR_reduc_smax_v2sf;
  if (HAVE_reduc_smax_v16qi)
    optab_handler (reduc_smax_optab, V16QImode)->insn_code = CODE_FOR_reduc_smax_v16qi;
  if (HAVE_reduc_smax_v8hi)
    optab_handler (reduc_smax_optab, V8HImode)->insn_code = CODE_FOR_reduc_smax_v8hi;
  if (HAVE_reduc_smax_v4si)
    optab_handler (reduc_smax_optab, V4SImode)->insn_code = CODE_FOR_reduc_smax_v4si;
  if (HAVE_reduc_smax_v4sf)
    optab_handler (reduc_smax_optab, V4SFmode)->insn_code = CODE_FOR_reduc_smax_v4sf;
  if (HAVE_reduc_umin_v8qi)
    optab_handler (reduc_umin_optab, V8QImode)->insn_code = CODE_FOR_reduc_umin_v8qi;
  if (HAVE_reduc_umin_v4hi)
    optab_handler (reduc_umin_optab, V4HImode)->insn_code = CODE_FOR_reduc_umin_v4hi;
  if (HAVE_reduc_umin_v2si)
    optab_handler (reduc_umin_optab, V2SImode)->insn_code = CODE_FOR_reduc_umin_v2si;
  if (HAVE_reduc_umin_v16qi)
    optab_handler (reduc_umin_optab, V16QImode)->insn_code = CODE_FOR_reduc_umin_v16qi;
  if (HAVE_reduc_umin_v8hi)
    optab_handler (reduc_umin_optab, V8HImode)->insn_code = CODE_FOR_reduc_umin_v8hi;
  if (HAVE_reduc_umin_v4si)
    optab_handler (reduc_umin_optab, V4SImode)->insn_code = CODE_FOR_reduc_umin_v4si;
  if (HAVE_reduc_umax_v8qi)
    optab_handler (reduc_umax_optab, V8QImode)->insn_code = CODE_FOR_reduc_umax_v8qi;
  if (HAVE_reduc_umax_v4hi)
    optab_handler (reduc_umax_optab, V4HImode)->insn_code = CODE_FOR_reduc_umax_v4hi;
  if (HAVE_reduc_umax_v2si)
    optab_handler (reduc_umax_optab, V2SImode)->insn_code = CODE_FOR_reduc_umax_v2si;
  if (HAVE_reduc_umax_v16qi)
    optab_handler (reduc_umax_optab, V16QImode)->insn_code = CODE_FOR_reduc_umax_v16qi;
  if (HAVE_reduc_umax_v8hi)
    optab_handler (reduc_umax_optab, V8HImode)->insn_code = CODE_FOR_reduc_umax_v8hi;
  if (HAVE_reduc_umax_v4si)
    optab_handler (reduc_umax_optab, V4SImode)->insn_code = CODE_FOR_reduc_umax_v4si;
  if (HAVE_vcondv8qi)
    vcond_gen_code[V8QImode] = CODE_FOR_vcondv8qi;
  if (HAVE_vcondv16qi)
    vcond_gen_code[V16QImode] = CODE_FOR_vcondv16qi;
  if (HAVE_vcondv4hi)
    vcond_gen_code[V4HImode] = CODE_FOR_vcondv4hi;
  if (HAVE_vcondv8hi)
    vcond_gen_code[V8HImode] = CODE_FOR_vcondv8hi;
  if (HAVE_vcondv2si)
    vcond_gen_code[V2SImode] = CODE_FOR_vcondv2si;
  if (HAVE_vcondv4si)
    vcond_gen_code[V4SImode] = CODE_FOR_vcondv4si;
  if (HAVE_vcondv2sf)
    vcond_gen_code[V2SFmode] = CODE_FOR_vcondv2sf;
  if (HAVE_vcondv4sf)
    vcond_gen_code[V4SFmode] = CODE_FOR_vcondv4sf;
  if (HAVE_vconduv8qi)
    vcondu_gen_code[V8QImode] = CODE_FOR_vconduv8qi;
  if (HAVE_vconduv16qi)
    vcondu_gen_code[V16QImode] = CODE_FOR_vconduv16qi;
  if (HAVE_vconduv4hi)
    vcondu_gen_code[V4HImode] = CODE_FOR_vconduv4hi;
  if (HAVE_vconduv8hi)
    vcondu_gen_code[V8HImode] = CODE_FOR_vconduv8hi;
  if (HAVE_vconduv2si)
    vcondu_gen_code[V2SImode] = CODE_FOR_vconduv2si;
  if (HAVE_vconduv4si)
    vcondu_gen_code[V4SImode] = CODE_FOR_vconduv4si;
  if (HAVE_vec_unpacks_hi_v16qi)
    optab_handler (vec_unpacks_hi_optab, V16QImode)->insn_code = CODE_FOR_vec_unpacks_hi_v16qi;
  if (HAVE_vec_unpacku_hi_v16qi)
    optab_handler (vec_unpacku_hi_optab, V16QImode)->insn_code = CODE_FOR_vec_unpacku_hi_v16qi;
  if (HAVE_vec_unpacks_hi_v8hi)
    optab_handler (vec_unpacks_hi_optab, V8HImode)->insn_code = CODE_FOR_vec_unpacks_hi_v8hi;
  if (HAVE_vec_unpacku_hi_v8hi)
    optab_handler (vec_unpacku_hi_optab, V8HImode)->insn_code = CODE_FOR_vec_unpacku_hi_v8hi;
  if (HAVE_vec_unpacks_hi_v4si)
    optab_handler (vec_unpacks_hi_optab, V4SImode)->insn_code = CODE_FOR_vec_unpacks_hi_v4si;
  if (HAVE_vec_unpacku_hi_v4si)
    optab_handler (vec_unpacku_hi_optab, V4SImode)->insn_code = CODE_FOR_vec_unpacku_hi_v4si;
  if (HAVE_vec_unpacks_lo_v16qi)
    optab_handler (vec_unpacks_lo_optab, V16QImode)->insn_code = CODE_FOR_vec_unpacks_lo_v16qi;
  if (HAVE_vec_unpacku_lo_v16qi)
    optab_handler (vec_unpacku_lo_optab, V16QImode)->insn_code = CODE_FOR_vec_unpacku_lo_v16qi;
  if (HAVE_vec_unpacks_lo_v8hi)
    optab_handler (vec_unpacks_lo_optab, V8HImode)->insn_code = CODE_FOR_vec_unpacks_lo_v8hi;
  if (HAVE_vec_unpacku_lo_v8hi)
    optab_handler (vec_unpacku_lo_optab, V8HImode)->insn_code = CODE_FOR_vec_unpacku_lo_v8hi;
  if (HAVE_vec_unpacks_lo_v4si)
    optab_handler (vec_unpacks_lo_optab, V4SImode)->insn_code = CODE_FOR_vec_unpacks_lo_v4si;
  if (HAVE_vec_unpacku_lo_v4si)
    optab_handler (vec_unpacku_lo_optab, V4SImode)->insn_code = CODE_FOR_vec_unpacku_lo_v4si;
  if (HAVE_vec_widen_smult_lo_v16qi)
    optab_handler (vec_widen_smult_lo_optab, V16QImode)->insn_code = CODE_FOR_vec_widen_smult_lo_v16qi;
  if (HAVE_vec_widen_umult_lo_v16qi)
    optab_handler (vec_widen_umult_lo_optab, V16QImode)->insn_code = CODE_FOR_vec_widen_umult_lo_v16qi;
  if (HAVE_vec_widen_smult_lo_v8hi)
    optab_handler (vec_widen_smult_lo_optab, V8HImode)->insn_code = CODE_FOR_vec_widen_smult_lo_v8hi;
  if (HAVE_vec_widen_umult_lo_v8hi)
    optab_handler (vec_widen_umult_lo_optab, V8HImode)->insn_code = CODE_FOR_vec_widen_umult_lo_v8hi;
  if (HAVE_vec_widen_smult_lo_v4si)
    optab_handler (vec_widen_smult_lo_optab, V4SImode)->insn_code = CODE_FOR_vec_widen_smult_lo_v4si;
  if (HAVE_vec_widen_umult_lo_v4si)
    optab_handler (vec_widen_umult_lo_optab, V4SImode)->insn_code = CODE_FOR_vec_widen_umult_lo_v4si;
  if (HAVE_vec_widen_smult_hi_v16qi)
    optab_handler (vec_widen_smult_hi_optab, V16QImode)->insn_code = CODE_FOR_vec_widen_smult_hi_v16qi;
  if (HAVE_vec_widen_umult_hi_v16qi)
    optab_handler (vec_widen_umult_hi_optab, V16QImode)->insn_code = CODE_FOR_vec_widen_umult_hi_v16qi;
  if (HAVE_vec_widen_smult_hi_v8hi)
    optab_handler (vec_widen_smult_hi_optab, V8HImode)->insn_code = CODE_FOR_vec_widen_smult_hi_v8hi;
  if (HAVE_vec_widen_umult_hi_v8hi)
    optab_handler (vec_widen_umult_hi_optab, V8HImode)->insn_code = CODE_FOR_vec_widen_umult_hi_v8hi;
  if (HAVE_vec_widen_smult_hi_v4si)
    optab_handler (vec_widen_smult_hi_optab, V4SImode)->insn_code = CODE_FOR_vec_widen_smult_hi_v4si;
  if (HAVE_vec_widen_umult_hi_v4si)
    optab_handler (vec_widen_umult_hi_optab, V4SImode)->insn_code = CODE_FOR_vec_widen_umult_hi_v4si;
  if (HAVE_vec_unpacks_lo_v8qi)
    optab_handler (vec_unpacks_lo_optab, V8QImode)->insn_code = CODE_FOR_vec_unpacks_lo_v8qi;
  if (HAVE_vec_unpacku_lo_v8qi)
    optab_handler (vec_unpacku_lo_optab, V8QImode)->insn_code = CODE_FOR_vec_unpacku_lo_v8qi;
  if (HAVE_vec_unpacks_lo_v4hi)
    optab_handler (vec_unpacks_lo_optab, V4HImode)->insn_code = CODE_FOR_vec_unpacks_lo_v4hi;
  if (HAVE_vec_unpacku_lo_v4hi)
    optab_handler (vec_unpacku_lo_optab, V4HImode)->insn_code = CODE_FOR_vec_unpacku_lo_v4hi;
  if (HAVE_vec_unpacks_lo_v2si)
    optab_handler (vec_unpacks_lo_optab, V2SImode)->insn_code = CODE_FOR_vec_unpacks_lo_v2si;
  if (HAVE_vec_unpacku_lo_v2si)
    optab_handler (vec_unpacku_lo_optab, V2SImode)->insn_code = CODE_FOR_vec_unpacku_lo_v2si;
  if (HAVE_vec_unpacks_hi_v8qi)
    optab_handler (vec_unpacks_hi_optab, V8QImode)->insn_code = CODE_FOR_vec_unpacks_hi_v8qi;
  if (HAVE_vec_unpacku_hi_v8qi)
    optab_handler (vec_unpacku_hi_optab, V8QImode)->insn_code = CODE_FOR_vec_unpacku_hi_v8qi;
  if (HAVE_vec_unpacks_hi_v4hi)
    optab_handler (vec_unpacks_hi_optab, V4HImode)->insn_code = CODE_FOR_vec_unpacks_hi_v4hi;
  if (HAVE_vec_unpacku_hi_v4hi)
    optab_handler (vec_unpacku_hi_optab, V4HImode)->insn_code = CODE_FOR_vec_unpacku_hi_v4hi;
  if (HAVE_vec_unpacks_hi_v2si)
    optab_handler (vec_unpacks_hi_optab, V2SImode)->insn_code = CODE_FOR_vec_unpacks_hi_v2si;
  if (HAVE_vec_unpacku_hi_v2si)
    optab_handler (vec_unpacku_hi_optab, V2SImode)->insn_code = CODE_FOR_vec_unpacku_hi_v2si;
  if (HAVE_vec_widen_smult_hi_v8qi)
    optab_handler (vec_widen_smult_hi_optab, V8QImode)->insn_code = CODE_FOR_vec_widen_smult_hi_v8qi;
  if (HAVE_vec_widen_umult_hi_v8qi)
    optab_handler (vec_widen_umult_hi_optab, V8QImode)->insn_code = CODE_FOR_vec_widen_umult_hi_v8qi;
  if (HAVE_vec_widen_smult_hi_v4hi)
    optab_handler (vec_widen_smult_hi_optab, V4HImode)->insn_code = CODE_FOR_vec_widen_smult_hi_v4hi;
  if (HAVE_vec_widen_umult_hi_v4hi)
    optab_handler (vec_widen_umult_hi_optab, V4HImode)->insn_code = CODE_FOR_vec_widen_umult_hi_v4hi;
  if (HAVE_vec_widen_smult_hi_v2si)
    optab_handler (vec_widen_smult_hi_optab, V2SImode)->insn_code = CODE_FOR_vec_widen_smult_hi_v2si;
  if (HAVE_vec_widen_umult_hi_v2si)
    optab_handler (vec_widen_umult_hi_optab, V2SImode)->insn_code = CODE_FOR_vec_widen_umult_hi_v2si;
  if (HAVE_vec_widen_smult_lo_v8qi)
    optab_handler (vec_widen_smult_lo_optab, V8QImode)->insn_code = CODE_FOR_vec_widen_smult_lo_v8qi;
  if (HAVE_vec_widen_umult_lo_v8qi)
    optab_handler (vec_widen_umult_lo_optab, V8QImode)->insn_code = CODE_FOR_vec_widen_umult_lo_v8qi;
  if (HAVE_vec_widen_smult_lo_v4hi)
    optab_handler (vec_widen_smult_lo_optab, V4HImode)->insn_code = CODE_FOR_vec_widen_smult_lo_v4hi;
  if (HAVE_vec_widen_umult_lo_v4hi)
    optab_handler (vec_widen_umult_lo_optab, V4HImode)->insn_code = CODE_FOR_vec_widen_umult_lo_v4hi;
  if (HAVE_vec_widen_smult_lo_v2si)
    optab_handler (vec_widen_smult_lo_optab, V2SImode)->insn_code = CODE_FOR_vec_widen_smult_lo_v2si;
  if (HAVE_vec_widen_umult_lo_v2si)
    optab_handler (vec_widen_umult_lo_optab, V2SImode)->insn_code = CODE_FOR_vec_widen_umult_lo_v2si;
  if (HAVE_sync_compare_and_swapsi)
    sync_compare_and_swap[SImode] = CODE_FOR_sync_compare_and_swapsi;
  if (HAVE_sync_compare_and_swapqi)
    sync_compare_and_swap[QImode] = CODE_FOR_sync_compare_and_swapqi;
  if (HAVE_sync_compare_and_swaphi)
    sync_compare_and_swap[HImode] = CODE_FOR_sync_compare_and_swaphi;
  if (HAVE_sync_lock_test_and_setsi)
    sync_lock_test_and_set[SImode] = CODE_FOR_sync_lock_test_and_setsi;
  if (HAVE_sync_lock_test_and_setqi)
    sync_lock_test_and_set[QImode] = CODE_FOR_sync_lock_test_and_setqi;
  if (HAVE_sync_lock_test_and_sethi)
    sync_lock_test_and_set[HImode] = CODE_FOR_sync_lock_test_and_sethi;
  if (HAVE_sync_addsi)
    sync_add_optab[SImode] = CODE_FOR_sync_addsi;
  if (HAVE_sync_subsi)
    sync_sub_optab[SImode] = CODE_FOR_sync_subsi;
  if (HAVE_sync_iorsi)
    sync_ior_optab[SImode] = CODE_FOR_sync_iorsi;
  if (HAVE_sync_xorsi)
    sync_xor_optab[SImode] = CODE_FOR_sync_xorsi;
  if (HAVE_sync_andsi)
    sync_and_optab[SImode] = CODE_FOR_sync_andsi;
  if (HAVE_sync_nandsi)
    sync_nand_optab[SImode] = CODE_FOR_sync_nandsi;
  if (HAVE_sync_addqi)
    sync_add_optab[QImode] = CODE_FOR_sync_addqi;
  if (HAVE_sync_subqi)
    sync_sub_optab[QImode] = CODE_FOR_sync_subqi;
  if (HAVE_sync_iorqi)
    sync_ior_optab[QImode] = CODE_FOR_sync_iorqi;
  if (HAVE_sync_xorqi)
    sync_xor_optab[QImode] = CODE_FOR_sync_xorqi;
  if (HAVE_sync_andqi)
    sync_and_optab[QImode] = CODE_FOR_sync_andqi;
  if (HAVE_sync_addhi)
    sync_add_optab[HImode] = CODE_FOR_sync_addhi;
  if (HAVE_sync_subhi)
    sync_sub_optab[HImode] = CODE_FOR_sync_subhi;
  if (HAVE_sync_iorhi)
    sync_ior_optab[HImode] = CODE_FOR_sync_iorhi;
  if (HAVE_sync_xorhi)
    sync_xor_optab[HImode] = CODE_FOR_sync_xorhi;
  if (HAVE_sync_andhi)
    sync_and_optab[HImode] = CODE_FOR_sync_andhi;
  if (HAVE_sync_nandqi)
    sync_nand_optab[QImode] = CODE_FOR_sync_nandqi;
  if (HAVE_sync_nandhi)
    sync_nand_optab[HImode] = CODE_FOR_sync_nandhi;
  if (HAVE_sync_new_addsi)
    sync_new_add_optab[SImode] = CODE_FOR_sync_new_addsi;
  if (HAVE_sync_new_subsi)
    sync_new_sub_optab[SImode] = CODE_FOR_sync_new_subsi;
  if (HAVE_sync_new_iorsi)
    sync_new_ior_optab[SImode] = CODE_FOR_sync_new_iorsi;
  if (HAVE_sync_new_xorsi)
    sync_new_xor_optab[SImode] = CODE_FOR_sync_new_xorsi;
  if (HAVE_sync_new_andsi)
    sync_new_and_optab[SImode] = CODE_FOR_sync_new_andsi;
  if (HAVE_sync_new_nandsi)
    sync_new_nand_optab[SImode] = CODE_FOR_sync_new_nandsi;
  if (HAVE_sync_new_addqi)
    sync_new_add_optab[QImode] = CODE_FOR_sync_new_addqi;
  if (HAVE_sync_new_subqi)
    sync_new_sub_optab[QImode] = CODE_FOR_sync_new_subqi;
  if (HAVE_sync_new_iorqi)
    sync_new_ior_optab[QImode] = CODE_FOR_sync_new_iorqi;
  if (HAVE_sync_new_xorqi)
    sync_new_xor_optab[QImode] = CODE_FOR_sync_new_xorqi;
  if (HAVE_sync_new_andqi)
    sync_new_and_optab[QImode] = CODE_FOR_sync_new_andqi;
  if (HAVE_sync_new_addhi)
    sync_new_add_optab[HImode] = CODE_FOR_sync_new_addhi;
  if (HAVE_sync_new_subhi)
    sync_new_sub_optab[HImode] = CODE_FOR_sync_new_subhi;
  if (HAVE_sync_new_iorhi)
    sync_new_ior_optab[HImode] = CODE_FOR_sync_new_iorhi;
  if (HAVE_sync_new_xorhi)
    sync_new_xor_optab[HImode] = CODE_FOR_sync_new_xorhi;
  if (HAVE_sync_new_andhi)
    sync_new_and_optab[HImode] = CODE_FOR_sync_new_andhi;
  if (HAVE_sync_new_nandqi)
    sync_new_nand_optab[QImode] = CODE_FOR_sync_new_nandqi;
  if (HAVE_sync_new_nandhi)
    sync_new_nand_optab[HImode] = CODE_FOR_sync_new_nandhi;
  if (HAVE_sync_old_addsi)
    sync_old_add_optab[SImode] = CODE_FOR_sync_old_addsi;
  if (HAVE_sync_old_subsi)
    sync_old_sub_optab[SImode] = CODE_FOR_sync_old_subsi;
  if (HAVE_sync_old_iorsi)
    sync_old_ior_optab[SImode] = CODE_FOR_sync_old_iorsi;
  if (HAVE_sync_old_xorsi)
    sync_old_xor_optab[SImode] = CODE_FOR_sync_old_xorsi;
  if (HAVE_sync_old_andsi)
    sync_old_and_optab[SImode] = CODE_FOR_sync_old_andsi;
  if (HAVE_sync_old_nandsi)
    sync_old_nand_optab[SImode] = CODE_FOR_sync_old_nandsi;
  if (HAVE_sync_old_addqi)
    sync_old_add_optab[QImode] = CODE_FOR_sync_old_addqi;
  if (HAVE_sync_old_subqi)
    sync_old_sub_optab[QImode] = CODE_FOR_sync_old_subqi;
  if (HAVE_sync_old_iorqi)
    sync_old_ior_optab[QImode] = CODE_FOR_sync_old_iorqi;
  if (HAVE_sync_old_xorqi)
    sync_old_xor_optab[QImode] = CODE_FOR_sync_old_xorqi;
  if (HAVE_sync_old_andqi)
    sync_old_and_optab[QImode] = CODE_FOR_sync_old_andqi;
  if (HAVE_sync_old_addhi)
    sync_old_add_optab[HImode] = CODE_FOR_sync_old_addhi;
  if (HAVE_sync_old_subhi)
    sync_old_sub_optab[HImode] = CODE_FOR_sync_old_subhi;
  if (HAVE_sync_old_iorhi)
    sync_old_ior_optab[HImode] = CODE_FOR_sync_old_iorhi;
  if (HAVE_sync_old_xorhi)
    sync_old_xor_optab[HImode] = CODE_FOR_sync_old_xorhi;
  if (HAVE_sync_old_andhi)
    sync_old_and_optab[HImode] = CODE_FOR_sync_old_andhi;
  if (HAVE_sync_old_nandqi)
    sync_old_nand_optab[QImode] = CODE_FOR_sync_old_nandqi;
  if (HAVE_sync_old_nandhi)
    sync_old_nand_optab[HImode] = CODE_FOR_sync_old_nandhi;

#ifdef FIXUNS_TRUNC_LIKE_FIX_TRUNC
  /* This flag says the same insns that convert to a signed fixnum
     also convert validly to an unsigned one.  */
  for (i = 0; i < NUM_MACHINE_MODES; i++)
    for (j = 0; j < NUM_MACHINE_MODES; j++)
      convert_optab_handler (ufixtrunc_optab, i, j)->insn_code
      = convert_optab_handler (sfixtrunc_optab, i, j)->insn_code;
#endif
}
