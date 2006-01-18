
# Automake file for fits-handler
#
# 5/17/2005 jhrg
#
# $Id: Makefile.am 12972 2006-01-05 15:41:07Z pwest $

AUTOMAKE_OPTIONS = foreign check-news

ACLOCAL_AMFLAGS = -I conf

bin_PROGRAMS = dap_fits_handler

if OPENDAPSERVER
    noinst_PROGRAMS = bes_fits_handler
endif

FITSSRCS:=fits_read_attributes.cc fits_read_descriptors.cc

FITSHDRS:=fits_read_attributes.h fits_read_descriptors.h

DAP_HANDLER_SRC = dap_fits_handler.cc DODSAutoPtr.cc config_fits.h

BES_HANDLER_SRC = bes_fits_handler.cc fits_module.cc FitsHandlerApp.cc \
	FitsRequestHandler.cc FitsHandlerApp.h FitsRequestHandler.h \
	FitsResponseNames.h config_fits.h

dap_fits_handler_SOURCES = $(FITSSRCS) $(DAP_HANDLER_SRC) $(FITSHDRS)

bes_fits_handler_SOURCES = $(FITSSRCS) $(BES_HANDLER_SRC) $(FITSHDRS)
bes_fits_handler_CPPFLAGS = $(BES_CPPFLAGS)
bes_fits_handler_LDFLAGS = $(BES_LDFLAGS) 
bes_fits_handler_LDADD = $(BES_LDADD)

EXTRA_PROGRAMS = bes_fits_handler

EXTRA_DIST = COPYRIGHT COPYING

CLEANFILES = *~

###########################################################################

# Build linux RPMs

srpm:
	rpmbuild -ts fits_handler-@PACKAGE_VERSION@.tar.gz

rpm:
	rpmbuild -tb fits_handler-@PACKAGE_VERSION@.tar.gz
