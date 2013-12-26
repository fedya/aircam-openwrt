/* Generated automatically by the program `genextract'
   from the machine description file `md'.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "insn-config.h"
#include "recog.h"
#include "toplev.h"

/* This variable is used as the "location" of any missing operand
   whose numbers are skipped by a given pattern.  */
static rtx junk ATTRIBUTE_UNUSED;

void
insn_extract (rtx insn)
{
  rtx *ro = recog_data.operand;
  rtx **ro_loc = recog_data.operand_loc;
  rtx pat = PATTERN (insn);
  int i ATTRIBUTE_UNUSED; /* only for peepholes */

#ifdef ENABLE_CHECKING
  memset (ro, 0xab, sizeof (*ro) * MAX_RECOG_OPERANDS);
  memset (ro_loc, 0xab, sizeof (*ro_loc) * MAX_RECOG_OPERANDS);
#endif

  switch (INSN_CODE (insn))
    {
    default:
      /* Control reaches here if insn_extract has been called with an
         unrecognizable insn (code -1), or an insn whose INSN_CODE
         corresponds to a DEFINE_EXPAND in the machine description;
         either way, a bug.  */
      if (INSN_CODE (insn) < 0)
        fatal_insn ("unrecognizable insn:", insn);
      else
        fatal_insn ("insn with invalid code number:", insn);

    case 2005:
    case 2004:
    case 2003:
    case 2002:
    case 2001:
    case 2000:
      for (i = XVECLEN (pat, 0) - 1; i >= 0; i--)
          ro[i] = *(ro_loc[i] = &XVECEXP (pat, 0, i));
      break;

    case 3019:  /* *p vec_setv2di_internal */
    case 3018:  /* *p vec_setv4sf_internal */
    case 3017:  /* *p vec_setv4si_internal */
    case 3016:  /* *p vec_setv8hi_internal */
    case 3015:  /* *p vec_setv16qi_internal */
    case 3014:  /* *p vec_setv2sf_internal */
    case 3013:  /* *p vec_setv2si_internal */
    case 3012:  /* *p vec_setv4hi_internal */
    case 3011:  /* *p vec_setv8qi_internal */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 2));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 3006:  /* *p *thumb2_negsi2_short */
    case 3005:  /* *p *thumb2_one_cmplsi2_short */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (pat, 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 3004:  /* *p *thumb2_mulsi_short */
    case 3003:  /* *p *thumb2_subsi_short */
    case 3000:  /* *p *thumb2_addsi_short */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2999:  /* *p *thumb2_movsi_shortim */
    case 2998:  /* *p *thumb2_movhi_shortim */
    case 2997:  /* *p *thumb2_movqi_shortim */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (pat, 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2996:  /* *p *thumb2_shiftsi3_short */
    case 2995:  /* *p *thumb2_alusi3_short */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2972:  /* *p *cmpdf_trap_vfp */
    case 2971:  /* *p *cmpdf_vfp */
    case 2970:  /* *p *cmpsf_trap_vfp */
    case 2969:  /* *p *cmpsf_vfp */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (pat, 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2954:  /* *p *muldf3negdfsubdf_vfp */
    case 2953:  /* *p *mulsf3negsfsubsf_vfp */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 1));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2952:  /* *p *fmuldf3negdfadddf_vfp */
    case 2951:  /* *p *mulsf3negsfaddsf_vfp */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2950:  /* *p *muldf3subdf_vfp */
    case 2949:  /* *p *mulsf3subsf_vfp */
    case 2948:  /* *p *muldf3adddf_vfp */
    case 2947:  /* *p *mulsf3addsf_vfp */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 1));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2921:  /* *p iwmmxt_tmcr */
      ro[0] = *(ro_loc[0] = &XVECEXP (XEXP (pat, 1), 0, 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (pat, 1), 0, 1));
      ro[2] = *(ro_loc[2] = &XEXP (pat, 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2919:  /* *p iwmmxt_walign */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2909:  /* *p iwmmxt_tmiatt */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 1), 0), 0), 0));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2908:  /* *p iwmmxt_tmiabt */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 1), 0), 0), 0));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2907:  /* *p iwmmxt_tmiatb */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 1), 0), 0));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2906:  /* *p iwmmxt_tmiabb */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 1), 0), 0));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2905:  /* *p iwmmxt_tmiaph */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0), 1), 0), 0));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 1), 0), 0), 0), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 1), 1), 0), 0), 0);
      recog_data.dup_num[1] = 3;
      break;

    case 2904:  /* *p iwmmxt_tmia */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2841:  /* *p gtv2si3 */
    case 2840:  /* *p gtv4hi3 */
    case 2839:  /* *p gtv8qi3 */
    case 2838:  /* *p gtuv2si3 */
    case 2837:  /* *p gtuv4hi3 */
    case 2836:  /* *p gtuv8qi3 */
    case 2835:  /* *p eqv2si3 */
    case 2834:  /* *p eqv4hi3 */
    case 2833:  /* *p eqv8qi3 */
      ro[0] = *(ro_loc[0] = &XVECEXP (XEXP (pat, 1), 0, 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (pat, 1), 0, 1));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (pat, 1), 0, 2));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 3028:  /* *p vec_extractv2di */
    case 3027:  /* *p vec_extractv4sf */
    case 3026:  /* *p vec_extractv4si */
    case 3025:  /* *p vec_extractv8hi */
    case 3024:  /* *p vec_extractv16qi */
    case 3023:  /* *p vec_extractv2sf */
    case 3022:  /* *p vec_extractv2si */
    case 3021:  /* *p vec_extractv4hi */
    case 3020:  /* *p vec_extractv8qi */
    case 2831:  /* *p iwmmxt_textrmw */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0, 0));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 3044:  /* *p neon_vget_lanev4sf_zext_internal */
    case 3043:  /* *p neon_vget_lanev4si_zext_internal */
    case 3042:  /* *p neon_vget_lanev8hi_zext_internal */
    case 3041:  /* *p neon_vget_lanev16qi_zext_internal */
    case 3040:  /* *p neon_vget_lanev4sf_sext_internal */
    case 3039:  /* *p neon_vget_lanev4si_sext_internal */
    case 3038:  /* *p neon_vget_lanev8hi_sext_internal */
    case 3037:  /* *p neon_vget_lanev16qi_sext_internal */
    case 3036:  /* *p neon_vget_lanev2sf_zext_internal */
    case 3035:  /* *p neon_vget_lanev2si_zext_internal */
    case 3034:  /* *p neon_vget_lanev4hi_zext_internal */
    case 3033:  /* *p neon_vget_lanev8qi_zext_internal */
    case 3032:  /* *p neon_vget_lanev2sf_sext_internal */
    case 3031:  /* *p neon_vget_lanev2si_sext_internal */
    case 3030:  /* *p neon_vget_lanev4hi_sext_internal */
    case 3029:  /* *p neon_vget_lanev8qi_sext_internal */
    case 2830:  /* *p iwmmxt_textrmsh */
    case 2829:  /* *p iwmmxt_textrmuh */
    case 2828:  /* *p iwmmxt_textrmsb */
    case 2827:  /* *p iwmmxt_textrmub */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 1), 0, 0));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2826:  /* *p iwmmxt_tinsrw */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 2));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2825:  /* *p iwmmxt_tinsrh */
    case 2824:  /* *p iwmmxt_tinsrb */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 2));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2823:  /* *p iwmmxt_psadbw */
    case 2822:  /* *p iwmmxt_uavgv4hi3 */
    case 2821:  /* *p iwmmxt_uavgv8qi3 */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2820:  /* *p iwmmxt_uavgrndv4hi3 */
    case 2819:  /* *p iwmmxt_uavgrndv8qi3 */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2925:  /* *p iwmmxt_wsadhz */
    case 2924:  /* *p iwmmxt_wsadbz */
    case 2923:  /* *p iwmmxt_wsadh */
    case 2922:  /* *p iwmmxt_wsadb */
    case 2903:  /* *p iwmmxt_wmaddu */
    case 2902:  /* *p iwmmxt_wmadds */
    case 2832:  /* *p iwmmxt_wshufh */
    case 2814:  /* *p iwmmxt_wmacuz */
    case 2812:  /* *p iwmmxt_wmacsz */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (XEXP (pat, 1), 1), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XEXP (pat, 1), 1), 0, 1));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2813:  /* *p iwmmxt_wmacu */
    case 2811:  /* *p iwmmxt_wmacs */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (XEXP (pat, 1), 1), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XEXP (pat, 1), 1), 0, 1));
      ro[3] = *(ro_loc[3] = &XVECEXP (XEXP (XEXP (pat, 1), 1), 0, 2));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2810:  /* *p umulv4hi3_highpart */
    case 2809:  /* *p smulv4hi3_highpart */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2962:  /* *p fixuns_truncdfsi2 */
    case 2961:  /* *p fixuns_truncsfsi2 */
    case 2960:  /* *p *truncsidf2_vfp */
    case 2959:  /* *p *truncsisf2_vfp */
    case 2877:  /* *p iwmmxt_wunpckelsw */
    case 2876:  /* *p iwmmxt_wunpckelsh */
    case 2875:  /* *p iwmmxt_wunpckelsb */
    case 2874:  /* *p iwmmxt_wunpckeluw */
    case 2873:  /* *p iwmmxt_wunpckeluh */
    case 2872:  /* *p iwmmxt_wunpckelub */
    case 2871:  /* *p iwmmxt_wunpckehsw */
    case 2870:  /* *p iwmmxt_wunpckehsh */
    case 2869:  /* *p iwmmxt_wunpckehsb */
    case 2868:  /* *p iwmmxt_wunpckehuw */
    case 2867:  /* *p iwmmxt_wunpckehuh */
    case 2866:  /* *p iwmmxt_wunpckehub */
    case 2776:  /* *p *fix_truncdfsi2_fpa */
    case 2775:  /* *p *fix_truncsfsi2_fpa */
    case 2772:  /* *p *sqrtdf_esfdf_fpa */
    case 2769:  /* *p *absdf_esfdf_fpa */
    case 2766:  /* *p *negdf_esfdf_fpa */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (pat, 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2786:  /* *p iwmmxt_nanddi3 */
    case 2762:  /* *p *moddf_df_esfdf_fpa */
    case 2757:  /* *p *divdf_df_esfdf_fpa */
    case 2752:  /* *p *muldf_df_esfdf_fpa */
    case 2747:  /* *p *subdf_df_esfdf_fpa */
    case 2743:  /* *p *adddf_df_esfdf_fpa */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2738:  /* *p *arm_movtas_ze */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (pat, 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2818:  /* *p *iwmmxt_clrv2si */
    case 2817:  /* *p *iwmmxt_clrv4hi */
    case 2816:  /* *p *iwmmxt_clrv8qi */
    case 2815:  /* *p iwmmxt_clrdi */
    case 2737:  /* *p load_tp_hard */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (pat, 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2920:  /* *p iwmmxt_tmrc */
    case 2918:  /* *p iwmmxt_waccw */
    case 2917:  /* *p iwmmxt_wacch */
    case 2916:  /* *p iwmmxt_waccb */
    case 2915:  /* *p iwmmxt_tmovmskw */
    case 2914:  /* *p iwmmxt_tmovmskh */
    case 2913:  /* *p iwmmxt_tmovmskb */
    case 2736:  /* *p rbitsi2 */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (XEXP (pat, 1), 1), 0, 0));
      ro[2] = *(ro_loc[2] = &XEXP (pat, 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2734:  /* *p *arith_adjacentmem */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 0));
      ro[5] = *(ro_loc[5] = &XEXP (pat, 0));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2979:  /* *p *thumb2_sub_shiftsi */
    case 2733:  /* *p *sub_shiftsi */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 1));
      ro[5] = *(ro_loc[5] = &XEXP (pat, 0));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2978:  /* *p *thumb2_arith_shiftsi */
    case 2732:  /* *p *arith_shiftsi */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 1));
      ro[6] = *(ro_loc[6] = &XEXP (pat, 0));
      ro[7] = *(ro_loc[7] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2731:  /* *p *load_indirect_jump */
    case 2730:  /* *p *arm_indirect_jump */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 1));
      ro[1] = *(ro_loc[1] = &XEXP (pat, 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2729:  /* *p *arm_return */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2728:  /* *p *arm_jump */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (pat, 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2727:  /* *p *stmsi2 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 1));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 0), 0), 0);
      recog_data.dup_num[0] = 1;
      break;

    case 2726:  /* *p *stmsi3 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 1));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 2), 1));
      ro[5] = *(ro_loc[5] = &XEXP (pat, 0));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (pat, 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 2), 0), 0), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 0), 0), 0);
      recog_data.dup_num[1] = 1;
      break;

    case 2725:  /* *p *stmsi4 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 1));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 2), 1));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 3), 1));
      ro[6] = *(ro_loc[6] = &XEXP (pat, 0));
      ro[7] = *(ro_loc[7] = &XEXP (XEXP (pat, 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 3), 0), 0), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 2), 0), 0), 0);
      recog_data.dup_num[1] = 1;
      recog_data.dup_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 0), 0), 0);
      recog_data.dup_num[2] = 1;
      break;

    case 2724:  /* *p *stmsi_postinc2 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 1));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 2), 1));
      ro[5] = *(ro_loc[5] = &XEXP (pat, 0));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (pat, 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 2), 0), 0), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 0), 0);
      recog_data.dup_num[1] = 2;
      break;

    case 2723:  /* *p *stmsi_postinc3 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 1));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 2), 1));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 3), 1));
      ro[6] = *(ro_loc[6] = &XEXP (pat, 0));
      ro[7] = *(ro_loc[7] = &XEXP (XEXP (pat, 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 3), 0), 0), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 2), 0), 0), 0);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 0), 0);
      recog_data.dup_num[2] = 2;
      break;

    case 2722:  /* *p *stmsi_postinc4 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 1));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 2), 1));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 3), 1));
      ro[6] = *(ro_loc[6] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 4), 1));
      ro[7] = *(ro_loc[7] = &XEXP (pat, 0));
      ro[8] = *(ro_loc[8] = &XEXP (XEXP (pat, 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 4), 0), 0), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 3), 0), 0), 0);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 2), 0), 0), 0);
      recog_data.dup_num[2] = 2;
      recog_data.dup_loc[3] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 0), 0);
      recog_data.dup_num[3] = 2;
      break;

    case 2721:  /* *p *ldmsi2 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 1));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 1), 0), 0);
      recog_data.dup_num[0] = 1;
      break;

    case 2720:  /* *p *ldmsi3 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 1));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 2), 0));
      ro[5] = *(ro_loc[5] = &XEXP (pat, 0));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (pat, 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 2), 1), 0), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 1), 0), 0);
      recog_data.dup_num[1] = 1;
      break;

    case 2719:  /* *p *ldmsi4 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 1));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 2), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 3), 0));
      ro[6] = *(ro_loc[6] = &XEXP (pat, 0));
      ro[7] = *(ro_loc[7] = &XEXP (XEXP (pat, 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 3), 1), 0), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 2), 1), 0), 0);
      recog_data.dup_num[1] = 1;
      recog_data.dup_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 1), 0), 0);
      recog_data.dup_num[2] = 1;
      break;

    case 2718:  /* *p *ldmsi_postinc2 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 1));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 2), 0));
      ro[5] = *(ro_loc[5] = &XEXP (pat, 0));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (pat, 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 2), 1), 0), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 1), 0);
      recog_data.dup_num[1] = 2;
      break;

    case 2717:  /* *p *ldmsi_postinc3 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 1));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 2), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 3), 0));
      ro[6] = *(ro_loc[6] = &XEXP (pat, 0));
      ro[7] = *(ro_loc[7] = &XEXP (XEXP (pat, 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 3), 1), 0), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 2), 1), 0), 0);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 1), 0);
      recog_data.dup_num[2] = 2;
      break;

    case 2716:  /* *p *ldmsi_postinc4 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 1));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 2), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 3), 0));
      ro[6] = *(ro_loc[6] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 4), 0));
      ro[7] = *(ro_loc[7] = &XEXP (pat, 0));
      ro[8] = *(ro_loc[8] = &XEXP (XEXP (pat, 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 4), 1), 0), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 3), 1), 0), 0);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 2), 1), 0), 0);
      recog_data.dup_num[2] = 2;
      recog_data.dup_loc[3] = &XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 1), 0);
      recog_data.dup_num[3] = 2;
      break;

    case 2710:  /* *p tls_load_dot_plus_eight */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0, 2));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2709:  /* *p pic_add_dot_plus_eight */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (XEXP (pat, 1), 1), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XEXP (pat, 1), 1), 0, 2));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2687:  /* *p extzv_t2 */
    case 2686:  /* *p extv */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 2));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2974:  /* *p *thumb2_notsi_shiftsi */
    case 2685:  /* *p *arm_notsi_shiftsi */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2684:  /* *p *arm_shiftsi3 */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 3053:  /* *p neon_vdup_nv2di */
    case 3052:  /* *p neon_vdup_nv4sf */
    case 3051:  /* *p neon_vdup_nv4si */
    case 3050:  /* *p neon_vdup_nv2sf */
    case 3049:  /* *p neon_vdup_nv2si */
    case 3048:  /* *p neon_vdup_nv8hi */
    case 3047:  /* *p neon_vdup_nv16qi */
    case 3046:  /* *p neon_vdup_nv4hi */
    case 3045:  /* *p neon_vdup_nv8qi */
    case 2994:  /* *p *thumb2_zero_extendqisi2_v6 */
    case 2993:  /* *p *thumb2_zero_extendhisi2_v6 */
    case 2992:  /* *p *thumb2_extendqisi_v6 */
    case 2990:  /* *p *thumb2_extendqidi2 */
    case 2988:  /* *p *thumb2_extendhidi2 */
    case 2986:  /* *p *thumb2_extendsidi2 */
    case 2984:  /* *p *thumb2_zero_extendqidi2 */
    case 2982:  /* *p *thumb2_zero_extendhidi2 */
    case 2980:  /* *p *thumb2_zero_extendsidi2 */
    case 2968:  /* *p *sqrtdf2_vfp */
    case 2967:  /* *p *sqrtsf2_vfp */
    case 2966:  /* *p floatunssidf2 */
    case 2965:  /* *p floatunssisf2 */
    case 2964:  /* *p *floatsidf2_vfp */
    case 2963:  /* *p *floatsisf2_vfp */
    case 2958:  /* *p truncsfhf2 */
    case 2957:  /* *p extendhfsf2 */
    case 2956:  /* *p *truncdfsf2_vfp */
    case 2955:  /* *p *extendsfdf2_vfp */
    case 2935:  /* *p *negdf2_vfp */
    case 2934:  /* *p *negsf2_vfp */
    case 2933:  /* *p *absdf2_vfp */
    case 2932:  /* *p *abssf2_vfp */
    case 2912:  /* *p iwmmxt_tbcstsi */
    case 2911:  /* *p iwmmxt_tbcsthi */
    case 2910:  /* *p iwmmxt_tbcstqi */
    case 2778:  /* *p *extendsfdf2_fpa */
    case 2777:  /* *p *truncdfsf2_fpa */
    case 2774:  /* *p *floatsidf2_fpa */
    case 2773:  /* *p *floatsisf2_fpa */
    case 2771:  /* *p *sqrtdf2_fpa */
    case 2770:  /* *p *sqrtsf2_fpa */
    case 2768:  /* *p *absdf2_fpa */
    case 2767:  /* *p *abssf2_fpa */
    case 2765:  /* *p *negdf2_fpa */
    case 2764:  /* *p *negsf2_fpa */
    case 2739:  /* *p *arm_rev */
    case 2735:  /* *p clzsi2 */
    case 2705:  /* *p *arm_extendqisi_v6 */
    case 2704:  /* *p *arm_extendqisi */
    case 2703:  /* *p *arm_extendqihi_insn */
    case 2702:  /* *p *arm_extendhisi2_v6 */
    case 2701:  /* *p *arm_extendhisi2 */
    case 2699:  /* *p *arm_zero_extendqisi2_v6 */
    case 2698:  /* *p *arm_zero_extendqisi2 */
    case 2696:  /* *p *arm_zero_extendhisi2_v6 */
    case 2695:  /* *p *arm_zero_extendhisi2 */
    case 2694:  /* *p *arm_extendsidi2 */
    case 2693:  /* *p *arm_zero_extendqidi2 */
    case 2692:  /* *p *arm_zero_extendsidi2 */
    case 2691:  /* *p *arm_one_cmplsi2 */
    case 2689:  /* *p one_cmpldi2 */
    case 2688:  /* *p *arm_negsi2 */
    case 2683:  /* *p *smin_0 */
    case 2682:  /* *p *smax_m1 */
    case 2681:  /* *p *smax_0 */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (pat, 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2680:  /* *p *andsi_iorsi3_notsi */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 3008:  /* *p *thumb_orsi_not_shiftsi_si */
    case 2973:  /* *p *thumb_andsi_not_shiftsi_si */
    case 2670:  /* *p andsi_not_shiftsi_si */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0));
      ro[5] = *(ro_loc[5] = &XEXP (pat, 0));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 3007:  /* *p orsi_notsi_si */
    case 2678:  /* *p *xordi_sesidi_di */
    case 2677:  /* *p *xordi_zesidi_di */
    case 2673:  /* *p *iordi_sesidi_di */
    case 2672:  /* *p *iordi_zesidi_di */
    case 2669:  /* *p andsi_notsi_si */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2667:  /* *p *anddi_notsesidi_di */
    case 2665:  /* *p *anddi_notzesidi_di */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2946:  /* *p *muldf3negdf_vfp */
    case 2945:  /* *p *mulsf3negsf_vfp */
    case 2761:  /* *p *moddf_esfdf_df_fpa */
    case 2756:  /* *p *divdf_esfdf_df_fpa */
    case 2751:  /* *p *muldf_esfdf_df_fpa */
    case 2746:  /* *p *subdf_esfdf_df_fpa */
    case 2742:  /* *p *adddf_esfdf_df_fpa */
    case 2706:  /* *p *arm_extendqisi2addsi */
    case 2700:  /* *p *arm_zero_extendqisi2addsi */
    case 2697:  /* *p *arm_zero_extendhisi2addsi */
    case 2663:  /* *p *anddi_notdi_di */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2662:  /* *p insv_t2 */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 2));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2661:  /* *p insv_zero */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 2));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2658:  /* *p *maddhidi4 */
    case 2657:  /* *p maddhisi4 */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2652:  /* *p *umulsi3_highpart_v6 */
    case 2651:  /* *p *umulsi3_highpart_nov6 */
    case 2650:  /* *p *smulsi3_highpart_v6 */
    case 2649:  /* *p *smulsi3_highpart_nov6 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1), 0), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1), 0), 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2865:  /* *p iwmmxt_wunpckilw */
    case 2864:  /* *p iwmmxt_wunpckilh */
    case 2863:  /* *p iwmmxt_wunpckilb */
    case 2862:  /* *p iwmmxt_wunpckihw */
    case 2861:  /* *p iwmmxt_wunpckihh */
    case 2860:  /* *p iwmmxt_wunpckihb */
    case 2859:  /* *p iwmmxt_wpackdus */
    case 2858:  /* *p iwmmxt_wpackwus */
    case 2857:  /* *p iwmmxt_wpackhus */
    case 2856:  /* *p iwmmxt_wpackdss */
    case 2855:  /* *p iwmmxt_wpackwss */
    case 2854:  /* *p iwmmxt_wpackhss */
    case 2763:  /* *p *moddf_esfdf_esfdf_fpa */
    case 2758:  /* *p *divdf_esfdf_esfdf_fpa */
    case 2753:  /* *p *muldf_esfdf_esfdf_fpa */
    case 2748:  /* *p *subdf_esfdf_esfdf_fpa */
    case 2744:  /* *p *adddf_esfdf_esfdf_fpa */
    case 2656:  /* *p *mulhisi3tt */
    case 2655:  /* *p *mulhisi3bt */
    case 2654:  /* *p *mulhisi3tb */
    case 2653:  /* *p mulhisi3 */
    case 2646:  /* *p *umulsidi3_v6 */
    case 2645:  /* *p *umulsidi3_nov6 */
    case 2644:  /* *p *mulsidi3_v6 */
    case 2643:  /* *p *mulsidi3_nov6 */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2648:  /* *p *umulsidi3adddi_v6 */
    case 2647:  /* *p *umulsidi3adddi */
    case 2642:  /* *p *mulsidi3adddi_v6 */
    case 2641:  /* *p *mulsidi3adddi */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2640:  /* *p *mulsi3subsi */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2639:  /* *p *mulsi3addsi_v6 */
    case 2638:  /* *p *mulsi3addsi */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (pat, 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 2636:  /* *p *arm_mulsi3 */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 3009:  /* *p *thumb2_iorsi3 */
    case 3002:  /* *p udivsi3 */
    case 3001:  /* *p divsi3 */
    case 2944:  /* *p *muldf3_vfp */
    case 2943:  /* *p *mulsf3_vfp */
    case 2942:  /* *p *divdf3_vfp */
    case 2941:  /* *p *divsf3_vfp */
    case 2940:  /* *p *subdf3_vfp */
    case 2939:  /* *p *subsf3_vfp */
    case 2938:  /* *p *adddf3_vfp */
    case 2937:  /* *p *addsf3_vfp */
    case 2901:  /* *p ashldi3_di */
    case 2900:  /* *p ashlv2si3_di */
    case 2899:  /* *p ashlv4hi3_di */
    case 2898:  /* *p lshrdi3_di */
    case 2897:  /* *p lshrv2si3_di */
    case 2896:  /* *p lshrv4hi3_di */
    case 2895:  /* *p ashrdi3_di */
    case 2894:  /* *p ashrv2si3_di */
    case 2893:  /* *p ashrv4hi3_di */
    case 2892:  /* *p rordi3_di */
    case 2891:  /* *p rorv2si3_di */
    case 2890:  /* *p rorv4hi3_di */
    case 2889:  /* *p ashldi3_iwmmxt */
    case 2888:  /* *p ashlv2si3_iwmmxt */
    case 2887:  /* *p ashlv4hi3_iwmmxt */
    case 2886:  /* *p lshrdi3_iwmmxt */
    case 2885:  /* *p lshrv2si3_iwmmxt */
    case 2884:  /* *p lshrv4hi3_iwmmxt */
    case 2883:  /* *p ashrdi3_iwmmxt */
    case 2882:  /* *p ashrv2si3_iwmmxt */
    case 2881:  /* *p ashrv4hi3_iwmmxt */
    case 2880:  /* *p rordi3 */
    case 2879:  /* *p rorv2si3 */
    case 2878:  /* *p rorv4hi3 */
    case 2853:  /* *p *uminv8qi3_iwmmxt */
    case 2852:  /* *p *uminv4hi3_iwmmxt */
    case 2851:  /* *p *uminv2si3_iwmmxt */
    case 2850:  /* *p *sminv8qi3_iwmmxt */
    case 2849:  /* *p *sminv4hi3_iwmmxt */
    case 2848:  /* *p *sminv2si3_iwmmxt */
    case 2847:  /* *p *umaxv8qi3_iwmmxt */
    case 2846:  /* *p *umaxv4hi3_iwmmxt */
    case 2845:  /* *p *umaxv2si3_iwmmxt */
    case 2844:  /* *p *smaxv8qi3_iwmmxt */
    case 2843:  /* *p *smaxv4hi3_iwmmxt */
    case 2842:  /* *p *smaxv2si3_iwmmxt */
    case 2808:  /* *p *mulv4hi3_iwmmxt */
    case 2807:  /* *p ussubv2si3 */
    case 2806:  /* *p ussubv4hi3 */
    case 2805:  /* *p ussubv8qi3 */
    case 2804:  /* *p sssubv2si3 */
    case 2803:  /* *p sssubv4hi3 */
    case 2802:  /* *p sssubv8qi3 */
    case 2801:  /* *p *subv8qi3_iwmmxt */
    case 2800:  /* *p *subv4hi3_iwmmxt */
    case 2799:  /* *p *subv2si3_iwmmxt */
    case 2798:  /* *p usaddv2si3 */
    case 2797:  /* *p usaddv4hi3 */
    case 2796:  /* *p usaddv8qi3 */
    case 2795:  /* *p ssaddv2si3 */
    case 2794:  /* *p ssaddv4hi3 */
    case 2793:  /* *p ssaddv8qi3 */
    case 2792:  /* *p *addv8qi3_iwmmxt */
    case 2791:  /* *p *addv4hi3_iwmmxt */
    case 2790:  /* *p *addv2si3_iwmmxt */
    case 2785:  /* *p iwmmxt_anddi3 */
    case 2784:  /* *p iwmmxt_xordi3 */
    case 2783:  /* *p iwmmxt_iordi3 */
    case 2760:  /* *p *moddf3_fpa */
    case 2759:  /* *p *modsf3_fpa */
    case 2755:  /* *p *divdf3_fpa */
    case 2754:  /* *p *divsf3_fpa */
    case 2750:  /* *p *muldf3_fpa */
    case 2749:  /* *p *mulsf3_fpa */
    case 2745:  /* *p *subdf3_fpa */
    case 2741:  /* *p *adddf3_fpa */
    case 2740:  /* *p *addsf3_fpa */
    case 2707:  /* *p *arm_movt */
    case 2679:  /* *p *arm_xorsi3 */
    case 2676:  /* *p *xordi3_insn */
    case 2674:  /* *p *arm_iorsi3 */
    case 2671:  /* *p *iordi3_insn */
    case 2659:  /* *p *arm_andsi3_insn */
    case 2637:  /* *p *arm_mulsi3_v6 */
    case 2634:  /* *p *arm_subsi3_insn */
    case 2632:  /* *p *arm_addsi3 */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 1776:  /* *memory_barrier */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      recog_data.dup_loc[0] = &XVECEXP (XEXP (pat, 1), 0, 0);
      recog_data.dup_num[0] = 0;
      break;

    case 1775:  /* arm_sync_old_nandhi */
    case 1774:  /* arm_sync_old_nandqi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 3), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 4), 0));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 1);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0);
      recog_data.dup_num[2] = 1;
      break;

    case 1773:  /* arm_sync_old_andhi */
    case 1772:  /* arm_sync_old_xorhi */
    case 1771:  /* arm_sync_old_iorhi */
    case 1770:  /* arm_sync_old_subhi */
    case 1769:  /* arm_sync_old_addhi */
    case 1768:  /* arm_sync_old_andqi */
    case 1767:  /* arm_sync_old_xorqi */
    case 1766:  /* arm_sync_old_iorqi */
    case 1765:  /* arm_sync_old_subqi */
    case 1764:  /* arm_sync_old_addqi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 3), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 4), 0));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 1);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0);
      recog_data.dup_num[2] = 1;
      break;

    case 1763:  /* arm_sync_old_nandsi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 3), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 4), 0));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 1);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0);
      recog_data.dup_num[2] = 1;
      break;

    case 1762:  /* arm_sync_old_andsi */
    case 1761:  /* arm_sync_old_xorsi */
    case 1760:  /* arm_sync_old_iorsi */
    case 1759:  /* arm_sync_old_subsi */
    case 1758:  /* arm_sync_old_addsi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 3), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 4), 0));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 1);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0);
      recog_data.dup_num[2] = 1;
      break;

    case 1757:  /* arm_sync_new_nandhi */
    case 1756:  /* arm_sync_new_nandqi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 3), 0));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 1);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0);
      recog_data.dup_num[2] = 1;
      break;

    case 1755:  /* arm_sync_new_andhi */
    case 1754:  /* arm_sync_new_xorhi */
    case 1753:  /* arm_sync_new_iorhi */
    case 1752:  /* arm_sync_new_subhi */
    case 1751:  /* arm_sync_new_addhi */
    case 1750:  /* arm_sync_new_andqi */
    case 1749:  /* arm_sync_new_xorqi */
    case 1748:  /* arm_sync_new_iorqi */
    case 1747:  /* arm_sync_new_subqi */
    case 1746:  /* arm_sync_new_addqi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 3), 0));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 1);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0);
      recog_data.dup_num[2] = 1;
      break;

    case 1745:  /* arm_sync_new_nandsi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 3), 0));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 1);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0);
      recog_data.dup_num[2] = 1;
      break;

    case 1744:  /* arm_sync_new_andsi */
    case 1743:  /* arm_sync_new_xorsi */
    case 1742:  /* arm_sync_new_iorsi */
    case 1741:  /* arm_sync_new_subsi */
    case 1740:  /* arm_sync_new_addsi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 3), 0));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 1);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0);
      recog_data.dup_num[2] = 1;
      break;

    case 1739:  /* arm_sync_lock_test_and_sethi */
    case 1738:  /* arm_sync_lock_test_and_setqi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 3), 0));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0);
      recog_data.dup_num[0] = 1;
      break;

    case 1737:  /* arm_sync_lock_test_and_setsi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 0), 1));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 3), 0));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0);
      recog_data.dup_num[0] = 1;
      break;

    case 1736:  /* arm_sync_compare_and_swaphi */
    case 1735:  /* arm_sync_compare_and_swapqi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0, 1));
      ro[3] = *(ro_loc[3] = &XVECEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0, 2));
      recog_data.dup_loc[0] = &XVECEXP (XEXP (XVECEXP (pat, 0, 2), 1), 0, 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XVECEXP (pat, 0, 1), 0);
      recog_data.dup_num[1] = 1;
      recog_data.dup_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0);
      recog_data.dup_num[2] = 2;
      break;

    case 1734:  /* arm_sync_compare_and_swapsi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 1));
      ro[3] = *(ro_loc[3] = &XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 2));
      recog_data.dup_loc[0] = &XVECEXP (XEXP (XVECEXP (pat, 0, 2), 1), 0, 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XVECEXP (pat, 0, 1), 0);
      recog_data.dup_num[1] = 1;
      recog_data.dup_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0);
      recog_data.dup_num[2] = 2;
      break;

    case 1721:  /* neon_vec_umult_hi_v4si */
    case 1720:  /* neon_vec_smult_hi_v4si */
    case 1719:  /* neon_vec_umult_hi_v8hi */
    case 1718:  /* neon_vec_smult_hi_v8hi */
    case 1717:  /* neon_vec_umult_hi_v16qi */
    case 1716:  /* neon_vec_smult_hi_v16qi */
    case 1715:  /* neon_vec_umult_lo_v4si */
    case 1714:  /* neon_vec_smult_lo_v4si */
    case 1713:  /* neon_vec_umult_lo_v8hi */
    case 1712:  /* neon_vec_smult_lo_v8hi */
    case 1711:  /* neon_vec_umult_lo_v16qi */
    case 1710:  /* neon_vec_smult_lo_v16qi */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 1);
      recog_data.dup_num[0] = 2;
      break;

    case 1690:  /* neon_vst4qbv4sf */
    case 1689:  /* neon_vst4qbv4si */
    case 1688:  /* neon_vst4qbv8hi */
    case 1687:  /* neon_vst4qbv16qi */
    case 1686:  /* neon_vst4qav4sf */
    case 1685:  /* neon_vst4qav4si */
    case 1684:  /* neon_vst4qav8hi */
    case 1683:  /* neon_vst4qav16qi */
    case 1645:  /* neon_vst3qbv4sf */
    case 1644:  /* neon_vst3qbv4si */
    case 1643:  /* neon_vst3qbv8hi */
    case 1642:  /* neon_vst3qbv16qi */
    case 1641:  /* neon_vst3qav4sf */
    case 1640:  /* neon_vst3qav4si */
    case 1639:  /* neon_vst3qav8hi */
    case 1638:  /* neon_vst3qav16qi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[0] = 1;
      break;

    case 1665:  /* neon_vld4qbv4sf */
    case 1664:  /* neon_vld4qbv4si */
    case 1663:  /* neon_vld4qbv8hi */
    case 1662:  /* neon_vld4qbv16qi */
    case 1661:  /* neon_vld4qav4sf */
    case 1660:  /* neon_vld4qav4si */
    case 1659:  /* neon_vld4qav8hi */
    case 1658:  /* neon_vld4qav16qi */
    case 1620:  /* neon_vld3qbv4sf */
    case 1619:  /* neon_vld3qbv4si */
    case 1618:  /* neon_vld3qbv8hi */
    case 1617:  /* neon_vld3qbv16qi */
    case 1616:  /* neon_vld3qav4sf */
    case 1615:  /* neon_vld3qav4si */
    case 1614:  /* neon_vld3qav8hi */
    case 1613:  /* neon_vld3qav16qi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 1));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[0] = 3;
      break;

    case 1697:  /* neon_vst4_lanev4sf */
    case 1696:  /* neon_vst4_lanev4si */
    case 1695:  /* neon_vst4_lanev8hi */
    case 1694:  /* neon_vst4_lanev2sf */
    case 1693:  /* neon_vst4_lanev2si */
    case 1692:  /* neon_vst4_lanev4hi */
    case 1691:  /* neon_vst4_lanev8qi */
    case 1652:  /* neon_vst3_lanev4sf */
    case 1651:  /* neon_vst3_lanev4si */
    case 1650:  /* neon_vst3_lanev8hi */
    case 1649:  /* neon_vst3_lanev2sf */
    case 1648:  /* neon_vst3_lanev2si */
    case 1647:  /* neon_vst3_lanev4hi */
    case 1646:  /* neon_vst3_lanev8qi */
    case 1607:  /* neon_vst2_lanev4sf */
    case 1606:  /* neon_vst2_lanev4si */
    case 1605:  /* neon_vst2_lanev8hi */
    case 1604:  /* neon_vst2_lanev2sf */
    case 1603:  /* neon_vst2_lanev2si */
    case 1602:  /* neon_vst2_lanev4hi */
    case 1601:  /* neon_vst2_lanev8qi */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 0), 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (pat, 1), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (pat, 1), 0, 1));
      break;

    case 1570:  /* neon_vst1_lanev2di */
    case 1569:  /* neon_vst1_lanev4sf */
    case 1568:  /* neon_vst1_lanev4si */
    case 1567:  /* neon_vst1_lanev8hi */
    case 1566:  /* neon_vst1_lanev16qi */
    case 1565:  /* neon_vst1_lanedi */
    case 1564:  /* neon_vst1_lanev2sf */
    case 1563:  /* neon_vst1_lanev2si */
    case 1562:  /* neon_vst1_lanev4hi */
    case 1561:  /* neon_vst1_lanev8qi */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XEXP (pat, 1), 1), 0, 0));
      break;

    case 1682:  /* neon_vst4di */
    case 1681:  /* neon_vst4v2sf */
    case 1680:  /* neon_vst4v2si */
    case 1679:  /* neon_vst4v4hi */
    case 1678:  /* neon_vst4v8qi */
    case 1637:  /* neon_vst3di */
    case 1636:  /* neon_vst3v2sf */
    case 1635:  /* neon_vst3v2si */
    case 1634:  /* neon_vst3v4hi */
    case 1633:  /* neon_vst3v8qi */
    case 1600:  /* neon_vst2v4sf */
    case 1599:  /* neon_vst2v4si */
    case 1598:  /* neon_vst2v8hi */
    case 1597:  /* neon_vst2v16qi */
    case 1596:  /* neon_vst2di */
    case 1595:  /* neon_vst2v2sf */
    case 1594:  /* neon_vst2v2si */
    case 1593:  /* neon_vst2v4hi */
    case 1592:  /* neon_vst2v8qi */
    case 1560:  /* neon_vst1v2di */
    case 1559:  /* neon_vst1di */
    case 1558:  /* neon_vst1v4sf */
    case 1557:  /* neon_vst1v2sf */
    case 1556:  /* neon_vst1v4si */
    case 1555:  /* neon_vst1v2si */
    case 1554:  /* neon_vst1v8hi */
    case 1553:  /* neon_vst1v4hi */
    case 1552:  /* neon_vst1v16qi */
    case 1551:  /* neon_vst1v8qi */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 0), 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (pat, 1), 0, 0));
      break;

    case 1672:  /* neon_vld4_lanev4sf */
    case 1671:  /* neon_vld4_lanev4si */
    case 1670:  /* neon_vld4_lanev8hi */
    case 1669:  /* neon_vld4_lanev2sf */
    case 1668:  /* neon_vld4_lanev2si */
    case 1667:  /* neon_vld4_lanev4hi */
    case 1666:  /* neon_vld4_lanev8qi */
    case 1627:  /* neon_vld3_lanev4sf */
    case 1626:  /* neon_vld3_lanev4si */
    case 1625:  /* neon_vld3_lanev8hi */
    case 1624:  /* neon_vld3_lanev2sf */
    case 1623:  /* neon_vld3_lanev2si */
    case 1622:  /* neon_vld3_lanev4hi */
    case 1621:  /* neon_vld3_lanev8qi */
    case 1586:  /* neon_vld2_lanev4sf */
    case 1585:  /* neon_vld2_lanev4si */
    case 1584:  /* neon_vld2_lanev8hi */
    case 1583:  /* neon_vld2_lanev2sf */
    case 1582:  /* neon_vld2_lanev2si */
    case 1581:  /* neon_vld2_lanev4hi */
    case 1580:  /* neon_vld2_lanev8qi */
    case 1540:  /* neon_vld1_lanev2di */
    case 1539:  /* neon_vld1_lanev4sf */
    case 1538:  /* neon_vld1_lanev4si */
    case 1537:  /* neon_vld1_lanev8hi */
    case 1536:  /* neon_vld1_lanev16qi */
    case 1535:  /* neon_vld1_lanedi */
    case 1534:  /* neon_vld1_lanev2sf */
    case 1533:  /* neon_vld1_lanev2si */
    case 1532:  /* neon_vld1_lanev4hi */
    case 1531:  /* neon_vld1_lanev8qi */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (pat, 1), 0, 1));
      ro[3] = *(ro_loc[3] = &XVECEXP (XEXP (pat, 1), 0, 2));
      break;

    case 1677:  /* neon_vld4_dupdi */
    case 1676:  /* neon_vld4_dupv2sf */
    case 1675:  /* neon_vld4_dupv2si */
    case 1674:  /* neon_vld4_dupv4hi */
    case 1673:  /* neon_vld4_dupv8qi */
    case 1657:  /* neon_vld4di */
    case 1656:  /* neon_vld4v2sf */
    case 1655:  /* neon_vld4v2si */
    case 1654:  /* neon_vld4v4hi */
    case 1653:  /* neon_vld4v8qi */
    case 1632:  /* neon_vld3_dupdi */
    case 1631:  /* neon_vld3_dupv2sf */
    case 1630:  /* neon_vld3_dupv2si */
    case 1629:  /* neon_vld3_dupv4hi */
    case 1628:  /* neon_vld3_dupv8qi */
    case 1612:  /* neon_vld3di */
    case 1611:  /* neon_vld3v2sf */
    case 1610:  /* neon_vld3v2si */
    case 1609:  /* neon_vld3v4hi */
    case 1608:  /* neon_vld3v8qi */
    case 1591:  /* neon_vld2_dupdi */
    case 1590:  /* neon_vld2_dupv2sf */
    case 1589:  /* neon_vld2_dupv2si */
    case 1588:  /* neon_vld2_dupv4hi */
    case 1587:  /* neon_vld2_dupv8qi */
    case 1579:  /* neon_vld2v4sf */
    case 1578:  /* neon_vld2v4si */
    case 1577:  /* neon_vld2v8hi */
    case 1576:  /* neon_vld2v16qi */
    case 1575:  /* neon_vld2di */
    case 1574:  /* neon_vld2v2sf */
    case 1573:  /* neon_vld2v2si */
    case 1572:  /* neon_vld2v4hi */
    case 1571:  /* neon_vld2v8qi */
    case 1550:  /* neon_vld1_dupv2di */
    case 1549:  /* neon_vld1_dupv4sf */
    case 1548:  /* neon_vld1_dupv4si */
    case 1547:  /* neon_vld1_dupv8hi */
    case 1546:  /* neon_vld1_dupv16qi */
    case 1545:  /* neon_vld1_dupdi */
    case 1544:  /* neon_vld1_dupv2sf */
    case 1543:  /* neon_vld1_dupv2si */
    case 1542:  /* neon_vld1_dupv4hi */
    case 1541:  /* neon_vld1_dupv8qi */
    case 1530:  /* neon_vld1v2di */
    case 1529:  /* neon_vld1di */
    case 1528:  /* neon_vld1v4sf */
    case 1527:  /* neon_vld1v2sf */
    case 1526:  /* neon_vld1v4si */
    case 1525:  /* neon_vld1v2si */
    case 1524:  /* neon_vld1v8hi */
    case 1523:  /* neon_vld1v4hi */
    case 1522:  /* neon_vld1v16qi */
    case 1521:  /* neon_vld1v8qi */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0));
      break;

    case 1520:  /* neon_vuzpv4sf_internal */
    case 1519:  /* neon_vuzpv2sf_internal */
    case 1518:  /* neon_vuzpv4si_internal */
    case 1517:  /* neon_vuzpv2si_internal */
    case 1516:  /* neon_vuzpv8hi_internal */
    case 1515:  /* neon_vuzpv4hi_internal */
    case 1514:  /* neon_vuzpv16qi_internal */
    case 1513:  /* neon_vuzpv8qi_internal */
    case 1512:  /* neon_vzipv4sf_internal */
    case 1511:  /* neon_vzipv2sf_internal */
    case 1510:  /* neon_vzipv4si_internal */
    case 1509:  /* neon_vzipv2si_internal */
    case 1508:  /* neon_vzipv8hi_internal */
    case 1507:  /* neon_vzipv4hi_internal */
    case 1506:  /* neon_vzipv16qi_internal */
    case 1505:  /* neon_vzipv8qi_internal */
    case 1504:  /* neon_vtrnv4sf_internal */
    case 1503:  /* neon_vtrnv2sf_internal */
    case 1502:  /* neon_vtrnv4si_internal */
    case 1501:  /* neon_vtrnv2si_internal */
    case 1500:  /* neon_vtrnv8hi_internal */
    case 1499:  /* neon_vtrnv4hi_internal */
    case 1498:  /* neon_vtrnv16qi_internal */
    case 1497:  /* neon_vtrnv8qi_internal */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[3] = *(ro_loc[3] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0));
      break;

    case 1369:  /* neon_vqdmlsl_lanev2si */
    case 1368:  /* neon_vqdmlsl_lanev4hi */
    case 1367:  /* neon_vmlsl_lanev2si */
    case 1366:  /* neon_vmlsl_lanev4hi */
    case 1365:  /* neon_vmls_lanev4sf */
    case 1364:  /* neon_vmls_lanev4si */
    case 1363:  /* neon_vmls_lanev8hi */
    case 1362:  /* neon_vmls_lanev2sf */
    case 1361:  /* neon_vmls_lanev2si */
    case 1360:  /* neon_vmls_lanev4hi */
    case 1359:  /* neon_vqdmlal_lanev2si */
    case 1358:  /* neon_vqdmlal_lanev4hi */
    case 1357:  /* neon_vmlal_lanev2si */
    case 1356:  /* neon_vmlal_lanev4hi */
    case 1355:  /* neon_vmla_lanev4sf */
    case 1354:  /* neon_vmla_lanev4si */
    case 1353:  /* neon_vmla_lanev8hi */
    case 1352:  /* neon_vmla_lanev2sf */
    case 1351:  /* neon_vmla_lanev2si */
    case 1350:  /* neon_vmla_lanev4hi */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (pat, 1), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (pat, 1), 0, 1));
      ro[3] = *(ro_loc[3] = &XVECEXP (XEXP (pat, 1), 0, 2));
      ro[4] = *(ro_loc[4] = &XVECEXP (XEXP (pat, 1), 0, 3));
      ro[5] = *(ro_loc[5] = &XVECEXP (XEXP (pat, 1), 0, 4));
      break;

    case 1193:  /* neon_vabalv2si */
    case 1192:  /* neon_vabalv4hi */
    case 1191:  /* neon_vabalv8qi */
    case 1190:  /* neon_vabav4si */
    case 1189:  /* neon_vabav2si */
    case 1188:  /* neon_vabav8hi */
    case 1187:  /* neon_vabav4hi */
    case 1186:  /* neon_vabav16qi */
    case 1185:  /* neon_vabav8qi */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XEXP (pat, 1), 1), 0, 0));
      ro[3] = *(ro_loc[3] = &XVECEXP (XEXP (XEXP (pat, 1), 1), 0, 1));
      ro[4] = *(ro_loc[4] = &XVECEXP (XEXP (XEXP (pat, 1), 1), 0, 2));
      break;

    case 1472:  /* neon_vsra_nv2di */
    case 1471:  /* neon_vsra_ndi */
    case 1470:  /* neon_vsra_nv4si */
    case 1469:  /* neon_vsra_nv2si */
    case 1468:  /* neon_vsra_nv8hi */
    case 1467:  /* neon_vsra_nv4hi */
    case 1466:  /* neon_vsra_nv16qi */
    case 1465:  /* neon_vsra_nv8qi */
    case 1349:  /* neon_vqdmulh_lanev2si */
    case 1348:  /* neon_vqdmulh_lanev4hi */
    case 1347:  /* neon_vqdmulh_lanev4si */
    case 1346:  /* neon_vqdmulh_lanev8hi */
    case 1345:  /* neon_vqdmull_lanev2si */
    case 1344:  /* neon_vqdmull_lanev4hi */
    case 1343:  /* neon_vmull_lanev2si */
    case 1342:  /* neon_vmull_lanev4hi */
    case 1341:  /* neon_vmul_lanev4sf */
    case 1340:  /* neon_vmul_lanev4si */
    case 1339:  /* neon_vmul_lanev8hi */
    case 1338:  /* neon_vmul_lanev2sf */
    case 1337:  /* neon_vmul_lanev2si */
    case 1336:  /* neon_vmul_lanev4hi */
    case 1085:  /* neon_vqdmlslv2si */
    case 1084:  /* neon_vqdmlslv4hi */
    case 1083:  /* neon_vqdmlalv2si */
    case 1082:  /* neon_vqdmlalv4hi */
    case 1077:  /* neon_vmlslv2si */
    case 1076:  /* neon_vmlslv4hi */
    case 1075:  /* neon_vmlslv8qi */
    case 1065:  /* neon_vmlalv2si */
    case 1064:  /* neon_vmlalv4hi */
    case 1063:  /* neon_vmlalv8qi */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (pat, 1), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (pat, 1), 0, 1));
      ro[3] = *(ro_loc[3] = &XVECEXP (XEXP (pat, 1), 0, 2));
      ro[4] = *(ro_loc[4] = &XVECEXP (XEXP (pat, 1), 0, 3));
      break;

    case 977:  /* move_lo_quad_v16qi */
    case 976:  /* move_lo_quad_v8hi */
    case 975:  /* move_lo_quad_v4sf */
    case 974:  /* move_lo_quad_v4si */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (pat, 1), 1), 0);
      recog_data.dup_num[0] = 0;
      break;

    case 973:  /* quad_halves_umaxv16qi */
    case 972:  /* quad_halves_uminv16qi */
    case 971:  /* quad_halves_smaxv16qi */
    case 970:  /* quad_halves_sminv16qi */
    case 969:  /* quad_halves_plusv16qi */
    case 968:  /* quad_halves_umaxv8hi */
    case 967:  /* quad_halves_uminv8hi */
    case 966:  /* quad_halves_smaxv8hi */
    case 965:  /* quad_halves_sminv8hi */
    case 964:  /* quad_halves_plusv8hi */
    case 963:  /* quad_halves_smaxv4sf */
    case 962:  /* quad_halves_sminv4sf */
    case 961:  /* quad_halves_plusv4sf */
    case 960:  /* quad_halves_umaxv4si */
    case 959:  /* quad_halves_uminv4si */
    case 958:  /* quad_halves_smaxv4si */
    case 957:  /* quad_halves_sminv4si */
    case 956:  /* quad_halves_plusv4si */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (pat, 1), 1), 0);
      recog_data.dup_num[0] = 1;
      break;

    case 770:  /* vec_setv2di_internal */
    case 769:  /* vec_setv4sf_internal */
    case 768:  /* vec_setv4si_internal */
    case 767:  /* vec_setv8hi_internal */
    case 766:  /* vec_setv16qi_internal */
    case 765:  /* vec_setv2sf_internal */
    case 764:  /* vec_setv2si_internal */
    case 763:  /* vec_setv4hi_internal */
    case 762:  /* vec_setv8qi_internal */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 1), 2));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 1));
      break;

    case 722:  /* *thumb2_cbnz */
    case 721:  /* *thumb2_cbz */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      break;

    case 711:  /* *thumb2_movsi_shortim */
    case 710:  /* *thumb2_movhi_shortim */
    case 709:  /* *thumb2_movqi_shortim */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 0), 1));
      break;

    case 704:  /* thumb2_casesi_internal_pic */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 2), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (pat, 0, 3), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 4), 0), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0), 0), 0);
      recog_data.dup_num[1] = 0;
      break;

    case 703:  /* thumb2_casesi_internal */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 2), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 3), 0), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0), 0), 0);
      recog_data.dup_num[1] = 0;
      break;

    case 672:  /* *thumb2_cmpsi_neg_shiftsi */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      break;

    case 666:  /* tls_load_dot_plus_four */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0, 0));
      ro[3] = *(ro_loc[3] = &XVECEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0, 2));
      break;

    case 626:  /* *muldf3negdfsubdf_vfp */
    case 625:  /* *mulsf3negsfsubsf_vfp */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      break;

    case 826:  /* mulv2di3negv2diaddv2di_neon */
    case 825:  /* mulv4sf3negv4sfaddv4sf_neon */
    case 824:  /* mulv2sf3negv2sfaddv2sf_neon */
    case 823:  /* mulv4si3negv4siaddv4si_neon */
    case 822:  /* mulv2si3negv2siaddv2si_neon */
    case 821:  /* mulv8hi3negv8hiaddv8hi_neon */
    case 820:  /* mulv4hi3negv4hiaddv4hi_neon */
    case 819:  /* mulv16qi3negv16qiaddv16qi_neon */
    case 818:  /* mulv8qi3negv8qiaddv8qi_neon */
    case 624:  /* *fmuldf3negdfadddf_vfp */
    case 623:  /* *mulsf3negsfaddsf_vfp */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      break;

    case 817:  /* mulv2di3addv2di_neon */
    case 816:  /* mulv4sf3addv4sf_neon */
    case 815:  /* mulv2sf3addv2sf_neon */
    case 814:  /* mulv4si3addv4si_neon */
    case 813:  /* mulv2si3addv2si_neon */
    case 812:  /* mulv8hi3addv8hi_neon */
    case 811:  /* mulv4hi3addv4hi_neon */
    case 810:  /* mulv16qi3addv16qi_neon */
    case 809:  /* mulv8qi3addv8qi_neon */
    case 622:  /* *muldf3subdf_vfp */
    case 621:  /* *mulsf3subsf_vfp */
    case 620:  /* *muldf3adddf_vfp */
    case 619:  /* *mulsf3addsf_vfp */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      break;

    case 585:  /* iwmmxt_tmcr */
      ro[0] = *(ro_loc[0] = &XVECEXP (pat, 0, 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (pat, 0, 1));
      break;

    case 583:  /* iwmmxt_walign */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 1), 0));
      break;

    case 573:  /* iwmmxt_tmiatt */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0), 0), 0));
      break;

    case 572:  /* iwmmxt_tmiabt */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0), 0), 0));
      break;

    case 571:  /* iwmmxt_tmiatb */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0), 0));
      break;

    case 570:  /* iwmmxt_tmiabb */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0), 0));
      break;

    case 569:  /* iwmmxt_tmiaph */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 1), 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0), 0), 0), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 1), 0), 0), 0);
      recog_data.dup_num[1] = 3;
      break;

    case 568:  /* iwmmxt_tmia */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0));
      break;

    case 505:  /* gtv2si3 */
    case 504:  /* gtv4hi3 */
    case 503:  /* gtv8qi3 */
    case 502:  /* gtuv2si3 */
    case 501:  /* gtuv4hi3 */
    case 500:  /* gtuv8qi3 */
    case 499:  /* eqv2si3 */
    case 498:  /* eqv4hi3 */
    case 497:  /* eqv8qi3 */
      ro[0] = *(ro_loc[0] = &XVECEXP (pat, 0, 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (pat, 0, 1));
      ro[2] = *(ro_loc[2] = &XVECEXP (pat, 0, 2));
      break;

    case 779:  /* vec_extractv2di */
    case 778:  /* vec_extractv4sf */
    case 777:  /* vec_extractv4si */
    case 776:  /* vec_extractv8hi */
    case 775:  /* vec_extractv16qi */
    case 774:  /* vec_extractv2sf */
    case 773:  /* vec_extractv2si */
    case 772:  /* vec_extractv4hi */
    case 771:  /* vec_extractv8qi */
    case 495:  /* iwmmxt_textrmw */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XEXP (pat, 1), 1), 0, 0));
      break;

    case 1300:  /* neon_vdup_lanev4sf_internal */
    case 1299:  /* neon_vdup_lanev2sf_internal */
    case 1298:  /* neon_vdup_lanev4si_internal */
    case 1297:  /* neon_vdup_lanev2si_internal */
    case 1296:  /* neon_vdup_lanev8hi_internal */
    case 1295:  /* neon_vdup_lanev4hi_internal */
    case 1294:  /* neon_vdup_lanev16qi_internal */
    case 1293:  /* neon_vdup_lanev8qi_internal */
    case 1283:  /* neon_vget_lanev4sf_zext_internal */
    case 1282:  /* neon_vget_lanev4si_zext_internal */
    case 1281:  /* neon_vget_lanev8hi_zext_internal */
    case 1280:  /* neon_vget_lanev16qi_zext_internal */
    case 1279:  /* neon_vget_lanev4sf_sext_internal */
    case 1278:  /* neon_vget_lanev4si_sext_internal */
    case 1277:  /* neon_vget_lanev8hi_sext_internal */
    case 1276:  /* neon_vget_lanev16qi_sext_internal */
    case 1275:  /* neon_vget_lanev2sf_zext_internal */
    case 1274:  /* neon_vget_lanev2si_zext_internal */
    case 1273:  /* neon_vget_lanev4hi_zext_internal */
    case 1272:  /* neon_vget_lanev8qi_zext_internal */
    case 1271:  /* neon_vget_lanev2sf_sext_internal */
    case 1270:  /* neon_vget_lanev2si_sext_internal */
    case 1269:  /* neon_vget_lanev4hi_sext_internal */
    case 1268:  /* neon_vget_lanev8qi_sext_internal */
    case 494:  /* iwmmxt_textrmsh */
    case 493:  /* iwmmxt_textrmuh */
    case 492:  /* iwmmxt_textrmsb */
    case 491:  /* iwmmxt_textrmub */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XEXP (XEXP (pat, 1), 0), 1), 0, 0));
      break;

    case 490:  /* iwmmxt_tinsrw */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 2));
      break;

    case 489:  /* iwmmxt_tinsrh */
    case 488:  /* iwmmxt_tinsrb */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 2));
      break;

    case 484:  /* iwmmxt_uavgrndv4hi3 */
    case 483:  /* iwmmxt_uavgrndv8qi3 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 1));
      break;

    case 1492:  /* neon_vtbl4v8qi */
    case 1491:  /* neon_vtbl3v8qi */
    case 1490:  /* neon_vtbl2v8qi */
    case 1489:  /* neon_vtbl1v8qi */
    case 1394:  /* neon_vrev16v16qi */
    case 1393:  /* neon_vrev16v8qi */
    case 1392:  /* neon_vrev32v8hi */
    case 1391:  /* neon_vrev32v16qi */
    case 1390:  /* neon_vrev32v4hi */
    case 1389:  /* neon_vrev32v8qi */
    case 1388:  /* neon_vrev64v2di */
    case 1387:  /* neon_vrev64v4sf */
    case 1386:  /* neon_vrev64v2sf */
    case 1385:  /* neon_vrev64v4si */
    case 1384:  /* neon_vrev64v2si */
    case 1383:  /* neon_vrev64v8hi */
    case 1382:  /* neon_vrev64v4hi */
    case 1381:  /* neon_vrev64v16qi */
    case 1380:  /* neon_vrev64v8qi */
    case 1335:  /* neon_vmovlv2si */
    case 1334:  /* neon_vmovlv4hi */
    case 1333:  /* neon_vmovlv8qi */
    case 1332:  /* neon_vqmovunv2di */
    case 1331:  /* neon_vqmovunv4si */
    case 1330:  /* neon_vqmovunv8hi */
    case 1329:  /* neon_vqmovnv2di */
    case 1328:  /* neon_vqmovnv4si */
    case 1327:  /* neon_vqmovnv8hi */
    case 1326:  /* neon_vmovnv2di */
    case 1325:  /* neon_vmovnv4si */
    case 1324:  /* neon_vmovnv8hi */
    case 1319:  /* neon_vcvtv4si */
    case 1318:  /* neon_vcvtv2si */
    case 1317:  /* neon_vcvtv4sf */
    case 1316:  /* neon_vcvtv2sf */
    case 1267:  /* neon_vrsqrtev4sf */
    case 1266:  /* neon_vrsqrtev4si */
    case 1265:  /* neon_vrsqrtev2sf */
    case 1264:  /* neon_vrsqrtev2si */
    case 1263:  /* neon_vrecpev4sf */
    case 1262:  /* neon_vrecpev4si */
    case 1261:  /* neon_vrecpev2sf */
    case 1260:  /* neon_vrecpev2si */
    case 1251:  /* neon_vclsv4si */
    case 1250:  /* neon_vclsv2si */
    case 1249:  /* neon_vclsv8hi */
    case 1248:  /* neon_vclsv4hi */
    case 1247:  /* neon_vclsv16qi */
    case 1246:  /* neon_vclsv8qi */
    case 1245:  /* neon_vqnegv4si */
    case 1244:  /* neon_vqnegv2si */
    case 1243:  /* neon_vqnegv8hi */
    case 1242:  /* neon_vqnegv4hi */
    case 1241:  /* neon_vqnegv16qi */
    case 1240:  /* neon_vqnegv8qi */
    case 1239:  /* neon_vqabsv4si */
    case 1238:  /* neon_vqabsv2si */
    case 1237:  /* neon_vqabsv8hi */
    case 1236:  /* neon_vqabsv4hi */
    case 1235:  /* neon_vqabsv16qi */
    case 1234:  /* neon_vqabsv8qi */
    case 1215:  /* neon_vpaddlv4si */
    case 1214:  /* neon_vpaddlv2si */
    case 1213:  /* neon_vpaddlv8hi */
    case 1212:  /* neon_vpaddlv4hi */
    case 1211:  /* neon_vpaddlv16qi */
    case 1210:  /* neon_vpaddlv8qi */
    case 1100:  /* neon_vsubv2di_unspec */
    case 1099:  /* neon_vsubdi_unspec */
    case 1098:  /* neon_vsubv4sf_unspec */
    case 1097:  /* neon_vsubv2sf_unspec */
    case 1096:  /* neon_vsubv4si_unspec */
    case 1095:  /* neon_vsubv2si_unspec */
    case 1094:  /* neon_vsubv8hi_unspec */
    case 1093:  /* neon_vsubv4hi_unspec */
    case 1092:  /* neon_vsubv16qi_unspec */
    case 1091:  /* neon_vsubv8qi_unspec */
    case 1022:  /* neon_vaddv2di_unspec */
    case 1021:  /* neon_vadddi_unspec */
    case 1020:  /* neon_vaddv4sf_unspec */
    case 1019:  /* neon_vaddv2sf_unspec */
    case 1018:  /* neon_vaddv4si_unspec */
    case 1017:  /* neon_vaddv2si_unspec */
    case 1016:  /* neon_vaddv8hi_unspec */
    case 1015:  /* neon_vaddv4hi_unspec */
    case 1014:  /* neon_vaddv16qi_unspec */
    case 1013:  /* neon_vaddv8qi_unspec */
    case 996:  /* neon_vpumaxv2si */
    case 995:  /* neon_vpumaxv4hi */
    case 994:  /* neon_vpumaxv8qi */
    case 993:  /* neon_vpuminv2si */
    case 992:  /* neon_vpuminv4hi */
    case 991:  /* neon_vpuminv8qi */
    case 990:  /* neon_vpsmaxv2sf */
    case 989:  /* neon_vpsmaxv2si */
    case 988:  /* neon_vpsmaxv4hi */
    case 987:  /* neon_vpsmaxv8qi */
    case 986:  /* neon_vpsminv2sf */
    case 985:  /* neon_vpsminv2si */
    case 984:  /* neon_vpsminv4hi */
    case 983:  /* neon_vpsminv8qi */
    case 982:  /* neon_vpadd_internalv2sf */
    case 981:  /* neon_vpadd_internalv2si */
    case 980:  /* neon_vpadd_internalv4hi */
    case 979:  /* neon_vpadd_internalv8qi */
    case 949:  /* ashlv2di3_unsigned */
    case 948:  /* ashlv4si3_unsigned */
    case 947:  /* ashlv2si3_unsigned */
    case 946:  /* ashlv8hi3_unsigned */
    case 945:  /* ashlv4hi3_unsigned */
    case 944:  /* ashlv16qi3_unsigned */
    case 943:  /* ashlv8qi3_unsigned */
    case 942:  /* ashlv2di3_signed */
    case 941:  /* ashlv4si3_signed */
    case 940:  /* ashlv2si3_signed */
    case 939:  /* ashlv8hi3_signed */
    case 938:  /* ashlv4hi3_signed */
    case 937:  /* ashlv16qi3_signed */
    case 936:  /* ashlv8qi3_signed */
    case 589:  /* iwmmxt_wsadhz */
    case 588:  /* iwmmxt_wsadbz */
    case 587:  /* iwmmxt_wsadh */
    case 586:  /* iwmmxt_wsadb */
    case 567:  /* iwmmxt_wmaddu */
    case 566:  /* iwmmxt_wmadds */
    case 496:  /* iwmmxt_wshufh */
    case 478:  /* iwmmxt_wmacuz */
    case 476:  /* iwmmxt_wmacsz */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (pat, 1), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (pat, 1), 0, 1));
      break;

    case 1496:  /* neon_vtbx4v8qi */
    case 1495:  /* neon_vtbx3v8qi */
    case 1494:  /* neon_vtbx2v8qi */
    case 1493:  /* neon_vtbx1v8qi */
    case 1488:  /* neon_vsli_nv2di */
    case 1487:  /* neon_vsli_ndi */
    case 1486:  /* neon_vsli_nv4si */
    case 1485:  /* neon_vsli_nv2si */
    case 1484:  /* neon_vsli_nv8hi */
    case 1483:  /* neon_vsli_nv4hi */
    case 1482:  /* neon_vsli_nv16qi */
    case 1481:  /* neon_vsli_nv8qi */
    case 1480:  /* neon_vsri_nv2di */
    case 1479:  /* neon_vsri_ndi */
    case 1478:  /* neon_vsri_nv4si */
    case 1477:  /* neon_vsri_nv2si */
    case 1476:  /* neon_vsri_nv8hi */
    case 1475:  /* neon_vsri_nv4hi */
    case 1474:  /* neon_vsri_nv16qi */
    case 1473:  /* neon_vsri_nv8qi */
    case 1464:  /* neon_vshll_nv2si */
    case 1463:  /* neon_vshll_nv4hi */
    case 1462:  /* neon_vshll_nv8qi */
    case 1461:  /* neon_vqshlu_nv2di */
    case 1460:  /* neon_vqshlu_ndi */
    case 1459:  /* neon_vqshlu_nv4si */
    case 1458:  /* neon_vqshlu_nv2si */
    case 1457:  /* neon_vqshlu_nv8hi */
    case 1456:  /* neon_vqshlu_nv4hi */
    case 1455:  /* neon_vqshlu_nv16qi */
    case 1454:  /* neon_vqshlu_nv8qi */
    case 1453:  /* neon_vqshl_nv2di */
    case 1452:  /* neon_vqshl_ndi */
    case 1451:  /* neon_vqshl_nv4si */
    case 1450:  /* neon_vqshl_nv2si */
    case 1449:  /* neon_vqshl_nv8hi */
    case 1448:  /* neon_vqshl_nv4hi */
    case 1447:  /* neon_vqshl_nv16qi */
    case 1446:  /* neon_vqshl_nv8qi */
    case 1445:  /* neon_vshl_nv2di */
    case 1444:  /* neon_vshl_ndi */
    case 1443:  /* neon_vshl_nv4si */
    case 1442:  /* neon_vshl_nv2si */
    case 1441:  /* neon_vshl_nv8hi */
    case 1440:  /* neon_vshl_nv4hi */
    case 1439:  /* neon_vshl_nv16qi */
    case 1438:  /* neon_vshl_nv8qi */
    case 1437:  /* neon_vqshrun_nv2di */
    case 1436:  /* neon_vqshrun_nv4si */
    case 1435:  /* neon_vqshrun_nv8hi */
    case 1434:  /* neon_vqshrn_nv2di */
    case 1433:  /* neon_vqshrn_nv4si */
    case 1432:  /* neon_vqshrn_nv8hi */
    case 1431:  /* neon_vshrn_nv2di */
    case 1430:  /* neon_vshrn_nv4si */
    case 1429:  /* neon_vshrn_nv8hi */
    case 1428:  /* neon_vshr_nv2di */
    case 1427:  /* neon_vshr_ndi */
    case 1426:  /* neon_vshr_nv4si */
    case 1425:  /* neon_vshr_nv2si */
    case 1424:  /* neon_vshr_nv8hi */
    case 1423:  /* neon_vshr_nv4hi */
    case 1422:  /* neon_vshr_nv16qi */
    case 1421:  /* neon_vshr_nv8qi */
    case 1420:  /* neon_vqshlv2di */
    case 1419:  /* neon_vqshldi */
    case 1418:  /* neon_vqshlv4si */
    case 1417:  /* neon_vqshlv2si */
    case 1416:  /* neon_vqshlv8hi */
    case 1415:  /* neon_vqshlv4hi */
    case 1414:  /* neon_vqshlv16qi */
    case 1413:  /* neon_vqshlv8qi */
    case 1412:  /* neon_vshlv2di */
    case 1411:  /* neon_vshldi */
    case 1410:  /* neon_vshlv4si */
    case 1409:  /* neon_vshlv2si */
    case 1408:  /* neon_vshlv8hi */
    case 1407:  /* neon_vshlv4hi */
    case 1406:  /* neon_vshlv16qi */
    case 1405:  /* neon_vshlv8qi */
    case 1404:  /* neon_vbslv2di_internal */
    case 1403:  /* neon_vbsldi_internal */
    case 1402:  /* neon_vbslv4sf_internal */
    case 1401:  /* neon_vbslv2sf_internal */
    case 1400:  /* neon_vbslv4si_internal */
    case 1399:  /* neon_vbslv2si_internal */
    case 1398:  /* neon_vbslv8hi_internal */
    case 1397:  /* neon_vbslv4hi_internal */
    case 1396:  /* neon_vbslv16qi_internal */
    case 1395:  /* neon_vbslv8qi_internal */
    case 1379:  /* neon_vextv2di */
    case 1378:  /* neon_vextdi */
    case 1377:  /* neon_vextv4sf */
    case 1376:  /* neon_vextv2sf */
    case 1375:  /* neon_vextv4si */
    case 1374:  /* neon_vextv2si */
    case 1373:  /* neon_vextv8hi */
    case 1372:  /* neon_vextv4hi */
    case 1371:  /* neon_vextv16qi */
    case 1370:  /* neon_vextv8qi */
    case 1323:  /* neon_vcvt_nv4si */
    case 1322:  /* neon_vcvt_nv2si */
    case 1321:  /* neon_vcvt_nv4sf */
    case 1320:  /* neon_vcvt_nv2sf */
    case 1233:  /* neon_vrsqrtsv4sf */
    case 1232:  /* neon_vrsqrtsv2sf */
    case 1231:  /* neon_vrecpsv4sf */
    case 1230:  /* neon_vrecpsv2sf */
    case 1229:  /* neon_vpminv2sf */
    case 1228:  /* neon_vpminv2si */
    case 1227:  /* neon_vpminv4hi */
    case 1226:  /* neon_vpminv8qi */
    case 1225:  /* neon_vpmaxv2sf */
    case 1224:  /* neon_vpmaxv2si */
    case 1223:  /* neon_vpmaxv4hi */
    case 1222:  /* neon_vpmaxv8qi */
    case 1221:  /* neon_vpadalv4si */
    case 1220:  /* neon_vpadalv2si */
    case 1219:  /* neon_vpadalv8hi */
    case 1218:  /* neon_vpadalv4hi */
    case 1217:  /* neon_vpadalv16qi */
    case 1216:  /* neon_vpadalv8qi */
    case 1209:  /* neon_vminv4sf */
    case 1208:  /* neon_vminv2sf */
    case 1207:  /* neon_vminv4si */
    case 1206:  /* neon_vminv2si */
    case 1205:  /* neon_vminv8hi */
    case 1204:  /* neon_vminv4hi */
    case 1203:  /* neon_vminv16qi */
    case 1202:  /* neon_vminv8qi */
    case 1201:  /* neon_vmaxv4sf */
    case 1200:  /* neon_vmaxv2sf */
    case 1199:  /* neon_vmaxv4si */
    case 1198:  /* neon_vmaxv2si */
    case 1197:  /* neon_vmaxv8hi */
    case 1196:  /* neon_vmaxv4hi */
    case 1195:  /* neon_vmaxv16qi */
    case 1194:  /* neon_vmaxv8qi */
    case 1184:  /* neon_vabdlv2si */
    case 1183:  /* neon_vabdlv4hi */
    case 1182:  /* neon_vabdlv8qi */
    case 1181:  /* neon_vabdv4sf */
    case 1180:  /* neon_vabdv2sf */
    case 1179:  /* neon_vabdv4si */
    case 1178:  /* neon_vabdv2si */
    case 1177:  /* neon_vabdv8hi */
    case 1176:  /* neon_vabdv4hi */
    case 1175:  /* neon_vabdv16qi */
    case 1174:  /* neon_vabdv8qi */
    case 1173:  /* neon_vtstv4si */
    case 1172:  /* neon_vtstv2si */
    case 1171:  /* neon_vtstv8hi */
    case 1170:  /* neon_vtstv4hi */
    case 1169:  /* neon_vtstv16qi */
    case 1168:  /* neon_vtstv8qi */
    case 1167:  /* neon_vcagtv4sf */
    case 1166:  /* neon_vcagtv2sf */
    case 1165:  /* neon_vcagev4sf */
    case 1164:  /* neon_vcagev2sf */
    case 1163:  /* neon_vcltv4sf */
    case 1162:  /* neon_vcltv2sf */
    case 1161:  /* neon_vcltv4si */
    case 1160:  /* neon_vcltv2si */
    case 1159:  /* neon_vcltv8hi */
    case 1158:  /* neon_vcltv4hi */
    case 1157:  /* neon_vcltv16qi */
    case 1156:  /* neon_vcltv8qi */
    case 1155:  /* neon_vclev4sf */
    case 1154:  /* neon_vclev2sf */
    case 1153:  /* neon_vclev4si */
    case 1152:  /* neon_vclev2si */
    case 1151:  /* neon_vclev8hi */
    case 1150:  /* neon_vclev4hi */
    case 1149:  /* neon_vclev16qi */
    case 1148:  /* neon_vclev8qi */
    case 1147:  /* neon_vcgtv4sf */
    case 1146:  /* neon_vcgtv2sf */
    case 1145:  /* neon_vcgtv4si */
    case 1144:  /* neon_vcgtv2si */
    case 1143:  /* neon_vcgtv8hi */
    case 1142:  /* neon_vcgtv4hi */
    case 1141:  /* neon_vcgtv16qi */
    case 1140:  /* neon_vcgtv8qi */
    case 1139:  /* neon_vcgev4sf */
    case 1138:  /* neon_vcgev2sf */
    case 1137:  /* neon_vcgev4si */
    case 1136:  /* neon_vcgev2si */
    case 1135:  /* neon_vcgev8hi */
    case 1134:  /* neon_vcgev4hi */
    case 1133:  /* neon_vcgev16qi */
    case 1132:  /* neon_vcgev8qi */
    case 1131:  /* neon_vceqv4sf */
    case 1130:  /* neon_vceqv2sf */
    case 1129:  /* neon_vceqv4si */
    case 1128:  /* neon_vceqv2si */
    case 1127:  /* neon_vceqv8hi */
    case 1126:  /* neon_vceqv4hi */
    case 1125:  /* neon_vceqv16qi */
    case 1124:  /* neon_vceqv8qi */
    case 1123:  /* neon_vsubhnv2di */
    case 1122:  /* neon_vsubhnv4si */
    case 1121:  /* neon_vsubhnv8hi */
    case 1120:  /* neon_vhsubv4si */
    case 1119:  /* neon_vhsubv2si */
    case 1118:  /* neon_vhsubv8hi */
    case 1117:  /* neon_vhsubv4hi */
    case 1116:  /* neon_vhsubv16qi */
    case 1115:  /* neon_vhsubv8qi */
    case 1114:  /* neon_vqsubv2di */
    case 1113:  /* neon_vqsubdi */
    case 1112:  /* neon_vqsubv4si */
    case 1111:  /* neon_vqsubv2si */
    case 1110:  /* neon_vqsubv8hi */
    case 1109:  /* neon_vqsubv4hi */
    case 1108:  /* neon_vqsubv16qi */
    case 1107:  /* neon_vqsubv8qi */
    case 1106:  /* neon_vsubwv2si */
    case 1105:  /* neon_vsubwv4hi */
    case 1104:  /* neon_vsubwv8qi */
    case 1103:  /* neon_vsublv2si */
    case 1102:  /* neon_vsublv4hi */
    case 1101:  /* neon_vsublv8qi */
    case 1090:  /* neon_vqdmullv2si */
    case 1089:  /* neon_vqdmullv4hi */
    case 1088:  /* neon_vmullv2si */
    case 1087:  /* neon_vmullv4hi */
    case 1086:  /* neon_vmullv8qi */
    case 1081:  /* neon_vqdmulhv4si */
    case 1080:  /* neon_vqdmulhv8hi */
    case 1079:  /* neon_vqdmulhv2si */
    case 1078:  /* neon_vqdmulhv4hi */
    case 1074:  /* neon_vmlsv2di_unspec */
    case 1073:  /* neon_vmlsv4sf_unspec */
    case 1072:  /* neon_vmlsv2sf_unspec */
    case 1071:  /* neon_vmlsv4si_unspec */
    case 1070:  /* neon_vmlsv2si_unspec */
    case 1069:  /* neon_vmlsv8hi_unspec */
    case 1068:  /* neon_vmlsv4hi_unspec */
    case 1067:  /* neon_vmlsv16qi_unspec */
    case 1066:  /* neon_vmlsv8qi_unspec */
    case 1062:  /* neon_vmlav2di_unspec */
    case 1061:  /* neon_vmlav4sf_unspec */
    case 1060:  /* neon_vmlav2sf_unspec */
    case 1059:  /* neon_vmlav4si_unspec */
    case 1058:  /* neon_vmlav2si_unspec */
    case 1057:  /* neon_vmlav8hi_unspec */
    case 1056:  /* neon_vmlav4hi_unspec */
    case 1055:  /* neon_vmlav16qi_unspec */
    case 1054:  /* neon_vmlav8qi_unspec */
    case 1053:  /* neon_vmulv4sf */
    case 1052:  /* neon_vmulv2sf */
    case 1051:  /* neon_vmulv4si */
    case 1050:  /* neon_vmulv2si */
    case 1049:  /* neon_vmulv8hi */
    case 1048:  /* neon_vmulv4hi */
    case 1047:  /* neon_vmulv16qi */
    case 1046:  /* neon_vmulv8qi */
    case 1045:  /* neon_vaddhnv2di */
    case 1044:  /* neon_vaddhnv4si */
    case 1043:  /* neon_vaddhnv8hi */
    case 1042:  /* neon_vqaddv2di */
    case 1041:  /* neon_vqadddi */
    case 1040:  /* neon_vqaddv4si */
    case 1039:  /* neon_vqaddv2si */
    case 1038:  /* neon_vqaddv8hi */
    case 1037:  /* neon_vqaddv4hi */
    case 1036:  /* neon_vqaddv16qi */
    case 1035:  /* neon_vqaddv8qi */
    case 1034:  /* neon_vhaddv4si */
    case 1033:  /* neon_vhaddv2si */
    case 1032:  /* neon_vhaddv8hi */
    case 1031:  /* neon_vhaddv4hi */
    case 1030:  /* neon_vhaddv16qi */
    case 1029:  /* neon_vhaddv8qi */
    case 1028:  /* neon_vaddwv2si */
    case 1027:  /* neon_vaddwv4hi */
    case 1026:  /* neon_vaddwv8qi */
    case 1025:  /* neon_vaddlv2si */
    case 1024:  /* neon_vaddlv4hi */
    case 1023:  /* neon_vaddlv8qi */
    case 477:  /* iwmmxt_wmacu */
    case 475:  /* iwmmxt_wmacs */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (pat, 1), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (pat, 1), 0, 1));
      ro[3] = *(ro_loc[3] = &XVECEXP (XEXP (pat, 1), 0, 2));
      break;

    case 474:  /* umulv4hi3_highpart */
    case 473:  /* smulv4hi3_highpart */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 1), 0));
      break;

    case 2977:  /* *p *thumb2_movsf_soft_insn */
    case 2976:  /* *p *thumb2_movhi_insn */
    case 2975:  /* *p *thumb2_movsi_insn */
    case 2931:  /* *p *movdf_vfp */
    case 2930:  /* *p *thumb2_movsf_vfp */
    case 2929:  /* *p *movsf_vfp */
    case 2928:  /* *p *arm_movdi_vfp_cortexa8 */
    case 2927:  /* *p *thumb2_movsi_vfp */
    case 2926:  /* *p *arm_movsi_vfp */
    case 2789:  /* *p movv8qi_internal */
    case 2788:  /* *p movv4hi_internal */
    case 2787:  /* *p movv2si_internal */
    case 2782:  /* *p *thumb2_movsf_fpa */
    case 2781:  /* *p *movxf_fpa */
    case 2780:  /* *p *movdf_fpa */
    case 2779:  /* *p *movsf_fpa */
    case 2715:  /* *p *arm_movsf_soft_insn */
    case 2714:  /* *p *arm32_movhf */
    case 2713:  /* *p *arm_movqi_insn */
    case 2712:  /* *p *movhi_bytes */
    case 2711:  /* *p *movhi_insn_arch4 */
    case 2708:  /* *p *arm_movsi_insn */
    case 450:  /* *cond_iwmmxt_movsi_insn */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (pat, 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 0), 0));
      break;

    case 407:  /* *cmp_df_esfdf_trap_fpa */
    case 403:  /* *cmpdf_esfdf_fpa */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      break;

    case 634:  /* fixuns_truncdfsi2 */
    case 633:  /* fixuns_truncsfsi2 */
    case 632:  /* *truncsidf2_vfp */
    case 631:  /* *truncsisf2_vfp */
    case 541:  /* iwmmxt_wunpckelsw */
    case 540:  /* iwmmxt_wunpckelsh */
    case 539:  /* iwmmxt_wunpckelsb */
    case 538:  /* iwmmxt_wunpckeluw */
    case 537:  /* iwmmxt_wunpckeluh */
    case 536:  /* iwmmxt_wunpckelub */
    case 535:  /* iwmmxt_wunpckehsw */
    case 534:  /* iwmmxt_wunpckehsh */
    case 533:  /* iwmmxt_wunpckehsb */
    case 532:  /* iwmmxt_wunpckehuw */
    case 531:  /* iwmmxt_wunpckehuh */
    case 530:  /* iwmmxt_wunpckehub */
    case 391:  /* *fix_truncdfsi2_fpa */
    case 390:  /* *fix_truncsfsi2_fpa */
    case 387:  /* *sqrtdf_esfdf_fpa */
    case 384:  /* *absdf_esfdf_fpa */
    case 381:  /* *negdf_esfdf_fpa */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      break;

    case 865:  /* bicv2di3_neon */
    case 864:  /* bicv4sf3_neon */
    case 863:  /* bicv2sf3_neon */
    case 862:  /* bicv4si3_neon */
    case 861:  /* bicv2si3_neon */
    case 860:  /* bicv8hi3_neon */
    case 859:  /* bicv4hi3_neon */
    case 858:  /* bicv16qi3_neon */
    case 857:  /* bicv8qi3_neon */
    case 856:  /* orndi3_neon */
    case 855:  /* ornv2di3_neon */
    case 854:  /* ornv4sf3_neon */
    case 853:  /* ornv2sf3_neon */
    case 852:  /* ornv4si3_neon */
    case 851:  /* ornv2si3_neon */
    case 850:  /* ornv8hi3_neon */
    case 849:  /* ornv4hi3_neon */
    case 848:  /* ornv16qi3_neon */
    case 847:  /* ornv8qi3_neon */
    case 447:  /* iwmmxt_nanddi3 */
    case 377:  /* *moddf_df_esfdf_fpa */
    case 372:  /* *divdf_df_esfdf_fpa */
    case 367:  /* *muldf_df_esfdf_fpa */
    case 362:  /* *subdf_df_esfdf_fpa */
    case 357:  /* *adddf_df_esfdf_fpa */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      break;

    case 351:  /* *arm_movtas_ze */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (pat, 1));
      break;

    case 706:  /* thumb2_eh_return */
    case 348:  /* thumb_eh_return */
    case 347:  /* arm_eh_return */
      ro[0] = *(ro_loc[0] = &XVECEXP (XVECEXP (pat, 0, 0), 0, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 1), 0));
      break;

    case 345:  /* prefetch */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (pat, 1));
      ro[2] = *(ro_loc[2] = &XEXP (pat, 2));
      break;

    case 342:  /* *thumb1_tablejump */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 1));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 0), 0));
      break;

    case 346:  /* prologue_use */
    case 341:  /* consttable_16 */
    case 340:  /* consttable_8 */
    case 339:  /* consttable_4 */
    case 338:  /* consttable_2 */
    case 337:  /* consttable_1 */
      ro[0] = *(ro_loc[0] = &XVECEXP (pat, 0, 0));
      break;

    case 330:  /* stack_tie */
      ro[0] = *(ro_loc[0] = &XVECEXP (XEXP (pat, 1), 0, 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (pat, 1), 0, 1));
      break;

    case 650:  /* *push_multi_vfp */
    case 331:  /* *push_fp_multi */
    case 329:  /* *push_multi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0));
      ro[2] = *(ro_loc[2] = &PATTERN (insn));
      break;

    case 328:  /* *not_signextract_onebit */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 2));
      break;

    case 327:  /* *sign_extract_onebit */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2));
      break;

    case 323:  /* *arith_adjacentmem */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 1), 0));
      break;

    case 318:  /* *if_arith_not */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 2), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 1), 0));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (pat, 1), 1));
      break;

    case 317:  /* *ifcompare_arith_not */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[7] = *(ro_loc[7] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      break;

    case 316:  /* *if_not_arith */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 2), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 2), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 1), 0));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (pat, 1), 2));
      break;

    case 315:  /* *ifcompare_not_arith */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[7] = *(ro_loc[7] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2));
      break;

    case 313:  /* *ifcompare_shift_shift */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2), 1));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[7] = *(ro_loc[7] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[8] = *(ro_loc[8] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[9] = *(ro_loc[9] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2));
      break;

    case 312:  /* *if_move_shift */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 2), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 2), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 1), 2));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 1), 0));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      break;

    case 310:  /* *if_shift_move */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 2));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 1), 1));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 1), 0));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      break;

    case 309:  /* *ifcompare_shift_move */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[7] = *(ro_loc[7] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      break;

    case 320:  /* *if_neg_move */
    case 308:  /* *if_not_move */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 2));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 1), 0));
      break;

    case 319:  /* *ifcompare_neg_move */
    case 307:  /* *ifcompare_not_move */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      break;

    case 326:  /* *cond_move_not */
    case 322:  /* *if_move_neg */
    case 306:  /* *if_move_not */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 2), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 1), 0));
      break;

    case 321:  /* *ifcompare_move_neg */
    case 305:  /* *ifcompare_move_not */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      break;

    case 304:  /* *if_move_arith */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 2), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 2), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 1), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 1), 2));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      break;

    case 311:  /* *ifcompare_move_shift */
    case 303:  /* *ifcompare_move_arith */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[7] = *(ro_loc[7] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2));
      break;

    case 302:  /* *if_arith_move */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 2));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 1), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 1), 1));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      break;

    case 301:  /* *ifcompare_arith_move */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 1));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[7] = *(ro_loc[7] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      break;

    case 314:  /* *if_shift_shift */
    case 300:  /* *if_arith_arith */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 2), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (pat, 1), 2), 1));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (pat, 1), 0));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (pat, 1), 1));
      ro[7] = *(ro_loc[7] = &XEXP (XEXP (pat, 1), 2));
      ro[8] = *(ro_loc[8] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      break;

    case 299:  /* *ifcompare_arith_arith */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2), 1));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[7] = *(ro_loc[7] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2));
      ro[8] = *(ro_loc[8] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[9] = *(ro_loc[9] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      break;

    case 298:  /* *if_move_plus */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 2), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 2), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 1), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      break;

    case 297:  /* *ifcompare_move_plus */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      break;

    case 296:  /* *if_plus_move */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 2));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 1), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      break;

    case 295:  /* *ifcompare_plus_move */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      break;

    case 693:  /* *thumb2_movcond */
    case 294:  /* movcond */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      break;

    case 692:  /* *thumb2_negscc */
    case 293:  /* *negscc */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      break;

    case 291:  /* *and_scc_scc_cmp */
    case 289:  /* *ior_scc_scc_cmp */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1), 1));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[7] = *(ro_loc[7] = &XEXP (XVECEXP (pat, 0, 1), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[0] = 3;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0), 1);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0), 0);
      recog_data.dup_num[2] = 1;
      recog_data.dup_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1);
      recog_data.dup_num[3] = 6;
      recog_data.dup_loc[4] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1), 1);
      recog_data.dup_num[4] = 5;
      recog_data.dup_loc[5] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1), 0);
      recog_data.dup_num[5] = 4;
      break;

    case 292:  /* *and_scc_scc_nodom */
    case 290:  /* *and_scc_scc */
    case 288:  /* *ior_scc_scc */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 1));
      ro[6] = *(ro_loc[6] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      break;

    case 287:  /* *cmp_ior */
    case 286:  /* *cmp_and */
    case 285:  /* *cmp_ite1 */
    case 284:  /* *cmp_ite0 */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      ro[6] = *(ro_loc[6] = &XEXP (pat, 0));
      break;

    case 691:  /* *thumb2_cond_sub */
    case 283:  /* *cond_sub */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      break;

    case 690:  /* *thumb2_cond_arith */
    case 282:  /* *cond_arith */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (pat, 0, 0), 1));
      break;

    case 689:  /* *thumb2_cond_move */
    case 281:  /* *cond_move */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 1), 2));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0));
      break;

    case 280:  /* *compare_scc */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      break;

    case 687:  /* *thumb2_and_scc */
    case 278:  /* *and_scc */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      break;

    case 686:  /* *thumb2_sub_shiftsi_compare0_scratch */
    case 277:  /* *sub_shiftsi_compare0_scratch */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1), 1));
      break;

    case 685:  /* *thumb2_sub_shiftsi_compare0 */
    case 276:  /* *sub_shiftsi_compare0 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1), 1));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1), 1);
      recog_data.dup_num[2] = 4;
      recog_data.dup_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1), 0);
      recog_data.dup_num[3] = 3;
      break;

    case 684:  /* *thumb2_sub_shiftsi */
    case 275:  /* *sub_shiftsi */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      break;

    case 683:  /* *thumb2_arith_shiftsi_compare0_scratch */
    case 274:  /* *arith_shiftsi_compare0_scratch */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 1));
      break;

    case 682:  /* *thumb2_arith_shiftsi_compare0 */
    case 273:  /* *arith_shiftsi_compare0 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 1));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 1);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[2] = 3;
      recog_data.dup_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0), 1);
      recog_data.dup_num[3] = 5;
      recog_data.dup_loc[4] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0), 0);
      recog_data.dup_num[4] = 4;
      break;

    case 681:  /* *thumb2_arith_shiftsi */
    case 272:  /* *arith_shiftsi */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (pat, 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      break;

    case 680:  /* *thumb2_indirect_jump */
    case 270:  /* *thumb1_indirect_jump */
    case 269:  /* *load_indirect_jump */
    case 268:  /* *arm_indirect_jump */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 1));
      break;

    case 267:  /* thumb1_casesi_dispatch */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 1), 0));
      break;

    case 266:  /* arm_casesi_internal */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 2), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 2), 0), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0), 0), 0);
      recog_data.dup_num[1] = 0;
      break;

    case 482:  /* *iwmmxt_clrv2si */
    case 481:  /* *iwmmxt_clrv4hi */
    case 480:  /* *iwmmxt_clrv8qi */
    case 479:  /* iwmmxt_clrdi */
    case 349:  /* load_tp_hard */
    case 264:  /* *check_arch2 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      break;

    case 263:  /* *cond_return_inverted */
    case 262:  /* *cond_return */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      break;

    case 705:  /* *thumb2_return */
    case 641:  /* *movcc_vfp */
    case 350:  /* load_tp_soft */
    case 336:  /* consttable_end */
    case 335:  /* align_32 */
    case 334:  /* align_16 */
    case 333:  /* align_8 */
    case 332:  /* align_4 */
    case 325:  /* *epilogue_insns */
    case 324:  /* sibcall_epilogue */
    case 271:  /* nop */
    case 265:  /* blockage */
    case 261:  /* *arm_return */
      break;

    case 260:  /* *sibcall_value_insn */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 2), 0));
      break;

    case 259:  /* *sibcall_insn */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (pat, 0, 2), 0));
      break;

    case 679:  /* *call_value_reg_thumb2 */
    case 258:  /* *call_value_insn */
    case 256:  /* *call_value_symbol */
    case 254:  /* *call_value_reg_thumb1 */
    case 253:  /* *call_value_reg_thumb1_v5 */
    case 252:  /* *call_value_mem */
    case 251:  /* *call_value_reg_arm */
    case 250:  /* *call_value_reg_armv5 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      break;

    case 678:  /* *call_reg_thumb2 */
    case 257:  /* *call_insn */
    case 255:  /* *call_symbol */
    case 249:  /* *call_reg_thumb1 */
    case 248:  /* *call_reg_thumb1_v5 */
    case 247:  /* *call_mem */
    case 246:  /* *call_reg_arm */
    case 245:  /* *call_reg_armv5 */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (pat, 0, 1), 0));
      break;

    case 677:  /* *thumb2_movsfcc_soft_insn */
    case 676:  /* *thumb2_movsicc_insn */
    case 604:  /* *thumb2_movdfcc_vfp */
    case 603:  /* *movdfcc_vfp */
    case 602:  /* *thumb2_movsfcc_vfp */
    case 601:  /* *movsfcc_vfp */
    case 411:  /* *thumb2_movdfcc_fpa */
    case 410:  /* *thumb2_movsfcc_fpa */
    case 409:  /* *movdfcc_fpa */
    case 408:  /* *movsfcc_fpa */
    case 242:  /* *movsfcc_soft_insn */
    case 241:  /* *movsicc_insn */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 1), 2));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      break;

    case 240:  /* thumb1_addsi3_addgeu */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      break;

    case 675:  /* *thumb2_mov_notscc */
    case 674:  /* *thumb2_mov_negscc */
    case 235:  /* *mov_notscc */
    case 234:  /* *mov_negscc */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      break;

    case 673:  /* *thumb2_mov_scc */
    case 233:  /* *mov_scc */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (pat, 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 1), 0));
      break;

    case 232:  /* *arm_cond_branch_reversed */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (pat, 1), 2), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      break;

    case 231:  /* *arm_cond_branch */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      break;

    case 230:  /* *deleted_compare */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      recog_data.dup_loc[0] = &XEXP (pat, 1);
      recog_data.dup_num[0] = 0;
      break;

    case 226:  /* *thumb_cmpdi_zero */
    case 225:  /* *arm_cmpdi_zero */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 1), 0));
      break;

    case 223:  /* *arm_cmpdi_insn */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (pat, 0, 1), 0));
      break;

    case 222:  /* *arm_cmpsi_negshiftsi_si */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 1));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 1));
      break;

    case 671:  /* *thumb2_cmpsi_shiftsi_swp */
    case 221:  /* *arm_cmpsi_shiftsi_swp */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 1));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 0));
      break;

    case 670:  /* *thumb2_cmpsi_shiftsi */
    case 220:  /* *arm_cmpsi_shiftsi */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 1));
      break;

    case 649:  /* *cmpdf_trap_vfp */
    case 648:  /* *cmpdf_vfp */
    case 647:  /* *cmpsf_trap_vfp */
    case 646:  /* *cmpsf_vfp */
    case 645:  /* *cmpdf_trap_split_vfp */
    case 644:  /* *cmpdf_split_vfp */
    case 643:  /* *cmpsf_trap_split_vfp */
    case 642:  /* *cmpsf_split_vfp */
    case 405:  /* *cmpdf_trap_fpa */
    case 404:  /* *cmpsf_trap_fpa */
    case 401:  /* *cmpdf_fpa */
    case 400:  /* *cmpsf_fpa */
    case 229:  /* *cirrus_cmpdi */
    case 228:  /* *cirrus_cmpdf */
    case 227:  /* *cirrus_cmpsf */
    case 224:  /* *arm_cmpdi_unsigned */
    case 219:  /* *arm_cmpsi_insn */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 1));
      break;

    case 218:  /* *subsi3_cbranch_scratch */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      break;

    case 216:  /* *addsi3_cbranch_scratch */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      break;

    case 217:  /* *subsi3_cbranch */
    case 215:  /* *addsi3_cbranch */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 2), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1);
      recog_data.dup_num[1] = 3;
      break;

    case 214:  /* *cbranchne_decr1 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 2), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[0] = 2;
      break;

    case 213:  /* *bicsi3_cbranch */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 2), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0), 0);
      recog_data.dup_num[0] = 3;
      recog_data.dup_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1);
      recog_data.dup_num[1] = 2;
      break;

    case 212:  /* *bicsi3_cbranch_scratch */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      break;

    case 210:  /* *xorsi3_cbranch_scratch */
    case 208:  /* *orrsi3_cbranch_scratch */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      break;

    case 211:  /* *xorsi3_cbranch */
    case 209:  /* *orrsi3_cbranch */
    case 207:  /* *andsi3_cbranch */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 2), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1);
      recog_data.dup_num[1] = 3;
      break;

    case 206:  /* *tstsi3_cbranch */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 0));
      break;

    case 205:  /* *tlobits_cbranch */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 1), 0));
      break;

    case 204:  /* *tbit_cbranch */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 2));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 1), 0));
      break;

    case 203:  /* *negated_cbranchsi4 */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      break;

    case 202:  /* *movsi_cbranchsi4 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 1);
      recog_data.dup_num[0] = 1;
      break;

    case 201:  /* cbranchsi4_scratch */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      break;

    case 200:  /* cbranchsi4_insn */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      break;

    case 199:  /* movmem8b */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 2), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 3), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 4), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (pat, 0, 5), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 3), 1), 0);
      recog_data.dup_num[0] = 3;
      recog_data.dup_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 2), 1), 0);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 0), 0), 0);
      recog_data.dup_num[2] = 2;
      recog_data.dup_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0), 0);
      recog_data.dup_num[3] = 3;
      break;

    case 198:  /* movmem12b */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 3), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 4), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 5), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (pat, 0, 6), 0));
      ro[6] = *(ro_loc[6] = &XEXP (XVECEXP (pat, 0, 7), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 4), 1), 0);
      recog_data.dup_num[0] = 3;
      recog_data.dup_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 3), 1), 0);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 2), 0), 0), 0);
      recog_data.dup_num[2] = 2;
      recog_data.dup_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 2), 1), 0), 0);
      recog_data.dup_num[3] = 3;
      recog_data.dup_loc[4] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 0), 0), 0);
      recog_data.dup_num[4] = 2;
      recog_data.dup_loc[5] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0), 0);
      recog_data.dup_num[5] = 3;
      break;

    case 197:  /* *stmsi2 */
      ro[0] = *(ro_loc[0] = &PATTERN (insn));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (pat, 0, 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 1));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 0), 0), 0);
      recog_data.dup_num[0] = 1;
      break;

    case 196:  /* *stmsi3 */
      ro[0] = *(ro_loc[0] = &PATTERN (insn));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (pat, 0, 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 2), 1));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 2), 0), 0), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 0), 0), 0);
      recog_data.dup_num[1] = 1;
      break;

    case 195:  /* *stmsi4 */
      ro[0] = *(ro_loc[0] = &PATTERN (insn));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (pat, 0, 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 2), 1));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (pat, 0, 3), 1));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 3), 0), 0), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 2), 0), 0), 0);
      recog_data.dup_num[1] = 1;
      recog_data.dup_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 0), 0), 0);
      recog_data.dup_num[2] = 1;
      break;

    case 194:  /* *stmsi_postinc2 */
      ro[0] = *(ro_loc[0] = &PATTERN (insn));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 2), 1));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 2), 0), 0), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 0), 0);
      recog_data.dup_num[1] = 2;
      break;

    case 193:  /* *stmsi_postinc3 */
      ro[0] = *(ro_loc[0] = &PATTERN (insn));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 2), 1));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (pat, 0, 3), 1));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 3), 0), 0), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 2), 0), 0), 0);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 0), 0);
      recog_data.dup_num[2] = 2;
      break;

    case 192:  /* *stmsi_postinc4_thumb1 */
    case 191:  /* *stmsi_postinc4 */
      ro[0] = *(ro_loc[0] = &PATTERN (insn));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 2), 1));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (pat, 0, 3), 1));
      ro[6] = *(ro_loc[6] = &XEXP (XVECEXP (pat, 0, 4), 1));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 4), 0), 0), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 3), 0), 0), 0);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 2), 0), 0), 0);
      recog_data.dup_num[2] = 2;
      recog_data.dup_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 0), 0);
      recog_data.dup_num[3] = 2;
      break;

    case 190:  /* *ldmsi2 */
      ro[0] = *(ro_loc[0] = &PATTERN (insn));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0), 0);
      recog_data.dup_num[0] = 1;
      break;

    case 189:  /* *ldmsi3 */
      ro[0] = *(ro_loc[0] = &PATTERN (insn));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 2), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 2), 1), 0), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0), 0);
      recog_data.dup_num[1] = 1;
      break;

    case 188:  /* *ldmsi4 */
      ro[0] = *(ro_loc[0] = &PATTERN (insn));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 2), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (pat, 0, 3), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 3), 1), 0), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 2), 1), 0), 0);
      recog_data.dup_num[1] = 1;
      recog_data.dup_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0), 0);
      recog_data.dup_num[2] = 1;
      break;

    case 187:  /* *ldmsi_postinc2 */
      ro[0] = *(ro_loc[0] = &PATTERN (insn));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 2), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 2), 1), 0), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[1] = 2;
      break;

    case 186:  /* *ldmsi_postinc3 */
      ro[0] = *(ro_loc[0] = &PATTERN (insn));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 2), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (pat, 0, 3), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 3), 1), 0), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 2), 1), 0), 0);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[2] = 2;
      break;

    case 185:  /* *ldmsi_postinc4_thumb1 */
    case 184:  /* *ldmsi_postinc4 */
      ro[0] = *(ro_loc[0] = &PATTERN (insn));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 2), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (pat, 0, 3), 0));
      ro[6] = *(ro_loc[6] = &XEXP (XVECEXP (pat, 0, 4), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 4), 1), 0), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 3), 1), 0), 0);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 2), 1), 0), 0);
      recog_data.dup_num[2] = 2;
      recog_data.dup_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[3] = 2;
      break;

    case 172:  /* *movsi_compare0 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 1);
      recog_data.dup_num[0] = 1;
      break;

    case 171:  /* tls_load_dot_plus_eight */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (XEXP (pat, 1), 0), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XEXP (pat, 1), 0), 0, 2));
      break;

    case 170:  /* pic_add_dot_plus_eight */
    case 169:  /* pic_add_dot_plus_four */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (pat, 1), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (pat, 1), 0, 2));
      break;

    case 978:  /* reduc_splus_v2di */
    case 761:  /* *movmisalignv2di_neon_load */
    case 760:  /* *movmisalignv4sf_neon_load */
    case 759:  /* *movmisalignv4si_neon_load */
    case 758:  /* *movmisalignv8hi_neon_load */
    case 757:  /* *movmisalignv16qi_neon_load */
    case 756:  /* *movmisalignv2di_neon_store */
    case 755:  /* *movmisalignv4sf_neon_store */
    case 754:  /* *movmisalignv4si_neon_store */
    case 753:  /* *movmisalignv8hi_neon_store */
    case 752:  /* *movmisalignv16qi_neon_store */
    case 751:  /* *movmisaligndi_neon_load */
    case 750:  /* *movmisalignv2sf_neon_load */
    case 749:  /* *movmisalignv2si_neon_load */
    case 748:  /* *movmisalignv4hi_neon_load */
    case 747:  /* *movmisalignv8qi_neon_load */
    case 746:  /* *movmisaligndi_neon_store */
    case 745:  /* *movmisalignv2sf_neon_store */
    case 744:  /* *movmisalignv2si_neon_store */
    case 743:  /* *movmisalignv4hi_neon_store */
    case 742:  /* *movmisalignv8qi_neon_store */
    case 584:  /* iwmmxt_tmrc */
    case 582:  /* iwmmxt_waccw */
    case 581:  /* iwmmxt_wacch */
    case 580:  /* iwmmxt_waccb */
    case 579:  /* iwmmxt_tmovmskw */
    case 578:  /* iwmmxt_tmovmskh */
    case 577:  /* iwmmxt_tmovmskb */
    case 344:  /* rbitsi2 */
    case 168:  /* pic_load_addr_thumb1 */
    case 167:  /* pic_load_addr_32bit */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (pat, 1), 0, 0));
      break;

    case 741:  /* *neon_movxi */
    case 740:  /* *neon_movci */
    case 739:  /* *neon_movoi */
    case 738:  /* *neon_movei */
    case 737:  /* *neon_movti */
    case 736:  /* *neon_movv2di */
    case 735:  /* *neon_movv4sf */
    case 734:  /* *neon_movv4si */
    case 733:  /* *neon_movv8hi */
    case 732:  /* *neon_movv16qi */
    case 731:  /* *neon_movv2sf */
    case 730:  /* *neon_movv2si */
    case 729:  /* *neon_movv4hi */
    case 728:  /* *neon_movv8qi */
    case 669:  /* *thumb2_movdf_soft_insn */
    case 668:  /* *thumb2_movsf_soft_insn */
    case 667:  /* *thumb2_movhi_insn */
    case 665:  /* *thumb2_movsi_insn */
    case 664:  /* *thumb2_movdi */
    case 600:  /* *thumb2_movdf_vfp */
    case 599:  /* *movdf_vfp */
    case 598:  /* *thumb2_movsf_vfp */
    case 597:  /* *movsf_vfp */
    case 596:  /* *movhf_vfp */
    case 595:  /* *movhf_vfp_neon */
    case 594:  /* *thumb2_movdi_vfp */
    case 593:  /* *arm_movdi_vfp_cortexa8 */
    case 592:  /* *arm_movdi_vfp */
    case 591:  /* *thumb2_movsi_vfp */
    case 590:  /* *arm_movsi_vfp */
    case 453:  /* movv8qi_internal */
    case 452:  /* movv4hi_internal */
    case 451:  /* movv2si_internal */
    case 449:  /* *iwmmxt_movsi_insn */
    case 448:  /* *iwmmxt_arm_movdi */
    case 443:  /* *thumb2_cirrus_movdf_hard_insn */
    case 442:  /* *thumb2_cirrus_movsf_hard_insn */
    case 441:  /* *cirrus_thumb2_movdi */
    case 440:  /* *cirrus_movdf_hard_insn */
    case 439:  /* *cirrus_movsf_hard_insn */
    case 438:  /* *cirrus_arm_movdi */
    case 399:  /* *thumb2_movxf_fpa */
    case 398:  /* *thumb2_movdf_fpa */
    case 397:  /* *thumb2_movsf_fpa */
    case 396:  /* *movxf_fpa */
    case 395:  /* *movdf_fpa */
    case 394:  /* *movsf_fpa */
    case 183:  /* *thumb_movdf_insn */
    case 182:  /* *movdf_soft_insn */
    case 181:  /* *thumb1_movsf_insn */
    case 180:  /* *arm_movsf_soft_insn */
    case 179:  /* *thumb1_movhf */
    case 178:  /* *arm32_movhf */
    case 177:  /* *thumb1_movqi_insn */
    case 176:  /* *arm_movqi_insn */
    case 175:  /* *movhi_bytes */
    case 174:  /* *movhi_insn_arch4 */
    case 173:  /* *thumb1_movhi_insn */
    case 166:  /* *thumb1_movsi_insn */
    case 165:  /* *arm_movsi_insn */
    case 163:  /* *thumb1_movdi_insn */
    case 162:  /* *arm_movdi */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (pat, 1));
      break;

    case 244:  /* *thumb_jump */
    case 243:  /* *arm_jump */
    case 152:  /* *compareqi_eq0 */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      break;

    case 139:  /* *notsi_compare0_scratch */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      break;

    case 138:  /* *notsi_compare0 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[0] = 1;
      break;

    case 435:  /* cirrus_truncdfsi2 */
    case 434:  /* cirrus_truncsfsi2 */
    case 134:  /* *thumb1_neg_abssi2 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (pat, 0, 1), 0));
      break;

    case 663:  /* *thumb2_neg_abssi2 */
    case 133:  /* *arm_neg_abssi2 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      break;

    case 431:  /* cirrus_floatsidf2 */
    case 430:  /* cirrus_floatsisf2 */
    case 237:  /* *cstoresi_ne0_thumb1_insn */
    case 236:  /* *cstoresi_eq0_thumb1_insn */
    case 153:  /* thumb1_extendhisi2 */
    case 132:  /* *thumb1_abssi2 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (pat, 0, 1), 0));
      break;

    case 126:  /* extzv_t2 */
    case 125:  /* extv */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 2));
      break;

    case 660:  /* *thumb2_not_shiftsi_compare0_scratch */
    case 124:  /* *arm_not_shiftsi_compare0_scratch */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      break;

    case 659:  /* *thumb2_notsi_shiftsi_compare0 */
    case 123:  /* *arm_notsi_shiftsi_compare0 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[0] = 3;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0), 1);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0), 0);
      recog_data.dup_num[2] = 1;
      break;

    case 658:  /* *thumb2_notsi_shiftsi */
    case 122:  /* *arm_notsi_shiftsi */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 0));
      break;

    case 121:  /* *shiftsi3_compare0_scratch */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      break;

    case 120:  /* *shiftsi3_compare0 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 1);
      recog_data.dup_num[0] = 3;
      recog_data.dup_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[2] = 1;
      break;

    case 119:  /* *arm_shiftsi3 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 1));
      break;

    case 724:  /* *thumb2_negsi2_short */
    case 723:  /* *thumb2_one_cmplsi2_short */
    case 662:  /* *thumb2_abssi2 */
    case 661:  /* *thumb2_negdi2 */
    case 425:  /* *cirrus_negdi2 */
    case 131:  /* *arm_abssi2 */
    case 128:  /* *thumb1_negdi2 */
    case 127:  /* *arm_negdi2 */
    case 116:  /* arm_lshrdi3_1bit */
    case 114:  /* arm_ashrdi3_1bit */
    case 112:  /* arm_ashldi3_1bit */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      break;

    case 111:  /* *minmax_arithsi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 0), 1));
      ro[5] = *(ro_loc[5] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      break;

    case 708:  /* *thumb2_shiftsi3_short */
    case 707:  /* *thumb2_alusi3_short */
    case 110:  /* *store_minmaxsi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 0), 1));
      break;

    case 1727:  /* neon_unpacku_v2si */
    case 1726:  /* neon_unpacks_v2si */
    case 1725:  /* neon_unpacku_v4hi */
    case 1724:  /* neon_unpacks_v4hi */
    case 1723:  /* neon_unpacku_v8qi */
    case 1722:  /* neon_unpacks_v8qi */
    case 1315:  /* neon_vget_lowv2di */
    case 1314:  /* neon_vget_lowv4sf */
    case 1313:  /* neon_vget_lowv4si */
    case 1312:  /* neon_vget_lowv8hi */
    case 1311:  /* neon_vget_lowv16qi */
    case 1310:  /* neon_vget_highv2di */
    case 1309:  /* neon_vget_highv4sf */
    case 1308:  /* neon_vget_highv4si */
    case 1307:  /* neon_vget_highv8hi */
    case 1306:  /* neon_vget_highv16qi */
    case 1292:  /* neon_vdup_nv2di */
    case 1291:  /* neon_vdup_nv4sf */
    case 1290:  /* neon_vdup_nv4si */
    case 1289:  /* neon_vdup_nv2sf */
    case 1288:  /* neon_vdup_nv2si */
    case 1287:  /* neon_vdup_nv8hi */
    case 1286:  /* neon_vdup_nv16qi */
    case 1285:  /* neon_vdup_nv4hi */
    case 1284:  /* neon_vdup_nv8qi */
    case 1259:  /* popcountv16qi2 */
    case 1258:  /* popcountv8qi2 */
    case 1257:  /* clzv4si2 */
    case 1256:  /* clzv2si2 */
    case 1255:  /* clzv8hi2 */
    case 1254:  /* clzv4hi2 */
    case 1253:  /* clzv16qi2 */
    case 1252:  /* clzv8qi2 */
    case 901:  /* negv4sf2 */
    case 900:  /* negv2sf2 */
    case 899:  /* negv4si2 */
    case 898:  /* negv2si2 */
    case 897:  /* negv8hi2 */
    case 896:  /* negv4hi2 */
    case 895:  /* negv16qi2 */
    case 894:  /* negv8qi2 */
    case 893:  /* absv4sf2 */
    case 892:  /* absv2sf2 */
    case 891:  /* absv4si2 */
    case 890:  /* absv2si2 */
    case 889:  /* absv8hi2 */
    case 888:  /* absv4hi2 */
    case 887:  /* absv16qi2 */
    case 886:  /* absv8qi2 */
    case 885:  /* one_cmplv2di2 */
    case 884:  /* one_cmplv4sf2 */
    case 883:  /* one_cmplv2sf2 */
    case 882:  /* one_cmplv4si2 */
    case 881:  /* one_cmplv2si2 */
    case 880:  /* one_cmplv8hi2 */
    case 879:  /* one_cmplv4hi2 */
    case 878:  /* one_cmplv16qi2 */
    case 877:  /* one_cmplv8qi2 */
    case 702:  /* *thumb2_zero_extendqisi2_v6 */
    case 701:  /* *thumb2_zero_extendhisi2_v6 */
    case 700:  /* *thumb2_extendqisi_v6 */
    case 699:  /* *thumb2_extendqidi2 */
    case 698:  /* *thumb2_extendhidi2 */
    case 697:  /* *thumb2_extendsidi2 */
    case 696:  /* *thumb2_zero_extendqidi2 */
    case 695:  /* *thumb2_zero_extendhidi2 */
    case 694:  /* *thumb2_zero_extendsidi2 */
    case 640:  /* *sqrtdf2_vfp */
    case 639:  /* *sqrtsf2_vfp */
    case 638:  /* floatunssidf2 */
    case 637:  /* floatunssisf2 */
    case 636:  /* *floatsidf2_vfp */
    case 635:  /* *floatsisf2_vfp */
    case 630:  /* truncsfhf2 */
    case 629:  /* extendhfsf2 */
    case 628:  /* *truncdfsf2_vfp */
    case 627:  /* *extendsfdf2_vfp */
    case 608:  /* *negdf2_vfp */
    case 607:  /* *negsf2_vfp */
    case 606:  /* *absdf2_vfp */
    case 605:  /* *abssf2_vfp */
    case 576:  /* iwmmxt_tbcstsi */
    case 575:  /* iwmmxt_tbcsthi */
    case 574:  /* iwmmxt_tbcstqi */
    case 437:  /* *cirrus_extendsfdf2 */
    case 436:  /* *cirrus_truncdfsf2 */
    case 433:  /* floatdidf2 */
    case 432:  /* floatdisf2 */
    case 429:  /* *cirrus_absdf2 */
    case 428:  /* *cirrus_abssf2 */
    case 427:  /* *cirrus_negdf2 */
    case 426:  /* *cirrus_negsf2 */
    case 424:  /* *cirrus_absdi2 */
    case 393:  /* *extendsfdf2_fpa */
    case 392:  /* *truncdfsf2_fpa */
    case 389:  /* *floatsidf2_fpa */
    case 388:  /* *floatsisf2_fpa */
    case 386:  /* *sqrtdf2_fpa */
    case 385:  /* *sqrtsf2_fpa */
    case 383:  /* *absdf2_fpa */
    case 382:  /* *abssf2_fpa */
    case 380:  /* *negdf2_fpa */
    case 379:  /* *negsf2_fpa */
    case 353:  /* *thumb1_rev */
    case 352:  /* *arm_rev */
    case 343:  /* clzsi2 */
    case 161:  /* thumb1_extendqisi2 */
    case 159:  /* *arm_extendqisi_v6 */
    case 158:  /* *arm_extendqisi */
    case 157:  /* *arm_extendqihi_insn */
    case 155:  /* *arm_extendhisi2_v6 */
    case 154:  /* *arm_extendhisi2 */
    case 150:  /* *arm_zero_extendqisi2_v6 */
    case 149:  /* *arm_zero_extendqisi2 */
    case 148:  /* *thumb1_zero_extendqisi2_v6 */
    case 147:  /* *thumb1_zero_extendqisi2 */
    case 145:  /* *arm_zero_extendhisi2_v6 */
    case 144:  /* *arm_zero_extendhisi2 */
    case 143:  /* *thumb1_zero_extendhisi2 */
    case 142:  /* *arm_extendsidi2 */
    case 141:  /* *arm_zero_extendqidi2 */
    case 140:  /* *arm_zero_extendsidi2 */
    case 137:  /* *thumb1_one_cmplsi2 */
    case 136:  /* *arm_one_cmplsi2 */
    case 135:  /* one_cmpldi2 */
    case 130:  /* *thumb1_negsi2 */
    case 129:  /* *arm_negsi2 */
    case 106:  /* *smin_0 */
    case 104:  /* *smax_m1 */
    case 103:  /* *smax_0 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      break;

    case 102:  /* *andsi_iorsi3_notsi */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      break;

    case 720:  /* *thumb2_mulsi_short_compare0_scratch */
    case 94:  /* *iorsi3_compare0_scratch */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      break;

    case 87:  /* *andsi_notsi_si_compare0_scratch */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0));
      break;

    case 86:  /* *andsi_notsi_si_compare0 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1);
      recog_data.dup_num[1] = 1;
      break;

    case 726:  /* *thumb_orsi_not_shiftsi_si */
    case 653:  /* *thumb_andsi_not_shiftsi_si */
    case 85:  /* andsi_not_shiftsi_si */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      break;

    case 82:  /* *anddi_notsesidi_di */
    case 81:  /* *anddi_notzesidi_di */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0));
      break;

    case 955:  /* widen_usumv2si3 */
    case 954:  /* widen_usumv4hi3 */
    case 953:  /* widen_usumv8qi3 */
    case 952:  /* widen_ssumv2si3 */
    case 951:  /* widen_ssumv4hi3 */
    case 950:  /* widen_ssumv8qi3 */
    case 618:  /* *muldf3negdf_vfp */
    case 617:  /* *mulsf3negsf_vfp */
    case 376:  /* *moddf_esfdf_df_fpa */
    case 371:  /* *divdf_esfdf_df_fpa */
    case 366:  /* *muldf_esfdf_df_fpa */
    case 361:  /* *subdf_esfdf_df_fpa */
    case 356:  /* *adddf_esfdf_df_fpa */
    case 160:  /* *arm_extendqisi2addsi */
    case 156:  /* *arm_extendhisi2addsi */
    case 151:  /* *arm_zero_extendqisi2addsi */
    case 146:  /* *arm_zero_extendhisi2addsi */
    case 84:  /* bicsi3 */
    case 80:  /* *anddi_notdi_di */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 1), 1));
      break;

    case 79:  /* insv_t2 */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 0), 2));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 1));
      break;

    case 78:  /* insv_zero */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 0), 2));
      break;

    case 77:  /* *ite_ne_zeroextractsi_shifted */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      break;

    case 76:  /* *ite_ne_zeroextractsi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 2));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      break;

    case 75:  /* *ne_zeroextractsi_shifted */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      break;

    case 74:  /* *ne_zeroextractsi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 2));
      break;

    case 73:  /* *zeroextractsi_compare0_scratch */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 2));
      break;

    case 72:  /* *andsi3_compare0_scratch */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (pat, 0, 1), 0));
      break;

    case 866:  /* bicdi3_neon */
    case 725:  /* orsi_notsi_si */
    case 97:  /* *xordi_sesidi_di */
    case 96:  /* *xordi_zesidi_di */
    case 90:  /* *iordi_sesidi_di */
    case 89:  /* *iordi_zesidi_di */
    case 83:  /* andsi_notsi_si */
    case 68:  /* *anddi_sesdi_di */
    case 67:  /* *anddi_zesidi_di */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      break;

    case 65:  /* *maddhidi4 */
    case 64:  /* maddhisi4 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 1));
      break;

    case 59:  /* *umulsi3_highpart_v6 */
    case 58:  /* *umulsi3_highpart_nov6 */
    case 57:  /* *smulsi3_highpart_v6 */
    case 56:  /* *smulsi3_highpart_nov6 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      break;

    case 1733:  /* neon_vec_umult_v2si */
    case 1732:  /* neon_vec_smult_v2si */
    case 1731:  /* neon_vec_umult_v4hi */
    case 1730:  /* neon_vec_smult_v4hi */
    case 1729:  /* neon_vec_umult_v8qi */
    case 1728:  /* neon_vec_smult_v8qi */
    case 529:  /* iwmmxt_wunpckilw */
    case 528:  /* iwmmxt_wunpckilh */
    case 527:  /* iwmmxt_wunpckilb */
    case 526:  /* iwmmxt_wunpckihw */
    case 525:  /* iwmmxt_wunpckihh */
    case 524:  /* iwmmxt_wunpckihb */
    case 523:  /* iwmmxt_wpackdus */
    case 522:  /* iwmmxt_wpackwus */
    case 521:  /* iwmmxt_wpackhus */
    case 520:  /* iwmmxt_wpackdss */
    case 519:  /* iwmmxt_wpackwss */
    case 518:  /* iwmmxt_wpackhss */
    case 378:  /* *moddf_esfdf_esfdf_fpa */
    case 373:  /* *divdf_esfdf_esfdf_fpa */
    case 368:  /* *muldf_esfdf_esfdf_fpa */
    case 363:  /* *subdf_esfdf_esfdf_fpa */
    case 358:  /* *adddf_esfdf_esfdf_fpa */
    case 63:  /* *mulhisi3tt */
    case 62:  /* *mulhisi3bt */
    case 61:  /* *mulhisi3tb */
    case 60:  /* mulhisi3 */
    case 53:  /* *umulsidi3_v6 */
    case 52:  /* *umulsidi3_nov6 */
    case 51:  /* *mulsidi3_v6 */
    case 50:  /* *mulsidi3_nov6 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      break;

    case 55:  /* *umulsidi3adddi_v6 */
    case 54:  /* *umulsidi3adddi */
    case 49:  /* *mulsidi3adddi_v6 */
    case 48:  /* *mulsidi3adddi */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 1), 0));
      break;

    case 47:  /* *mulsi3subsi */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 0));
      break;

    case 46:  /* *mulsi3addsi_compare0_scratch_v6 */
    case 45:  /* *mulsi3addsi_compare0_scratch */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      break;

    case 44:  /* *mulsi3addsi_compare0_v6 */
    case 43:  /* *mulsi3addsi_compare0 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0), 1);
      recog_data.dup_num[1] = 1;
      recog_data.dup_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1);
      recog_data.dup_num[2] = 3;
      break;

    case 42:  /* *mulsi3addsi_v6 */
    case 41:  /* *mulsi3addsi */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 1));
      break;

    case 40:  /* *mulsi_compare0_scratch_v6 */
    case 39:  /* *mulsi_compare0_scratch */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      break;

    case 38:  /* *mulsi3_compare0_v6 */
    case 37:  /* *mulsi3_compare0 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1);
      recog_data.dup_num[1] = 1;
      break;

    case 418:  /* muldi3 */
    case 33:  /* *arm_mulsi3 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 1), 0));
      break;

    case 652:  /* *thumb2_decscc */
    case 32:  /* *arm_decscc */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      break;

    case 31:  /* *subsi3_compare */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1);
      recog_data.dup_num[1] = 2;
      break;

    case 27:  /* *subdi_zesidi_zesidi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      break;

    case 24:  /* *subdi_di_sesidi */
    case 23:  /* *subdi_di_zesidi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      break;

    case 688:  /* *thumb2_ior_scc */
    case 651:  /* *thumb2_incscc */
    case 279:  /* *ior_scc */
    case 20:  /* *arm_incscc */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      break;

    case 19:  /* *addsi3_carryin_shift_geu */
    case 18:  /* *addsi3_carryin_shift_ltu */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 1));
      break;

    case 17:  /* *addsi3_carryin_alt2_geu */
    case 16:  /* *addsi3_carryin_alt2_ltu */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 1), 1));
      break;

    case 1709:  /* neon_vec_unpacku_hi_v4si */
    case 1708:  /* neon_vec_unpacks_hi_v4si */
    case 1707:  /* neon_vec_unpacku_hi_v8hi */
    case 1706:  /* neon_vec_unpacks_hi_v8hi */
    case 1705:  /* neon_vec_unpacku_hi_v16qi */
    case 1704:  /* neon_vec_unpacks_hi_v16qi */
    case 1703:  /* neon_vec_unpacku_lo_v4si */
    case 1702:  /* neon_vec_unpacks_lo_v4si */
    case 1701:  /* neon_vec_unpacku_lo_v8hi */
    case 1700:  /* neon_vec_unpacks_lo_v8hi */
    case 1699:  /* neon_vec_unpacku_lo_v16qi */
    case 1698:  /* neon_vec_unpacks_lo_v16qi */
    case 487:  /* iwmmxt_psadbw */
    case 486:  /* iwmmxt_uavgv4hi3 */
    case 485:  /* iwmmxt_uavgv8qi3 */
    case 238:  /* cstoresi_nltu_thumb1 */
    case 15:  /* *addsi3_carryin_geu */
    case 14:  /* *addsi3_carryin_ltu */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      break;

    case 13:  /* *compare_addsi2_op1 */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      recog_data.dup_loc[0] = &XEXP (XEXP (pat, 1), 1);
      recog_data.dup_num[0] = 1;
      break;

    case 12:  /* *compare_addsi2_op0 */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      recog_data.dup_loc[0] = &XEXP (XEXP (pat, 1), 1);
      recog_data.dup_num[0] = 0;
      break;

    case 11:  /* *addsi3_compare_op2 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1);
      recog_data.dup_num[2] = 2;
      break;

    case 10:  /* *addsi3_compare_op1 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1);
      recog_data.dup_num[2] = 1;
      break;

    case 9:  /* *cmpsi2_addneg */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[0] = 1;
      break;

    case 406:  /* *cmp_esfdf_df_trap_fpa */
    case 402:  /* *cmpesfdf_df_fpa */
    case 8:  /* *compare_negsi_si */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 1));
      break;

    case 717:  /* *thumb2_addsi3_compare0_scratch */
    case 101:  /* *xorsi3_compare0_scratch */
    case 7:  /* *addsi3_compare0_scratch */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      break;

    case 719:  /* *thumb2_mulsi_short_compare0 */
    case 716:  /* *thumb2_addsi3_compare0 */
    case 100:  /* *xorsi3_compare0 */
    case 93:  /* *iorsi3_compare0 */
    case 71:  /* *andsi3_compare0 */
    case 30:  /* *subsi3_compare0 */
    case 6:  /* *addsi3_compare0 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1);
      recog_data.dup_num[1] = 2;
      break;

    case 1305:  /* neon_vcombinedi */
    case 1304:  /* neon_vcombinev2sf */
    case 1303:  /* neon_vcombinev2si */
    case 1302:  /* neon_vcombinev4hi */
    case 1301:  /* neon_vcombinev8qi */
    case 1012:  /* *us_subv2sf_neon */
    case 1011:  /* *us_subv2si_neon */
    case 1010:  /* *us_subv4hi_neon */
    case 1009:  /* *us_subv8qi_neon */
    case 1008:  /* *ss_subv2sf_neon */
    case 1007:  /* *ss_subv2si_neon */
    case 1006:  /* *ss_subv4hi_neon */
    case 1005:  /* *ss_subv8qi_neon */
    case 1004:  /* *us_addv2sf_neon */
    case 1003:  /* *us_addv2si_neon */
    case 1002:  /* *us_addv4hi_neon */
    case 1001:  /* *us_addv8qi_neon */
    case 1000:  /* *ss_addv2sf_neon */
    case 999:  /* *ss_addv2si_neon */
    case 998:  /* *ss_addv4hi_neon */
    case 997:  /* *ss_addv8qi_neon */
    case 935:  /* vashlv4si3 */
    case 934:  /* vashlv2si3 */
    case 933:  /* vashlv8hi3 */
    case 932:  /* vashlv4hi3 */
    case 931:  /* vashlv16qi3 */
    case 930:  /* vashlv8qi3 */
    case 929:  /* *smaxv4sf3_neon */
    case 928:  /* *smaxv2sf3_neon */
    case 927:  /* *smaxv4si3_neon */
    case 926:  /* *smaxv2si3_neon */
    case 925:  /* *smaxv8hi3_neon */
    case 924:  /* *smaxv4hi3_neon */
    case 923:  /* *smaxv16qi3_neon */
    case 922:  /* *smaxv8qi3_neon */
    case 921:  /* *sminv4sf3_neon */
    case 920:  /* *sminv2sf3_neon */
    case 919:  /* *sminv4si3_neon */
    case 918:  /* *sminv2si3_neon */
    case 917:  /* *sminv8hi3_neon */
    case 916:  /* *sminv4hi3_neon */
    case 915:  /* *sminv16qi3_neon */
    case 914:  /* *sminv8qi3_neon */
    case 913:  /* *umaxv4si3_neon */
    case 912:  /* *umaxv2si3_neon */
    case 911:  /* *umaxv8hi3_neon */
    case 910:  /* *umaxv4hi3_neon */
    case 909:  /* *umaxv16qi3_neon */
    case 908:  /* *umaxv8qi3_neon */
    case 907:  /* *uminv4si3_neon */
    case 906:  /* *uminv2si3_neon */
    case 905:  /* *uminv8hi3_neon */
    case 904:  /* *uminv4hi3_neon */
    case 903:  /* *uminv16qi3_neon */
    case 902:  /* *uminv8qi3_neon */
    case 876:  /* xordi3_neon */
    case 875:  /* xorv2di3 */
    case 874:  /* xorv4sf3 */
    case 873:  /* xorv2sf3 */
    case 872:  /* xorv4si3 */
    case 871:  /* xorv2si3 */
    case 870:  /* xorv8hi3 */
    case 869:  /* xorv4hi3 */
    case 868:  /* xorv16qi3 */
    case 867:  /* xorv8qi3 */
    case 846:  /* anddi3_neon */
    case 845:  /* andv2di3 */
    case 844:  /* andv4sf3 */
    case 843:  /* andv2sf3 */
    case 842:  /* andv4si3 */
    case 841:  /* andv2si3 */
    case 840:  /* andv8hi3 */
    case 839:  /* andv4hi3 */
    case 838:  /* andv16qi3 */
    case 837:  /* andv8qi3 */
    case 836:  /* iordi3_neon */
    case 835:  /* iorv2di3 */
    case 834:  /* iorv4sf3 */
    case 833:  /* iorv2sf3 */
    case 832:  /* iorv4si3 */
    case 831:  /* iorv2si3 */
    case 830:  /* iorv8hi3 */
    case 829:  /* iorv4hi3 */
    case 828:  /* iorv16qi3 */
    case 827:  /* iorv8qi3 */
    case 808:  /* *mulv2di3_neon */
    case 807:  /* *mulv4sf3_neon */
    case 806:  /* *mulv2sf3_neon */
    case 805:  /* *mulv4si3_neon */
    case 804:  /* *mulv2si3_neon */
    case 803:  /* *mulv8hi3_neon */
    case 802:  /* *mulv4hi3_neon */
    case 801:  /* *mulv16qi3_neon */
    case 800:  /* *mulv8qi3_neon */
    case 798:  /* *subv2di3_neon */
    case 797:  /* *subv4sf3_neon */
    case 796:  /* *subv2sf3_neon */
    case 795:  /* *subv4si3_neon */
    case 794:  /* *subv2si3_neon */
    case 793:  /* *subv8hi3_neon */
    case 792:  /* *subv4hi3_neon */
    case 791:  /* *subv16qi3_neon */
    case 790:  /* *subv8qi3_neon */
    case 788:  /* *addv2di3_neon */
    case 787:  /* *addv4sf3_neon */
    case 786:  /* *addv2sf3_neon */
    case 785:  /* *addv4si3_neon */
    case 784:  /* *addv2si3_neon */
    case 783:  /* *addv8hi3_neon */
    case 782:  /* *addv4hi3_neon */
    case 781:  /* *addv16qi3_neon */
    case 780:  /* *addv8qi3_neon */
    case 727:  /* *thumb2_iorsi3 */
    case 714:  /* udivsi3 */
    case 713:  /* divsi3 */
    case 616:  /* *muldf3_vfp */
    case 615:  /* *mulsf3_vfp */
    case 614:  /* *divdf3_vfp */
    case 613:  /* *divsf3_vfp */
    case 612:  /* *subdf3_vfp */
    case 611:  /* *subsf3_vfp */
    case 610:  /* *adddf3_vfp */
    case 609:  /* *addsf3_vfp */
    case 565:  /* ashldi3_di */
    case 564:  /* ashlv2si3_di */
    case 563:  /* ashlv4hi3_di */
    case 562:  /* lshrdi3_di */
    case 561:  /* lshrv2si3_di */
    case 560:  /* lshrv4hi3_di */
    case 559:  /* ashrdi3_di */
    case 558:  /* ashrv2si3_di */
    case 557:  /* ashrv4hi3_di */
    case 556:  /* rordi3_di */
    case 555:  /* rorv2si3_di */
    case 554:  /* rorv4hi3_di */
    case 553:  /* ashldi3_iwmmxt */
    case 552:  /* ashlv2si3_iwmmxt */
    case 551:  /* ashlv4hi3_iwmmxt */
    case 550:  /* lshrdi3_iwmmxt */
    case 549:  /* lshrv2si3_iwmmxt */
    case 548:  /* lshrv4hi3_iwmmxt */
    case 547:  /* ashrdi3_iwmmxt */
    case 546:  /* ashrv2si3_iwmmxt */
    case 545:  /* ashrv4hi3_iwmmxt */
    case 544:  /* rordi3 */
    case 543:  /* rorv2si3 */
    case 542:  /* rorv4hi3 */
    case 517:  /* *uminv8qi3_iwmmxt */
    case 516:  /* *uminv4hi3_iwmmxt */
    case 515:  /* *uminv2si3_iwmmxt */
    case 514:  /* *sminv8qi3_iwmmxt */
    case 513:  /* *sminv4hi3_iwmmxt */
    case 512:  /* *sminv2si3_iwmmxt */
    case 511:  /* *umaxv8qi3_iwmmxt */
    case 510:  /* *umaxv4hi3_iwmmxt */
    case 509:  /* *umaxv2si3_iwmmxt */
    case 508:  /* *smaxv8qi3_iwmmxt */
    case 507:  /* *smaxv4hi3_iwmmxt */
    case 506:  /* *smaxv2si3_iwmmxt */
    case 472:  /* *mulv4hi3_iwmmxt */
    case 471:  /* ussubv2si3 */
    case 470:  /* ussubv4hi3 */
    case 469:  /* ussubv8qi3 */
    case 468:  /* sssubv2si3 */
    case 467:  /* sssubv4hi3 */
    case 466:  /* sssubv8qi3 */
    case 465:  /* *subv8qi3_iwmmxt */
    case 464:  /* *subv4hi3_iwmmxt */
    case 463:  /* *subv2si3_iwmmxt */
    case 462:  /* usaddv2si3 */
    case 461:  /* usaddv4hi3 */
    case 460:  /* usaddv8qi3 */
    case 459:  /* ssaddv2si3 */
    case 458:  /* ssaddv4hi3 */
    case 457:  /* ssaddv8qi3 */
    case 456:  /* *addv8qi3_iwmmxt */
    case 455:  /* *addv4hi3_iwmmxt */
    case 454:  /* *addv2si3_iwmmxt */
    case 446:  /* iwmmxt_anddi3 */
    case 445:  /* iwmmxt_xordi3 */
    case 444:  /* iwmmxt_iordi3 */
    case 423:  /* cirrus_ashiftrtdi_const */
    case 422:  /* cirrus_ashldi_const */
    case 421:  /* ashldi3_cirrus */
    case 420:  /* *cirrus_muldf3 */
    case 419:  /* *cirrus_mulsf3 */
    case 417:  /* *cirrus_subdf3 */
    case 416:  /* *cirrus_subsf3 */
    case 415:  /* cirrus_subdi3 */
    case 414:  /* *cirrus_adddf3 */
    case 413:  /* *cirrus_addsf3 */
    case 412:  /* cirrus_adddi3 */
    case 375:  /* *moddf3_fpa */
    case 374:  /* *modsf3_fpa */
    case 370:  /* *divdf3_fpa */
    case 369:  /* *divsf3_fpa */
    case 365:  /* *muldf3_fpa */
    case 364:  /* *mulsf3_fpa */
    case 360:  /* *subdf3_fpa */
    case 359:  /* *subsf3_fpa */
    case 355:  /* *adddf3_fpa */
    case 354:  /* *addsf3_fpa */
    case 239:  /* cstoresi_ltu_thumb1 */
    case 164:  /* *arm_movt */
    case 118:  /* *thumb1_rotrsi3 */
    case 117:  /* *thumb1_lshrsi3 */
    case 115:  /* *thumb1_ashrsi3 */
    case 113:  /* *thumb1_ashlsi3 */
    case 99:  /* *thumb1_xorsi3 */
    case 98:  /* *arm_xorsi3 */
    case 95:  /* *xordi3_insn */
    case 92:  /* *thumb1_iorsi3 */
    case 91:  /* *arm_iorsi3 */
    case 88:  /* *iordi3_insn */
    case 70:  /* *thumb1_andsi3_insn */
    case 69:  /* *arm_andsi3_insn */
    case 66:  /* *anddi3_insn */
    case 36:  /* *thumb_mulsi3_v6 */
    case 35:  /* *thumb_mulsi3 */
    case 34:  /* *arm_mulsi3_v6 */
    case 29:  /* *arm_subsi3_insn */
    case 28:  /* *thumb1_subsi3_insn */
    case 5:  /* *thumb1_addsi3 */
    case 4:  /* *arm_addsi3 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 1), 1));
      break;

    case 26:  /* *subdi_sesidi_di */
    case 25:  /* *subdi_zesidi_di */
    case 3:  /* *adddi_zesidi_di */
    case 2:  /* *adddi_sesidi_di */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      break;

    case 799:  /* subdi3_neon */
    case 789:  /* adddi3_neon */
    case 718:  /* *thumb2_mulsi_short */
    case 715:  /* *thumb2_subsi_short */
    case 712:  /* *thumb2_addsi_short */
    case 657:  /* *thumb2_uminsi3 */
    case 656:  /* *thumb32_umaxsi3 */
    case 655:  /* *thumb2_sminsi3 */
    case 654:  /* *thumb2_smaxsi3 */
    case 109:  /* *arm_uminsi3 */
    case 108:  /* *arm_umaxsi3 */
    case 107:  /* *arm_smin_insn */
    case 105:  /* *arm_smax_insn */
    case 22:  /* *thumb_subdi3 */
    case 21:  /* *arm_subdi3 */
    case 1:  /* *arm_adddi3 */
    case 0:  /* *thumb1_adddi3 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      break;

    }
}
