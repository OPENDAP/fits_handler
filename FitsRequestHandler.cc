// FitsRequestHandler.cc

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

// (c) COPYRIGHT University Corporation for Atmostpheric Research 2004-2005
// Please read the full copyright statement in the file COPYRIGHT_UCAR.
//
// Authors:
//      pwest       Patrick West <pwest@ucar.edu>
//      jgarcia     Jose Garcia <jgarcia@ucar.edu>

#include "config.h"

#include <DAS.h>
#include <DDS.h>
#include <Ancillary.h>
#include <InternalErr.h>

#include "FitsRequestHandler.h"
#include <BESResponseHandler.h>
#include <BESDapError.h>

#include <BESDapNames.h>
#include <BESResponseNames.h>
#include <BESDASResponse.h>

#include <BESDDSResponse.h>
#include <BESDataDDSResponse.h>

#include <BESVersionInfo.h>
#include <BESConstraintFuncs.h>
#include <BESServiceRegistry.h>
#include <BESUtil.h>

#include <fitsio.h>

#include "FitsResponseNames.h"
#include "fits_read_attributes.h"
#include "fits_read_descriptors.h"

FitsRequestHandler::FitsRequestHandler(const string &name) :
		BESRequestHandler(name)
{
	add_handler(DAS_RESPONSE, FitsRequestHandler::fits_build_das);
	add_handler(DDS_RESPONSE, FitsRequestHandler::fits_build_dds);
	add_handler(DATA_RESPONSE, FitsRequestHandler::fits_build_data);
	add_handler(VERS_RESPONSE, FitsRequestHandler::fits_build_vers);
	add_handler(HELP_RESPONSE, FitsRequestHandler::fits_build_help);
}

FitsRequestHandler::~FitsRequestHandler()
{
}

bool FitsRequestHandler::fits_build_das(BESDataHandlerInterface &dhi)
{
	BESResponseObject *response = dhi.response_handler->get_response_object();
	BESDASResponse *bdas = dynamic_cast<BESDASResponse *>(response);
	if (!bdas) throw BESInternalError("cast error", __FILE__, __LINE__);

	try {
		bdas->set_container(dhi.container->get_symbolic_name());
		DAS *das = bdas->get_das();
		string accessed = dhi.container->access();
		string fits_error;
		if (!fits_handler::fits_read_attributes(*das, accessed, fits_error)) {
			throw BESDapError(fits_error, false, unknown_error, __FILE__, __LINE__);
		}
		Ancillary::read_ancillary_das(*das, accessed);
		bdas->clear_container();
	}
	catch( InternalErr &e ) {
		throw BESDapError(e.get_error_message(), true, e.get_error_code(), __FILE__, __LINE__);
	}
	catch( Error &e ) {
		throw BESDapError(e.get_error_message(), false, e.get_error_code(), __FILE__, __LINE__);
	}
	catch( ... ) {
		string err = "Unknown exception caught building FITS das response";
		throw BESDapError(err, true, unknown_error, __FILE__, __LINE__);
	}
	return true;
}

bool FitsRequestHandler::fits_build_dds(BESDataHandlerInterface &dhi)
{
	BESResponseObject *response = dhi.response_handler->get_response_object();
	BESDDSResponse *bdds = dynamic_cast<BESDDSResponse *>(response);
	if (!bdds) throw BESInternalError("cast error", __FILE__, __LINE__);

	try {
		bdds->set_container(dhi.container->get_symbolic_name());
		DDS *dds = bdds->get_dds();
		string accessed = dhi.container->access();
		string fits_error;

		if (!fits_handler::fits_read_descriptors(*dds, accessed, fits_error)) {
			throw BESDapError(fits_error, false, unknown_error, __FILE__, __LINE__);
		}

		Ancillary::read_ancillary_dds(*dds, accessed);
		DAS *das = new DAS;
		BESDASResponse bdas(das);
		bdas.set_container(dhi.container->get_symbolic_name());
		if (!fits_handler::fits_read_attributes(*das, accessed, fits_error)) {
			throw BESDapError(fits_error, false, unknown_error, __FILE__, __LINE__);
		}
		Ancillary::read_ancillary_das(*das, accessed);

		dds->transfer_attributes(das);

		bdds->set_constraint(dhi);

		bdds->clear_container();
	}
	catch( InternalErr &e ) {
		throw BESDapError(e.get_error_message(), true, e.get_error_code(), __FILE__, __LINE__);
	}
	catch( Error &e ) {
		throw BESDapError(e.get_error_message(), false, e.get_error_code(), __FILE__, __LINE__);
	}
	catch( ... ) {
		string err = "Unknown exception caught building FITS dds response";
		throw BESDapError(err, true, unknown_error, __FILE__, __LINE__);
	}

	return true;
}

bool FitsRequestHandler::fits_build_data(BESDataHandlerInterface &dhi)
{
	BESResponseObject *response = dhi.response_handler->get_response_object();
	BESDataDDSResponse *bdds = dynamic_cast<BESDataDDSResponse *>(response);
	if (!bdds) throw BESInternalError("cast error", __FILE__, __LINE__);

	try {
		bdds->set_container(dhi.container->get_symbolic_name());
		DataDDS *dds = bdds->get_dds();
		string accessed = dhi.container->access();
		string fits_error;
		if (!fits_handler::fits_read_descriptors(*dds, accessed, fits_error)) {
			throw BESDapError(fits_error, false, unknown_error, __FILE__, __LINE__);
		}
		Ancillary::read_ancillary_dds(*dds, accessed);

		DAS *das = new DAS;
		BESDASResponse bdas(das);
		bdas.set_container(dhi.container->get_symbolic_name());
		if (!fits_handler::fits_read_attributes(*das, accessed, fits_error)) {
			throw BESDapError(fits_error, false, unknown_error, __FILE__, __LINE__);
		}
		Ancillary::read_ancillary_das(*das, accessed);

		dds->transfer_attributes(das);

		bdds->set_constraint(dhi);

		bdds->clear_container();
	}
	catch( InternalErr &e ) {
		throw BESDapError(e.get_error_message(), true, e.get_error_code(), __FILE__, __LINE__);
	}
	catch( Error &e ) {
		throw BESDapError(e.get_error_message(), false, e.get_error_code(), __FILE__, __LINE__);
	}
	catch( ... ) {
		string err = "Unknown exception caught building FITS data response";
		throw BESDapError(err, true, unknown_error, __FILE__, __LINE__);
	}

	return true;
}

bool FitsRequestHandler::fits_build_vers(BESDataHandlerInterface &dhi)
{
	BESResponseObject *response = dhi.response_handler->get_response_object();
	BESVersionInfo *info = dynamic_cast<BESVersionInfo *>(response);
	if (!info) throw BESInternalError("cast error", __FILE__, __LINE__);

	info->add_module(PACKAGE_NAME, PACKAGE_VERSION);

	return true;
}

bool FitsRequestHandler::fits_build_help(BESDataHandlerInterface &dhi)
{
	BESResponseObject *response = dhi.response_handler->get_response_object();
	BESInfo *info = dynamic_cast<BESInfo *>(response);
	if (!info) throw BESInternalError("cast error", __FILE__, __LINE__);

	map<string, string> attrs;
	attrs["name"] = PACKAGE_NAME;
	attrs["version"] = PACKAGE_VERSION;
	list<string> services;
	BESServiceRegistry::TheRegistry()->services_handled(FITS_NAME, services);
	if (services.size() > 0) {
		string handles = BESUtil::implode(services, ',');
		attrs["handles"] = handles;
	}
	info->begin_tag("module", &attrs);
	info->end_tag("module");

	return true;
}

/** @brief dumps information about this object
 *
 * Displays the pointer value of this instance, the name of the request
 * handler, and the names of all registered handler functions
 *
 * @param strm C++ i/o stream to dump the information to
 */
void FitsRequestHandler::dump(ostream &strm) const
{
	strm << BESIndent::LMarg << "FitsRequestHandler::dump - (" << (void *) this << ")" << endl;
	BESIndent::Indent();
	BESRequestHandler::dump(strm);
	BESIndent::UnIndent();
}

