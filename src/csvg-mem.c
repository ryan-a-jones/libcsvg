/*
 *CSVG - SVG Parser
 *Copyright © 2016 Ryan Jones <ryan@rjones.io>
 *
 *Permission is hereby granted, free of charge, to any person obtaining
 *a copy of this software and associated documentation files (the "Software"),
 *to deal in the Software without restriction, including without limitation
 *the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *and/or sell copies of the Software, and to permit persons to whom the
 *Software is furnished to do so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included
 *in all copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 *OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdlib.h> //Default malloc/ free functions
#include <string.h> //memset
#include "csvg-mem.h"

/*****************************************************************/
/*                      Global Variables                         */
/*****************************************************************/

/*Memory allocation functions*/
svg_malloc_t mallocf = malloc;
svg_free_t freef = free;

/*****************************************************************/
/*                  CSVG Interface Functions                     */
/*****************************************************************/

void svg_set_alloc(svg_malloc_t new_mallocf, svg_free_t new_freef)
{
    mallocf = new_mallocf;
    freef = new_freef;
}

/*****************************************************************/
/*                    CSVG Private Macros                        */
/*****************************************************************/
#define REGION_REWIND( reg )        \
    while ((reg) && (reg)->prev) {  \
        (reg) = (reg)->prev;        \
    }

#define REGION_FASTFORWARD( reg )   \
    while ((reg) && (reg)->next) {  \
        (reg) = (reg)->next;        \
    }

#define REG_TYPE_FOR(type) reg_ ## type
#define STRUCT_FOR(type) struct _reg_ ## type

#define DECL_MEM_REGION( type )          \
    typedef STRUCT_FOR(type) {           \
        struct {                         \
            type * begin, * end;         \
        } region, data;                  \
        STRUCT_FOR(type) * next, * prev; \
    } REG_TYPE_FOR(type)

#define NEW_REGION(type) callocf(1, sizeof(REG_TYPE_FOR(type)))
#define FREE_REGION(ptr) freef((ptr))

#define GET_NEW_REGION_SIZE(reg) \
    ((((reg)->region.end - (reg)->region.begin)+1)<<1)

#define APPEND_NEW_MEM_REGION(ptr, type, size)              \
    do {                                                    \
        if(!(ptr)){                                         \
            (ptr) = NEW_REGION(type);                       \
        }else{                                              \
            REGION_FASTFORWARD((ptr));                      \
            (ptr)->next = NEW_REGION(type);                 \
            (ptr)->next->prev = (ptr);                      \
            (ptr) = (ptr)->next;                            \
        }                                                   \
        if(!ptr) continue;                                  \
        (ptr)->region.begin = mallocf((size) * sizeof(type));   \
        (ptr)->region.end = (ptr)->region.begin + ((size) -1);  \
        (ptr)->data.begin = (ptr)->region.begin;                \
        (ptr)->data.end = (ptr)->data.begin;                    \
    } while (0)

#define NEW_MEM_REGION_FAILED(ptr) (!((ptr) && (ptr)->region.begin))

#define FREE_MEM_REGIONS(ptr)                \
    do {                                     \
        REGION_REWIND(ptr);                  \
        while((ptr)){                        \
            if((ptr)->region.begin)          \
                freef((ptr)->region.begin);  \
            if(!(ptr)->next){                \
                FREE_REGION((ptr));          \
                (ptr) = NULL;                \
            }else{                           \
                (ptr) = (ptr)->next;         \
                FREE_REGION((ptr)->prev);    \
            }                                \
        }                                    \
    } while (0);


/*****************************************************************/
/*                   CSVG Memory Structures                      */
/*****************************************************************/

/*Element Region*/
DECL_MEM_REGION( svg_elem_t );
#define SVG_MEM_ELEM_SIZE 20

/*Element Attribute Region*/
DECL_MEM_REGION( svg_elem_attr_t );
#define SVG_MEM_ATTR_SIZE 20

/* svg_mem_t */
struct _svg_mem {
    svg_t * svgh;
    REG_TYPE_FOR(svg_elem_t) * elems;
    REG_TYPE_FOR(svg_elem_attr_t) * elem_attrs;
};

/*****************************************************************/
/*          CSVG Memory Private Function Prototypes              */
/*****************************************************************/

static void * callocf( size_t nmemb, size_t size );

/*****************************************************************/
/*              CSVG Memory Interface Functions                  */
/*****************************************************************/

svg_mem_t * svg_mem()
{
    svg_mem_t * memh;
    int error_status = 0;

    do {
        /*Allocate handle itself*/
        memh = callocf(1, sizeof( svg_mem_t ));
        if(!memh){
            error_status = -1;
            continue;
        }

        /*Allocate SVG Handle*/
        memh->svgh = callocf(1, sizeof( svg_t ));
        if(!memh->svgh){
            error_status = -1;
            continue;
        }

        /*Allocate Element Region*/
        APPEND_NEW_MEM_REGION(memh->elems, svg_elem_t, 20);
        if(NEW_MEM_REGION_FAILED(memh->elems)){
            error_status = -1;
            continue;
        }

        /*Allocate Element Attribute Region*/
        APPEND_NEW_MEM_REGION(memh->elem_attrs, svg_elem_attr_t, 20);
        if(NEW_MEM_REGION_FAILED(memh->elem_attrs)){
            error_status = -1;
            continue;
        }

    } while (0);

    if(error_status < 0 && memh){
        svg_mem_free(memh);
        memh = NULL;
    }

    return memh;
}

void svg_mem_free( svg_mem_t * memh)
{
    REG_TYPE_FOR(svg_elem_t) * tmp_elem;
    REG_TYPE_FOR(svg_elem_attr_t) * tmp_elem_attr;
    if(!memh) return;

    if(memh->svgh) freef(memh->svgh);

    if(memh->elems) FREE_MEM_REGIONS(memh->elems);

    if(memh->elem_attrs) FREE_MEM_REGIONS(memh->elem_attrs);

    freef(memh);
}

svg_t * svg_alloc(svg_mem_t * m)
{
    if(!m) return NULL;
    return m->svgh;
}
void svg_dealloc( svg_mem_t * _, svg_t * __) { return; }

svg_elem_t * svg_elem_alloc( svg_mem_t * memh)
{
    svg_elem_t * ret_elem;
    size_t new_size;

    if(!memh) return NULL;

    /*Get the element at the tail of the structure*/
    ret_elem = memh->elems->data.end;

    /*Return the new element if it was found*/
    if(memh->elems->region.end != memh->elems->data.end){
        memh->elems->data.end++;
        return ret_elem;
    }

    /*Double New Allocation Size*/
    new_size = GET_NEW_REGION_SIZE(memh->elems);

    APPEND_NEW_MEM_REGION(
        memh->elems,
        svg_elem_t,
        new_size
    );

    return ret_elem;
}
void svg_elem_dealloc( svg_mem_t * _, svg_elem_t * __)
{
    (void) _; (void) __;
    return;
}
svg_elem_attr_t * svg_elem_attr_alloc( svg_mem_t * memh)
{
    svg_elem_attr_t * ret_elem_attr;
    size_t new_size;

    if(!memh) return NULL;

    /*Get the element at the tail of the structure*/
    ret_elem_attr = memh->elem_attrs->data.end;

    /*Return the new element if it was found*/
    if(memh->elem_attrs->region.end != memh->elem_attrs->data.end){
        memh->elem_attrs->data.end++;
        return ret_elem_attr;
    }

    /*Double New Allocation Size*/
    new_size = GET_NEW_REGION_SIZE(memh->elem_attrs);

    APPEND_NEW_MEM_REGION(
        memh->elem_attrs,
        svg_elem_t,
        new_size
    );

    return ret_elem_attr;
}
void svg_elem_attr_dealloc( svg_mem_t * _, svg_elem_attr_t * __)
{
    (void) _; (void) __;
    return;
}

static void * callocf( size_t nmemb, size_t size )
{
    /*This function is by no means as efficient is system calloc*/
    /*and thus should probably not be used on large regions.    */

    void * alloc_region;
    size_t total_size = nmemb * size;

    if( alloc_region = mallocf( total_size ) ){
        memset(alloc_region, 0, total_size);
    }

    return alloc_region;
}
