#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>
#include "csvg-mem.c"

#include <stdio.h>

static void * talloc( size_t size )
{
    return test_malloc(size);
}
static void tfree(void * ptr)
{
    test_free(ptr);
}

#define TEST_ALLOC_SUCCESSFUL(memh, reg)                    \
    /*Test that allocation was successful*/                 \
    assert_non_null(memh->reg);                             \
    /*Make sure that data region has size greater than 0*/  \
    assert_false(                                           \
        memh->reg->region.begin ==                          \
        memh->reg->region.end                               \
    );                                                      \
    /*Make sure that the no data has been added to region*/ \
    assert_true(                                            \
        memh->reg->data.begin ==                            \
        memh->reg->region.begin                             \
    );                                                      \
    assert_true(                                            \
        memh->reg->data.begin ==                            \
        memh->reg->data.end                                 \
    )

/*
 * Test that element region has been created correctly
 */
void test_element_region( void ** _ )
{
    size_t region_size, region_count;
    svg_mem_t * memh;
    (void) _;

    memh = svg_mem();

    assert_non_null(memh);

    TEST_ALLOC_SUCCESSFUL(memh, elems);

    region_size = (memh->elems->region.end - memh->elems->region.begin) + 1;

    /*Fill up an entire region*/
    for(region_count = 0; region_count < region_size; region_count++){
        assert_non_null(
            svg_elem_alloc(memh)
        );
    }

    /*Assert that we are in a new region*/
    assert_true(
        memh->elems->data.end == memh->elems->data.begin
    );

    /*Assert that we are linked to the old region*/
    assert_non_null( memh->elems->prev );

    /*Assert that the old region is linked to us*/
    assert_true(
        memh->elems->prev->next ==
        memh->elems
    );

    svg_mem_free(memh);
}

/*
 * Test that element attr region has been created correctly
 */
void test_element_attr_region( void ** _ )
{
    size_t region_size, region_count;
    svg_mem_t * memh;
    (void) _;

    memh = svg_mem();

    assert_non_null(memh);

    TEST_ALLOC_SUCCESSFUL(memh, elem_attrs);

    region_size = (memh->elem_attrs->region.end - memh->elem_attrs->region.begin) + 1;

    /*Fill up an entire region*/
    for(region_count = 0; region_count < region_size; region_count++){
        assert_non_null(
            svg_elem_attr_alloc(memh)
        );
    }

    /*Assert that we are in a new region*/
    assert_true(
        memh->elem_attrs->data.end == memh->elem_attrs->data.begin
    );

    /*Assert that we are linked to the old region*/
    assert_non_null( memh->elem_attrs->prev );

    /*Assert that the old region is linked to us*/
    assert_true(
        memh->elem_attrs->prev->next ==
        memh->elem_attrs
    );

    svg_mem_free(memh);
}

/*
 *Test that interface functions can safely handle null inputs
 */
void test_null_inputs( void ** _ )
{
    svg_t * svgh;
    svg_mem_t * memh;
    svg_elem_t * elem;
    svg_elem_attr_t * attr;

    (void)_;

    assert_non_null(
        memh = svg_mem()
    );

    assert_null(
        svg_alloc(NULL)
    );

    assert_non_null(
        svgh = svg_alloc(memh)
    );

    svg_dealloc( NULL, NULL );
    svg_dealloc( memh, NULL );
    svg_dealloc( NULL, svgh );

    assert_null(
        svg_elem_alloc(NULL)
    );

    assert_non_null(
        elem = svg_elem_alloc(memh)
    );

    svg_elem_dealloc( NULL, NULL );
    svg_elem_dealloc( memh, NULL );
    svg_elem_dealloc( NULL, elem );

    assert_null(
        svg_elem_attr_alloc(NULL)
    );

    assert_non_null(
        attr = svg_elem_attr_alloc(memh)
    );

    svg_elem_attr_dealloc( NULL, NULL );
    svg_elem_attr_dealloc( memh, NULL );
    svg_elem_attr_dealloc( NULL, attr );

    svg_mem_free( NULL );
    svg_mem_free( memh );
}

int main(void)
{
    svg_set_alloc(talloc, tfree);
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( test_null_inputs ),
        cmocka_unit_test( test_element_region ),
        cmocka_unit_test( test_element_attr_region )
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
