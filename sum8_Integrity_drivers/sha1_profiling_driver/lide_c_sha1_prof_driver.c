
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
#include <time.h>

#include "lide_c_fifo.h"
#include "lide_c_sha1_src.h"
#include "lide_c_sha1_sink.h"
#include "lide_c_util.h"

#define ITERATIONS 500

#define BUFFER_CAPACITY 1024

/* An enumeration of the actors in this application. */
#define ACTOR_SHA1_SRC1 0
#define ACTOR_SHA1_SINK1 1

/* The total number of actors in the application. */
#define ACTOR_COUNT 2

/* 
    Usage: lide_c_inner_product_driver.exe m_file x_file y_file out_file
*/
int main(int argc, char **argv) {
        long tm1, tm2;
        double rate;

        int token_size = 0;  
        lide_c_actor_context_type *actors[ACTOR_COUNT];
        
        /* Connectivity: fifo1: (source,sum8), fifo2: (sum8,sink);
        */
        lide_c_fifo_pointer fifo1 = NULL;

        int i = 0;
        int arg_count = 2;
        
        /* actor descriptors (for diagnostic output) */
        char *descriptors[ACTOR_COUNT] = {"sha1_src1", "sha1_sink1"};

        /* Check program usage. */
        if (argc != arg_count) {
                fprintf(stderr, "lide_c_sum8_driver.exe error: arg count");
                exit(1);
        }   

        /* Open the input and output file(s). */
        i = 1;
        token_size = atoi(argv[i++]);

        /* Create the buffers. */
        fifo1  = lide_c_fifo_new(BUFFER_CAPACITY, sizeof(char));

        /* Create and connect the actors. */
        i = 0;

        actors[ACTOR_SHA1_SRC1] = (lide_c_actor_context_type
                *)(lide_c_sha1_new(token_size,fifo1));
        
        actors[ACTOR_SHA1_SINK1] = (lide_c_actor_context_type
                *)(lide_c_sha1_sink_new(token_size,fifo1));

       
        tm1 = clock();
        for (i=0;i<ITERATIONS;i++)
        {
                lide_c_util_guarded_execution(actors[ACTOR_SHA1_SRC1], descriptors[ACTOR_SHA1_SRC1]);
                lide_c_util_guarded_execution(actors[ACTOR_SHA1_SINK1], descriptors[ACTOR_SHA1_SINK1]);
        }
        tm2 = clock();

        printf("iterations = %d\n", i);
        rate = (double)CLOCKS_PER_SEC*65556.0*8.0/((double)(tm2 - tm1));
        printf("rate = %lf bit/s\n", rate);
    /* Normal termination. */
        return 0;
}
