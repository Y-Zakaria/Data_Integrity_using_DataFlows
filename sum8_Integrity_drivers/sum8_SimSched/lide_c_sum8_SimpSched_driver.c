
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
#include "lide_c_file_source.h"
#include "lide_c_file_sink.h"
#include "lide_c_sum8.h"
#include "lide_c_util.h"

#define BUFFER_CAPACITY 1024

/* An enumeration of the actors in this application. */
#define ACTOR_SOURCE 0
#define ACTOR_SUM8 1
#define ACTOR_SINK 2

/* The total number of actors in the application. */
#define ACTOR_COUNT 3

/* 
    Usage: lide_c_inner_product_driver.exe m_file x_file y_file out_file
*/
int main(int argc, char **argv) {
        int tokens_num = 0;  
        lide_c_actor_context_type *actors[ACTOR_COUNT];
        
        /* Connectivity: fifo1: (source,sum8), fifo2: (sum8,sink);
        */
        lide_c_fifo_pointer fifo1 = NULL, fifo2 = NULL;

        int token_size = 0;
        int i = 0;
        int arg_count = 2;
        int sched_step = 0;
        
        /* actor descriptors (for diagnostic output) */
        char *descriptors[ACTOR_COUNT] = {"source", "sum8", "sink"};

        /* Check program usage. */
        if (argc != arg_count) {
                fprintf(stderr, "lide_c_sum8_driver.exe error: arg count");
                exit(1);
        }   

        /* Open the input and output file(s). */
        i = 1;
        tokens_num = atoi(argv[i++]);

        /* Create the buffers. */
        token_size = sizeof(int);
        fifo1 = lide_c_fifo_new(BUFFER_CAPACITY, token_size);
        fifo2 = lide_c_fifo_new(BUFFER_CAPACITY, token_size);

        /* Create and connect the actors. */
        i = 0;

        actors[ACTOR_SOURCE] = (lide_c_actor_context_type
                *)(lide_c_file_source_new(fifo1));
        
        actors[ACTOR_SUM8] = (lide_c_actor_context_type
                *)(lide_c_sum8_new(fifo1, fifo2));
        
        actors[ACTOR_SINK] = (lide_c_actor_context_type *)
                (lide_c_file_sink_new(fifo2));

        sched_step = tokens_num/8;

        while (sched_step--){
                for (i=0; i < 8; i++){
                        lide_c_util_guarded_execution(actors[ACTOR_SOURCE], descriptors[ACTOR_SOURCE]);
                }

                lide_c_util_guarded_execution(actors[ACTOR_SUM8], descriptors[ACTOR_SUM8]);
                lide_c_util_guarded_execution(actors[ACTOR_SINK], descriptors[ACTOR_SINK]);
        }
    /* Normal termination. */
        return 0;
}
