#include "csvg-parser.c"
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>
#include "csvg.c"

static void * talloc( size_t size )
{
    return test_malloc(size);
}
static void tfree(void * ptr)
{
    test_free(ptr);
}

void test_do_nothing(void ** _)
{
    (void) _;
}

int main(void)
{
    svg_set_alloc(talloc, tfree);
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_do_nothing)
    };

    return cmocka_run_group_tests(tests,NULL,NULL);
}
