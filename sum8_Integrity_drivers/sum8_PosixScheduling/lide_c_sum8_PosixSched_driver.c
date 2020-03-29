
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

#define _GNU_SOURCE
#define CONFIGURE_INIT
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <time.h>

#include "lide_c_fifo.h"
#include "lide_c_file_source.h"
#include "lide_c_file_sink.h"
#include "lide_c_sum8.h"
#include "lide_c_util.h"

#define SUM8_IN_CAPACITY 8
#define SUM8_OUT_CAPACITY 1

/* An enumeration of the actors in this application. */
#define ACTOR_SINK 0
#define ACTOR_SOURCE 1
#define ACTOR_SUM8 2

/* The total number of actors in the application. */
#define ACTOR_COUNT 3


struct timespec critical_instant;

struct _posix_actor_type
{ 
    /* LIDEC Actor context */
    lide_c_actor_context_type* context;
    
    /* Attributes computed by ADFG */
    char* name;
    int processor;
    int priority;
    struct timespec period;
};
typedef struct _posix_actor_type posix_actor_type;

struct timespec ts_add (struct timespec time1, struct timespec time2)
{    
    struct  timespec  result ;

    result.tv_sec = time1.tv_sec + time2.tv_sec ;
    result.tv_nsec = time1.tv_nsec + time2.tv_nsec ;
    if (result.tv_nsec >= 1000000000L) {		/* Carry? */
        result.tv_sec++ ;  result.tv_nsec = result.tv_nsec - 1000000000L ;
    }

    return (result) ;

}

struct  timespec  ts_substract ( struct  timespec  time1, struct  timespec  time2)
{    
    struct  timespec  result ;
	
	
/* Subtract the second time from the first. */

    if ((time1.tv_sec < time2.tv_sec) ||
        ((time1.tv_sec == time2.tv_sec) &&
         (time1.tv_nsec <= time2.tv_nsec))) {		/* TIME1 <= TIME2? */
        result.tv_sec = result.tv_nsec = 0 ;
    } else {						/* TIME1 > TIME2 */
        result.tv_sec = time1.tv_sec - time2.tv_sec ;
        if (time1.tv_nsec < time2.tv_nsec) {
            result.tv_nsec = time1.tv_nsec + 1000000000L - time2.tv_nsec ;
            result.tv_sec-- ;				/* Borrow a second. */
        } else {
            result.tv_nsec = time1.tv_nsec - time2.tv_nsec ;
        }
    }

    return (result) ;

}

void* lide_c_actor_start_routine(void* _posix_actor) {		      
	posix_actor_type* posix_actor = (posix_actor_type*) _posix_actor;
    lide_c_actor_context_type* lide_c_actor = posix_actor->context;

	struct timespec nexttime=critical_instant;
	struct timespec waittime;
	struct timespec currenttime;
    int status;

    #ifdef DEBUG
    printf("Thread %lx : period = %ld:%ld\n",pthread_self(), posix_actor->period.tv_sec, posix_actor->period.tv_nsec);
    #endif

	while (1) {		
		if(lide_c_actor->enable(lide_c_actor)){
			lide_c_actor->invoke(lide_c_actor);		
            
            #ifdef DEBUG	
			printf("Actor visit complete (tid=%lx) \n", pthread_self());
            #endif
		}

		/* Compute the next release time */
  		status=clock_gettime(CLOCK_REALTIME, &currenttime);
		if(status<0) //returncode("clock_gettime",status);
            printf("clock_gettime %d\n",status);
		nexttime=ts_add(posix_actor->period, nexttime);
        
        /* Wait for the next release time */
		waittime=ts_substract(nexttime, currenttime);   
        
        #ifdef DEBUG
        printf("Current Time = %ld:%ld, Wait Time =  %ld:%ld, Next Time =  %ld:%ld (tid=%lx) \n",
                currenttime.tv_sec, currenttime.tv_nsec,
                waittime.tv_sec, waittime.tv_nsec,
                nexttime.tv_sec, nexttime.tv_nsec,
                pthread_self()); 
        #endif
    
		nanosleep(&waittime, NULL);	
	}	

	return 0;
}

int main(int argc, char **argv) {
        
        struct sched_param param;
        pthread_attr_t attr;
        pthread_t id[3];
        cpu_set_t cpuset;

        int rc,t;
	    void *status;
                
        /* Main thread priority */
        param.sched_priority=99; 
        pthread_setschedparam(pthread_self(), SCHED_OTHER, &param);

        /* LIDE-C Channels */
        lide_c_fifo_pointer sum8_in = NULL;
        lide_c_fifo_pointer sum8_out = NULL;

        sum8_in = lide_c_fifo_new(SUM8_IN_CAPACITY, 4);
        sum8_out = lide_c_fifo_new(SUM8_OUT_CAPACITY, 4);

        /* LIDE-C Actors */
        // actor descriptors (for diagnostic output)
        char *descriptors[ACTOR_COUNT] = {"sink", "source", "sum8"};
        int priorities[ACTOR_COUNT] = {3,1,2};
        int periods[ACTOR_COUNT] = {1000,1000,1000};   
        int processors[ACTOR_COUNT] = {1,1,1}; 

        lide_c_actor_context_type* actors[ACTOR_COUNT];    
        actors[ACTOR_SOURCE] = (lide_c_actor_context_type*) (lide_c_file_source_new (sum8_in));
        actors[ACTOR_SUM8] = (lide_c_actor_context_type*) (lide_c_sum8_new (sum8_in, sum8_out));
        actors[ACTOR_SINK] = (lide_c_actor_context_type*) (lide_c_file_sink_new (sum8_out));

        /* LIDE-C Actors --> RTEMS Actors*/
        posix_actor_type* posix_actors;
        posix_actors = malloc (sizeof(posix_actor_type)*ACTOR_COUNT);

        posix_actors[ACTOR_SINK].context = actors[ACTOR_SINK];
        posix_actors[ACTOR_SINK].name = descriptors[ACTOR_SINK];
        posix_actors[ACTOR_SINK].priority = priorities[ACTOR_SINK];
        posix_actors[ACTOR_SINK].period.tv_sec = periods[ACTOR_SINK];
        posix_actors[ACTOR_SINK].processor = processors[ACTOR_SINK];

        posix_actors[ACTOR_SOURCE].context = actors[ACTOR_SOURCE];
        posix_actors[ACTOR_SOURCE].name = descriptors[ACTOR_SOURCE];
        posix_actors[ACTOR_SOURCE].priority = priorities[ACTOR_SOURCE];
        posix_actors[ACTOR_SOURCE].period.tv_sec = periods[ACTOR_SOURCE];
        posix_actors[ACTOR_SOURCE].processor = processors[ACTOR_SOURCE];

        posix_actors[ACTOR_SUM8].context = actors[ACTOR_SUM8];
        posix_actors[ACTOR_SUM8].name = descriptors[ACTOR_SUM8];
        posix_actors[ACTOR_SUM8].priority = priorities[ACTOR_SUM8];
        posix_actors[ACTOR_SUM8].period.tv_sec = periods[ACTOR_SUM8];
        posix_actors[ACTOR_SUM8].processor = processors[ACTOR_SUM8];    

        /* Priority-based, preemptive scheduling with timeslicing */
        pthread_attr_init(&attr);	
        pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);	
        pthread_attr_setschedpolicy(&attr,SCHED_OTHER);	
        
        /* Create a thread for each actor: Set priority --> Create thread --> Mapping */
        
        param.sched_priority=posix_actors[ACTOR_SOURCE].priority;
        pthread_attr_setschedparam(&attr,&param);	
        pthread_create(&id[1],&attr,lide_c_actor_start_routine,&posix_actors[ACTOR_SOURCE]);
        CPU_ZERO(&cpuset);  
        CPU_SET(processors[ACTOR_SOURCE], &cpuset);
        pthread_setaffinity_np(id[1], sizeof(cpu_set_t), &cpuset);

        param.sched_priority=posix_actors[ACTOR_SUM8].priority; 
        pthread_attr_setschedparam(&attr,&param);	
        pthread_create(&id[2],&attr,lide_c_actor_start_routine,&posix_actors[ACTOR_SUM8]);
        pthread_setaffinity_np(id[2], sizeof(cpu_set_t), &cpuset);
        CPU_ZERO(&cpuset);  
        CPU_SET(processors[ACTOR_SUM8], &cpuset);
        pthread_setaffinity_np(id[2], sizeof(cpu_set_t), &cpuset);

        param.sched_priority=posix_actors[ACTOR_SINK].priority; 
        pthread_attr_setschedparam(&attr,&param);	
        pthread_create(&id[0],&attr,lide_c_actor_start_routine,&posix_actors[ACTOR_SINK]);   
        CPU_ZERO(&cpuset);  
        CPU_SET(processors[ACTOR_SINK], &cpuset);
        pthread_setaffinity_np(id[0], sizeof(cpu_set_t), &cpuset);

        /* Start time. Used later to compute the periodic releases */
        clock_gettime(CLOCK_REALTIME, &critical_instant);

        pthread_attr_destroy(&attr);

        for(t=0; t<3; t++)
        {
            rc = pthread_join(id[t], &status);
            if (rc)
            {
                printf("ERROR; le code de retour dupthread_join() est %d\n", rc);
                exit(-1);
            }
            printf("le join a fini avec le thread %d et a donne le status= %ld\n",t,(long)status);
        }

        pthread_exit(NULL);
        return 0;
}
