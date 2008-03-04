
# Automake file for fits_handler
#
# 5/17/2005 jhrg
#
# $Id: Makefile.am 12972 2006-01-05 15:41:07Z pwest $

AUTOMAKE_OPTIONS = foreign check-news

ACLOCAL_AMFLAGS = -I conf

SUBDIRS = testsuite

bin_PROGRAMS = dap_fits_handler


lib_LTLIBRARIES = libfits_handler.la
if OPENDAPSERVER

lib_besdir=$(prefix)/lib/bes
lib_bes_LTLIBRARIES = libfits_module.la
dist_bin_SCRIPTS = bes-fits-data.sh

endif

libfits_handler_la_SOURCES = $(FITSSRCS) $(FITSHDRS)
libfits_handler_la_LDFLAGS = -version-info $(LIBFITS_HANDLER_VERSION)

FITSSRCS:=fits_read_attributes.cc fits_read_descriptors.cc BESAutoPtr.cc

FITSHDRS:=fits_read_attributes.h fits_read_descriptors.h BESAutoPtr.h config_fits.h

BES_HANDLER_SRC = FitsRequestHandler.cc FitsModule.cc \
		  FitsRequestHandler.h FitsResponseNames.h \
		  FitsModule.h

dap_fits_handler_SOURCES = dap_fits_handler.cc
dap_fits_handler_LDADD = libfits_handler.la

libfits_module_la_SOURCES = $(BES_HANDLER_SRC)
libfits_module_la_CPPFLAGS = $(BES_CPPFLAGS)
libfits_module_la_LDFLAGS = -avoid-version -module 
libfits_module_la_LIBADD = libfits_handler.la $(BES_DAP_LIBS)

EXTRA_DIST = data COPYRIGHT COPYING fits_handler.spec \
	bes-fits-data.sh.in OSX_Resources

CLEANFILES = *~

if OPENDAPSERVER

CLEANFILES += bes-fits-data.sh

sample_datadir = $(datadir)/hyrax/data/fits
sample_data_DATA = data/20060308.172859.mk4.cpb.fts data/20060308.172859.mk4.rpb.fts data/20060328.185927.chp.bsh.fts data/20060328.190602.dpm.alr.fts


do_subst = sed -e 's,[@]sysconfdir[@],$(sysconfdir),g' \
		   	   -e 's,[@]libdir[@],$(libdir),g' \
		   	   -e 's,[@]datadir[@],$(datadir),g'

bes-fits-data.sh: bes-fits-data.sh.in config.status
	$(do_subst) < $(srcdir)/bes-fits-data.sh.in > bes-fits-data.sh
	chmod +x bes-fits-data.sh

# This makes sure that the handler modifies the bes which configure found
# even when distcheck is run.
bes-conf: bes-fits-data.sh
	(bes_prefix=`bes-config --prefix` ; \
	if test "$$bes_prefix" = "/usr" ; \
	 then \
	    ./bes-fits-data.sh /etc/bes/bes.conf /usr/lib/bes ;\
	else \
	    ./bes-fits-data.sh $$bes_prefix/etc/bes/bes.conf $$bes_prefix/lib/bes ; \
	fi)

endif

###########################################################################

# Fortify targets.

.PHONY: fortify
fortify:
	sourceanalyzer -b @PACKAGE@ $(MAKE)
	sourceanalyzer -b @PACKAGE@ -scan -f @PACKAGE@-@PACKAGE_VERSION@.fpr

# Use this to clean the fortify project.
.PHONY: fortifyclean
fortifyclean:
	sourceanalyzer -b @PACKAGE@ -clean

###########################################################################
#
# Build linux RPMs

srpm: dist
	rpmbuild -ts fits_handler-@PACKAGE_VERSION@.tar.gz

rpm: dist
	rpmbuild -tb fits_handler-@PACKAGE_VERSION@.tar.gz

###########################################################################

# Build OS/X Packages.

PACKAGEMAKER=/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker*

UNIV_ARCH=-arch ppc -arch i386
MAC_SYSROOT=Developer/SDKs/MacOSX10.4u.sdk

PKG_CONF_FLAGS=

clean-pkg:
	-rm -rf mac_osx @PACKAGE@-@PACKAGE_VERSION@.pkg @PACKAGE@-@PACKAGE_VERSION@

pkg-build:
	./configure --prefix=/usr --disable-dependency-tracking $(PKG_CONF_FLAGS)
	make clean all
	DESTDIR=${PWD}/mac_osx make install

pkg-main:
	./OSX_Resources/update_mac_package_contents.pl README "configure.ac" "mac_osx/usr"
	${PACKAGEMAKER} -build -p @PACKAGE@-@PACKAGE_VERSION@.pkg \
		-f mac_osx/usr \
		-ds -r OSX_Resources \
		-i OSX_Resources/Info.plist \
		-d OSX_Resources/Description.plist
	-rm -rf mac_osx

pkg-dmg:
	-rm -rf @PACKAGE@-@PACKAGE_VERSION@
	-rm -rf @PACKAGE@-@PACKAGE_VERSION@.dmg
	mkdir @PACKAGE@-@PACKAGE_VERSION@
	mv @PACKAGE@-@PACKAGE_VERSION@.pkg @PACKAGE@-@PACKAGE_VERSION@
	cp README NEWS @PACKAGE@-@PACKAGE_VERSION@
	dropdmg -i --sanitize-for-servers --format zlib @PACKAGE@-@PACKAGE_VERSION@
	-rm -rf @PACKAGE@-@PACKAGE_VERSION@

pkg: clean-pkg pkg-build pkg-main pkg-dmg

