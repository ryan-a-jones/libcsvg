lib_LTLIBRARIES = libcsvg.la
libcsvg_la_SOURCES = csvg.c csvg-mem.c
libcsvg_la_LDFLAGS = -version-info 0:0:0 $(XML2_LIBS)
libcsvg_la_CFLAGS = -Wall -Werror -Wextra $(XML2_CFLAGS) -I$(srcdir)/csvg/

nobase_include_HEADERS = csvg/csvg.h
