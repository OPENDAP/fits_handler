// fits_module.cc

// 2004 Copyright University Corporation for Atmospheric Research

#include <iostream>

using std::endl ;

#include "DODSInitList.h"
#include "DODSRequestHandlerList.h"
#include "FitsRequestHandler.h"
#include "DODSLog.h"
#include "FitsResponseNames.h"

static bool
FitsInit(int, char**)
{
    if( DODSLog::TheLog()->is_verbose() )
	(*DODSLog::TheLog()) << "Initializing Fits Handler:" << endl ;

    if( DODSLog::TheLog()->is_verbose() )
	(*DODSLog::TheLog()) << "    adding " << FITS_NAME << " request handler" << endl ;
    DODSRequestHandlerList::TheList()->add_handler( FITS_NAME, new FitsRequestHandler( FITS_NAME ) ) ;

    return true ;
}

static bool
FitsTerm(void)
{
    if( DODSLog::TheLog()->is_verbose() )
	(*DODSLog::TheLog()) << "Removing Fits Handlers" << endl;
    DODSRequestHandler *rh = DODSRequestHandlerList::TheList()->remove_handler( FITS_NAME ) ;
    if( rh ) delete rh ;
    return true ;
}

FUNINITQUIT( FitsInit, FitsTerm, 3 ) ;

// $Log: fits_module.cc,v $
