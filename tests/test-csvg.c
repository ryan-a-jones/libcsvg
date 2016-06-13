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

/*
 *Test SVG Handle Basic Allocation
 */
void test_svg_alloc( void ** _ )
{
    svg_t * svgh;
    (void)_;

    /*Get a new handle*/
    assert_non_null(
        svgh = svg()
    );

    /*Make sure that reference has been incremented*/
    assert_int_equal(svgh->ref, 1);

    /*Make sure that reference can be incremented*/
    svg_incref(svgh);
    assert_int_equal(svgh->ref, 2);

    /*... and decremented*/
    svg_decref(svgh);
    assert_int_equal(svgh->ref, 1);

    svg_decref(svgh);
}

/*
 *Test Safety With Bad Data Input
 */
void test_svg_null_input( void ** _ )
{
    svg_t * svgh;
    (void)_;

    /*Get a new handle*/
    assert_non_null(
        svgh = svg()
    );

    svg_incref(NULL);
    svg_decref(NULL);

    svg_decref(svgh);
}

int main(void)
{
    svg_set_alloc(talloc, tfree);
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_svg_alloc),
        cmocka_unit_test(test_svg_null_input)
    };

    return cmocka_run_group_tests(tests,NULL,NULL);
}
