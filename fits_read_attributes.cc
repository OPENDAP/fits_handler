#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#include <fitsio.h>

#include "fits_read_attributes.h"

using fits_handler::ltoa ;

#include "AttrTable.h"
#include "DAS.h"

static const char STRING[]="String";
static const char BYTE[]="Byte";
static const char INT32[]="Int32";
static const char FLOAT64[]="Float64";

bool
fits_handler::fits_read_attributes( DAS &das,
                                    const string &filename,
				    string &error )
{
  fitsfile *fptr;
  char type[]="String";
  char tmp[100];
  AttrTable *at = NULL ;
  int status, nkeys, keypos, hdutype, ii, jj;
  char name [FLEN_KEYWORD];
  char value [FLEN_VALUE];
  char comment [FLEN_COMMENT];
  status=0;
  if ( fits_open_file(&fptr, filename.c_str(), READONLY, &status) )
    {
      error="Can not open fits file ";
      error+=filename;
      return false;
    }
  for (ii = 1; !(fits_movabs_hdu(fptr, ii, &hdutype, &status) ); ii++) 
    {
      at=new AttrTable();
      /* get no. of keywords */
      if (fits_get_hdrpos(fptr, &nkeys, &keypos, &status) )
        return false;
      for (jj = 1; jj <= nkeys; jj++)  
	{
	  if ( fits_read_keyn(fptr, jj, name, value, comment, &status) )
	    return false;
	  if (at)
	    {
	      string com="\"";
	      com+=value;
	      com+=" / ";
	      com+=comment;
	      com+="\"";
	      at->append_attr(name, type, com.c_str());
	    }
	      
      }
      string str="HDU_";
      ltoa(ii,tmp,10);
      str+=tmp;
      das.add_table(str, at);
    }
  
  if (status == END_OF_FILE)   /* status values are defined in fitsioc.h */
    status = 0;              /* got the expected EOF error; reset = 0  */
  else
    return false;     /* got an unexpected error                */ 
  if ( fits_close_file(fptr, &status) )
         return false;
  return true;
}


char *
fits_handler::ltoa(
      long val,                                 /* value to be converted */
      char *buf,                                /* output string         */
      int base)                                 /* conversion base       */
{
      ldiv_t r;                                 /* result of val / base  */

      if (base > 36 || base < 2)          /* no conversion if wrong base */
      {
            *buf = '\0';
            return buf;
      }
      if (val < 0)
            *buf++ = '-';
      r = ldiv (labs(val), base);

      /* output digits of val/base first */

      if (r.quot > 0)
            buf = ltoa ( r.quot, buf, base);
      /* output last digit */

      *buf++ = "0123456789abcdefghijklmnopqrstuvwxyz"[(int)r.rem];
      *buf   = '\0';
      return buf;
}
