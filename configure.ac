#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([csvg], [0.0.1], [ryan@rjones.io])
AM_INIT_AUTOMAKE
LT_INIT
AC_CONFIG_SRCDIR([src/csvg.c])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
PKG_CHECK_MODULES(
    [XML2],
    [libxml-2.0]
)
AC_SUBST([XML2_CFLAGS])
AC_SUBST([XML2_LIBS])

PKG_CHECK_MODULES(
    [CMOCKA],
    [cmocka >= 1],
    AC_MSG_RESULT([Tests available by running 'make check']),
    AC_MSG_WARN([Cmocka not found - Cannot run tests.])
)
AC_SUBST([CMOCKA_CFLAGS])
AC_SUBST([CMOCKA_LIBS])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([memset])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 tests/Makefile])
AC_OUTPUT
