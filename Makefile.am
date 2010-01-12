
# Automake file for fits_handler
#
# 5/17/2005 jhrg
#
# $Id: Makefile.am 12972 2006-01-05 15:41:07Z pwest $

AUTOMAKE_OPTIONS = foreign check-news

ACLOCAL_AMFLAGS = -I conf

SUBDIRS = bes-testsuite

lib_besdir=$(prefix)/lib/bes
lib_bes_LTLIBRARIES = libfits_module.la

FITSSRCS:=fits_read_attributes.cc fits_read_descriptors.cc BESAutoPtr.cc

FITSHDRS:=fits_read_attributes.h fits_read_descriptors.h BESAutoPtr.h config_fits.h

BES_HANDLER_SRC = FitsRequestHandler.cc FitsModule.cc \
		  FitsRequestHandler.h FitsResponseNames.h \
		  FitsModule.h

libfits_module_la_SOURCES = $(FITSSRCS) $(FITSHDRS) $(BES_HANDLER_SRC)
libfits_module_la_CPPFLAGS = $(BES_CPPFLAGS)
libfits_module_la_LDFLAGS = -avoid-version -module 
libfits_module_la_LIBADD = $(BES_DAP_LIBS)

EXTRA_DIST = data COPYRIGHT COPYING fits_handler.spec \
	fits.conf.in OSX_Resources

CLEANFILES = *~ fits.conf

sample_datadir = $(datadir)/hyrax/data/fits
sample_data_DATA = data/20060308.172859.mk4.cpb.fts data/20060308.172859.mk4.rpb.fts data/20060328.185927.chp.bsh.fts data/20060328.190602.dpm.alr.fts

install-data-local: fits.conf
	@currdate=`date +"%y%m%d%H%M"`; \
	test -d $(DESTDIR)$(sysconfdir)/bes || $(MKDIR_P) $(DESTDIR)$(sysconfdir)/bes; \
	test -d $(DESTDIR)$(sysconfdir)/bes/modules || $(MKDIR_P) $(DESTDIR)$(sysconfdir)/bes/modules; \
	test -f $(DESTDIR)$(sysconfdir)/bes/modules/fits.conf && mv -f $(DESTDIR)$(sysconfdir)/bes/modules/fits.conf $(DESTDIR)$(sysconfdir)/bes/modules/fits.conf.$$currdate; \
	$(INSTALL_DATA) fits.conf $(DESTDIR)$(sysconfdir)/bes/modules/fits.conf

uninstall-local:
	rm -f $(DESTDIR)$(sysconfdir)/bes/modules/fits.conf

fits.conf: fits.conf.in config.status
	sed -e "s%[@]bes_modules_dir[@]%${lib_besdir}%" $< > fits.conf

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
	rpmbuild -ts --clean @PACKAGE@-@PACKAGE_VERSION@.tar.gz

rpm: dist
	rpmbuild -tb --clean @PACKAGE@-@PACKAGE_VERSION@.tar.gz

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

