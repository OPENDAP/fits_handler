dnl example of use
dnl AC_CHECK_CDF(
dnl   [
dnl       LIBS="$LIBS $CDF_LIBS"
dnl       LDFLAGS="$LDFLAGS $CDF_LDFLAGS"
dnl       CPPFLAGS="$CPPFLAGS $CDF_CPPFLAGS"
dnl   ],
dnl   [
dnl       echo "*** Use --with-cdf for the root cdf directory."
dnl       echo "*** Otherwise use --with-cdf-include switch for includes directory"
dnl       echo "*** and --with-cdf-libdir switch for libraries directory."
dnl       AC_MSG_ERROR([cdf library and cdf headers are required.])
dnl   ]
dnl )

# Check for the cdf library.
# AC_CHECK_CDF([ACTION-IF-FOUND],[ACTION-IF-NOT-FOUND],[INTERFACE-NR])
# if interface number is given, check for a specific interface
# sets CDF_LDFLAGS, CDF_LIBS, and, by calling other macros
# CDF_CPPFLAGS and maybe CDF_CDF_3_CPPFLAG
AC_DEFUN([AC_CHECK_CDF],
[
  AC_ARG_WITH([cdf],
            [AS_HELP_STRING([--with-cdf=ARG],[cdf directory])],
            [CDF_PATH=$withval], 
            [CDF_PATH=""])

  AC_ARG_WITH([cdf_include],
            [AS_HELP_STRING([--with-cdf-include=ARG],[cdf include directory])],
            [CDF_PATH_INC=$withval], 
            [CDF_PATH_INC=""])

  AC_ARG_WITH([cdf_libdir],
            [AS_HELP_STRING([--with-cdf-libdir=ARG],[cdf library directory])],
            [CDF_PATH_LIBDIR=$withval], 
            [CDF_PATH_LIBDIR=""])

  AS_IF([test "z$CDF_PATH_LIBDIR" = "z"],
    [AS_IF([test "z$CDF_PATH" != "z"],
      [CDF_PATH_LIBDIR="$CDF_PATH/lib"])])

  AS_IF([test "z$CDF_PATH_INC" = "z"],
    [AS_IF([test "z$CDF_PATH" != "z"],
      [CDF_PATH_INC="$CDF_PATH/include"])])

  ac_cdf_ok='no'

  CDF_LIBS=
  CDF_LDFLAGS=
  ac_cdf_save_LDFLAGS=$LDFLAGS
  ac_cdf_save_LIBS=$LIBS
  AS_IF([test "z$CDF_PATH_LIBDIR" != "z"],[LDFLAGS="$LDFLAGS -L$CDF_PATH_LIBDIR"])
  LDFLAGS="$LDFLAGS -L${CDF_PATH_LIBDIR}"
  ac_check_func_checked='ncopen'
  ac_check_interface=
dnl the interface number isn't quoted with "" otherwise a newline 
dnl following the number isn't stripped.
  m4_if([$3],[],[:],[ac_check_interface=$3])
  AS_IF([test "z$ac_check_interface" = 'z3'],
    [ac_check_func_checked='CDFopen'])
dnl the autoconf internal cache isn't avoided because we really check for
dnl libcdf, other libraries that implement the same api have other names
dnl  AC_LINK_IFELSE([AC_LANG_CALL([],[$ac_check_func_checked])],
  AC_CHECK_LIB([cdf],[$ac_check_func_checked],
    [
      ac_cdf_ok='yes'
      CDF_LIBS="-lcdf"
      CDF_LDFLAGS="-L$CDF_PATH_LIBDIR" 
    ],
    [
      ac_cdf_ok='no'
    ])
  LDFLAGS=$ac_cdf_save_LDFLAGS

  AC_SUBST([CDF_LDFLAGS])
  AC_SUBST([CDF_LIBS])

  AC_CHECK_CDF_HEADER([$CDF_PATH_INC],
    [ac_cdf_header='yes'],
    [ac_cdf_header='no'],
    [$3])

  AS_IF([test "$ac_cdf_ok" = 'no' -o "$ac_cdf_header" = 'no'],
    [m4_if([$2], [], [:], [$2])],
    [m4_if([$1], [], [:], [$1])])
])

