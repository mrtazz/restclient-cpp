AC_PREREQ([2.52])
AC_INIT([restclient-cpp], [0.4.4], [d@unwiredcouch.com])
AM_INIT_AUTOMAKE([foreign subdir-objects])
LT_INIT
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL

# check for headers
AC_CHECK_HEADERS([curl/curl.h])

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lcurl':
AC_CHECK_LIB([curl], [main])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T

AC_CONFIG_FILES([Makefile])

# enable code coverage with ./configure --enable-coverage
AC_ARG_ENABLE(coverage,
    AC_HELP_STRING([--enable-coverage],[Enable code coverage]), [CXXFLAGS=" -O0 -g -ftest-coverage -fprofile-arcs"])

AC_OUTPUT
