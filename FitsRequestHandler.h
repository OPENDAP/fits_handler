// FitsRequestHandler.h

// 2004 Copyright University Corporation for Atmospheric Research

#ifndef I_FitsRequestHandler_H
#define I_FitsRequestHandler_H

#include "DODSRequestHandler.h"

class FitsRequestHandler : public DODSRequestHandler {
public:
			FitsRequestHandler( string name ) ;
    virtual		~FitsRequestHandler( void ) ;

    static bool		fits_build_das( DODSDataHandlerInterface &dhi ) ;
    static bool		fits_build_dds( DODSDataHandlerInterface &dhi ) ;
    static bool		fits_build_data( DODSDataHandlerInterface &dhi ) ;
    static bool		fits_build_vers( DODSDataHandlerInterface &dhi ) ;
    static bool		fits_build_help( DODSDataHandlerInterface &dhi ) ;
};

#endif

// $Log: FitsRequestHandler.h,v $
