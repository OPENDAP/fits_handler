
# Automake file for fits-handler
#
# 5/17/2005 jhrg
#
# $Id: Makefile.am 12972 2006-01-05 15:41:07Z pwest $

AUTOMAKE_OPTIONS = foreign check-news

ACLOCAL_AMFLAGS = -I conf

bin_PROGRAMS = dap_fits_handler


lib_LTLIBRARIES = libfits_handler.la
if OPENDAPSERVER
    SRCS = $(FITSSRCS) $(FITSHDRS)
    lib_besdir=$(prefix)/lib/bes
    lib_bes_LTLIBRARIES = libfits_module.la
    noinst_PROGRAMS = bes_fits_handler
else
    SRCS = $(DAP_HANDLER_SRC) $(FITSSRCS) $(FITSHDRS)
endif

libfits_handler_la_SOURCES = $(SRCS)

FITSSRCS:=fits_read_attributes.cc fits_read_descriptors.cc

FITSHDRS:=fits_read_attributes.h fits_read_descriptors.h config_fits.h

DAP_HANDLER_SRC = BESAutoPtr.cc

BES_HANDLER_SRC = FitsRequestHandler.cc FitsModule.cc \
		  FitsRequestHandler.h FitsResponseNames.h \
		  FitsModule.h

dap_fits_handler_SOURCES = dap_fits_handler.cc
dap_fits_handler_LDADD = libfits_handler.la

libfits_module_la_SOURCES = $(BES_HANDLER_SRC)
libfits_module_la_CPPFLAGS = $(BES_CFLAGS)
libfits_module_la_LDFLAGS = libfits_handler.la $(BES_LDFLAGS)

bes_fits_handler_SOURCES = bes_fits_handler.cc fits_module.cc \
			   FitsHandlerApp.cc FitsHandlerApp.h
bes_fits_handler_CPPFLAGS = $(BES_CFLAGS)
bes_fits_handler_LDFLAGS = $(BES_LDFLAGS) 
bes_fits_handler_LDADD = libfits_handler.la libfits_module.la $(BES_LDADD)

EXTRA_PROGRAMS = bes_fits_handler

EXTRA_DIST = COPYRIGHT COPYING

CLEANFILES = *~

###########################################################################

# Build linux RPMs

srpm:
	rpmbuild -ts fits_handler-@PACKAGE_VERSION@.tar.gz

rpm:
	rpmbuild -tb fits_handler-@PACKAGE_VERSION@.tar.gz
