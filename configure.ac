#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(fontutf8, 0.1, yhfudev@gmail.com)
AC_CONFIG_SRCDIR([tools/getline.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE(-Wall subdir-objects)

#magic for conditional check in Makefile:
MK=''; AC_SUBST(MK)
SED=sed

# Checks for programs.
AC_PROG_AWK
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
#LT_PREREQ([2.2])
#LT_INIT([shared static])
#LT_INIT([disable-static])

# Checks for header files.
AC_CHECK_HEADERS([limits.h stdint.h stdlib.h string.h])

# debug
AC_ARG_ENABLE([debug],
	AS_HELP_STRING([--enable-debug],[Compile the debug version (default: disabled)]),
	[enable_debug=$enableval],
	[enable_debug=no])
AM_CONDITIONAL([DEBUG], [test $enable_debug = "yes"])
if test "x$enable_debug" = "xyes"; then
  changequote({,})
  CFLAGS=`echo "$CFLAGS" | $SED -e 's/-O[0-9s]*//g'`
  CXXFLAGS=`echo "$CXXFLAGS" | $SED -e 's/-O[0-9s]*//g'`
  changequote([,])
  dnl add -O0 only if GCC or ICC is used
  if test "$GCC" = "yes" || test "$ICC" = "yes"; then
    CFLAGS="$CFLAGS -g -O0 -Wall"
    CXXFLAGS="$CXXFLAGS -g -O0 -Wall"
  fi
else
  changequote({,})
  CFLAGS=`echo "$CFLAGS" | $SED -e 's/-g//g'`
  CXXFLAGS=`echo "$CXXFLAGS" | $SED -e 's/-g//g'`
  changequote([,])
fi

# for u8g lib
AC_SUBST([U8G_CFLAGS])
AC_SUBST([U8G_LDFLAGS])
AC_SUBST([U8G_PREFIX])
AC_SUBST([U8G_LIBDIR])
AC_SUBST([U8G_INCLUDE])

AC_ARG_WITH(u8g, [  --with-u8g=PREFIX      Prefix of your u8g installation], [u8g_prefix=$withval], [u8g_prefix="/usr/local/u8g"])
# Setting the prefix to the default if only --with-u8g was given
if test "$u8g_prefix" == "yes"; then
    if test "$withval" == "yes"; then
        u8g_prefix="/usr/local/u8g"
    fi
fi
#U8G_LIBDIR="$u8g_prefix/lib"
#U8G_INCLUDE="$u8g_prefix/include"
U8G_LIBDIR="$u8g_prefix/"
U8G_INCLUDE="$u8g_prefix/"

dnl Provide --with-u8g-lib=<dir> and --with-u8g-include=<dir>.
AC_ARG_WITH([u8g-lib],
    [AS_HELP_STRING([--with-u8g-lib=<dir>],
                    [Path where U8G libraries are installed (default: /usr/local/u8g/lib64)])],
    [U8G_LIBDIR="$withval"], [U8G_LIBDIR="$u8g_prefix/"]
) dnl AC_ARG_WITH
AC_ARG_WITH([u8g-include],
    [AS_HELP_STRING([--with-u8g-include=<dir>],
                    [Path where U8G includes are installed (default: /usr/local/u8g/include)])],
    [U8G_INCLUDE="$withval"], [U8G_INCLUDE="$u8g_prefix/"]
) dnl AC_ARG_WITH

U8G_PREFIX="$u8g_prefix"
U8G_CFLAGS+=" -I$U8G_INCLUDE -I$u8g_prefix/include -I$u8g_prefix/utility/ -I$u8g_prefix/csrc -I$u8g_prefix/cppsrc -I$u8g_prefix/nonarduino"
U8G_LDFLAGS="-L$U8G_LIBDIR -L$u8g_prefix/lib"


# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_CHECK_HEADER_STDBOOL
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([getdelim])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
