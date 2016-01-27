# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([fits_handler], [1.0.15], [opendap-tech@opendap.org])
AC_CONFIG_AUX_DIR([conf])
AC_CONFIG_MACRO_DIR([conf])

AM_INIT_AUTOMAKE(1.10)

AM_CONDITIONAL([DAP_MODULES], [false])

# The handler is a shared object library, so we don't normally need
# the static lib. Override this default by specifying `--enable-static',
# which can make running unit tests with valgrind far more pleasant. 
AM_DISABLE_STATIC

AC_CONFIG_TESTDIR(bes-testsuite, [.])

AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_HEADER([config.h])

dnl flags for the compilers and linkers - set these before locating the
dnl actual tools since some of the AC_PROG macros set these 'flag variables'
dnl to default values otherwise.

AC_CANONICAL_HOST
AC_SUBST(host)

dnl library visioning: Update these when the interface changes. Generally,
dnl assume that the interface tracks the major and minor release numbers.
LIB_FITS_HANDLER_CURRENT=1
LIB_FITS_HANDLER_AGE=0
LIB_FITS_HANDLER_REVISION=0
AC_SUBST(LIB_FITS_HANDLER_CURRENT)
AC_SUBST(LIB_FITS_HANDLER_AGE)
AC_SUBST(LIB_FITS_HANDLER_REVISION)
LIBFITS_HANDLER_VERSION="$LIB_FITS_HANDLER_CURRENT:$LIB_FITS_HANDLER_REVISION:$LIB_FITS_HANDLER_AGE"
AC_SUBST(LIBFITS_HANDLER_VERSION)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_CHECK_FUNCS([strchr])

AC_PROG_LIBTOOL

dnl Checks for specific libraries
AC_CHECK_LIBDAP([3.13.0],
 [
  LIBS="$LIBS $DAP_SERVER_LIBS"
  CPPFLAGS="$CPPFLAGS $DAP_CFLAGS"
 ],
 [ AC_MSG_ERROR([Cannot find libdap])
])
  
AC_CHECK_BES([3.13.0],
[
],
[ AC_MSG_ERROR([Cannot find bes])
])

dnl Check for the cfits library.
AC_DEFUN([AC_CHECK_CFITS],
[
AC_ARG_WITH([cfits],
            [AS_HELP_STRING([--with-cfits=ARG],[cfits directory])],
            [CFITS_PATH=${withval}], 
            [CFITS_PATH=""])

AC_ARG_WITH([cfits_inc],
            [AS_HELP_STRING([--with-cfits-include=ARG],[cfits include directory])],
            [CFITS_PATH_INC=${withval}], 
            [CFITS_PATH_INC="/usr/include/cfitsio"])

AC_ARG_WITH([cfits_libdir],
            [AS_HELP_STRING([--with-cfits-libdir=ARG],[cfits library directory])],
            [CFITS_PATH_LIBDIR=${withval}], 
            [CFITS_PATH_LIBDIR="/usr/lib"])

AS_IF([test -n "$CFITS_PATH"],[CFITS_PATH_LIBDIR="$CFITS_PATH/lib"])
AS_IF([test -n "$CFITS_PATH"],[CFITS_PATH_INC="$CFITS_PATH/include"])


ac_cfits_save_LDFLAGS="$LDFLAGS"
LDFLAGS="$LDFLAGS -L$CFITS_PATH_LIBDIR"
AC_CHECK_LIB(cfitsio, fits_is_url_absolute, 

[ac_cfits_ok='yes'
CFITS_LIBS="-lcfitsio"
CFITS_LDFLAGS="-L${CFITS_PATH_LIBDIR}" 
AC_SUBST([CFITS_LDFLAGS])
AC_SUBST([CFITS_LIBS])],

[ac_cfits_ok='no']
)
LDFLAGS="$ac_cfits_save_LDFLAGS"

CFITS_CPPFLAGS=
ac_cfits_h='no'
ac_cfits_save_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS -I$CFITS_PATH_INC"
AC_CHECK_HEADERS([fitsio.h],
  [ac_cfits_h='yes'],
  [ac_cfits_h='no']
)
CPPFLAGS=$ac_cfits_save_CPPFLAGS

AS_IF([test "$ac_cfits_h" = 'yes' ],
  [CFITS_CPPFLAGS="-I$CFITS_PATH_INC"]
)
AC_SUBST([CFITS_CPPFLAGS])

AS_IF([test "$ac_cfits_ok" = 'no' -o "$ac_cfits_h" = 'no'],
  [m4_if([$2], [], [:], [$2])],
  [m4_if([$1], [], [:], [$1])]
)

])

AC_CHECK_CFITS(
   [
       LIBS="$LIBS $CFITS_LIBS"
       LDFLAGS="$LDFLAGS $CFITS_LDFLAGS"
       CPPFLAGS="$CPPFLAGS $CFITS_CPPFLAGS"
   ],
   [
       echo "*** Use --with-cfits for the root cfits directory."
       echo "*** Otherwise use --with-cfits-include switch for includes directory"
       echo "*** and --with-cfits-libdir switch for libraries directory."
       AC_MSG_ERROR([cfits library and cfits headers are required.])
   ]
)

DODS_GCOV_VALGRIND

## This sets up the special symbols used to build the bes.conf file for
## the tests. Previously, this was done in the Makefile.am down in the
## test directory, but building the bes.conf here works better with
## autoconf overall and esp. with the 'modules' build. jhrg 11/19/14
modulesdir=$BES_MODULE_DIR
AC_SUBST(modulesdir)

AC_CONFIG_FILES([Makefile
		 bes-testsuite/atlocal
		 bes-testsuite/Makefile
		 bes-testsuite/bes.conf])
		
AC_CONFIG_FILES([bes-testsuite/generate_metadata_baseline.sh], [chmod +x bes-testsuite/generate_metadata_baseline.sh])
AC_CONFIG_FILES([bes-testsuite/generate_data_baseline.sh], [chmod +x bes-testsuite/generate_data_baseline.sh])

AC_OUTPUT
