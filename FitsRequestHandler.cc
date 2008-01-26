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
#include "BESDapError.h"
#include "InternalErr.h"
#include "BESResponseNames.h"
#include "BESDataNames.h"
#include "FitsResponseNames.h"
#include "fits_read_attributes.h"
#include "BESDASResponse.h"
#include "fits_read_descriptors.h"
#include "BESDDSResponse.h"
#include "BESDataDDSResponse.h"
#include "BESVersionInfo.h"
#include "BESConstraintFuncs.h"
#include "config_fits.h"

FitsRequestHandler::FitsRequestHandler( const string &name )
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
    BESDASResponse *bdas =
	dynamic_cast<BESDASResponse *>(dhi.response_handler->get_response_object());
    DAS *das = bdas->get_das() ;

    try
    {
	string fits_error ;
	if( !fits_handler::fits_read_attributes( *das,
				    dhi.container->access(),
				    fits_error ) )
	{
	    throw BESDapError( fits_error, false, unknown_error,
			       __FILE__, __LINE__ ) ;
	}
    }
    catch( InternalErr &e )
    {
	throw BESDapError( e.get_error_message(), true, e.get_error_code(),
			   __FILE__, __LINE__ ) ;
    }
    catch( Error &e )
    {
	throw BESDapError( e.get_error_message(), false,
			   e.get_error_code(), __FILE__, __LINE__ ) ;
    }
    catch( ... )
    {
	string err = "Unknown exception caught building cdf das response" ;
	throw BESDapError( err, true, unknown_error,
			   __FILE__, __LINE__ ) ;
    }
    return ret ;
}

bool
FitsRequestHandler::fits_build_dds( BESDataHandlerInterface &dhi )
{
    bool ret = true ;
    BESDDSResponse *bdds =
	dynamic_cast<BESDDSResponse *>(dhi.response_handler->get_response_object());
    DDS *dds = bdds->get_dds() ;

    try
    {
	string fits_error ;
	if( !fits_handler::fits_read_descriptors( *dds,
				     dhi.container->access(),
				     dhi.container->get_symbolic_name(),
				     fits_error ) )
	{
	    throw BESDapError( fits_error, false, unknown_error,
			       __FILE__, __LINE__ ) ;
	}
	dhi.data[POST_CONSTRAINT] = dhi.container->get_constraint();
    }
    catch( InternalErr &e )
    {
	throw BESDapError( e.get_error_message(), true, e.get_error_code(),
			   __FILE__, __LINE__ ) ;
    }
    catch( Error &e )
    {
	throw BESDapError( e.get_error_message(), false,
			   e.get_error_code(), __FILE__, __LINE__ ) ;
    }
    catch( ... )
    {
	string err = "Unknown exception caught building cdf dds response" ;
	throw BESDapError( err, true, unknown_error,
			   __FILE__, __LINE__ ) ;
    }
    return ret ;
}

bool
FitsRequestHandler::fits_build_data( BESDataHandlerInterface &dhi )
{
    BESDataDDSResponse *bdds =
	dynamic_cast<BESDataDDSResponse *>(dhi.response_handler->get_response_object());
    DataDDS *dds = bdds->get_dds() ;

    try
    {
	string fits_error ;
	if( !fits_handler::fits_read_descriptors( *dds,
				     dhi.container->access(),
				     dhi.container->get_symbolic_name(),
				     fits_error ) )
	{
	    throw BESDapError( fits_error, false, unknown_error,
	                       __FILE__, __LINE__ ) ;
	}
	dhi.data[POST_CONSTRAINT] = dhi.container->get_constraint();
    }
    catch( InternalErr &e )
    {
	throw BESDapError( e.get_error_message(), true, e.get_error_code(),
			   __FILE__, __LINE__ ) ;
    }
    catch( Error &e )
    {
	throw BESDapError( e.get_error_message(), false,
			   e.get_error_code(), __FILE__, __LINE__ ) ;
    }
    catch( ... )
    {
	string err = "Unknown exception caught building cdf data response" ;
	throw BESDapError( err, true, unknown_error,
			   __FILE__, __LINE__ ) ;
    }
    return true ;
}

bool
FitsRequestHandler::fits_build_vers( BESDataHandlerInterface &dhi )
{
    BESVersionInfo *info = dynamic_cast<BESVersionInfo *>(dhi.response_handler->get_response_object());
    info->addHandlerVersion( PACKAGE_NAME, PACKAGE_VERSION ) ;
    return true ;
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

/** @brief dumps information about this object
 *
 * Displays the pointer value of this instance, the name of the request
 * handler, and the names of all registered handler functions
 *
 * @param strm C++ i/o stream to dump the information to
 */
void
FitsRequestHandler::dump( ostream &strm ) const
{
    strm << BESIndent::LMarg << "FitsRequestHandler::dump - ("
			     << (void *)this << ")" << endl ;
    BESIndent::Indent() ;
    BESRequestHandler::dump( strm ) ;
    BESIndent::UnIndent() ;
}

