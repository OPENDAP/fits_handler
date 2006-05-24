// FitsHandlerApp.cc

#include <signal.h>
#include <unistd.h>

#include <iostream>

using std::cout ;
using std::cerr ;
using std::endl ;
using std::flush ;

#include "FitsHandlerApp.h"
#include "FitsResponseNames.h"
#include "DODSFilter.h"
#include "DODSCgi.h"
#include "default_module.h"
#include "opendap_commands.h"

FitsHandlerApp::FitsHandlerApp()
    : _df( 0 )
{
}

FitsHandlerApp::~FitsHandlerApp()
{
    if( _df )
    {
	delete _df ;
	_df = 0 ;
    }
}

int
FitsHandlerApp::initialize( int argc, char **argv )
{
    default_module::initialize( argc, argv ) ;
    opendap_commands::initialize( argc, argv ) ;

    OPeNDAPBaseApp::initialize( argc, argv ) ;

    _df = new DODSFilter( argc, argv ) ;

    return 0 ;
}

int
FitsHandlerApp::run()
{
    DODSCgi d( FITS_NAME, *_df ) ;
    d.execute_request() ;

    return 0 ;
}

