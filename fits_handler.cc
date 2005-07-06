// fits_handler.cc

// 2004 Copyright University Corporation for Atmospheric Research

// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of libdap, A C++ implementation of the OPeNDAP Data
// Access Protocol.

// Copyright (c) 2002,2003 OPeNDAP, Inc.
// Author: Patrick West <pwest@ucar.edu>
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
// You can contact OPeNDAP, Inc. at PO Box 112, Saunderstown, RI. 02874-0112.
 
#include "config_fits.h"

#include "FitsResponseNames.h"
#include "DODSCgi.h"
#include "DODSFilter.h"
#include "DODSGlobalIQ.h"
#include "DODSException.h"

int 
main(int argc, char *argv[])
{
    try
    {
	DODSGlobalIQ::DODSGlobalInit( argc, argv ) ;
    }
    catch( DODSException &e )
    {
	cerr << "Error initializing application" << endl ;
	cerr << "    " << e.get_error_description() << endl ;
	return 1 ;
    }

    DODSFilter df(argc, argv);

    DODSCgi d( FITS_NAME, df ) ;
    d.execute_request() ;

    DODSGlobalIQ::DODSGlobalQuit() ;

    return 0;
}

// $Log: fits_handler.cc,v $
