#ifndef __FRAMMAP_H__
#define __FRAMMAP_H__

#include "frammap_if.h"

#define FRM_VERSION_CODE "0.28.3"

#define GRANU_ORDER     0       /* which means 4K * 2^GRANU_ORDER, ex: 4K * 2^6 = 256K */
#define MAX_DDR_NR      3       /* how many DDRs include system memory */

#define DDR_ID_T        ddr_id_t
typedef unsigned int    SIZE_ID_T;

#ifdef CONFIG_PLATFORM_GM8181
#define CFG_FILE_PATH       "/mnt/mtd/config_8181"
#define CFG_FILE_TMPPATH    "/tmp/template_8181"
#elif defined(CONFIG_PLATFORM_GM8126)
#define CFG_FILE_PATH       "/mnt/mtd/config_8126"
#define CFG_FILE_TMPPATH    "/tmp/template_8126"
#else
    #error "Frammap: config file path needs to be configured!"
#endif

#endif //__FRAMMAP_H__
