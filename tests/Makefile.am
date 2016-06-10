check_PROGRAMS = testcsvgmem
TESTS = $(check_PROGRAMS)

TESTLIBS = $(CMOCKA_LIBS)
TESTCFLAGS = $(CMOCKA_CFLAGS) -I../src -I../../src -I../src/csvg -I../../src/csvg

testcsvgmem_SOURCES = test-csvg-mem.c
testcsvgmem_LDFLAGS = $(TESTLIBS)
testcsvgmem_CFLAGS = $(TESTCFLAGS)
