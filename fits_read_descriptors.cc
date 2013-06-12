// fits_read_descriptors.cc

// This file is part of fits_handler, a data handler for the OPeNDAP data
// server. 

// Copyright (c) 2004,2005 University Corporation for Atmospheric Research
// Author: Patrick West <pwest@ucar.edu> and Jose Garcia <jgarcia@ucar.edu>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// You can contact University Corporation for Atmospheric Research at
// 3080 Center Green Drive, Boulder, CO 80301

// (c) COPYRIGHT University Corporation for Atmospheric Research 2004-2005
// Please read the full copyright statement in the file COPYRIGHT_UCAR.
//
// Authors:
//      pwest       Patrick West <pwest@ucar.edu>
//      jgarcia     Jose Garcia <jgarcia@ucar.edu>

#include "fits_read_descriptors.h"

using fits_handler::ltoa;

#include <DDS.h>
#include <Structure.h>
#include <Str.h>
#include <Array.h>
#include <Byte.h>
#include <Int16.h>
#include <Int32.h>
#include <Float32.h>
#include <Float64.h>
#include <Error.h>
#include <mime_util.h>

#include <BESAutoPtr.cc>

#define buffsize 1000

// I just picked this value out of thin air. jhrg 1.9.12
const int max_axes_number = 1024;

string str;
string hdu;

void fits_handler::process_status(int status, string &error)
{
    if (status) {
        fits_report_error(stderr, status);
    }
    char error_description[30] = "";
    fits_get_errstatus(status, error_description);
    error = error_description;
    return;
}

bool fits_handler::fits_read_descriptors(DDS &dds, const string &filename, string &error)
{
    char tmp[100];
    fitsfile *fptr;
    int status = 0;
    int ii;
    int hdutype;
    if (fits_open_file(&fptr, filename.c_str(), READONLY, &status)) {
        error = "Can not open fits file ";
        error += filename;
        return false;
    }
    dds.set_dataset_name(name_path(filename));
    for (ii = 1; !(fits_movabs_hdu(fptr, ii, &hdutype, &status)); ii++) {
        hdu = "HDU_";
        str = "HDU_";
        ltoa(ii, tmp, 10);
        hdu += tmp;
        str += tmp;
        switch (hdutype) {
        case IMAGE_HDU:
            str += "_IMAGE";
            status = process_hdu_image(fptr, dds);
            process_status(status, error);
            break;
        case ASCII_TBL:
            str += "_ASCII";
            status = process_hdu_ascii_table(fptr, dds);
            process_status(status, error);
            break;
        case BINARY_TBL:
            str += "_BINARY";
            status = process_hdu_binary_table(fptr, dds);
            process_status(status, error);
            break;
        default:
            cerr << "Invalid HDU type in file " << filename << endl;
            process_status(1, error);
            break;
        }
    }
    if (status == END_OF_FILE) /* status values are defined in fitsioc.h */
        status = 0; /* got the expected EOF error; reset = 0  */
    else {
        process_status(status, error);
        return false; /* got an unexpected error                */
    }
    if (fits_close_file(fptr, &status)) {
        process_status(status, error);
        return false;
    }
    return true;
}

int fits_handler::process_hdu_image(fitsfile *fptr, DDS &dds)
{
    string datasetname = dds.get_dataset_name();

    BESAutoPtr<Structure> container(new Structure(hdu, datasetname));
    int status = 0;
    int anynull, nfound;
    long fpixel;
    char name[FLEN_KEYWORD];
    char value[FLEN_VALUE];
    char comment[FLEN_COMMENT];
    int nkeys, keypos, jj;
    if (fits_get_hdrpos(fptr, &nkeys, &keypos, &status))
        return status;
    for (jj = 1; jj <= nkeys; jj++) {
        char ttt[30];
        ltoa(jj, ttt, 10);
        string keya = "key_";
        keya += ttt;
        if (fits_read_keyn(fptr, jj, name, value, comment, &status))
            return status;
        BESAutoPtr<Structure> st(new Structure(keya, datasetname));
        BESAutoPtr<Str> s1(new Str("name", datasetname));
        string ppp = name;
        s1->set_value(ppp);
        BESAutoPtr<Str> s2(new Str("value", datasetname));
        ppp = value;
        s2->set_value(ppp);
        BESAutoPtr<Str> s3(new Str("comment", datasetname));
        ppp = comment;
        s3->set_value(ppp);
        st->add_var(s1.get());
        st->add_var(s2.get());
        st->add_var(s3.get());
        container->add_var(st.get());
    }
    if (fits_read_keyword(fptr, "BITPIX", value, NULL, &status))
        return status; // status is not 0, so something is wrong and we get until here...
    int bitpix = atoi(value);
    if (fits_read_keyword(fptr, "NAXIS", value, NULL, &status))
        return status;
    int number_axes = atoi(value);
    // long int *naxes = new long int[number_axes];
    if (number_axes < 0 || number_axes > max_axes_number)
        throw Error("Invalid axes number");

    vector<long int> naxes_vec(number_axes);
    long int *naxes = &naxes_vec[0];
    if (fits_read_keys_lng(fptr, "NAXIS", 1, number_axes, naxes, &nfound, &status))
        return status; // status is not 0, so something is wrong and we get until here...
    anynull = 0;
    fpixel = 1;
    switch (bitpix) {
    case BYTE_IMG: {
        BESAutoPtr<Byte> in(new Byte(str, datasetname));
        BESAutoPtr<Array> arr(new Array(str, datasetname, in.get()));
        long npixels = 1;
        for (register int w = 0; w < number_axes; w++) {
            string name_of_axy = "NAXIS";
            char rdf[90];
            ltoa(w, rdf, 10);
            name_of_axy += rdf;
            arr->append_dim(naxes[w], name_of_axy);
            npixels = npixels * naxes[w];
        }
        dods_byte nullval = 0;
        BESAutoPtr<dods_byte> buffer(new dods_byte[npixels], true);
        fits_read_img(fptr, TBYTE, fpixel, npixels, &nullval, buffer.get(), &anynull, &status);
        arr->set_value(buffer.get(), npixels);
        container->add_var(arr.get());
    }
        break;
    case SHORT_IMG: {

        BESAutoPtr<Int16> in(new Int16(str, datasetname));
        BESAutoPtr<Array> arr(new Array(str, datasetname, in.get()));
        long npixels = 1;
        for (register int w = 0; w < number_axes; w++) {
            string name_of_axy = "NAXIS";
            char rdf[90];
            ltoa(w, rdf, 10);
            name_of_axy += rdf;
            arr->append_dim(naxes[w], name_of_axy);
            npixels = npixels * naxes[w];
        }
        dods_int16 nullval = 0;
        BESAutoPtr<dods_int16> buffer(new dods_int16[npixels], true);
        fits_read_img(fptr, TSHORT, fpixel, npixels, &nullval, buffer.get(), &anynull, &status);
        arr->set_value(buffer.get(), npixels);
        container->add_var(arr.get());
    }
        break;
    case LONG_IMG: {
        BESAutoPtr<Int32> in(new Int32(str, datasetname));
        BESAutoPtr<Array> arr(new Array(str, datasetname, in.get()));
        long npixels = 1;
        for (register int w = 0; w < number_axes; w++) {
            string name_of_axy = "NAXIS";
            char rdf[90];
            ltoa(w, rdf, 10);
            name_of_axy += rdf;
            arr->append_dim(naxes[w], name_of_axy);
            npixels = npixels * naxes[w];
        }
        dods_int32 nullval = 0;
        BESAutoPtr<dods_int32> buffer(new dods_int32[npixels], true);
        fits_read_img(fptr, TLONG, fpixel, npixels, &nullval, buffer.get(), &anynull, &status);
        arr->set_value(buffer.get(), npixels);
        container->add_var(arr.get());
    }
        break;
    case FLOAT_IMG: {
        BESAutoPtr<Float32> in(new Float32(str, datasetname));
        BESAutoPtr<Array> arr(new Array(str, datasetname, in.get()));
        long npixels = 1;
        for (register int w = 0; w < number_axes; w++) {
            string name_of_axy = "NAXIS";
            char rdf[90];
            ltoa(w, rdf, 10);
            name_of_axy += rdf;
            arr->append_dim(naxes[w], name_of_axy);
            npixels = npixels * naxes[w];
        }
        dods_float32 nullval = 0;
        BESAutoPtr<dods_float32> buffer(new dods_float32[npixels], true);
        fits_read_img(fptr, TFLOAT, fpixel, npixels, &nullval, buffer.get(), &anynull, &status);
        arr->set_value(buffer.get(), npixels);
        container->add_var(arr.get());
    }
        break;
    case DOUBLE_IMG: {
        BESAutoPtr<Float64> in(new Float64(str, datasetname));
        BESAutoPtr<Array> arr(new Array(str, datasetname, in.get()));
        long npixels = 1;
        for (register int w = 0; w < number_axes; w++) {
            string name_of_axy = "NAXIS";
            char rdf[90];
            ltoa(w, rdf, 10);
            name_of_axy += rdf;
            arr->append_dim(naxes[w], name_of_axy);
            npixels = npixels * naxes[w];
        }
        dods_float64 nullval = 0;
        BESAutoPtr<dods_float64> buffer(new dods_float64[npixels], true);
        fits_read_img(fptr, TDOUBLE, fpixel, npixels, &nullval, buffer.get(), &anynull, &status);
        arr->set_value(buffer.get(), npixels);
        container->add_var(arr.get());
    }
        break;
    default:
        status = 1;
        break;
    }
    dds.add_var(container.get());
    return status;
}

int fits_handler::process_hdu_ascii_table(fitsfile *fptr, DDS &dds)
{
    string datasetname = dds.get_dataset_name();
    BESAutoPtr<Structure> container(new Structure(hdu, datasetname));
    int status = 0;
    int nfound, anynull;
    int ncols;
    long nrows;
    int nkeys, keypos;
    char name[FLEN_KEYWORD];
    char value[FLEN_VALUE];
    char comment[FLEN_COMMENT];
    anynull = 0;

    if (fits_get_hdrpos(fptr, &nkeys, &keypos, &status))
        return status;
    for (int jj = 1; jj <= nkeys; jj++) {
        char ttt[30];
        ltoa(jj, ttt, 10);
        string keya = "key_";
        keya += ttt;
        if (fits_read_keyn(fptr, jj, name, value, comment, &status))
            return status;
        BESAutoPtr<Structure> st(new Structure(keya, datasetname));
        BESAutoPtr<Str> s1(new Str("name", datasetname));
        string ppp = name;
        s1->set_value(ppp);
        BESAutoPtr<Str> s2(new Str("value", datasetname));
        ppp = value;
        s2->set_value(ppp);
        BESAutoPtr<Str> s3(new Str("comment", datasetname));
        ppp = comment;
        s3->set_value(ppp);
        st->add_var(s1.get());
        st->add_var(s2.get());
        st->add_var(s3.get());
        container->add_var(st.get());
    }

    fits_get_num_rows(fptr, &nrows, &status);
    fits_get_num_cols(fptr, &ncols, &status);

    // I am sure this is one of the most obscure piece of code you have ever seen
    // First I get an auto pointer object to hold securely an array of auto pointer
    // objects holding securely pointers to char...
    BESAutoPtr<BESAutoPtr<char> > ttype_autoptr(new BESAutoPtr<char> [ncols], true);

    // Now I set each one of my auto pointer objects holding pointers to char
    // to hold the address of a dinamically allocated piece of memory (array of chars)...
    for (int j = 0; j < ncols; j++) {
        (ttype_autoptr.get())[j].reset();
        (ttype_autoptr.get())[j].set(new char[FLEN_VALUE], true);
    }

    // Now I align my pointers to char inside each BESAutoPtr <char> object
    // inside my BESAutoPtr< BESAutoPtr <char> > object to a char** pointer.

    // Step 1:
    // I get a insecure pointer an get some memory on it...
    char **ttype = new char*[ncols];

    // Step 2;
    // Lets secure the memory area pointed by ttype inside
    // a BESAutoPtr<char*> object, Lets not forget ttype is a vector
    BESAutoPtr<char*> secure_ttype(ttype, true);

    // Step 3:
    // OK here we are, now we have ncols beautifully aligned pointers to char
    // let the pointer inside ttype point to the same address as the secure ones...
    for (int j = 0; j < ncols; j++)
        ttype[j] = (ttype_autoptr.get())[j].get();

    // Step 4:
    // Now we read the data!
    if (fits_read_keys_str(fptr, "TTYPE", 1, ncols, ttype, &nfound, &status))
        return status;

    // wasn't that fun ? :)


    BESAutoPtr<Structure> table(new Structure(str, datasetname));

    for (int h = 0; h < ncols; h++) {
        int typecode;
        long repeat, width;
        fits_get_coltype(fptr, h + 1, &typecode, &repeat, &width, &status);
        switch (typecode) {
        case TSTRING: {
            int p;
            BESAutoPtr<Str> in(new Str(ttype[h], datasetname));
            BESAutoPtr<Array> arr(new Array(ttype[h], datasetname, in.get()));
            arr->append_dim(nrows);
            char strnull[10] = "";
            char **name = new char*[nrows];
            // secure the pointer for exceptions, remember is an array so second parameter is true
            BESAutoPtr<char *> sa1(name, true);
            // get an auto pointer (sa3) object to hold securely an array of auto pointers to char
            BESAutoPtr<BESAutoPtr<char> > sa3(new BESAutoPtr<char> [nrows], true);
            for (p = 0; p < nrows; p++) {
                // get memory
                name[p] = new char[width + 1];
                // reset auto pointer
                (sa3.get())[p].reset();
                // storage safely the area in the heap pointed by name[p]  in an auto pointer
                (sa3.get())[p].set(name[p], true);
            }
            fits_read_col(fptr, typecode, h + 1, 1, 1, nrows, strnull, name, &anynull, &status);

            string *strings = new string[nrows];
            // secure the pointer for exceptions, remember is an array
            BESAutoPtr<string> sa2(strings, true);

            for (int p = 0; p < nrows; p++)
                strings[p] = name[p];
            arr->set_value(strings, nrows);

            table->add_var(arr.get());
        }
            break;
        case TSHORT: {
            BESAutoPtr<Int16> in(new Int16(ttype[h], datasetname));
            BESAutoPtr<Array> arr(new Array(ttype[h], datasetname, in.get()));
            arr->append_dim(nrows);
            dods_int16 nullval = 0;
            BESAutoPtr<dods_int16> buffer(new dods_int16[nrows], true);
            fits_read_col(fptr, typecode, h + 1, 1, 1, nrows, &nullval, buffer.get(), &anynull, &status);
            arr->set_value(buffer.get(), nrows);
            table->add_var(arr.get());
        }
            break;
        case TLONG: {
            BESAutoPtr<Int32> in(new Int32(ttype[h], datasetname));
            BESAutoPtr<Array> arr(new Array(ttype[h], datasetname, in.get()));
            arr->append_dim(nrows);
            dods_int32 nullval = 0;
            BESAutoPtr<dods_int32> buffer(new dods_int32[nrows], true);
            fits_read_col(fptr, typecode, h + 1, 1, 1, nrows, &nullval, buffer.get(), &anynull, &status);
            arr->set_value(buffer.get(), nrows);
            table->add_var(arr.get());
        }
            break;
        case TFLOAT: {
            BESAutoPtr<Float32> in(new Float32(ttype[h], datasetname));
            BESAutoPtr<Array> arr(new Array(ttype[h], datasetname, in.get()));
            arr->append_dim(nrows);
            dods_float32 nullval = 0;
            BESAutoPtr<dods_float32> buffer(new dods_float32[nrows], true);
            fits_read_col(fptr, typecode, h + 1, 1, 1, nrows, &nullval, buffer.get(), &anynull, &status);
            arr->set_value(buffer.get(), nrows);
            table->add_var(arr.get());
        }
            break;
        case TDOUBLE: {
            BESAutoPtr<Float64> in(new Float64(ttype[h], datasetname));
            BESAutoPtr<Array> arr(new Array(ttype[h], datasetname, in.get()));
            arr->append_dim(nrows);
            dods_float64 nullval = 0;
            BESAutoPtr<dods_float64> buffer(new dods_float64[nrows], true);
            fits_read_col(fptr, typecode, h + 1, 1, 1, nrows, &nullval, buffer.get(), &anynull, &status);
            arr->set_value(buffer.get(), nrows);
            table->add_var(arr.get());
        }
            break;
        }
    }
    container->add_var(table.get());
    dds.add_var(container.get());
    return status;
}

int fits_handler::process_hdu_binary_table(fitsfile *fptr, DDS &dds)
{
    return 0;
}

