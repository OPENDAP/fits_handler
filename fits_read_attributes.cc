// fits_read_attributes.cc

// This file is part of fits_handler, a data handler for the OPeNDAP data
// server. 

// Copyright (c) 2004,2005 University Corporation for Atmospheric Research
// Author: Patrick West <pwest@ucar.edu> and Jose Garcia <jgarcia@ucar.edu>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// You can contact University Corporation for Atmospheric Research at
// 3080 Center Green Drive, Boulder, CO 80301
 
// (c) COPYRIGHT University Corporation for Atmostpheric Research 2004-2005
// Please read the full copyright statement in the file COPYRIGHT_UCAR.
//
// Authors:
//      pwest       Patrick West <pwest@ucar.edu>
//      jgarcia     Jose Garcia <jgarcia@ucar.edu>

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
