#ifndef _error_h_
#define _error_h_

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
 

#include <stdarg.h>

/* fatal error: print an error message and exit */
void error_fatal(char *fmt, ...);

/* warning function: just print an error message; don't exit */
void error_warn(char *fmt, ...);

/* single-argument fatal error function (for backward compatibility) */
void error(char *msg);

/*******************************************************************************
Set the prefix to be used in subsequent error messages. This prefix
is added implictly to the message string passed in subsequent
calls to error_fatal. By default (if this function has not been called),
no prefix is included in error messages. A copy of the given prefix
is stored internally in this module independently of the storage
associated with the argument.
*******************************************************************************/
void error_set_prefix(char *prefix);

/*******************************************************************************
Return the current prefix for error messages --- i.e., the the prefix
that has been passed in the most recent call to error_set_prefix.
Return NULL if error_set_prefix has not yet been called.
*******************************************************************************/
char *error_get_prefix(void);

#endif
