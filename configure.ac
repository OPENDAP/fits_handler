# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(fits_handler, 1.0.6, opendap-tech@opendap.org)
AC_CONFIG_AUX_DIR([conf])
AC_CONFIG_MACRO_DIR([conf])

AM_INIT_AUTOMAKE(1.10)

# The handler is a shared object library, so we don't normally need
# the static lib. Override this default by specifying `--enable-static',
# which can make running unit tests with valgrind far more pleasant. 
AM_DISABLE_STATIC

AC_CONFIG_TESTDIR(bes-testsuite, [.])

AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_HEADER([config_fits.h])

dnl flags for the compilers and linkers - set these before locating the
dnl actual tools since some of the AC_PROG macros set these `flag variables'
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
AC_CHECK_LIBDAP([3.11.1],
 [
  LIBS="$LIBS $DAP_SERVER_LIBS"
  CPPFLAGS="$CPPFLAGS $DAP_CFLAGS"
 ],
 [ AC_MSG_ERROR([Cannot find libdap])
])
  
AC_CHECK_BES([3.9.1],
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
            [CFITS_PATH="/usr/local/cfits"])

AC_ARG_WITH([cfits_inc],
            [AS_HELP_STRING([--with-cfits-include=ARG],[cfits include directory])],
            [CFITS_PATH_INC=${withval}], 
            [CFITS_PATH_INC=""])

AC_ARG_WITH([cfits_libdir],
            [AS_HELP_STRING([--with-cfits-libdir=ARG],[cfits library directory])],
            [CFITS_PATH_LIBDIR=${withval}], 
            [CFITS_PATH_LIBDIR=""])

AS_IF([test "z$CFITS_PATH_LIBDIR" = "z"],[CFITS_PATH_LIBDIR="$CFITS_PATH/lib"])

AS_IF([test "z$CFITS_PATH_INC" = "z"],[CFITS_PATH_INC="$CFITS_PATH/include"])

ac_cfits_ok='yes'
CFITS_LIBS="-lcfitsio"
CFITS_LDFLAGS="-L${CFITS_PATH_LIBDIR}" 

AC_SUBST([CFITS_LDFLAGS])
AC_SUBST([CFITS_LIBS])

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

dnl if test "$CFITS_PATH"
dnl then
dnl     AC_MSG_RESULT([$CFITS_PATH])
dnl     LDFLAGS="$LDFLAGS -L${CFITS_PATH}/lib"
dnl     LIBS="$LIBS -lcfitsio"
dnl     CPPFLAGS="$CPPFLAGS -I${CFITS_PATH}/include"
dnl fi

OPENDAP_DEBUG_OPTION

AC_CONFIG_FILES([Makefile
		 bes-testsuite/atlocal
		 bes-testsuite/Makefile])
AC_OUTPUT
