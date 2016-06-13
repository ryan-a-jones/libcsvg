#include "csvg-parser.h"
#include "csvg-mem.h"
#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#define XML_MAX_DEPTH 20

typedef struct _sax_data {
    svg_t * svgh;
    svg_elem_t * elem_stack[XML_MAX_DEPTH];
    size_t elem_index;
} sax_data_t;

void start_doc( void * sax_data )
{
    printf("Shall we?\n");
    printf("ref %d\n", ((sax_data_t*)sax_data)->svgh->ref);
}

void end_doc( void * _sax_data )
{
    sax_data_t * sax_data = (sax_data_t*) _sax_data;

    /*Assert that the document is balanced*/
    if(sax_data->elem_index != 0)
        sax_data->svgh->error = -1;
}

void start_elem( void * _sax_data, const xmlChar * name, const xmlChar ** attrs )
{
    sax_data_t * sax_data = _sax_data;
    svg_t * svgh = sax_data->svgh;
    svg_elem_t * elem, * parent;
    (void) attrs; (void) name;

    do {
        /*See if an error has already occurred*/
        if(svgh->error < 0)
            continue;

        /*Check that our stack isn't full*/
        if(sax_data->elem_index == XML_MAX_DEPTH)
            continue;

        /*Allocate a new element*/
        elem = svg_elem_alloc(svgh->mem);
        if(!elem){
            svgh->error = -1;
            continue;
        }
        elem->prev = NULL; elem->next = NULL; elem->children = NULL;

        /*Find parent (if it exists)*/
        if(sax_data->elem_index > 0){
            parent = sax_data->elem_stack[
                (sax_data->elem_index - 1)
            ];

            /*Find out if we have siblings*/
            if(parent->children){
                /*Make sure that we are at the head of the siblings*/
                while(parent->children->next){
                    parent->children = parent->children->next;
                }

                /*Append to end of children list*/
                parent->children->next = elem;
                elem->prev = parent->children;
            }

            /*Append child to parent and vis versa*/
            parent->children = elem;
            elem->parent = parent;

        }else{
            svgh->root = elem;
        }

        /*Push new element to stack*/
        sax_data->elem_stack[sax_data->elem_index++] = elem;

    } while (0);
}

void end_elem( void * _sax_data, const xmlChar * name )
{
    sax_data_t * sax_data = (sax_data_t *)_sax_data;
    printf("Actual name: %s\n", name);
    sax_data->elem_index--;
}

int csvg_parse_from_file(svg_t * svgh, const char * filename)
{
    int error_status = 0;
    xmlSAXHandler sax_handler = {
        .startDocument = start_doc,
        .endDocument = end_doc,
        .startElement = start_elem,
        .endElement = end_elem
    };

    sax_data_t sax_data = {
        .svgh = svgh,
        .elem_index = 0
    };

    error_status = xmlSAXUserParseFile(
        &sax_handler, &sax_data, filename
    );

    if(sax_data.svgh->error != 0)
        error_status = sax_data.svgh->error;

    return error_status;
}
