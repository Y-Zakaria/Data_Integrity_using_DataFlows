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
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "myTiger.h"

#include "lide_c_fifo.h"
#include "lide_c_Tiger_sink.h"
#include "lide_c_util.h"

//extern word64 table[4*256];

/*******************************************************************************
SHA1 STRUCTURE DEFINITION
*******************************************************************************/

struct _lide_c_tiger_sink_context_struct { 
#include "lide_c_actor_context_type_common.h"

    /* variable. */
    int data_size;

    /* Input ports. */
    lide_c_fifo_pointer in2;

    /* Output port. */
};

/*******************************************************************************
IMPLEMENTATIONS OF INTERFACE FUNCTIONS.
*******************************************************************************/

lide_c_tiger_sink_context_type *lide_c_tiger_sink_new(
        int data_size,
        lide_c_fifo_pointer in2) { 

    lide_c_tiger_sink_context_type *context = NULL;

    context = lide_c_util_malloc(sizeof(lide_c_tiger_sink_context_type));
    context->enable =
            (lide_c_actor_enable_function_type)lide_c_tiger_sink_enable;
    context->invoke = 
            (lide_c_actor_invoke_function_type)lide_c_tiger_sink_invoke;
    context->data_size = data_size;
    context->in2 = in2;
    return context;
}

boolean lide_c_tiger_sink_enable(
        lide_c_tiger_sink_context_type *context) {
    boolean result = FALSE;

    result = (lide_c_fifo_population(context->in2) >= 3);

    return result;
}

void lide_c_tiger_sink_invoke(lide_c_tiger_sink_context_type *context) {
    
  // int value = 0;
    word64 hh[3]; 
    word64 h_rx[3];
char *fp ="/home/zakaria2/Desktop/Fault_Data_Detection_RTOS/m2lse2019/dspcad/lide/lang/c/src/gems/actors/tiger-profiling/test.txt";

    static unsigned char HMAC[17] = {0x73,0x75,0x6D,0x38,0x2D,0x61,0x70,0x70,
                                    0x6C,0x69,0x63,0x61,0x74,0x69,0x6F,0x6E, '\0'};  // ascii codes of sum8-application
    
    unsigned char *message = (unsigned char *) malloc(sizeof(char)*context->data_size + 16);

    lide_c_fifo_read(context->in2, &h_rx[0]);
    lide_c_fifo_read(context->in2, &h_rx[1]);  
    lide_c_fifo_read(context->in2, &h_rx[2]); 

    int fd = open(fp, O_RDONLY);
    int length = read(fd,message,65536);

    strcat(message, HMAC);

    length = strlen(message);
    //printf("%d\n",length);

    tiger(message,length,hh);

    close(fd);
    
    if (!tiger_verify_integrity(hh,h_rx))
        return;

    close(fd);
    //free(message);
}

void lide_c_tiger_sink_terminate(
        lide_c_tiger_sink_context_type *context) {
    free(context);
}
