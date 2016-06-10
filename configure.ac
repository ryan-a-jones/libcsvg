#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([csvg], [0.0.1], [ryan@rjones.io])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/csvg.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
PKG_CHECK_MODULES(
    [CMOCKA],
    [cmocka],
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
