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

#include "lide_c_fifo.h"
#include "lide_c_sum8.h"
#include "lide_c_util.h"

/*******************************************************************************
INNER PRODUCT STRUCTURE DEFINITION
*******************************************************************************/

struct _lide_c_sum8_context_struct { 
#include "lide_c_actor_context_type_common.h"

    /* Vector length variable. */

    /* Input ports. */
    lide_c_fifo_pointer x;

    /* Output port. */
    lide_c_fifo_pointer out;
};

/*******************************************************************************
IMPLEMENTATIONS OF INTERFACE FUNCTIONS.
*******************************************************************************/

lide_c_sum8_context_type *lide_c_sum8_new(
        lide_c_fifo_pointer x,
        lide_c_fifo_pointer out) { 

    lide_c_sum8_context_type *context = NULL;

    context = lide_c_util_malloc(sizeof(lide_c_sum8_context_type));
    context->enable =
            (lide_c_actor_enable_function_type)lide_c_sum8_enable;
    context->invoke = 
            (lide_c_actor_invoke_function_type)lide_c_sum8_invoke;
    context->x = x;
    context->out = out;
    return context;
}

boolean lide_c_sum8_enable(
        lide_c_sum8_context_type *context) {
    boolean result = FALSE;

    result = (lide_c_fifo_population(context->x) >= 8) &&
                (lide_c_fifo_population(context->out) <
                lide_c_fifo_capacity(context->out));
    //printf("sum8 result = %d\n", result);

    return result;
}

void lide_c_sum8_invoke(lide_c_sum8_context_type *context) {
    int i = 0;
    int sum = 0;
    int x_value = 0;

    for (i = 0; i<8;i++){ /*it is not correct! you consume 8 elements instead of 1 !!!!!!!!!!!!!!!!*/
        lide_c_fifo_read(context->x, &(x_value));
        sum += x_value;
    }
    //printf("sum8 : %d\n",sum);
    lide_c_fifo_write(context->out, &sum);

    return;
}

void lide_c_sum8_terminate(
        lide_c_sum8_context_type *context) {
    free(context);
}
