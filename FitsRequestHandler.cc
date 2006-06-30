// FitsRequestHandler.cc

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

#include "FitsRequestHandler.h"
#include "BESResponseHandler.h"
#include "BESResponseException.h"
#include "BESResponseNames.h"
#include "FitsResponseNames.h"
#include "fits_read_attributes.h"
#include "DAS.h"
#include "fits_read_descriptors.h"
#include "DDS.h"
#include "BESVersionInfo.h"
#include "BESConstraintFuncs.h"
#include "config_fits.h"

FitsRequestHandler::FitsRequestHandler( string name )
    : BESRequestHandler( name )
{
    add_handler( DAS_RESPONSE, FitsRequestHandler::fits_build_das ) ;
    add_handler( DDS_RESPONSE, FitsRequestHandler::fits_build_dds ) ;
    add_handler( DATA_RESPONSE, FitsRequestHandler::fits_build_data ) ;
    add_handler( VERS_RESPONSE, FitsRequestHandler::fits_build_vers ) ;
    add_handler( HELP_RESPONSE, FitsRequestHandler::fits_build_help ) ;
}

FitsRequestHandler::~FitsRequestHandler()
{
}

bool
FitsRequestHandler::fits_build_das( BESDataHandlerInterface &dhi )
{
    bool ret = true ;
    DAS *das = dynamic_cast<DAS *>(dhi.response_handler->get_response_object());
    string fits_error ;
    if( !fits_handler::fits_read_attributes( *das,
				dhi.container->get_real_name(),
				fits_error ) )
    {
	throw BESResponseException( fits_error, __FILE__, __LINE__ ) ;
    }
    return ret ;
}

bool
FitsRequestHandler::fits_build_dds( BESDataHandlerInterface &dhi )
{
    bool ret = true ;
    DDS *dds = dynamic_cast<DDS *>(dhi.response_handler->get_response_object());
    string fits_error ;
    if( !fits_handler::fits_read_descriptors( *dds,
                                 dhi.container->get_real_name(),
				 dhi.container->get_symbolic_name(),
				 fits_error ) )
    {
	throw BESResponseException( fits_error, __FILE__, __LINE__ ) ;
    }
    BESConstraintFuncs::post_append( dhi ) ;
    return ret ;
}

bool
FitsRequestHandler::fits_build_data( BESDataHandlerInterface &dhi )
{
    DDS *dds = dynamic_cast<DDS *>(dhi.response_handler->get_response_object());
    string fits_error ;
    if( !fits_handler::fits_read_descriptors( *dds,
                                 dhi.container->get_real_name(),
				 dhi.container->get_symbolic_name(),
				 fits_error ) )
    {
	throw BESResponseException( fits_error, __FILE__, __LINE__ ) ;
    }
    BESConstraintFuncs::post_append( dhi ) ;
    return true ;
}

bool
FitsRequestHandler::fits_build_vers( BESDataHandlerInterface &dhi )
{
    bool ret = true ;
    BESVersionInfo *info = dynamic_cast<BESVersionInfo *>(dhi.response_handler->get_response_object());
    info->addHandlerVersion( PACKAGE_NAME, PACKAGE_VERSION ) ;
    /*
    info->add_data( (string)"    " + fits_version() + "\n" ) ;
    float vers = 0.0 ;
    vers = ffvers( &vers ) ;
    char buf[16] ;
    sprintf( buf, "%.3f",vers ) ;
    info->add_data( (string)"        cfitsio: " + buf + "\n" ) ;
    */
    return ret ;
}

bool
FitsRequestHandler::fits_build_help( BESDataHandlerInterface &dhi )
{
    bool ret = true ;
    BESInfo *info = dynamic_cast<BESInfo *>(dhi.response_handler->get_response_object());
    info->begin_tag( "Handler" ) ;
    info->add_tag( "name", PACKAGE_NAME ) ;
    string handles = (string)DAS_RESPONSE
                     + "," + DDS_RESPONSE
                     + "," + DATA_RESPONSE
                     + "," + HELP_RESPONSE
                     + "," + VERS_RESPONSE ;
    info->add_tag( "handles", handles ) ;
    info->add_tag( "version", PACKAGE_STRING ) ;
    info->end_tag( "Handler" ) ;

    return ret ;
}

