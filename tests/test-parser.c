#include "csvg-parser.c"
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>
#include "csvg.c"
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static void * talloc( size_t size )
{
    return test_malloc(size);
}
static void tfree(void * ptr)
{
    test_free(ptr);
}

void test_simple_svg(void ** _)
{
    (void) _;
    int rc;
    char wdbuff[1000];
    char new_path[1000];
    svg_t * svgh = svg();

    /*Assert that handle was allocated*/
    assert_non_null(svgh);

    printf("%s\n", getcwd(wdbuff, 1000));
    realpath("svg/basic.svg", new_path);
    rc = csvg_parse_from_file(svgh, new_path);

    /*Assert that parsing was successful*/
    assert_int_equal(rc, 0);

    svg_decref(svgh);
}

int main(void)
{
    svg_set_alloc(talloc, tfree);
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_simple_svg)
    };

    return cmocka_run_group_tests(tests,NULL,NULL);
}
