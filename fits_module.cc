// fits_module.cc

// 2004 Copyright University Corporation for Atmospheric Research

#include <iostream>

using std::endl ;

#include "DODSInitList.h"
#include "TheRequestHandlerList.h"
#include "FitsRequestHandler.h"
#include "TheDODSLog.h"
#include "FitsResponseNames.h"

static bool
FitsInit(int, char**)
{
    if( TheDODSLog->is_verbose() )
	(*TheDODSLog) << "Initializing Fits Handler:" << endl ;

    if( TheDODSLog->is_verbose() )
	(*TheDODSLog) << "    adding " << FITS_NAME << " request handler" << endl ;
    TheRequestHandlerList->add_handler( FITS_NAME, new FitsRequestHandler( FITS_NAME ) ) ;

    return true ;
}

static bool
FitsTerm(void)
{
    if( TheDODSLog->is_verbose() )
	(*TheDODSLog) << "Removing Fits Handlers" << endl;
    DODSRequestHandler *rh = TheRequestHandlerList->remove_handler( FITS_NAME ) ;
    if( rh ) delete rh ;
    return true ;
}

FUNINITQUIT( FitsInit, FitsTerm, 3 ) ;

// $Log: fits_module.cc,v $
