#ifndef _lide_c_bmp_file_read_halo_h
#define _lide_c_bmp_file_read_halo_h

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
#include "lide_c_bmp.h"

/* Modes */
#define LIDE_C_BMP_FILE_READ_HALO_MODE_INACTIVE     0 
#define LIDE_C_BMP_FILE_READ_HALO_MODE_INFO         1
#define LIDE_C_BMP_FILE_READ_HALO_MODE_PROCESS      2    
#define LIDE_C_BMP_FILE_READ_HALO_MODE_HALO         3 

struct _lide_c_bmp_file_read_halo_context_type_struct;
typedef struct _lide_c_bmp_file_read_halo_context_type_struct
        lide_c_bmp_file_read_halo_context_type;

lide_c_bmp_file_read_halo_context_type * lide_c_bmp_file_read_halo_new(
        FILE *file, int tileX, int tileY, lide_c_fifo_pointer halo_in,
        lide_c_fifo_pointer tile_out, lide_c_fifo_pointer newrow_out,
        lide_c_fifo_pointer bmpinfo_out);

boolean lide_c_bmp_file_read_halo_enable(
        lide_c_bmp_file_read_halo_context_type *context); 

void lide_c_bmp_file_read_halo_invoke(
        lide_c_bmp_file_read_halo_context_type *context); 

void lide_c_bmp_file_read_halo_terminate(
        lide_c_bmp_file_read_halo_context_type *context);

#endif
