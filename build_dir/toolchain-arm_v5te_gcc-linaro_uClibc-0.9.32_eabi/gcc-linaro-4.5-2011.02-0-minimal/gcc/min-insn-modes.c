/* Generated automatically from machmode.def and config/arm/arm-modes.def
   by genmodes.  */

#include "bconfig.h"
#include "system.h"
#include "machmode.h"

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
