# Check for the cdf header.
# AC_CHECK_CDF_HEADER([INCLUDE-DIR],[ACTION-IF-FOUND],
# [ACTION-IF-NOT-FOUND],[INTERFACE-NR])
# if interface number is given, check for a specific interface
# sets CDF_CPPFLAGS and maybe CDF_CDF_3_CPPFLAG
AC_DEFUN([AC_CHECK_CDF_HEADER],
[
  CDF_CPPFLAGS=
  ac_cdf_h='no'
  ac_cdf_h_compile='no'
  ac_cdf_h_preproc='no'
  ac_cdf_include_dir=
  ac_cdf_header_interface=
  
  ac_cdf_save_CPPFLAGS=$CPPFLAGS
  m4_if([$1],[],[:],[
    ac_cdf_include_dir="$1"
    AS_IF([test "z$ac_cdf_include_dir" != "z"],
       [CPPFLAGS="$CPPFLAGS -I$ac_cdf_include_dir"])
  ])
  m4_if([$4],[],[:],[ac_cdf_header_interface=$4])
dnl dont use AC_CHECK_HEADERS to avoid autoconf internal caching
  AC_MSG_CHECKING([for cdf.h with compiler])
  AC_COMPILE_IFELSE([AC_LANG_SOURCE([[#include <cdf.h>]])],
    [
      AC_MSG_RESULT([yes])
      ac_cdf_h_compile='yes'
    ],
    [
      AC_MSG_RESULT([no])
      ac_cdf_h_compile='no'
    ])
    AC_MSG_CHECKING([for cdf.h with preprocessor])
    AC_PREPROC_IFELSE([AC_LANG_SOURCE([[#include <cdf.h>]])],
    [
      AC_MSG_RESULT([yes])
      ac_cdf_h_preproc='yes'
    ],
    [
      AC_MSG_RESULT([no])
      ac_cdf_h_preproc='no (ignored...)'
    ])
  CPPFLAGS="$ac_cdf_save_CPPFLAGS"
  AS_IF([test $ac_cdf_h_compile = 'yes'],
    [ac_cdf_h='yes'
    AS_IF([test "z$ac_cdf_header_interface" = 'z3'],
      [AC_CHECK_CDF_3_HEADER([$1],
         [ac_cdf_h='yes'],[ac_cdf_h='no'])])
    ])

  AS_IF([test "$ac_cdf_h" = 'yes'],
    [
      AS_IF([test "z$ac_cdf_include_dir" != "z"],
        [CDF_CPPFLAGS="-I$ac_cdf_include_dir"])
      m4_if([$2], [], [:], [$2])
    ],
    [m4_if([$3], [], [:], [$3])])

  AC_SUBST([CDF_CPPFLAGS])
])

AC_DEFUN([AC_CHECK_CDF_3_HEADER],
[
  CDF_CDF_3_CPPFLAG=
  ac_check_cdf_3_include=
  ac_check_cdf_3_header='no'
  ac_cdf_save_CPPFLAGS=$CPPFLAGS
  AC_MSG_CHECKING([for cdf 3 interface])
  m4_if([$1],[],[:],[
    ac_check_cdf_3_include="$1"
  ])
  AS_IF([test "z$ac_check_cdf_3_include" != "z"],
    [CPPFLAGS="$CPPFLAGS -I$ac_check_cdf_3_include"])
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <cdf.h>]],
    [[int status;
int ncid;
status = cdf_open("foo.nc", 0, &ncid);
char vernum;
vernum = *cdf_inq_libvers();]])],
    [
      AS_IF([test "z$ac_check_cdf_3_include" != "z"],
        [CDF_CDF_3_CPPFLAG="-I$ac_check_cdf_3_include"])
      ac_check_cdf_3_header='yes'
    ],[ac_check_cdf_3_header='no'])
  CPPFLAGS=$ac_cdf_save_CPPFLAGS
  AS_IF([test "$ac_check_cdf_3_header" = 'yes'],
    [
      AC_MSG_RESULT([yes])
      m4_if([$2], [], [:], [$2])
    ],
    [
      AC_MSG_RESULT([no])
      m4_if([$3], [], [:], [$3])
    ])
  
  AC_SUBST([CDF_CDF_3_CPPFLAG])
])
