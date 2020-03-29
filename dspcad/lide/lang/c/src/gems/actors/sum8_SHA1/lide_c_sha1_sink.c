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
#include <string.h>
#include "sha-1.h"

#include "lide_c_fifo.h"
#include "lide_c_sha1_sink.h"
#include "lide_c_util.h"


/*******************************************************************************
SHA1 STRUCTURE DEFINITION
*******************************************************************************/

struct _lide_c_sha1_sink_context_struct { 
#include "lide_c_actor_context_type_common.h"

    /* variable. */

    int data_size;

    /* Input ports. */
    lide_c_fifo_pointer in1;
    lide_c_fifo_pointer in2;

    /* Output port. */
    lide_c_fifo_pointer out;
};

/*******************************************************************************
IMPLEMENTATIONS OF INTERFACE FUNCTIONS.
*******************************************************************************/

lide_c_sha1_sink_context_type *lide_c_sha1_sink_new(
        int data_size,
        lide_c_fifo_pointer in1,
        lide_c_fifo_pointer in2,
        lide_c_fifo_pointer out) { 

    lide_c_sha1_sink_context_type *context = NULL;

    context = lide_c_util_malloc(sizeof(lide_c_sha1_sink_context_type));
    context->enable =
            (lide_c_actor_enable_function_type)lide_c_sha1_sink_enable;
    context->invoke = 
            (lide_c_actor_invoke_function_type)lide_c_sha1_sink_invoke;
    context->data_size = data_size;
    context->in1 = in1;
    context->in2 = in2;
    context->out = out;
    return context;
}

boolean lide_c_sha1_sink_enable(
        lide_c_sha1_sink_context_type *context) {
    boolean result = FALSE;

    result = (lide_c_fifo_population(context->in1) >= 1) &&
             (lide_c_fifo_population(context->in2) >= HS) &&
                (lide_c_fifo_population(context->out) <
                lide_c_fifo_capacity(context->out));
    return result;
}

void lide_c_sha1_sink_invoke(lide_c_sha1_sink_context_type *context) {
    unsigned char *message = (unsigned char *) malloc(context->data_size + 16);
    static unsigned char HMAC[17] = {0x73,0x75,0x6D,0x38,0x2D,0x61,0x70,0x70,
                                    0x6C,0x69,0x63,0x61,0x74,0x69,0x6F,0x6E, '\0'};  // ascii codes of sum8-application

    unsigned char hh[HS];  // hash output as 160 bit.
    unsigned char hh_rx[HS]; 
    int i = 0;
    int value = 0;

    lide_c_fifo_read(context->in1,&value);

    for (i = 0; i<HS; i++)
        lide_c_fifo_read(context->in2, &hh_rx[i]);

    sprintf(message,"%d", value);
    strcat(message, HMAC);

    int length = strlen(message);
    
    sha1(message,length,hh);
    if (!sha1_verify_integrity(hh,hh_rx))
        return;

    lide_c_fifo_write(context->out, &value);    
}

void lide_c_sha1_sink_terminate(
        lide_c_sha1_sink_context_type *context) {
    free(context);
}
