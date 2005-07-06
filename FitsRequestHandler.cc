// FitsRequestHandler.cc

// 2004 Copyright University Corporation for Atmospheric Research

#include "FitsRequestHandler.h"
#include "DODSResponseHandler.h"
#include "DODSResponseException.h"
#include "DODSResponseNames.h"
#include "FitsResponseNames.h"
#include "fits_read_attributes.h"
#include "DAS.h"
#include "fits_read_descriptors.h"
#include "DDS.h"
#include "DODSTextInfo.h"
#include "fits_version.h"
#include "DODSConstraintFuncs.h"

FitsRequestHandler::FitsRequestHandler( string name )
    : DODSRequestHandler( name )
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
FitsRequestHandler::fits_build_das( DODSDataHandlerInterface &dhi )
{
    bool ret = true ;
    DAS *das = dynamic_cast<DAS *>(dhi.response_handler->get_response_object());
    string fits_error ;
    if( !fits_handler::fits_read_attributes( *das,
				dhi.container->get_real_name(),
				fits_error ) )
    {
	throw DODSResponseException( fits_error ) ;
    }
    return ret ;
}

bool
FitsRequestHandler::fits_build_dds( DODSDataHandlerInterface &dhi )
{
    bool ret = true ;
    DDS *dds = dynamic_cast<DDS *>(dhi.response_handler->get_response_object());
    string fits_error ;
    if( !fits_handler::fits_read_descriptors( *dds,
                                 dhi.container->get_real_name(),
				 dhi.container->get_symbolic_name(),
				 fits_error ) )
    {
	throw DODSResponseException( fits_error ) ;
    }
    DODSConstraintFuncs::post_append( dhi ) ;
    return ret ;
}

bool
FitsRequestHandler::fits_build_data( DODSDataHandlerInterface &dhi )
{
    DDS *dds = dynamic_cast<DDS *>(dhi.response_handler->get_response_object());
    string fits_error ;
    if( !fits_handler::fits_read_descriptors( *dds,
                                 dhi.container->get_real_name(),
				 dhi.container->get_symbolic_name(),
				 fits_error ) )
    {
	throw DODSResponseException( fits_error ) ;
    }
    DODSConstraintFuncs::post_append( dhi ) ;
    return true ;
}

bool
FitsRequestHandler::fits_build_vers( DODSDataHandlerInterface &dhi )
{
    bool ret = true ;
    DODSTextInfo *info = dynamic_cast<DODSTextInfo *>(dhi.response_handler->get_response_object());
    info->add_data( (string)"    " + fits_version() + "\n" ) ;
    float vers = 0.0 ;
    vers = ffvers( &vers ) ;
    char buf[16] ;
    sprintf( buf, "%.3f",vers ) ;
    info->add_data( (string)"        cfitsio: " + buf + "\n" ) ;
    return ret ;
}

bool
FitsRequestHandler::fits_build_help( DODSDataHandlerInterface &dhi )
{
    bool ret = true ;
    DODSInfo *info = dynamic_cast<DODSInfo *>(dhi.response_handler->get_response_object());

    info->add_data( (string)"fits-handler help: " + fits_version() + "\n" ) ;

    string key ;
    if( dhi.transmit_protocol == "HTTP" )
	key = (string)"Fits.Help." + dhi.transmit_protocol ;
    else
	key = "Fits.Help.TXT" ;
    info->add_data_from_file( key, FITS_NAME ) ;

    return ret ;
}

// $Log: FitsRequestHandler.cc,v $
