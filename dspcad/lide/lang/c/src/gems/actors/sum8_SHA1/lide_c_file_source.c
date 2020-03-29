/*******************************************************************************
@ddblock_begin copyright

Copyright (c) 1997-2018
Maryland DSPCAD Research Group, The University of Maryland at College Park 

Permission is hereby granted, without written agreement and without
license or royalty fees, to use, copy, modify, and distribute this
software and its documentation for any purpose, provided that the above
copyright notice and the following two paragraphs appear in all copies
of this software.

IN NO EVENT SHALL THE UNIVERSITY OF MARYLAND BE LIABLE TO ANY PARTY
FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
THE UNIVERSITY OF MARYLAND HAS BEEN ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.

THE UNIVERSITY OF MARYLAND SPECIFICALLY DISCLAIMS ANY WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
MARYLAND HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS.

@ddblock_end copyright
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "lide_c_file_source.h"
#include "lide_c_util.h"

struct _lide_c_file_source_context_struct {
#include "lide_c_actor_context_type_common.h"
    int data;
    lide_c_fifo_pointer out;
};


lide_c_file_source_context_type *lide_c_file_source_new(
        lide_c_fifo_pointer out) {

    lide_c_file_source_context_type *context = NULL;

    context = lide_c_util_malloc(sizeof(lide_c_file_source_context_type));
    context->enable =
            (lide_c_actor_enable_function_type)lide_c_file_source_enable;
    context->invoke =
            (lide_c_actor_invoke_function_type)lide_c_file_source_invoke;

    context->data = 0;
        
    context->mode = LIDE_C_FILE_SOURCE_MODE_WRITE;
    
    context->out = out;
    return context;
}

boolean lide_c_file_source_enable(lide_c_file_source_context_type *context) {
    boolean result = FALSE;

    switch (context->mode) {
    case LIDE_C_FILE_SOURCE_MODE_WRITE:
        result = (lide_c_fifo_population(context->out) < 
                lide_c_fifo_capacity(context->out));
        break;
    default:
        result = FALSE;
        break;
    }
    return result;
}

void lide_c_file_source_invoke(lide_c_file_source_context_type *context) {
    switch (context->mode) {
    case LIDE_C_FILE_SOURCE_MODE_WRITE:
        printf("input : ");
        if (scanf("%d", &context->data) != 1){
            fprintf(stderr,"error in scanning variables");
            exit(1);
        }

        lide_c_fifo_write(context->out, &context->data);
        context->mode = LIDE_C_FILE_SOURCE_MODE_WRITE;
        break;

    default:
        context->mode = LIDE_C_FILE_SOURCE_MODE_WRITE;
        break;
    }
}

void lide_c_file_source_terminate(lide_c_file_source_context_type *context) {
    free(context);
}

