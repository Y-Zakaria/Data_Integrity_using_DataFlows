/*******************************************************************
@ddblock_begin copyright 

Copyright (c) 1997-2015
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
********************************************************************/

/*******************************************************************************
Utilities for error handling.
*******************************************************************************/

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "stlib.h"
#include "error.h"

/* A pointer to the error prefix buffer. */
char *error_prefix = NULL;

/* The number of characters that can be held in the current error prefix
   buffer, including the terminating NULL character.
*/
int error_prefix_capacity = 0;

void error_set_prefix(char *prefix) {
    if (error_prefix == NULL) {
        error_prefix = xmalloc((strlen(prefix) + 1) * sizeof(char));    
        error_prefix_capacity = strlen(prefix) + 1;
    } else if ((strlen(prefix) + 1) > error_prefix_capacity) {
        error_prefix = xrealloc(error_prefix,
                (strlen(prefix) + 1) * sizeof(char));    
        error_prefix_capacity = strlen(prefix) + 1;
    }
    strcpy(error_prefix, prefix);
}

char *error_get_prefix(void) {
    return error_prefix;
}

/* fatal error: print an error message and exit */
void error_fatal(char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    if (error_prefix != NULL) {
        fprintf(stderr, "%s", error_prefix);
    }
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}

/* warning function: just print an error message; don't exit */
void error_warn(char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, "error: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

/* single-argument fatal error function (for backward compatibility) */
void error(char *msg) {
        error_fatal(msg);
} 

