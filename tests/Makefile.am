check_PROGRAMS = testcsvgmem testcsvg testcsvgparser
TESTS = $(check_PROGRAMS)

csvgsrcdir = $(srcdir)/../src

TESTLIBS = $(CMOCKA_LIBS) $(XML2_LIBS)
TESTCFLAGS = -Wall -Werror -Wextra $(CMOCKA_CFLAGS) $(XML2_CFLAGS) -I$(csvgsrcdir) -I$(csvgsrcdir)/csvg  \
	-ftest-coverage -fprofile-arcs

testcsvgmem_SOURCES = test-csvg-mem.c
testcsvgmem_LDFLAGS = $(TESTLIBS)
testcsvgmem_CFLAGS = $(TESTCFLAGS)

testcsvg_SOURCES = test-csvg.c $(csvgsrcdir)/csvg-mem.c $(csvgsrcdir)/csvg-parser.c
testcsvg_LDFLAGS = $(TESTLIBS)
testcsvg_CFLAGS = $(TESTCFLAGS)

testcsvgparser_SOURCES = test-parser.c $(csvgsrcdir)/csvg-mem.c
testcsvgparser_LDFLAGS = $(TESTLIBS)
testcsvgparser_CFLAGS = $(TESTCFLAGS)
