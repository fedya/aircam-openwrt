#undef  STARTFILE_SPEC
#define STARTFILE_SPEC "\
%{msim:%{!shared:crt0%O%s}} \
crti%O%s crtbegin%O%s"

#undef  ENDFILE_SPEC
#define ENDFILE_SPEC	"crtend%O%s crtn%O%s"

#ifdef __UBICOM32_FDPIC__
#define CRT_CALL_STATIC_FUNCTION(SECTION_OP, FUNC)			\
  asm (SECTION_OP);							\
  asm ("move.4 a0, 0(sp);\n\t"						\
       "call a5," USER_LABEL_PREFIX #FUNC ";");				\
  asm (TEXT_SECTION_ASM_OP);
#endif

#undef SUBTARGET_DRIVER_SELF_SPECS
#define SUBTARGET_DRIVER_SELF_SPECS \
     "%{mfdpic:-msim} "

#define NO_IMPLICIT_EXTERN_C

/*
 * We need this to compile crtbegin/crtend. This should really be picked
 * up from elfos.h but at the moment including elfos.h causes other more
 * serous linker issues.
 */
#define INIT_SECTION_ASM_OP	"\t.section\t.init"
#define FINI_SECTION_ASM_OP	"\t.section\t.fini"
