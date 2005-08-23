// FitsHandlerApp.h

#include "OPeNDAPBaseApp.h"

class DODSFilter ;

class FitsHandlerApp : public OPeNDAPBaseApp
{
private:
    DODSFilter *		_df ;
public:
    				FitsHandlerApp() ;
    virtual			~FitsHandlerApp() ;
    virtual int			initialize( int argc, char **argv ) ;
    virtual int			run() ;
} ;

