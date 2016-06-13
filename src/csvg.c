#include "csvg.h"
#include "csvg-private.h"
#include "csvg-mem.h"
#include "csvg-parser.h"

/********************************************/
/*      Public Interface Functions          */
/********************************************/

svg_t *svg()
{
    svg_t * svgh = NULL;
    svg_mem_t * mem;
    int error_status = 0;

    do {
        /*Allocate Memory Handler*/
        mem = svg_mem();
        if(!mem){
            error_status = -1;
            continue;
        }

        /*Allocate handle from memory handler*/
        svgh = svg_alloc( mem );
        if(!svgh){
            error_status = -1;
            continue;
        }
        /*Increase reference to handle*/
        svg_incref(svgh);

    } while (0);

    /*Check to see if an error occurred*/
    if(error_status < 0 && mem){
        svg_mem_free(mem);
        svgh = NULL;
    }

    return svgh;
}

svg_t * svg_from_file(const char * filename, const svg_option_t opt)
{
    (void) opt;
    int rc;
    svg_t * svgh = svg();

    /*Check that allocation was successful*/
    if(!svgh) return NULL;

    /*Pass file to parser*/
    rc = csvg_parse_from_file(svgh, filename);

    /*Check return code for error*/
    if(rc > 0){
        svg_decref(svgh);
        svgh = NULL;
    }

    return svgh;
}

void svg_incref(svg_t * svgh)
{
    if(svgh) svgh->ref++;
}

void svg_decref(svg_t * svgh)
{
    if((svgh) && --svgh->ref == 0){
        svg_mem_free(svgh->mem);
    }
}
