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
#include "lide_c_sha1_src.h"
#include "lide_c_util.h"


/*******************************************************************************
SHA1 ACTOR STRUCTURE DEFINITION
*******************************************************************************/

struct _lide_c_sha1_context_struct { 
#include "lide_c_actor_context_type_common.h"

    /* variable. */

    int data_size;

    /* Input ports. */
    lide_c_fifo_pointer in;

    /* Output port. */
    lide_c_fifo_pointer out1;
    lide_c_fifo_pointer out2;
};


/*******************************************************************************
IMPLEMENTATIONS OF INTERFACE FUNCTIONS.
*******************************************************************************/

lide_c_sha1_context_type *lide_c_sha1_new(
        int data_size,
        lide_c_fifo_pointer in,
        lide_c_fifo_pointer out1,
        lide_c_fifo_pointer out2) { 

    lide_c_sha1_context_type *context = NULL;

    context = lide_c_util_malloc(sizeof(lide_c_sha1_context_type));
    context->enable =
            (lide_c_actor_enable_function_type)lide_c_sha1_enable;
    context->invoke = 
            (lide_c_actor_invoke_function_type)lide_c_sha1_invoke;
    context->data_size = data_size;
    context->in = in;
    context->out1 = out1;
    context->out2 = out2;
    return context;
}

boolean lide_c_sha1_enable(
        lide_c_sha1_context_type *context) {
    boolean result = FALSE;

    result = (lide_c_fifo_population(context->in) >= 1) &&
                (lide_c_fifo_population(context->out1) <
                lide_c_fifo_capacity(context->out1)) &&
                (lide_c_fifo_population(context->out2) <
                lide_c_fifo_capacity(context->out2));

    return result;
}

void lide_c_sha1_invoke(lide_c_sha1_context_type *context) {
    unsigned char *message = (unsigned char *) malloc(context->data_size + 16);
    static unsigned char HMAC[17] = {0x73,0x75,0x6D,0x38,0x2D,0x61,0x70,0x70,
                                    0x6C,0x69,0x63,0x61,0x74,0x69,0x6F,0x6E, '\0'};  // ascii codes of sum8-application

    unsigned char hh[HS];  // hash output as 160 bit.


    int i, value = 0;

    lide_c_fifo_read(context->in,&value);

    sprintf(message,"%d", value);

    strcat(message, HMAC);

    int length = strlen(message);

   
/** message processing **/
    
    sha1(message,length,hh);

    lide_c_fifo_write(context->out1, &value); 

    for(i = 0; i<HS; i++)
        lide_c_fifo_write(context->out2, &hh[i]);   
}

void lide_c_sha1_terminate(
        lide_c_sha1_context_type *context) {
    free(context);
}
