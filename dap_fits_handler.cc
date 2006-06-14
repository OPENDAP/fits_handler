// dap_fits_handler.cc

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

#include "config_fits.h"

#include <iostream>
#include <string>

#include "DODSFilter.h"
#include "DDS.h"
#include "DAS.h"
#include "DataDDS.h"

#include "ObjectType.h"
#include "cgi_util.h"
#include "ConstraintEvaluator.h"

#include "fits_read_attributes.h"
#include "fits_read_descriptors.h"

using namespace fits_handler ;

const string cgi_version = PACKAGE_VERSION;

int 
main(int argc, char *argv[])
{
    try { 
	DODSFilter df(argc, argv);
	if (df.get_cgi_version() == "")
	    df.set_cgi_version(cgi_version);

	switch (df.get_response()) {
	  case DODSFilter::DAS_Response: {
	    DAS das;

	    string fits_err ;
	    string dsn = df.get_dataset_name();
	    if( !fits_read_attributes( das, dsn, fits_err ) )
	    {
		throw InternalErr( __FILE__, __LINE__, fits_err ) ;
	    }
	    df.read_ancillary_das(das);
	    df.send_das(das);
	    break;
	  }

	  case DODSFilter::DDS_Response: {
	    DDS dds( NULL ) ;
            ConstraintEvaluator ce;

	    string dsn = df.get_dataset_name();
	    string name = name_path( dsn ) ;
	    string fits_err ;
	    if( !fits_read_descriptors( dds, dsn, name, fits_err ) )
	    {
		throw InternalErr( __FILE__, __LINE__, fits_err ) ;
	    }
	    df.read_ancillary_dds(dds);
	    df.send_dds(dds, ce, true);
	    break;
	  }

	  case DODSFilter::DataDDS_Response: {
	    DDS dds( NULL ) ;
            ConstraintEvaluator ce;

	    string dsn = df.get_dataset_name();
	    string name = name_path( dsn ) ;
	    dds.filename( dsn ) ;
	    string fits_err ;
	    if( !fits_read_descriptors( dds, dsn, name, fits_err ) )
	    {
		throw InternalErr( __FILE__, __LINE__, fits_err ) ;
	    }
	    df.read_ancillary_dds(dds);
	    df.send_data(dds, ce, stdout);
	    break;
	  }

	  case DODSFilter::DDX_Response: {
	    DDS dds( NULL ) ;
	    DAS das;
            ConstraintEvaluator ce;

	    string dsn = df.get_dataset_name();
	    string name = name_path( dsn ) ;

	    dds.filename( dsn ) ;

	    string fits_err ;

	    if( !fits_read_descriptors( dds, dsn, name, fits_err ) )
	    {
		throw InternalErr( __FILE__, __LINE__, fits_err ) ;
	    }
	    df.read_ancillary_dds(dds);

	    if( !fits_read_attributes( das, dsn, fits_err ) )
	    {
		throw InternalErr( __FILE__, __LINE__, fits_err ) ;
	    }
	    df.read_ancillary_das(das);

	    dds.transfer_attributes(&das);

	    df.send_ddx(dds, ce, stdout);
	    break;
	  }

	  case DODSFilter::Version_Response: {
	    df.send_version_info();

	    break;
	  }

	  default:
	    df.print_usage();	// Throws Error
	}
    }
    catch (Error &e) {
	set_mime_text(stdout, dods_error, cgi_version);
	e.print(stdout);
	return 1;
    }

    return 0;
}

// $Log: dap_fits_handler.cc,v $
