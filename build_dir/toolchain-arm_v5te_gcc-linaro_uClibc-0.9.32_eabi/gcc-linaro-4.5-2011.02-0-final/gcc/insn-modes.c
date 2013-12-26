/* Generated automatically from machmode.def and config/arm/arm-modes.def
   by genmodes.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "machmode.h"
#include "real.h"

const char *const mode_name[NUM_MACHINE_MODES] =
{
  "VOID",
  "BLK",
  "CC",
  "CC_NOOV",
  "CC_Z",
  "CC_CZ",
  "CC_NCV",
  "CC_SWP",
  "CCFP",
  "CCFPE",
  "CC_DNE",
  "CC_DEQ",
  "CC_DLE",
  "CC_DLT",
  "CC_DGE",
  "CC_DGT",
  "CC_DLEU",
  "CC_DLTU",
  "CC_DGEU",
  "CC_DGTU",
  "CC_C",
  "CC_N",
  "BI",
  "QI",
  "HI",
  "SI",
  "DI",
  "TI",
  "EI",
  "OI",
  "CI",
  "XI",
  "QQ",
  "HQ",
  "SQ",
  "DQ",
  "TQ",
  "UQQ",
  "UHQ",
  "USQ",
  "UDQ",
  "UTQ",
  "HA",
  "SA",
  "DA",
  "TA",
  "UHA",
  "USA",
  "UDA",
  "UTA",
  "HF",
  "SF",
  "DF",
  "XF",
  "SD",
  "DD",
  "TD",
  "CQI",
  "CHI",
  "CSI",
  "CDI",
  "CTI",
  "CEI",
  "COI",
  "CCI",
  "CXI",
  "HC",
  "SC",
  "DC",
  "XC",
  "V4QI",
  "V2HI",
  "V8QI",
  "V4HI",
  "V2SI",
  "V16QI",
  "V8HI",
  "V4SI",
  "V2DI",
  "V4HF",
  "V2SF",
  "V8HF",
  "V4SF",
  "V2DF",
};

const unsigned char mode_class[NUM_MACHINE_MODES] =
{
  MODE_RANDOM,             /* VOID */
  MODE_RANDOM,             /* BLK */
  MODE_CC,                 /* CC */
  MODE_CC,                 /* CC_NOOV */
  MODE_CC,                 /* CC_Z */
  MODE_CC,                 /* CC_CZ */
  MODE_CC,                 /* CC_NCV */
  MODE_CC,                 /* CC_SWP */
  MODE_CC,                 /* CCFP */
  MODE_CC,                 /* CCFPE */
  MODE_CC,                 /* CC_DNE */
  MODE_CC,                 /* CC_DEQ */
  MODE_CC,                 /* CC_DLE */
  MODE_CC,                 /* CC_DLT */
  MODE_CC,                 /* CC_DGE */
  MODE_CC,                 /* CC_DGT */
  MODE_CC,                 /* CC_DLEU */
  MODE_CC,                 /* CC_DLTU */
  MODE_CC,                 /* CC_DGEU */
  MODE_CC,                 /* CC_DGTU */
  MODE_CC,                 /* CC_C */
  MODE_CC,                 /* CC_N */
  MODE_INT,                /* BI */
  MODE_INT,                /* QI */
  MODE_INT,                /* HI */
  MODE_INT,                /* SI */
  MODE_INT,                /* DI */
  MODE_INT,                /* TI */
  MODE_INT,                /* EI */
  MODE_INT,                /* OI */
  MODE_INT,                /* CI */
  MODE_INT,                /* XI */
  MODE_FRACT,              /* QQ */
  MODE_FRACT,              /* HQ */
  MODE_FRACT,              /* SQ */
  MODE_FRACT,              /* DQ */
  MODE_FRACT,              /* TQ */
  MODE_UFRACT,             /* UQQ */
  MODE_UFRACT,             /* UHQ */
  MODE_UFRACT,             /* USQ */
  MODE_UFRACT,             /* UDQ */
  MODE_UFRACT,             /* UTQ */
  MODE_ACCUM,              /* HA */
  MODE_ACCUM,              /* SA */
  MODE_ACCUM,              /* DA */
  MODE_ACCUM,              /* TA */
  MODE_UACCUM,             /* UHA */
  MODE_UACCUM,             /* USA */
  MODE_UACCUM,             /* UDA */
  MODE_UACCUM,             /* UTA */
  MODE_FLOAT,              /* HF */
  MODE_FLOAT,              /* SF */
  MODE_FLOAT,              /* DF */
  MODE_FLOAT,              /* XF */
  MODE_DECIMAL_FLOAT,      /* SD */
  MODE_DECIMAL_FLOAT,      /* DD */
  MODE_DECIMAL_FLOAT,      /* TD */
  MODE_COMPLEX_INT,        /* CQI */
  MODE_COMPLEX_INT,        /* CHI */
  MODE_COMPLEX_INT,        /* CSI */
  MODE_COMPLEX_INT,        /* CDI */
  MODE_COMPLEX_INT,        /* CTI */
  MODE_COMPLEX_INT,        /* CEI */
  MODE_COMPLEX_INT,        /* COI */
  MODE_COMPLEX_INT,        /* CCI */
  MODE_COMPLEX_INT,        /* CXI */
  MODE_COMPLEX_FLOAT,      /* HC */
  MODE_COMPLEX_FLOAT,      /* SC */
  MODE_COMPLEX_FLOAT,      /* DC */
  MODE_COMPLEX_FLOAT,      /* XC */
  MODE_VECTOR_INT,         /* V4QI */
  MODE_VECTOR_INT,         /* V2HI */
  MODE_VECTOR_INT,         /* V8QI */
  MODE_VECTOR_INT,         /* V4HI */
  MODE_VECTOR_INT,         /* V2SI */
  MODE_VECTOR_INT,         /* V16QI */
  MODE_VECTOR_INT,         /* V8HI */
  MODE_VECTOR_INT,         /* V4SI */
  MODE_VECTOR_INT,         /* V2DI */
  MODE_VECTOR_FLOAT,       /* V4HF */
  MODE_VECTOR_FLOAT,       /* V2SF */
  MODE_VECTOR_FLOAT,       /* V8HF */
  MODE_VECTOR_FLOAT,       /* V4SF */
  MODE_VECTOR_FLOAT,       /* V2DF */
};

const unsigned short mode_precision[NUM_MACHINE_MODES] =
{
  0,                       /* VOID */
  0,                       /* BLK */
  4*BITS_PER_UNIT,         /* CC */
  4*BITS_PER_UNIT,         /* CC_NOOV */
  4*BITS_PER_UNIT,         /* CC_Z */
  4*BITS_PER_UNIT,         /* CC_CZ */
  4*BITS_PER_UNIT,         /* CC_NCV */
  4*BITS_PER_UNIT,         /* CC_SWP */
  4*BITS_PER_UNIT,         /* CCFP */
  4*BITS_PER_UNIT,         /* CCFPE */
  4*BITS_PER_UNIT,         /* CC_DNE */
  4*BITS_PER_UNIT,         /* CC_DEQ */
  4*BITS_PER_UNIT,         /* CC_DLE */
  4*BITS_PER_UNIT,         /* CC_DLT */
  4*BITS_PER_UNIT,         /* CC_DGE */
  4*BITS_PER_UNIT,         /* CC_DGT */
  4*BITS_PER_UNIT,         /* CC_DLEU */
  4*BITS_PER_UNIT,         /* CC_DLTU */
  4*BITS_PER_UNIT,         /* CC_DGEU */
  4*BITS_PER_UNIT,         /* CC_DGTU */
  4*BITS_PER_UNIT,         /* CC_C */
  4*BITS_PER_UNIT,         /* CC_N */
  1,                       /* BI */
  1*BITS_PER_UNIT,         /* QI */
  2*BITS_PER_UNIT,         /* HI */
  4*BITS_PER_UNIT,         /* SI */
  8*BITS_PER_UNIT,         /* DI */
  16*BITS_PER_UNIT,        /* TI */
  24*BITS_PER_UNIT,        /* EI */
  32*BITS_PER_UNIT,        /* OI */
  48*BITS_PER_UNIT,        /* CI */
  64*BITS_PER_UNIT,        /* XI */
  1*BITS_PER_UNIT,         /* QQ */
  2*BITS_PER_UNIT,         /* HQ */
  4*BITS_PER_UNIT,         /* SQ */
  8*BITS_PER_UNIT,         /* DQ */
  16*BITS_PER_UNIT,        /* TQ */
  1*BITS_PER_UNIT,         /* UQQ */
  2*BITS_PER_UNIT,         /* UHQ */
  4*BITS_PER_UNIT,         /* USQ */
  8*BITS_PER_UNIT,         /* UDQ */
  16*BITS_PER_UNIT,        /* UTQ */
  2*BITS_PER_UNIT,         /* HA */
  4*BITS_PER_UNIT,         /* SA */
  8*BITS_PER_UNIT,         /* DA */
  16*BITS_PER_UNIT,        /* TA */
  2*BITS_PER_UNIT,         /* UHA */
  4*BITS_PER_UNIT,         /* USA */
  8*BITS_PER_UNIT,         /* UDA */
  16*BITS_PER_UNIT,        /* UTA */
  2*BITS_PER_UNIT,         /* HF */
  4*BITS_PER_UNIT,         /* SF */
  8*BITS_PER_UNIT,         /* DF */
  12*BITS_PER_UNIT,        /* XF */
  4*BITS_PER_UNIT,         /* SD */
  8*BITS_PER_UNIT,         /* DD */
  16*BITS_PER_UNIT,        /* TD */
  2*BITS_PER_UNIT,         /* CQI */
  4*BITS_PER_UNIT,         /* CHI */
  8*BITS_PER_UNIT,         /* CSI */
  16*BITS_PER_UNIT,        /* CDI */
  32*BITS_PER_UNIT,        /* CTI */
  48*BITS_PER_UNIT,        /* CEI */
  64*BITS_PER_UNIT,        /* COI */
  96*BITS_PER_UNIT,        /* CCI */
  128*BITS_PER_UNIT,       /* CXI */
  4*BITS_PER_UNIT,         /* HC */
  8*BITS_PER_UNIT,         /* SC */
  16*BITS_PER_UNIT,        /* DC */
  24*BITS_PER_UNIT,        /* XC */
  4*BITS_PER_UNIT,         /* V4QI */
  4*BITS_PER_UNIT,         /* V2HI */
  8*BITS_PER_UNIT,         /* V8QI */
  8*BITS_PER_UNIT,         /* V4HI */
  8*BITS_PER_UNIT,         /* V2SI */
  16*BITS_PER_UNIT,        /* V16QI */
  16*BITS_PER_UNIT,        /* V8HI */
  16*BITS_PER_UNIT,        /* V4SI */
  16*BITS_PER_UNIT,        /* V2DI */
  8*BITS_PER_UNIT,         /* V4HF */
  8*BITS_PER_UNIT,         /* V2SF */
  16*BITS_PER_UNIT,        /* V8HF */
  16*BITS_PER_UNIT,        /* V4SF */
  16*BITS_PER_UNIT,        /* V2DF */
};

const unsigned char mode_size[NUM_MACHINE_MODES] = 
{
  0,                       /* VOID */
  0,                       /* BLK */
  4,                       /* CC */
  4,                       /* CC_NOOV */
  4,                       /* CC_Z */
  4,                       /* CC_CZ */
  4,                       /* CC_NCV */
  4,                       /* CC_SWP */
  4,                       /* CCFP */
  4,                       /* CCFPE */
  4,                       /* CC_DNE */
  4,                       /* CC_DEQ */
  4,                       /* CC_DLE */
  4,                       /* CC_DLT */
  4,                       /* CC_DGE */
  4,                       /* CC_DGT */
  4,                       /* CC_DLEU */
  4,                       /* CC_DLTU */
  4,                       /* CC_DGEU */
  4,                       /* CC_DGTU */
  4,                       /* CC_C */
  4,                       /* CC_N */
  1,                       /* BI */
  1,                       /* QI */
  2,                       /* HI */
  4,                       /* SI */
  8,                       /* DI */
  16,                      /* TI */
  24,                      /* EI */
  32,                      /* OI */
  48,                      /* CI */
  64,                      /* XI */
  1,                       /* QQ */
  2,                       /* HQ */
  4,                       /* SQ */
  8,                       /* DQ */
  16,                      /* TQ */
  1,                       /* UQQ */
  2,                       /* UHQ */
  4,                       /* USQ */
  8,                       /* UDQ */
  16,                      /* UTQ */
  2,                       /* HA */
  4,                       /* SA */
  8,                       /* DA */
  16,                      /* TA */
  2,                       /* UHA */
  4,                       /* USA */
  8,                       /* UDA */
  16,                      /* UTA */
  2,                       /* HF */
  4,                       /* SF */
  8,                       /* DF */
  12,                      /* XF */
  4,                       /* SD */
  8,                       /* DD */
  16,                      /* TD */
  2,                       /* CQI */
  4,                       /* CHI */
  8,                       /* CSI */
  16,                      /* CDI */
  32,                      /* CTI */
  48,                      /* CEI */
  64,                      /* COI */
  96,                      /* CCI */
  128,                     /* CXI */
  4,                       /* HC */
  8,                       /* SC */
  16,                      /* DC */
  24,                      /* XC */
  4,                       /* V4QI */
  4,                       /* V2HI */
  8,                       /* V8QI */
  8,                       /* V4HI */
  8,                       /* V2SI */
  16,                      /* V16QI */
  16,                      /* V8HI */
  16,                      /* V4SI */
  16,                      /* V2DI */
  8,                       /* V4HF */
  8,                       /* V2SF */
  16,                      /* V8HF */
  16,                      /* V4SF */
  16,                      /* V2DF */
};

const unsigned char mode_nunits[NUM_MACHINE_MODES] =
{
  0,                       /* VOID */
  0,                       /* BLK */
  1,                       /* CC */
  1,                       /* CC_NOOV */
  1,                       /* CC_Z */
  1,                       /* CC_CZ */
  1,                       /* CC_NCV */
  1,                       /* CC_SWP */
  1,                       /* CCFP */
  1,                       /* CCFPE */
  1,                       /* CC_DNE */
  1,                       /* CC_DEQ */
  1,                       /* CC_DLE */
  1,                       /* CC_DLT */
  1,                       /* CC_DGE */
  1,                       /* CC_DGT */
  1,                       /* CC_DLEU */
  1,                       /* CC_DLTU */
  1,                       /* CC_DGEU */
  1,                       /* CC_DGTU */
  1,                       /* CC_C */
  1,                       /* CC_N */
  1,                       /* BI */
  1,                       /* QI */
  1,                       /* HI */
  1,                       /* SI */
  1,                       /* DI */
  1,                       /* TI */
  1,                       /* EI */
  1,                       /* OI */
  1,                       /* CI */
  1,                       /* XI */
  1,                       /* QQ */
  1,                       /* HQ */
  1,                       /* SQ */
  1,                       /* DQ */
  1,                       /* TQ */
  1,                       /* UQQ */
  1,                       /* UHQ */
  1,                       /* USQ */
  1,                       /* UDQ */
  1,                       /* UTQ */
  1,                       /* HA */
  1,                       /* SA */
  1,                       /* DA */
  1,                       /* TA */
  1,                       /* UHA */
  1,                       /* USA */
  1,                       /* UDA */
  1,                       /* UTA */
  1,                       /* HF */
  1,                       /* SF */
  1,                       /* DF */
  1,                       /* XF */
  1,                       /* SD */
  1,                       /* DD */
  1,                       /* TD */
  2,                       /* CQI */
  2,                       /* CHI */
  2,                       /* CSI */
  2,                       /* CDI */
  2,                       /* CTI */
  2,                       /* CEI */
  2,                       /* COI */
  2,                       /* CCI */
  2,                       /* CXI */
  2,                       /* HC */
  2,                       /* SC */
  2,                       /* DC */
  2,                       /* XC */
  4,                       /* V4QI */
  2,                       /* V2HI */
  8,                       /* V8QI */
  4,                       /* V4HI */
  2,                       /* V2SI */
  16,                      /* V16QI */
  8,                       /* V8HI */
  4,                       /* V4SI */
  2,                       /* V2DI */
  4,                       /* V4HF */
  2,                       /* V2SF */
  8,                       /* V8HF */
  4,                       /* V4SF */
  2,                       /* V2DF */
};

const unsigned char mode_wider[NUM_MACHINE_MODES] =
{
  VOIDmode,                /* VOID */
  VOIDmode,                /* BLK */
  VOIDmode,                /* CC */
  VOIDmode,                /* CC_NOOV */
  VOIDmode,                /* CC_Z */
  VOIDmode,                /* CC_CZ */
  VOIDmode,                /* CC_NCV */
  VOIDmode,                /* CC_SWP */
  VOIDmode,                /* CCFP */
  VOIDmode,                /* CCFPE */
  VOIDmode,                /* CC_DNE */
  VOIDmode,                /* CC_DEQ */
  VOIDmode,                /* CC_DLE */
  VOIDmode,                /* CC_DLT */
  VOIDmode,                /* CC_DGE */
  VOIDmode,                /* CC_DGT */
  VOIDmode,                /* CC_DLEU */
  VOIDmode,                /* CC_DLTU */
  VOIDmode,                /* CC_DGEU */
  VOIDmode,                /* CC_DGTU */
  VOIDmode,                /* CC_C */
  VOIDmode,                /* CC_N */
  QImode,                  /* BI */
  HImode,                  /* QI */
  SImode,                  /* HI */
  DImode,                  /* SI */
  TImode,                  /* DI */
  EImode,                  /* TI */
  OImode,                  /* EI */
  CImode,                  /* OI */
  XImode,                  /* CI */
  VOIDmode,                /* XI */
  HQmode,                  /* QQ */
  SQmode,                  /* HQ */
  DQmode,                  /* SQ */
  TQmode,                  /* DQ */
  VOIDmode,                /* TQ */
  UHQmode,                 /* UQQ */
  USQmode,                 /* UHQ */
  UDQmode,                 /* USQ */
  UTQmode,                 /* UDQ */
  VOIDmode,                /* UTQ */
  SAmode,                  /* HA */
  DAmode,                  /* SA */
  TAmode,                  /* DA */
  VOIDmode,                /* TA */
  USAmode,                 /* UHA */
  UDAmode,                 /* USA */
  UTAmode,                 /* UDA */
  VOIDmode,                /* UTA */
  SFmode,                  /* HF */
  DFmode,                  /* SF */
  XFmode,                  /* DF */
  VOIDmode,                /* XF */
  DDmode,                  /* SD */
  TDmode,                  /* DD */
  VOIDmode,                /* TD */
  CHImode,                 /* CQI */
  CSImode,                 /* CHI */
  CDImode,                 /* CSI */
  CTImode,                 /* CDI */
  CEImode,                 /* CTI */
  COImode,                 /* CEI */
  CCImode,                 /* COI */
  CXImode,                 /* CCI */
  VOIDmode,                /* CXI */
  SCmode,                  /* HC */
  DCmode,                  /* SC */
  XCmode,                  /* DC */
  VOIDmode,                /* XC */
  V2HImode,                /* V4QI */
  V8QImode,                /* V2HI */
  V4HImode,                /* V8QI */
  V2SImode,                /* V4HI */
  V16QImode,               /* V2SI */
  V8HImode,                /* V16QI */
  V4SImode,                /* V8HI */
  V2DImode,                /* V4SI */
  VOIDmode,                /* V2DI */
  V2SFmode,                /* V4HF */
  V8HFmode,                /* V2SF */
  V4SFmode,                /* V8HF */
  V2DFmode,                /* V4SF */
  VOIDmode,                /* V2DF */
};

const unsigned char mode_2xwider[NUM_MACHINE_MODES] =
{
  VOIDmode,                /* VOID */
  BLKmode,                 /* BLK */
  VOIDmode,                /* CC */
  VOIDmode,                /* CC_NOOV */
  VOIDmode,                /* CC_Z */
  VOIDmode,                /* CC_CZ */
  VOIDmode,                /* CC_NCV */
  VOIDmode,                /* CC_SWP */
  VOIDmode,                /* CCFP */
  VOIDmode,                /* CCFPE */
  VOIDmode,                /* CC_DNE */
  VOIDmode,                /* CC_DEQ */
  VOIDmode,                /* CC_DLE */
  VOIDmode,                /* CC_DLT */
  VOIDmode,                /* CC_DGE */
  VOIDmode,                /* CC_DGT */
  VOIDmode,                /* CC_DLEU */
  VOIDmode,                /* CC_DLTU */
  VOIDmode,                /* CC_DGEU */
  VOIDmode,                /* CC_DGTU */
  VOIDmode,                /* CC_C */
  VOIDmode,                /* CC_N */
  VOIDmode,                /* BI */
  HImode,                  /* QI */
  SImode,                  /* HI */
  DImode,                  /* SI */
  TImode,                  /* DI */
  OImode,                  /* TI */
  CImode,                  /* EI */
  XImode,                  /* OI */
  VOIDmode,                /* CI */
  VOIDmode,                /* XI */
  HQmode,                  /* QQ */
  SQmode,                  /* HQ */
  DQmode,                  /* SQ */
  TQmode,                  /* DQ */
  VOIDmode,                /* TQ */
  UHQmode,                 /* UQQ */
  USQmode,                 /* UHQ */
  UDQmode,                 /* USQ */
  UTQmode,                 /* UDQ */
  VOIDmode,                /* UTQ */
  SAmode,                  /* HA */
  DAmode,                  /* SA */
  TAmode,                  /* DA */
  VOIDmode,                /* TA */
  USAmode,                 /* UHA */
  UDAmode,                 /* USA */
  UTAmode,                 /* UDA */
  VOIDmode,                /* UTA */
  SFmode,                  /* HF */
  DFmode,                  /* SF */
  VOIDmode,                /* DF */
  VOIDmode,                /* XF */
  DDmode,                  /* SD */
  TDmode,                  /* DD */
  VOIDmode,                /* TD */
  CHImode,                 /* CQI */
  CSImode,                 /* CHI */
  CDImode,                 /* CSI */
  CTImode,                 /* CDI */
  COImode,                 /* CTI */
  CCImode,                 /* CEI */
  CXImode,                 /* COI */
  VOIDmode,                /* CCI */
  VOIDmode,                /* CXI */
  SCmode,                  /* HC */
  DCmode,                  /* SC */
  VOIDmode,                /* DC */
  VOIDmode,                /* XC */
  V8QImode,                /* V4QI */
  V8QImode,                /* V2HI */
  V16QImode,               /* V8QI */
  V16QImode,               /* V4HI */
  V16QImode,               /* V2SI */
  VOIDmode,                /* V16QI */
  VOIDmode,                /* V8HI */
  VOIDmode,                /* V4SI */
  VOIDmode,                /* V2DI */
  V8HFmode,                /* V4HF */
  V8HFmode,                /* V2SF */
  VOIDmode,                /* V8HF */
  VOIDmode,                /* V4SF */
  VOIDmode,                /* V2DF */
};

const unsigned HOST_WIDE_INT mode_mask_array[NUM_MACHINE_MODES] =
{
#define MODE_MASK(m)                          \
  ((m) >= HOST_BITS_PER_WIDE_INT)             \
   ? ~(unsigned HOST_WIDE_INT) 0              \
   : ((unsigned HOST_WIDE_INT) 1 << (m)) - 1

  MODE_MASK (0),           /* VOID */
  MODE_MASK (0),           /* BLK */
  MODE_MASK (4*BITS_PER_UNIT),   /* CC */
  MODE_MASK (4*BITS_PER_UNIT),   /* CC_NOOV */
  MODE_MASK (4*BITS_PER_UNIT),   /* CC_Z */
  MODE_MASK (4*BITS_PER_UNIT),   /* CC_CZ */
  MODE_MASK (4*BITS_PER_UNIT),   /* CC_NCV */
  MODE_MASK (4*BITS_PER_UNIT),   /* CC_SWP */
  MODE_MASK (4*BITS_PER_UNIT),   /* CCFP */
  MODE_MASK (4*BITS_PER_UNIT),   /* CCFPE */
  MODE_MASK (4*BITS_PER_UNIT),   /* CC_DNE */
  MODE_MASK (4*BITS_PER_UNIT),   /* CC_DEQ */
  MODE_MASK (4*BITS_PER_UNIT),   /* CC_DLE */
  MODE_MASK (4*BITS_PER_UNIT),   /* CC_DLT */
  MODE_MASK (4*BITS_PER_UNIT),   /* CC_DGE */
  MODE_MASK (4*BITS_PER_UNIT),   /* CC_DGT */
  MODE_MASK (4*BITS_PER_UNIT),   /* CC_DLEU */
  MODE_MASK (4*BITS_PER_UNIT),   /* CC_DLTU */
  MODE_MASK (4*BITS_PER_UNIT),   /* CC_DGEU */
  MODE_MASK (4*BITS_PER_UNIT),   /* CC_DGTU */
  MODE_MASK (4*BITS_PER_UNIT),   /* CC_C */
  MODE_MASK (4*BITS_PER_UNIT),   /* CC_N */
  MODE_MASK (1),           /* BI */
  MODE_MASK (1*BITS_PER_UNIT),   /* QI */
  MODE_MASK (2*BITS_PER_UNIT),   /* HI */
  MODE_MASK (4*BITS_PER_UNIT),   /* SI */
  MODE_MASK (8*BITS_PER_UNIT),   /* DI */
  MODE_MASK (16*BITS_PER_UNIT),    /* TI */
  MODE_MASK (24*BITS_PER_UNIT),    /* EI */
  MODE_MASK (32*BITS_PER_UNIT),    /* OI */
  MODE_MASK (48*BITS_PER_UNIT),    /* CI */
  MODE_MASK (64*BITS_PER_UNIT),    /* XI */
  MODE_MASK (1*BITS_PER_UNIT),   /* QQ */
  MODE_MASK (2*BITS_PER_UNIT),   /* HQ */
  MODE_MASK (4*BITS_PER_UNIT),   /* SQ */
  MODE_MASK (8*BITS_PER_UNIT),   /* DQ */
  MODE_MASK (16*BITS_PER_UNIT),    /* TQ */
  MODE_MASK (1*BITS_PER_UNIT),   /* UQQ */
  MODE_MASK (2*BITS_PER_UNIT),   /* UHQ */
  MODE_MASK (4*BITS_PER_UNIT),   /* USQ */
  MODE_MASK (8*BITS_PER_UNIT),   /* UDQ */
  MODE_MASK (16*BITS_PER_UNIT),    /* UTQ */
  MODE_MASK (2*BITS_PER_UNIT),   /* HA */
  MODE_MASK (4*BITS_PER_UNIT),   /* SA */
  MODE_MASK (8*BITS_PER_UNIT),   /* DA */
  MODE_MASK (16*BITS_PER_UNIT),    /* TA */
  MODE_MASK (2*BITS_PER_UNIT),   /* UHA */
  MODE_MASK (4*BITS_PER_UNIT),   /* USA */
  MODE_MASK (8*BITS_PER_UNIT),   /* UDA */
  MODE_MASK (16*BITS_PER_UNIT),    /* UTA */
  MODE_MASK (2*BITS_PER_UNIT),   /* HF */
  MODE_MASK (4*BITS_PER_UNIT),   /* SF */
  MODE_MASK (8*BITS_PER_UNIT),   /* DF */
  MODE_MASK (12*BITS_PER_UNIT),    /* XF */
  MODE_MASK (4*BITS_PER_UNIT),   /* SD */
  MODE_MASK (8*BITS_PER_UNIT),   /* DD */
  MODE_MASK (16*BITS_PER_UNIT),    /* TD */
  MODE_MASK (2*BITS_PER_UNIT),   /* CQI */
  MODE_MASK (4*BITS_PER_UNIT),   /* CHI */
  MODE_MASK (8*BITS_PER_UNIT),   /* CSI */
  MODE_MASK (16*BITS_PER_UNIT),    /* CDI */
  MODE_MASK (32*BITS_PER_UNIT),    /* CTI */
  MODE_MASK (48*BITS_PER_UNIT),    /* CEI */
  MODE_MASK (64*BITS_PER_UNIT),    /* COI */
  MODE_MASK (96*BITS_PER_UNIT),    /* CCI */
  MODE_MASK (128*BITS_PER_UNIT),     /* CXI */
  MODE_MASK (4*BITS_PER_UNIT),   /* HC */
  MODE_MASK (8*BITS_PER_UNIT),   /* SC */
  MODE_MASK (16*BITS_PER_UNIT),    /* DC */
  MODE_MASK (24*BITS_PER_UNIT),    /* XC */
  MODE_MASK (4*BITS_PER_UNIT),   /* V4QI */
  MODE_MASK (4*BITS_PER_UNIT),   /* V2HI */
  MODE_MASK (8*BITS_PER_UNIT),   /* V8QI */
  MODE_MASK (8*BITS_PER_UNIT),   /* V4HI */
  MODE_MASK (8*BITS_PER_UNIT),   /* V2SI */
  MODE_MASK (16*BITS_PER_UNIT),    /* V16QI */
  MODE_MASK (16*BITS_PER_UNIT),    /* V8HI */
  MODE_MASK (16*BITS_PER_UNIT),    /* V4SI */
  MODE_MASK (16*BITS_PER_UNIT),    /* V2DI */
  MODE_MASK (8*BITS_PER_UNIT),   /* V4HF */
  MODE_MASK (8*BITS_PER_UNIT),   /* V2SF */
  MODE_MASK (16*BITS_PER_UNIT),    /* V8HF */
  MODE_MASK (16*BITS_PER_UNIT),    /* V4SF */
  MODE_MASK (16*BITS_PER_UNIT),    /* V2DF */
#undef MODE_MASK
};

const unsigned char mode_inner[NUM_MACHINE_MODES] =
{
  VOIDmode,                /* VOID */
  VOIDmode,                /* BLK */
  VOIDmode,                /* CC */
  VOIDmode,                /* CC_NOOV */
  VOIDmode,                /* CC_Z */
  VOIDmode,                /* CC_CZ */
  VOIDmode,                /* CC_NCV */
  VOIDmode,                /* CC_SWP */
  VOIDmode,                /* CCFP */
  VOIDmode,                /* CCFPE */
  VOIDmode,                /* CC_DNE */
  VOIDmode,                /* CC_DEQ */
  VOIDmode,                /* CC_DLE */
  VOIDmode,                /* CC_DLT */
  VOIDmode,                /* CC_DGE */
  VOIDmode,                /* CC_DGT */
  VOIDmode,                /* CC_DLEU */
  VOIDmode,                /* CC_DLTU */
  VOIDmode,                /* CC_DGEU */
  VOIDmode,                /* CC_DGTU */
  VOIDmode,                /* CC_C */
  VOIDmode,                /* CC_N */
  VOIDmode,                /* BI */
  VOIDmode,                /* QI */
  VOIDmode,                /* HI */
  VOIDmode,                /* SI */
  VOIDmode,                /* DI */
  VOIDmode,                /* TI */
  VOIDmode,                /* EI */
  VOIDmode,                /* OI */
  VOIDmode,                /* CI */
  VOIDmode,                /* XI */
  VOIDmode,                /* QQ */
  VOIDmode,                /* HQ */
  VOIDmode,                /* SQ */
  VOIDmode,                /* DQ */
  VOIDmode,                /* TQ */
  VOIDmode,                /* UQQ */
  VOIDmode,                /* UHQ */
  VOIDmode,                /* USQ */
  VOIDmode,                /* UDQ */
  VOIDmode,                /* UTQ */
  VOIDmode,                /* HA */
  VOIDmode,                /* SA */
  VOIDmode,                /* DA */
  VOIDmode,                /* TA */
  VOIDmode,                /* UHA */
  VOIDmode,                /* USA */
  VOIDmode,                /* UDA */
  VOIDmode,                /* UTA */
  VOIDmode,                /* HF */
  VOIDmode,                /* SF */
  VOIDmode,                /* DF */
  VOIDmode,                /* XF */
  VOIDmode,                /* SD */
  VOIDmode,                /* DD */
  VOIDmode,                /* TD */
  QImode,                  /* CQI */
  HImode,                  /* CHI */
  SImode,                  /* CSI */
  DImode,                  /* CDI */
  TImode,                  /* CTI */
  EImode,                  /* CEI */
  OImode,                  /* COI */
  CImode,                  /* CCI */
  XImode,                  /* CXI */
  HFmode,                  /* HC */
  SFmode,                  /* SC */
  DFmode,                  /* DC */
  XFmode,                  /* XC */
  QImode,                  /* V4QI */
  HImode,                  /* V2HI */
  QImode,                  /* V8QI */
  HImode,                  /* V4HI */
  SImode,                  /* V2SI */
  QImode,                  /* V16QI */
  HImode,                  /* V8HI */
  SImode,                  /* V4SI */
  DImode,                  /* V2DI */
  HFmode,                  /* V4HF */
  SFmode,                  /* V2SF */
  HFmode,                  /* V8HF */
  SFmode,                  /* V4SF */
  DFmode,                  /* V2DF */
};

const unsigned char mode_base_align[NUM_MACHINE_MODES] = 
{
  0,                       /* VOID */
  0,                       /* BLK */
  4,                       /* CC */
  4,                       /* CC_NOOV */
  4,                       /* CC_Z */
  4,                       /* CC_CZ */
  4,                       /* CC_NCV */
  4,                       /* CC_SWP */
  4,                       /* CCFP */
  4,                       /* CCFPE */
  4,                       /* CC_DNE */
  4,                       /* CC_DEQ */
  4,                       /* CC_DLE */
  4,                       /* CC_DLT */
  4,                       /* CC_DGE */
  4,                       /* CC_DGT */
  4,                       /* CC_DLEU */
  4,                       /* CC_DLTU */
  4,                       /* CC_DGEU */
  4,                       /* CC_DGTU */
  4,                       /* CC_C */
  4,                       /* CC_N */
  1,                       /* BI */
  1,                       /* QI */
  2,                       /* HI */
  4,                       /* SI */
  8,                       /* DI */
  16,                      /* TI */
  8,                       /* EI */
  32,                      /* OI */
  16,                      /* CI */
  64,                      /* XI */
  1,                       /* QQ */
  2,                       /* HQ */
  4,                       /* SQ */
  8,                       /* DQ */
  16,                      /* TQ */
  1,                       /* UQQ */
  2,                       /* UHQ */
  4,                       /* USQ */
  8,                       /* UDQ */
  16,                      /* UTQ */
  2,                       /* HA */
  4,                       /* SA */
  8,                       /* DA */
  16,                      /* TA */
  2,                       /* UHA */
  4,                       /* USA */
  8,                       /* UDA */
  16,                      /* UTA */
  2,                       /* HF */
  4,                       /* SF */
  8,                       /* DF */
  4,                       /* XF */
  4,                       /* SD */
  8,                       /* DD */
  16,                      /* TD */
  1,                       /* CQI */
  2,                       /* CHI */
  4,                       /* CSI */
  8,                       /* CDI */
  16,                      /* CTI */
  8,                       /* CEI */
  32,                      /* COI */
  16,                      /* CCI */
  64,                      /* CXI */
  2,                       /* HC */
  4,                       /* SC */
  8,                       /* DC */
  4,                       /* XC */
  4,                       /* V4QI */
  4,                       /* V2HI */
  8,                       /* V8QI */
  8,                       /* V4HI */
  8,                       /* V2SI */
  16,                      /* V16QI */
  16,                      /* V8HI */
  16,                      /* V4SI */
  16,                      /* V2DI */
  8,                       /* V4HF */
  8,                       /* V2SF */
  16,                      /* V8HF */
  16,                      /* V4SF */
  16,                      /* V2DF */
};

const unsigned char class_narrowest_mode[MAX_MODE_CLASS] =
{
  MIN_MODE_RANDOM,         /* VOID */
  MIN_MODE_CC,             /* CC */
  MIN_MODE_INT,            /* QI */
  MIN_MODE_PARTIAL_INT,    /* VOID */
  MIN_MODE_FRACT,          /* QQ */
  MIN_MODE_UFRACT,         /* UQQ */
  MIN_MODE_ACCUM,          /* HA */
  MIN_MODE_UACCUM,         /* UHA */
  MIN_MODE_FLOAT,          /* HF */
  MIN_MODE_DECIMAL_FLOAT,  /* SD */
  MIN_MODE_COMPLEX_INT,    /* CQI */
  MIN_MODE_COMPLEX_FLOAT,  /* HC */
  MIN_MODE_VECTOR_INT,     /* V4QI */
  MIN_MODE_VECTOR_FRACT,   /* VOID */
  MIN_MODE_VECTOR_UFRACT,  /* VOID */
  MIN_MODE_VECTOR_ACCUM,   /* VOID */
  MIN_MODE_VECTOR_UACCUM,  /* VOID */
  MIN_MODE_VECTOR_FLOAT,   /* V4HF */
};

const struct real_format *
 real_format_for_mode[MAX_MODE_FLOAT - MIN_MODE_FLOAT + 1 + MAX_MODE_DECIMAL_FLOAT - MIN_MODE_DECIMAL_FLOAT + 1] =
{
  0,                       /* HF */
  &ieee_single_format,     /* SF */
  &ieee_double_format,     /* DF */
  0,                       /* XF */
  &decimal_single_format,  /* SD */
  &decimal_double_format,  /* DD */
  &decimal_quad_format,    /* TD */
};

void
init_adjust_machine_modes (void)
{
  size_t s ATTRIBUTE_UNUSED;

  /* config/arm/arm-modes.def:31 */
  REAL_MODE_FORMAT (HFmode) = ((arm_fp16_format == ARM_FP16_FORMAT_ALTERNATIVE) ? &arm_half_format : &ieee_half_format);
}

const unsigned char mode_ibit[NUM_MACHINE_MODES] = 
{
  0,                       /* VOID */
  0,                       /* BLK */
  0,                       /* CC */
  0,                       /* CC_NOOV */
  0,                       /* CC_Z */
  0,                       /* CC_CZ */
  0,                       /* CC_NCV */
  0,                       /* CC_SWP */
  0,                       /* CCFP */
  0,                       /* CCFPE */
  0,                       /* CC_DNE */
  0,                       /* CC_DEQ */
  0,                       /* CC_DLE */
  0,                       /* CC_DLT */
  0,                       /* CC_DGE */
  0,                       /* CC_DGT */
  0,                       /* CC_DLEU */
  0,                       /* CC_DLTU */
  0,                       /* CC_DGEU */
  0,                       /* CC_DGTU */
  0,                       /* CC_C */
  0,                       /* CC_N */
  0,                       /* BI */
  0,                       /* QI */
  0,                       /* HI */
  0,                       /* SI */
  0,                       /* DI */
  0,                       /* TI */
  0,                       /* EI */
  0,                       /* OI */
  0,                       /* CI */
  0,                       /* XI */
  0,                       /* QQ */
  0,                       /* HQ */
  0,                       /* SQ */
  0,                       /* DQ */
  0,                       /* TQ */
  0,                       /* UQQ */
  0,                       /* UHQ */
  0,                       /* USQ */
  0,                       /* UDQ */
  0,                       /* UTQ */
  8,                       /* HA */
  16,                      /* SA */
  32,                      /* DA */
  64,                      /* TA */
  8,                       /* UHA */
  16,                      /* USA */
  32,                      /* UDA */
  64,                      /* UTA */
  0,                       /* HF */
  0,                       /* SF */
  0,                       /* DF */
  0,                       /* XF */
  0,                       /* SD */
  0,                       /* DD */
  0,                       /* TD */
  0,                       /* CQI */
  0,                       /* CHI */
  0,                       /* CSI */
  0,                       /* CDI */
  0,                       /* CTI */
  0,                       /* CEI */
  0,                       /* COI */
  0,                       /* CCI */
  0,                       /* CXI */
  0,                       /* HC */
  0,                       /* SC */
  0,                       /* DC */
  0,                       /* XC */
  0,                       /* V4QI */
  0,                       /* V2HI */
  0,                       /* V8QI */
  0,                       /* V4HI */
  0,                       /* V2SI */
  0,                       /* V16QI */
  0,                       /* V8HI */
  0,                       /* V4SI */
  0,                       /* V2DI */
  0,                       /* V4HF */
  0,                       /* V2SF */
  0,                       /* V8HF */
  0,                       /* V4SF */
  0,                       /* V2DF */
};

const unsigned char mode_fbit[NUM_MACHINE_MODES] = 
{
  0,                       /* VOID */
  0,                       /* BLK */
  0,                       /* CC */
  0,                       /* CC_NOOV */
  0,                       /* CC_Z */
  0,                       /* CC_CZ */
  0,                       /* CC_NCV */
  0,                       /* CC_SWP */
  0,                       /* CCFP */
  0,                       /* CCFPE */
  0,                       /* CC_DNE */
  0,                       /* CC_DEQ */
  0,                       /* CC_DLE */
  0,                       /* CC_DLT */
  0,                       /* CC_DGE */
  0,                       /* CC_DGT */
  0,                       /* CC_DLEU */
  0,                       /* CC_DLTU */
  0,                       /* CC_DGEU */
  0,                       /* CC_DGTU */
  0,                       /* CC_C */
  0,                       /* CC_N */
  0,                       /* BI */
  0,                       /* QI */
  0,                       /* HI */
  0,                       /* SI */
  0,                       /* DI */
  0,                       /* TI */
  0,                       /* EI */
  0,                       /* OI */
  0,                       /* CI */
  0,                       /* XI */
  7,                       /* QQ */
  15,                      /* HQ */
  31,                      /* SQ */
  63,                      /* DQ */
  127,                     /* TQ */
  8,                       /* UQQ */
  16,                      /* UHQ */
  32,                      /* USQ */
  64,                      /* UDQ */
  128,                     /* UTQ */
  7,                       /* HA */
  15,                      /* SA */
  31,                      /* DA */
  63,                      /* TA */
  8,                       /* UHA */
  16,                      /* USA */
  32,                      /* UDA */
  64,                      /* UTA */
  0,                       /* HF */
  0,                       /* SF */
  0,                       /* DF */
  0,                       /* XF */
  0,                       /* SD */
  0,                       /* DD */
  0,                       /* TD */
  0,                       /* CQI */
  0,                       /* CHI */
  0,                       /* CSI */
  0,                       /* CDI */
  0,                       /* CTI */
  0,                       /* CEI */
  0,                       /* COI */
  0,                       /* CCI */
  0,                       /* CXI */
  0,                       /* HC */
  0,                       /* SC */
  0,                       /* DC */
  0,                       /* XC */
  0,                       /* V4QI */
  0,                       /* V2HI */
  0,                       /* V8QI */
  0,                       /* V4HI */
  0,                       /* V2SI */
  0,                       /* V16QI */
  0,                       /* V8HI */
  0,                       /* V4SI */
  0,                       /* V2DI */
  0,                       /* V4HF */
  0,                       /* V2SF */
  0,                       /* V8HF */
  0,                       /* V4SF */
  0,                       /* V2DF */
};
