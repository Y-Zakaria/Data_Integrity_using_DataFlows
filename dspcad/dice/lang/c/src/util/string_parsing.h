#ifndef _string_parsing_h
#define _string_parsing_h

/****************************************************************************
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
******************************************************************************/

/*****************************************************************************
Utilities for parsing strings
*****************************************************************************/


/* Character constant that terminates a string. */
#define SP_NULL_CHAR '\0'

/*****************************************************************************
$ddbegin
\ddfheader{sp_first_field}{s}{skipped_space}
Go to the next field (sequence of non-white chars) in a string, unless we are
already on a field, in which case a pointer to the current field will be
returned.  If we are not already on a field (i.e., (s[0]) is not a white space
character), then return a pointer to the next field.  If there are no more
fields, then return a null string (a string that contains only the terminating
NULL character).  Any white space that is skipped in moving to the current
string position (s[0]) to the first field (or the end of the string) is
returned in "skipped_space" if skipped_space is non-NULL.  In this case, the
skipped_space array must have enough space to store all the skipped white space
and a terminating NULL character.  If skipped space is NULL, then leading white
space in the string is skipped without storing it.
$ddend
*****************************************************************************/
char *sp_first_field(char *s, char *skipped_space);

/*****************************************************************************
This is the same as sp_first_field(s, NULL). 
*****************************************************************************/
char *sp_firstfield(char *s);

/*****************************************************************************
This function uses the index argument (\ddarg{i}) to determine whether
to call sp_first_field or sp_next_field. The former is called
if $i = 0$; the latter is called if $i > 0$ A fatal error occurs if
$i < 0$.
*****************************************************************************/
char *sp_get_field(char *s, int i, char *skipped_space);

/*****************************************************************************
This is the same as sp_get_field(s, i, NULL). 
*****************************************************************************/
char *sp_getfield(char *s, int i);

/*****************************************************************************
Go to the next field (sequence of non-white chars) in string. If we are
already on a field, then that field will be skipped. If there are no
more fields, then return a null string.
Skipped white space is conditionally stored in the \ddcode{skipped_space} 
argument, as in \ddfunction{sp_first_field}.
*****************************************************************************/
char *sp_next_field(char *s, char *skipped_space);

/*****************************************************************************
This is the same as sp_next_field(s, NULL). 
*****************************************************************************/
char *sp_nextfield(char *s);


#endif
