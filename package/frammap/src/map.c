#include <linux/syscalls.h>
#include <linux/fs.h> 
#include <asm/segment.h> 
#include <asm/uaccess.h> 
#include <linux/buffer_head.h>

#define LINE_LEN    100
#define H264E_DMAMASK0_DEF  "0x07243C"
#define H264E_DMAMASK1_DEF  "0x18DBC3"

char h264e_dmamask0[11], h264e_dmamask1[11];

char title1[] = "** SYSTEM: system memory, DDR-1: 2nd memory, DDR-2: 3rd memory, ... **\n";
char title2[] = "** Note: The name needs to be continuous without space. **\n";

/*
 * String/ID array
 */
typedef struct 
{
    int         rmmp_index;
    char        *string;     
    DDR_ID_T    ddr_id;
} id_string_t;

#ifdef CONFIG_PLATFORM_GM8126
id_string_t  id_string_arry[] =
{
    {FRMIDX_MIN,      "Unknown",                DDR_ID_SYSTEM},
    {FRMIDX_H264E_BS, "H264E_BitStream",        DDR_ID_SYSTEM},
    {FRMIDX_H264E_G1, "H264E_G1(DMA_CHAIN)",    DDR_ID_SYSTEM},
    {FRMIDX_H264E_G2, "H264E_G2(REFER+RECON)",  DDR_ID_SYSTEM},
    {FRMIDX_MMAP_M1,  "MMAP",                   DDR_ID_SYSTEM},
    {FRMIDX_MMAP_M2,  "MMAP",                   DDR_ID_SYSTEM},
    {FRMIDX_H264D_REF, "H264D_Reference",       DDR_ID_SYSTEM},
    {FRMIDX_FFB0_M,   "LCD_FB0",                DDR_ID_SYSTEM},
    {FRMIDX_FFB0_S1,  "LCD_FB1",                DDR_ID_SYSTEM},
    {FRMIDX_FFB0_S2,  "LCD_FB2",                DDR_ID_SYSTEM},
    {FRMIDX_SCAL0,    "Scale_0",                DDR_ID_SYSTEM},
    {FRMIDX_SCAL1,    "Scale_1",                DDR_ID_SYSTEM},
    {FRMIDX_CAP0,     "Capture0",               DDR_ID_SYSTEM},
    {FRMIDX_CAP1,     "Capture1",               DDR_ID_SYSTEM},
    {FRMIDX_CAP2,     "Capture2",               DDR_ID_SYSTEM},
    {FRMIDX_CAP3,     "Capture3",               DDR_ID_SYSTEM},
    {FRMIDX_CAP4,     "Capture4",               DDR_ID_SYSTEM},
    {FRMIDX_CAP5,     "Capture5",               DDR_ID_SYSTEM},
    {FRMIDX_CAP6,     "Capture6",               DDR_ID_SYSTEM},
    {FRMIDX_CAP7,     "Capture7",               DDR_ID_SYSTEM},
    {FRMIDX_CAP8,     "Capture8",               DDR_ID_SYSTEM},
    {FRMIDX_CAP9,     "Capture9",               DDR_ID_SYSTEM},
    {FRMIDX_CAP10,    "Capture10",              DDR_ID_SYSTEM},
    {FRMIDX_CAP11,    "Capture11",              DDR_ID_SYSTEM},
    {FRMIDX_CAP12,    "Capture12",              DDR_ID_SYSTEM},
    {FRMIDX_CAP13,    "Capture13",              DDR_ID_SYSTEM},
    {FRMIDX_CAP14,    "Capture14",              DDR_ID_SYSTEM},
    {FRMIDX_CAP15,    "Capture15",              DDR_ID_SYSTEM},
    {FRMIDX_CAP16,    "Capture16",              DDR_ID_SYSTEM},
    {FRMIDX_CAP17,    "Capture17",              DDR_ID_SYSTEM},
    {FRMIDX_CAP18,    "Capture18",              DDR_ID_SYSTEM},
    {FRMIDX_CAP19,    "Capture19",              DDR_ID_SYSTEM},
    {FRMIDX_CAP20,    "Capture20",              DDR_ID_SYSTEM},
    {FRMIDX_CAP21,    "Capture21",              DDR_ID_SYSTEM},
    {FRMIDX_CAP22,    "Capture22",              DDR_ID_SYSTEM},
    {FRMIDX_CAP23,    "Capture23",              DDR_ID_SYSTEM},
    {FRMIDX_CAP24,    "Capture24",              DDR_ID_SYSTEM},
    {FRMIDX_CAP25,    "Capture25",              DDR_ID_SYSTEM},
    {FRMIDX_CAP26,    "Capture26",              DDR_ID_SYSTEM},
    {FRMIDX_CAP27,    "Capture27",              DDR_ID_SYSTEM},
    {FRMIDX_CAP28,    "Capture28",              DDR_ID_SYSTEM},
    {FRMIDX_CAP29,    "Capture29",              DDR_ID_SYSTEM},
    {FRMIDX_CAP30,    "Capture30",              DDR_ID_SYSTEM},
    {FRMIDX_CAP31,    "Capture31",              DDR_ID_SYSTEM},
    {FRMIDX_H264E_SYS,"H264E_G1(SYSINFO)",      DDR_ID_SYSTEM},
    {FRMIDX_H264E_DMAMASK0, "H26E_DMAMASK0",    DDR_ID_SYSTEM},
    {FRMIDX_H264E_DMAMASK1, "H26E_DMAMASK1",    DDR_ID_SYSTEM},
    {FRMIDX_USR0,     "User0",                  DDR_ID_SYSTEM},
    {FRMIDX_USR1,     "User1",                  DDR_ID_SYSTEM},
    {FRMIDX_USR2,     "User2",                  DDR_ID_SYSTEM},
    {FRMIDX_USR3,     "User3",                  DDR_ID_SYSTEM},
    {FRMIDX_USR4,     "User4",                  DDR_ID_SYSTEM},
    {FRMIDX_USR5,     "User5",                  DDR_ID_SYSTEM},
    {FRMIDX_USR6,     "User6",                  DDR_ID_SYSTEM},
    {FRMIDX_USR7,     "User7",                  DDR_ID_SYSTEM},
    {FRMIDX_USR8,     "User8",                  DDR_ID_SYSTEM},
    {FRMIDX_USR9,     "User9",                  DDR_ID_SYSTEM},
    {FRMIDX_USR10,    "User10",                 DDR_ID_SYSTEM},
    {FRMIDX_USR11,    "User11",                 DDR_ID_SYSTEM},
    {FRMIDX_USR12,    "User12",                 DDR_ID_SYSTEM},
    {FRMIDX_USR13,    "User13",                 DDR_ID_SYSTEM},
    {FRMIDX_USR14,    "User14",                 DDR_ID_SYSTEM},
    {FRMIDX_USR15,   	"User15",                 DDR_ID_SYSTEM},
};
#elif defined(CONFIG_PLATFORM_GM8181)
id_string_t  id_string_arry[] = 
{    
    {FRMIDX_MIN,      "Unknown",                DDR_ID_SYSTEM},  
    {FRMIDX_H264E_BS, "H264E_BitStream",        DDR_ID_SYSTEM},
    {FRMIDX_H264E_G1, "H264E_G1(DMA_CHAIN)",    DDR_ID1},
    {FRMIDX_H264E_G2, "H264E_G2(REFER+RECON)",  DDR_ID1},
    {FRMIDX_MMAP_M1,  "MMAP",                   DDR_ID_SYSTEM},
    {FRMIDX_MMAP_M2,  "MMAP",                   DDR_ID_SYSTEM},  
    {FRMIDX_H264D_REF, "H264D_Reference",       DDR_ID_SYSTEM},
    {FRMIDX_FFB0_M,   "FFB0(HD)_Master",        DDR_ID_SYSTEM},
    {FRMIDX_FFB0_S1,  "FFB0(HD)_Slave_1",       DDR_ID_SYSTEM},
    {FRMIDX_FFB0_S2,  "FFB0(HD)_Slave_2",       DDR_ID_SYSTEM},
    {FRMIDX_SCAL0,    "Scale_0",                DDR_ID_SYSTEM},
    {FRMIDX_SCAL1,    "Scale_1",                DDR_ID_SYSTEM},
    {FRMIDX_CAP0,     "Capture0",               DDR_ID_SYSTEM},
    {FRMIDX_CAP1,     "Capture1",               DDR_ID1},
    {FRMIDX_CAP2,     "Capture2",               DDR_ID_SYSTEM},
    {FRMIDX_CAP3,     "Capture3",               DDR_ID1},
    {FRMIDX_CAP4,     "Capture4",               DDR_ID_SYSTEM},
    {FRMIDX_CAP5,     "Capture5",               DDR_ID1},
    {FRMIDX_CAP6,     "Capture6",               DDR_ID_SYSTEM},
    {FRMIDX_CAP7,     "Capture7",               DDR_ID1},
    {FRMIDX_CAP8,     "Capture8",               DDR_ID_SYSTEM},
    {FRMIDX_CAP9,     "Capture9",               DDR_ID1},
    {FRMIDX_CAP10,    "Capture10",              DDR_ID_SYSTEM},
    {FRMIDX_CAP11,    "Capture11",              DDR_ID1},
    {FRMIDX_CAP12,    "Capture12",              DDR_ID_SYSTEM},
    {FRMIDX_CAP13,    "Capture13",              DDR_ID1},
    {FRMIDX_CAP14,    "Capture14",              DDR_ID_SYSTEM},
    {FRMIDX_CAP15,    "Capture15",              DDR_ID1},
    {FRMIDX_CAP16,    "Capture16",              DDR_ID_SYSTEM},
    {FRMIDX_CAP17,    "Capture17",              DDR_ID_SYSTEM},
    {FRMIDX_CAP18,    "Capture18",              DDR_ID_SYSTEM},
    {FRMIDX_CAP19,    "Capture19",              DDR_ID_SYSTEM},
    {FRMIDX_CAP20,    "Capture20",              DDR_ID_SYSTEM},
    {FRMIDX_CAP21,    "Capture21",              DDR_ID_SYSTEM},
    {FRMIDX_CAP22,    "Capture22",              DDR_ID_SYSTEM},
    {FRMIDX_CAP23,    "Capture23",              DDR_ID_SYSTEM},
    {FRMIDX_CAP24,    "Capture24",              DDR_ID_SYSTEM},
    {FRMIDX_CAP25,    "Capture25",              DDR_ID_SYSTEM},
    {FRMIDX_CAP26,    "Capture26",              DDR_ID_SYSTEM},
    {FRMIDX_CAP27,    "Capture27",              DDR_ID_SYSTEM},
    {FRMIDX_CAP28,    "Capture28",              DDR_ID_SYSTEM},
    {FRMIDX_CAP29,    "Capture29",              DDR_ID_SYSTEM},
    {FRMIDX_CAP30,    "Capture30",              DDR_ID_SYSTEM},
    {FRMIDX_CAP31,    "Capture31",              DDR_ID_SYSTEM},
    {FRMIDX_H264E_SYS,"H264E_G1(SYSINFO)",      DDR_ID_SYSTEM},
    {FRMIDX_H264E_DMAMASK0, "H26E_DMAMASK0",    DDR_ID_SYSTEM},
    {FRMIDX_H264E_DMAMASK1, "H26E_DMAMASK1",    DDR_ID_SYSTEM},
    {FRMIDX_USR0,     "User0",                  DDR_ID_SYSTEM},
    {FRMIDX_USR1,     "User1",                  DDR_ID_SYSTEM},
    {FRMIDX_USR2,     "User2",                  DDR_ID_SYSTEM},
    {FRMIDX_USR3,     "User3",                  DDR_ID_SYSTEM},
    {FRMIDX_USR4,     "User4",                  DDR_ID_SYSTEM},
    {FRMIDX_USR5,     "User5",                  DDR_ID_SYSTEM},
    {FRMIDX_USR6,     "User6",                  DDR_ID_SYSTEM},
    {FRMIDX_USR7,     "User7",                  DDR_ID_SYSTEM},
    {FRMIDX_USR8,     "User8",                  DDR_ID_SYSTEM},
    {FRMIDX_USR9,     "User9",                  DDR_ID_SYSTEM},
    {FRMIDX_USR10,    "User10",                 DDR_ID_SYSTEM},
    {FRMIDX_USR11,    "User11",                 DDR_ID_SYSTEM},
    {FRMIDX_USR12,    "User12",                 DDR_ID_SYSTEM},
    {FRMIDX_USR13,    "User13",                 DDR_ID_SYSTEM},
    {FRMIDX_USR14,    "User14",                 DDR_ID_SYSTEM},
    {FRMIDX_USR15,    "User15",                 DDR_ID_SYSTEM},    
};

#else
    #error "needs id_string_arry[] table!"
#endif

/*
 * Create a configuration file with default content in user space
 */ 
static int frmmap_write_cfgfile(char *filename)
{   
    int             i, err, ret, retVal = 0;
    unsigned long long offset = 0;
    char            str[LINE_LEN+1];
	mm_segment_t    fs; 
	char            *name[DDR_ID_MAX+1] = {"SYSTEM", "DDR-1", "DDR-2", ""};
	struct  file    *filp = NULL;
	
	/* get current->addr_limit. It should be 0-3G */
	fs = get_fs();
	/* set current->addr_limit to 4G */
	set_fs(get_ds());
	
	filp = filp_open(filename, O_WRONLY|O_CREAT, S_IRWXU);
	if (IS_ERR(filp)) 
	{ 
        err = PTR_ERR(filp);
	    set_fs(fs);
		printk("Frammap fails to open file %s \n", filename);
		return -1;
	} 
	
	vfs_write(filp, (unsigned char *)title1, (unsigned int)strlen(title1), &offset);
	vfs_write(filp, (unsigned char *)title2, (unsigned int)strlen(title2), &offset);

	for (i = 1; i < (sizeof(id_string_arry) / sizeof(id_string_t)); i ++)
	{
	    if (id_string_arry[i].ddr_id >= DDR_ID_MAX)  {
	        printk("Error in id_string_arry[], skip this record! \n");
	        continue;
        }	        
	    memset(str, 0, sizeof(str));
	    sprintf(str, "%-30s      %s\n", id_string_arry[i].string, name[id_string_arry[i].ddr_id]);
#if 1 /* special case, for H264E only */
        if (id_string_arry[i].rmmp_index == FRMIDX_H264E_DMAMASK0)
        {
            memset(str, 0, sizeof(str));
            sprintf(str, "%-30s      %s\n", id_string_arry[i].string, H264E_DMAMASK0_DEF);
        }
        
        if (id_string_arry[i].rmmp_index == FRMIDX_H264E_DMAMASK1)
        {
            memset(str, 0, sizeof(str));
            sprintf(str, "%-30s      %s\n", id_string_arry[i].string, H264E_DMAMASK1_DEF);
        }
#endif
	    if (strlen(str) > LINE_LEN) {
	        printk("name %s is too big, skip it! \n", id_string_arry[i].string);
	        continue;
	    }
	    ret = vfs_write(filp, str, strlen(str), &offset);
	    if (ret <= 0) {
	        printk("Fail to write file %s! \n", filename);
	        retVal = -1;
	        goto exit;
	    }
	}

exit:	
    if (!IS_ERR(filp))
	    filp_close(filp, NULL);
	    
	/* restore the addr_limit */
	set_fs(fs);
	
	return retVal;
}

/*
 * change ddr id by configuration file from usr space
 */
void frmmap_update_ddrid(char *string, DDR_ID_T ddr_id)
{
    int     i;
    
    for (i = 1; i < (sizeof(id_string_arry) / sizeof(id_string_t)); i ++)
    {
        if (!strcmp(id_string_arry[i].string, string))
        {
            id_string_arry[i].ddr_id = ddr_id;
            break;
        }
    }
}

/*
 * Read configuration file from user space.
 * Note: The line return in Windows: 0x0D(CR) 0x0A(LF), in Linux: 0x0A(LF)
 */
static int frmmap_read_cfgfile(char *filename)
{
    int             ret, off = 0, retVal = 0;
    char            name_state, ch;
    unsigned char   str[LINE_LEN+1], ddr_str[20], i;
	mm_segment_t    fs;
	unsigned char   *name[DDR_ID_MAX+1] = {"SYSTEM", "DDR-1", "DDR-2", ""};
	DDR_ID_T        ddr_id;
	struct  file    *filp = NULL;
	unsigned long long offset = 0;
	
	memset(h264e_dmamask0, 0, sizeof(h264e_dmamask0));
	strcpy(h264e_dmamask0, H264E_DMAMASK0_DEF);
	memset(h264e_dmamask1, 0, sizeof(h264e_dmamask1));
	strcpy(h264e_dmamask1, H264E_DMAMASK1_DEF);
	
	/* get current->addr_limit. It should be 0-3G */
	fs = get_fs();
	/* set current->addr_limit to 4G */
	set_fs(get_ds());
	
	filp = filp_open(filename, O_RDONLY, S_IRWXU);
	if (IS_ERR(filp)) 
	{	    
		printk("Frammap: fail to open %s, %s is created.\n", filename, CFG_FILE_TMPPATH);
		retVal = frmmap_write_cfgfile(CFG_FILE_TMPPATH);
	    goto exit;
	} 
		
	/* read out the first line */
	vfs_read(filp, str, strlen(title1), &offset);  // can't use sizeof, it contains null terminated!
	memset(str, 0, sizeof(str));
	vfs_read(filp, str, strlen(title2), &offset);  // can't use sizeof, it contains null terminated!	
    memset(str, 0, sizeof(str));
    memset(ddr_str, 0, sizeof(ddr_str));
    name_state = 1;
    off = 0;
	while ((ret = vfs_read(filp, &ch, 1, &offset)) > 0)
	{
	    if (ch == 0xD)
	        continue;   // next one should be 0xA
	    if (ch == 0x20)
	    {
	        name_state = 0; // next one is DDR string
	        off = 0;
	        continue;       // skip space
	    }
	        
	    if (ch != 0xA)
	    {
	        if (name_state)
	            str[off++] = ch;
            else
                ddr_str[off++] = ch;
	    }    
	    else
	    {
	        for (i = DDR_ID_SYSTEM; i < DDR_ID_MAX; i++)
	        {
	            if (!strcmp(&ddr_str[0], name[i]))
	            {
	                ddr_id = i;
	                break;   
	            }
	        }
	        
	        if (i < DDR_ID_MAX)
	        {
	            frmmap_update_ddrid(str, ddr_id);
	        }
	        else
	        {
	            int     i;
	            
	            /* the following is only for H264E only, it is special case. H264E stores its register setting here. */
	            for (i = 1; i < (sizeof(id_string_arry) / sizeof(id_string_t)); i ++)
                {
                    if (id_string_arry[i].rmmp_index == FRMIDX_H264E_DMAMASK0)
                    {
                        if (!strcmp(id_string_arry[i].string, str))
                        {
                            strcpy(h264e_dmamask0, ddr_str);
                            h264e_dmamask0[strlen(ddr_str)] = 0;
                        }
                    }
                    if (id_string_arry[i].rmmp_index == FRMIDX_H264E_DMAMASK1)
                    {
                        if (!strcmp(id_string_arry[i].string, str))
                        {
                            strcpy(h264e_dmamask1, ddr_str);
                            h264e_dmamask1[strlen(ddr_str)] = 0;
                        }
                    }
                } /* end of special case */
	        }
	           
	        /* new line */
	        memset(str, 0, sizeof(str));
	        memset(ddr_str, 0, sizeof(ddr_str));
	        name_state = 1;
	        off = 0;
	    }
	}
	
exit:
	if (!IS_ERR(filp))
	    filp_close(filp, NULL);
	
	/* restore the addr_limit */
	set_fs(fs);
	
	return retVal;
}

/* return name string
 */
char *id_to_namestring(int rmmp_index)
{
    int     i;
    char    *ddr_string[] = {"DDR0_USR", "DDR1_USR", "DDR2_USR", "AP1_USR", "AP2_USR", "AP3_USR", ""};
    
    switch (rmmp_index)
    {
      case FRMIDX_DDR0:
        return ddr_string[0];
        break;
      case FRMIDX_DDR1:
        return ddr_string[1];
        break;
      case FRMIDX_DDR2:  
        return ddr_string[2];
        break;
      case FRMIDX_AP1:
        return ddr_string[3];
        break;
      case FRMIDX_AP2:
        return ddr_string[4];
        break;
      case FRMIDX_AP3:  
        return ddr_string[5];
        break;
      default:
        break;
    }
    
    for (i = 1; i < (sizeof(id_string_arry) / sizeof(id_string_t)); i ++)
    {
        if (id_string_arry[i].rmmp_index == rmmp_index)
            return id_string_arry[i].string;
    }
    
    return id_string_arry[0].string;
}

/* For H264Ecode only
 */
#include <linux/ctype.h> 
int frm_get_h264e_register(int id, unsigned int *value)
{
    unsigned char   *string, *ptr, i, *prefix = "0x";    
    int             pos, tmp;
    
    if (unlikely((id != FRMIDX_H264E_DMAMASK0) && (id != FRMIDX_H264E_DMAMASK1)))
        return -1;
    
    string = (id == FRMIDX_H264E_DMAMASK0) ? h264e_dmamask0 : h264e_dmamask1;
    /* to lower case first */
    for (i = 0; i < strlen(string); i++)
        string[i] = (unsigned char)tolower(string[i]);
    
    if (!strncmp(string, prefix, 2))
        ptr = &string[2];
    else
        ptr = &string[0];
    
    /* start to translate it into hexdecimal value */
    *value = 0;
    pos = strlen(ptr);
    
    do
    {
        if (isdigit(*ptr))
			tmp = *ptr - 0x30;
        else
			tmp = *ptr - 0x57; // why? try!
        
        *value |= (tmp << (pos - 1) * 4);
        
        pos --;
        ptr ++;
    } while (pos >= 0);
    
    return 0;
}

EXPORT_SYMBOL(frm_get_h264e_register);
