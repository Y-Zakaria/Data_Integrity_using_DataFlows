
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
#include <pthread.h>

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

lide_c_actor_context_type *actors[ACTOR_COUNT];
int tokens_num = 0; 
int count =0;

/***/ 
pthread_mutex_t verrou;
pthread_cond_t finish;
pthread_cond_t continu;

pthread_t thread[2];

/* actor descriptors (for diagnostic output) */
        char *descriptors[ACTOR_COUNT] = {"source",
                "sum8", "sink"};

void *srcJob(void *arg){
        int i = 0;
        for (i=0; i < tokens_num; i++){
                pthread_mutex_lock(&verrou);
                lide_c_util_guarded_execution(actors[ACTOR_SOURCE], descriptors[ACTOR_SOURCE]);
                count++;
                if(count == 8) {
                        pthread_cond_signal(&finish);
                        pthread_cond_wait(&continu,&verrou);
                        //printf("from src\n");
                }
                pthread_mutex_unlock(&verrou);
        }
        pthread_exit((void *) 0);
}

void *sumJob(void *arg){
        int i = 0;
        for (i=0; i < tokens_num/8; i++){
                pthread_mutex_lock(&verrou);
                if(count < 8) pthread_cond_wait(&finish,&verrou);
                lide_c_util_guarded_execution(actors[ACTOR_SUM8], descriptors[ACTOR_SUM8]);
                lide_c_util_guarded_execution(actors[ACTOR_SINK], descriptors[ACTOR_SINK]);
                count = 0;
                pthread_cond_signal(&continu);
                pthread_mutex_unlock(&verrou);
        }
        pthread_exit((void *) 0);
}

int main(int argc, char **argv) {
        
        /* Connectivity: fifo1: (source,sum8), fifo2: (sum8,sink);
        */
        lide_c_fifo_pointer fifo1 = NULL, fifo2 = NULL;

        int token_size = 0;
        int i = 0;
        int arg_count = 2;
        

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


        /** initialize the condition variable and mutex**/
	pthread_cond_init(&finish,0);
        pthread_cond_init(&continu,0);
        pthread_mutex_init(&verrou,0);

	pthread_create(&thread[0], NULL, srcJob, NULL);
        pthread_create(&thread[1], NULL, sumJob, NULL);

	pthread_join(thread[0], NULL);
        pthread_join(thread[1], NULL);
	pthread_mutex_destroy(&verrou);
        pthread_cond_destroy(&finish);
        pthread_cond_destroy(&continu);

    /* Normal termination. */
        return 0;
}
