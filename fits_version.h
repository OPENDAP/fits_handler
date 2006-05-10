// fits_version.h

// 2004 Copyright University Corporation for Atmospheric Research

#ifndef E_fits_version_h
#define E_fits_version_h 1

#include <string>

using std::string ;

#include "config_fits.h"

inline string
fits_version()
{
    return (string)PACKAGE_STRING + ": compiled on " + __DATE__ + ":" + __TIME__ ;
}

#endif // E_fits_version_h

// $Log: fits_version.h,v $
