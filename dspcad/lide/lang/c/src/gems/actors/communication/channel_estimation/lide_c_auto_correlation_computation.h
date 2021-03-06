#ifndef _lide_c_auto_correlation_computation_h
#define _lide_c_auto_correlation_computation_h

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
#include "lide_c_actor.h"
#include "lide_c_fifo.h"

/*******************************************************************************
This actor computes and outputs the auto-correlation matrix of pilots. There 
are two modes. At the PROCESS mode, the auto-correlation matrix is computed 
and set the SLEEP mode as next one. The actor stays idle at the SLEEP mode.
*******************************************************************************/

/* constant */
#define PI    3.14159265358979323846

/* Actor modes */
#define LIDE_C_AUTO_CORRELATION_COMPUTATION_MODE_ERROR        0
#define LIDE_C_AUTO_CORRELATION_COMPUTATION_MODE_PROCESS      1
#define LIDE_C_AUTO_CORRELATION_COMPUTATION_MODE_SLEEP        2

/*******************************************************************************
TYPE DEFINITIONS
*******************************************************************************/

/* Structure and pointer types associated with the actor. */
struct _lide_c_auto_correlation_computation_context_struct;
typedef struct _lide_c_auto_correlation_computation_context_struct
        lide_c_auto_correlation_computation_context_type;

/*******************************************************************************
INTERFACE FUNCTIONS
*******************************************************************************/

/*****************************************************************************
Construct function of the lide_c_auto_correlation_computation actor.
Create a new actor object with the parameters:
pilot_count: the number of pilots
pilot_index: the pilot index vector with length pilot_count
channel_window: the normalized channel power-delay window
doppler_window: the normalized Doppler power-delay window
ffp_output_auto_correlation_matrix: the output auto-correlation matrix
*****************************************************************************/
lide_c_auto_correlation_computation_context_type *
        lide_c_auto_correlation_computation_new(int pilot_count, 
        int *pilot_index, double channel_window, double doppler_window, 
        lide_c_fifo_pointer ffp_output_auto_correlation_matrix);

/*****************************************************************************
Enable function of the lide_c_auto_correlation_computation actor.
*****************************************************************************/
boolean lide_c_auto_correlation_computation_enable(
        lide_c_auto_correlation_computation_context_type *context);

/*****************************************************************************
Invoke function of the lide_c_auto_correlation_computation actor.
*****************************************************************************/
void lide_c_auto_correlation_computation_invoke(
        lide_c_auto_correlation_computation_context_type *context);

/*****************************************************************************
Terminate function of the lide_c_auto_correlation_computation actor.
*****************************************************************************/
void lide_c_auto_correlation_computation_terminate(
        lide_c_auto_correlation_computation_context_type *context);

#endif
