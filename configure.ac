#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.52])
AC_INIT([restclient-cpp], [0.1.2], [d@unwiredcouch.com])
AM_INIT_AUTOMAKE([0.1.2 foreign subdir-objects])
LT_INIT
AC_CONFIG_SRCDIR([include/restclient-cpp/meta.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL

# check for headers
AC_CHECK_HEADERS([curl/curl.h],
                  [restclient_cpp_curl_headers_found=yes; break;])

AS_IF([test "x$restclient_cpp_curl_headers_found" != "xyes"],
        [AC_MSG_ERROR([Unable to find the curl headers])])

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lcurl':
AC_CHECK_LIB([curl], [main])
# FIXME: Replace `main' with a function in `-lgtest':
AC_CHECK_LIB([gtest], [main])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
