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

