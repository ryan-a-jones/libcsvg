#ifndef _CSVG_H
#define _CSVG_H

#include <stdlib.h>

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

/****************************************************/
/*                Type-Definitions                  */
/****************************************************/
typedef struct _svg svg_t;

typedef struct _svg_elem svg_elem_t;
typedef enum {
    SVG_ELEM_PATH
} svg_elem_type_t;

typedef struct _svg_elem_attr svg_elem_attr_t;

typedef struct _svg_error svg_error_t;

typedef void * (*svg_malloc_t)(size_t size);
typedef void (*svg_free_t)(void * ptr);

typedef unsigned int svg_option_t;

#define SVG_OPT_NONE 0x00000000

/****************************************************/
/*                  Structures                      */
/****************************************************/

#define SVG_ERROR_TEXT_LENGTH 160

struct _svg_error {
    char text[SVG_ERROR_TEXT_LENGTH];
};

/****************************************************/
/*        Allocation/ Reference Functions           */
/****************************************************/

void svg_set_alloc(svg_malloc_t, svg_free_t);

svg_t *svg();

svg_t * svg_from_file(const char * filename, const svg_option_t);

void svg_incref(svg_t *);

void svg_decref(svg_t *);

/****************************************************/
/*                 SVG Elements                     */
/****************************************************/

svg_elem_t * svg_elem( svg_t *, const svg_elem_type_t );

svg_elem_type_t svg_elem_get_type( svg_elem_t *);

#endif
