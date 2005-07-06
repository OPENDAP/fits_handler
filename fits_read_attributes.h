#ifndef fits_read_attributes_h_
#define fits_read_attributes_h_

#include <string>

using std::string ;

class DAS ;

namespace fits_handler
{
    bool fits_read_attributes( DAS &das,
                               const string &filename,
			       string &error ) ;

    char *ltoa( long val, char *buf, int base ) ;
}

#endif // fits_read_attributes_h_

