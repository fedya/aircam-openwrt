
#ifndef  __UTIL_CFG_H__
#define  __UTIL_CFG_H__ 
  

/* 
 * Set line length in configuration files   
 */
 
#define  FALSE		0
#define  TRUE		1
#define  LINE_LEN	128 //64
#define  RESET		-9999
#define  CHAR_0		'0'
#define  CHAR_9		'9'
#define  CHAR_SPACE	' '
#define  CHAR_NEWLINE	'\n'
#define  CHAR_CR	'\n'
#define  CHAR_RET	'\r'
#define  CHAR_NULL	'\0'
#define  CHAR_TAB	'\t'
#define  CHAR_EQUAL	'='
#define  CHAR_EQUAL	'='
#define  CHAR_LSQR	'['
#define  CHAR_RSQR	']'
#define  CHAR_COMMENT1	'#'
#define  CHAR_COMMENT2	'/'
#define  STR_COMMENT2	"//"
  
/* 
 * Define return error code value 
 */ 
#define ERR_NONE 0       /* read configuration file successfully */ 
#define ERR_NOFILE 2     /* not find or open configuration file */ 
#define ERR_READFILE 3   /* error occur in reading configuration file */ 
#define ERR_FORMAT 4     /* invalid format in configuration file */ 
#define ERR_NOTHING 5     /* not find section or key name in configuration file */ 

/* 
 * Utilitiy macros
 */ 
#define  is_digit(c)		( ((c) >= CHAR_0) && ((c) <= CHAR_9) )
#define  is_not_digit(c)	( ((c) < CHAR_0) || ((c) > CHAR_9) )

/* 
 *   File structure and operations
 */ 
typedef  struct file_s  {
    int		  fd, offset ;
    mm_segment_t  fs ; 
}  file_t ;

#define  file_reset(f)	do { file_lseek((f)->fd,0,0); (f)->offset = 0; } while(0)
extern file_t *gmcfg_open( const char*, const int ) ;
extern void gmcfg_close(file_t *f);

#define  StrToLong	simple_strtol
#define  StrToULong	simple_strtoul
#define  SEE		printk
//#define  file_close(f)	do { sys_close(f->fd); set_fs(f->fs); kfree(f); } while(0)
#define  file_lseek	sys_lseek
#define  file_read	sys_read
#define  file_write	sys_write
#define  alloc_mem(s)	kmalloc((s),GFP_KERNEL)



/* 
 * Read the value of key name in string form 
 */ 
/*
EXT char *getconfigstr(const char* section,		// points to section name
			const char* keyname,	// points to key name
			file_t *cfile);		// points to configuration file
*/
/* 
 * Read the value of key name in integer form 
 */ 
extern int gmcfg_getfieldint(const char* section,	// points to section name
			const char* keyname,	// points to key name
			int* keyvalue,		// points to destination address
			file_t *cfile);		// points to configuration file

#if  0	  // strtoul() doesn't work well
extern int getconfiguint(const char* section,		// points to section name
			 const char* keyname,		// points to key name 
			 unsigned long int* keyvalue,	// points to destination address 
			 file_t *cfile);		// points to configuration file
#endif

#endif  // __UTIL_CFG_H__



