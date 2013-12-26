#define __UTIL_CFG_C__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/syscalls.h>
#include <linux/ctype.h>
#include <linux/string.h>

#include "util_cfg.h"


inline file_t *gmcfg_open( const char* name, const int flag )  {
    file_t  *cfile ;
    cfile = (file_t*) alloc_mem( sizeof(file_t) ) ;
    cfile->offset = 0 ;
    cfile->fs = get_fs();
    set_fs(KERNEL_DS);
    cfile->fd = sys_open( name, flag, S_IRWXU ) ;
    if ( cfile->fd < 0 )    return ( NULL ) ;
    return ( cfile ) ;
}

void gmcfg_close(file_t *f)
{
    do { sys_close(f->fd); set_fs(f->fs); kfree(f); } while(0);
}


int  is_delimeter( const char c )  {
    switch ( c )  {
	case  CHAR_RET :
	case  CHAR_NULL :
	case  CHAR_COMMENT2 :
	case  CHAR_COMMENT1 :
	case  CHAR_NEWLINE :
	case  CHAR_TAB :
	case  CHAR_EQUAL :
	case  CHAR_SPACE :
	    return ( TRUE ) ;
    }
    return ( FALSE ) ;
}

/*  Get a line from a file
 *	Limition : There must be a newline charactor within line length 'size'
 */

static  int  readline( file_t *cfile, char *line, const int size )  {
  int   n ;
  char  *p1, *p2 ;
    if ( file_read(cfile->fd, line, size) < 1 )  {
	return ( -1 ) ;		// End of file
    }
    //n = (int) line + size ;
    if ( NULL != ( p1 = strchr( line, CHAR_NEWLINE ) ) )  {
	*p1 = CHAR_NULL ;	// replace newline with NULL char
    } else  {
	p1 = line + size ;	// newline char not found
    }

    if ( NULL != ( p2 = strchr( line, CHAR_RET ) ) )  {
	*p2 = CHAR_NULL ;	// replace the 'return' char with NULL
    } else  {
	p2 = line + size ;	// 'return' char not found
    }

    if ( ((line+size) == p1) && ((line+size) == p2) )  {
	return ( -1 ) ;		// neither 'newline' nor 'return' char found
    }
    n = (int) ( ( p1 < p2 ) ? p1 : p2 ) - (int) line ;	// calculate length of the line
    cfile->offset += ( n + 1 ) ;		   // record & move current
    file_lseek( cfile->fd, cfile->offset, 0 ) ;	   //  read/write pointer in file

    /**  discard comments  **/
    if ( NULL != ( p1 = strchr( line, CHAR_COMMENT1 ) ) )
	*p1 = CHAR_NULL ;	// discard words behind '#'
    if ( NULL != ( p1 = strstr( line, STR_COMMENT2 ) ) )
	*p1 = CHAR_NULL ;	// discard words behind '//'

//    SEE("%s() line=[%s]\n", __func__,line) ;
    return ( n ) ;
}


int  find_section( const char *section, file_t *cfile )  {
    char  *s1, *s2 ;
    char  line[LINE_LEN] ;

    while ( readline( cfile, line, LINE_LEN ) >= 0 )  {
	if ( NULL == ( s1 = strchr(line,CHAR_LSQR) ) )  {
	    continue ;		// '[' not found
	}
	if ( NULL != ( s2 = strchr(line,CHAR_RSQR) ) )  {
	    *s2 = CHAR_NULL ;	// ']' found, replace by NULL char
	}
	if ( strcmp( ++s1, section ) )	{	// compare the section name
	    continue ;		// section name not match
	}
	return ( TRUE ) ;   // section name matched
    }
    return ( FALSE ) ;		// specified section name not found
}

char *find_keystring( const char *keyname, file_t *cfile, char *line )  {
  char  *s1, *s2 ;
  int   keylen =  strlen(keyname) ;

    while ( readline( cfile, line, LINE_LEN ) >= 0 )  {
	//  search the keyname in this line
	if ( NULL == ( s1 = strstr(line,keyname) ) )  {
	    continue ;		// keyname is not a substring of this line
	}
	    
	//  determine the end of the candidate keyname (s1)
	if ( ! is_delimeter( s1[keylen] ) )  {
	    continue ;		// s1 is a super set of keyname, not exactly match.
	}

	//  keyname matched, looking for the equal sigh '=' after keyname
	if ( NULL != ( s2 = strchr(s1+keylen, CHAR_EQUAL) ) )  
	    {
	    for ( ++s2; is_not_digit(*s2); ++s2 )
		        ;		// skip non-digit charactors next to '='.
	}

	return ( s2 ) ;		// return the value string
    }
    return ( (char*) NULL ) ;
}

int  gmcfg_getfieldint(const char *section, const char *keyname, int *keyvalue, file_t *cfile )
{
    char *keystr ;
    char  line[LINE_LEN];
    
    file_reset( cfile ) ;
    if ( ! find_section( section, cfile ) )
        return ( -1 ) ;		// section not found
    
    if ( NULL == ( keystr = find_keystring( keyname, cfile,line) ) )
        return ( -1 ) ;		// keyname not found
    
    *keyvalue = StrToLong( keystr, NULL, 0 ) ;	// convert the string of key value
    
    return ( 1 ) ;
}




EXPORT_SYMBOL(gmcfg_open);
EXPORT_SYMBOL(gmcfg_close);
EXPORT_SYMBOL(gmcfg_getfieldint);


