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

#ifndef _CSVG_MEM_H
#define _CSVG_MEM_H

#include "csvg.h"
#include "csvg-private.h"


/****************************************************/
/*                Type-Definitions                  */
/****************************************************/

typedef struct _svg_mem svg_mem_t;

svg_mem_t * svg_mem();
void svg_mem_free( svg_mem_t * );

svg_t * svg_alloc( svg_mem_t * );
void svg_dealloc( svg_mem_t *, svg_t * );

svg_elem_t * svg_elem_alloc( svg_mem_t * );
void svg_elem_dealloc( svg_mem_t *, svg_elem_t * );

svg_elem_attr_t * svg_elem_attr_alloc( svg_mem_t * );
void svg_elem_attr_dealloc( svg_mem_t *, svg_elem_attr_t * );

#endif
