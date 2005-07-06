#ifndef fits_read_descriptors_h_
#define fits_read_descriptors_h_

#include <string>

using std::string ;

#include "fitsio.h"

class DDS ;
class Structure ;

namespace fits_handler
{
    bool fits_read_descriptors( DDS &dds, const string &filename,
			   const string &name, string &error ) ;

    int process_hdu_image( fitsfile *fptr,Structure & ) ;

    int process_hdu_ascii_table( fitsfile *fptr, Structure & ) ;

    int process_hdu_binary_table( fitsfile *fptr, Structure & ) ;

    void process_status( int status, string &error ) ;

    char *ltoa( long val, char *buf, int base ) ;
}

#endif // fits_read_descriptors_h_

