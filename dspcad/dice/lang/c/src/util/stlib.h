#ifndef _stlib_h_
#define _stlib_h_

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
Miscellaneous utilities packaged without enough regard to organization.
This module is under active reorganization.

Author: Shuvra S. Bhattacharyya
*******************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"

#define FREE(p) (void)free((void*)(p))

/* Boolean type and constants. */
#define TRUE 1
#define FALSE 0
typedef int boolean;


/* macros for printf */
#define stlib_PRT (void)printf
#define stlib_FPRT (void)fprintf
#define stlib_prt (void)printf
#define stlib_fprt (void)fprintf

/* exchange two variables through a temporary variable; the
   temporary variable ("temp") must already be declared */
#define exchange(a, b, temp) {temp = a; a = b; b = temp;}

/* memory allocation functions that print an error message exit if the 
requested amount of memory is not available.
*/

void *xmalloc(size_t size);
void *xcalloc(size_t nelem, size_t elsize);
void *xrealloc(void *pointer, int new_size);

/* allocate an n x n matrix wholse elements are of type "mtype"
   using xcalloc */
#define allocate_matrix(n, mtype) \
        ((mtype*) xcalloc((unsigned)((n)*(n)), sizeof(mtype)))
 
/* allocate an m x n matrix wholse elements are of type "mtype"
   using xcalloc */
#define allocate_rectangular_matrix(m, n, mtype) \
        ((mtype*) xcalloc((unsigned)((m)*(n)), sizeof(mtype)))
 

/* allocate an n-element array wholse elements are of type "mtype" 
   using xcalloc */
#define allocate_array(n, mtype) \
        ((mtype*) xcalloc((unsigned)(n), sizeof(mtype)))

/* allocate a single object of type mtype using xmalloc */
#define allocate_scalar(mtype) \
        ((mtype*) xmalloc(sizeof(mtype)))

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define stlib_newline printf("\n") 
#define stlib_space printf(" ") 

int randomly_set_rand_seed();

/*******************************************************************************
Read a line from the given file into the given character array ("buffer").
The value of "limit" specifies a limit on the lenght of the input 
line. In other words, the input line can contain at most "limit"
characters, excluding any trailing newline character, and exculding any
trailing null character that is used in the internal (string) representation of
the line. If this line length limit is exceeded, then the program exits with an
error message. Otherwise, the line is returned --- with the trailing newline
removed --- in "buffer". Upon entry to this function, "buffer" should point to
a character array of lenght at least (limit + 2).  The two additional spaces
are needed for the trailing newline character (before its removal) and the
trailing null character.

For Windows file format compatibility: if the trailing newline on a line
is preceded by a carriage return, then the carriage return character
is also removed (in addition to the newline) from "buffer".

Return 0 if end of file is encountered before any characters are read
Return 1 otherwise
*******************************************************************************/
int read_line(char *buffer, int limit, FILE *fp);

void strsub(char *str1, char *str2, FILE *fp);
boolean blankstr(char *s);
void strlower(char *s);

/* direction to round, when calling function "dround", 
 * if the fractional part of the first argument equals
 * 0.5 
 */
typedef enum {dround_up, dround_down} dround_direction ;

/* any function that compares the data d1 pointed to by its first 
 * argument to the data d2 pointed to by the second arg.
 * If d1>d2, 1 is returned, if d1<d2 -1 is returned,
 * and if d1=d2 0 is returned 
 * 
 * Used to implement generic comparisons; e.g. to sort
 * or find data in a generic way.
 */
typedef int (*ty_comparison_function)(const void *, const void *);  

/* pointer to a generic function that returns nothing and
 * takes a pointer as its single argument */
typedef void (*ty_generic_onearg)(void *);

/* Various type definitions for pointers to generic functions.
   The naming convention here is:

	stlib_<return_type>_<number_of_args>arg_t

	For example, a stlib_void_threearg_t is a pointer to
	a function that returns void and takes three generic-pointer arguments. 

	a "p" suffix in the return type indicates a pointer---e.g.,
    "voidp" stands for "pointer to void".
*/
typedef void (*stlib_void_onearg_t)(void *);
typedef void (*stlib_void_twoarg_t)(void *, void *);
typedef void (*stlib_void_threearg_t)(void *, void *, void *);
typedef double (*stlib_double_twoarg_t)(void *, void *);
typedef int (*stlib_int_onearg_t)(void *, void *);
typedef int (*stlib_int_twoarg_t)(void *, void *);
typedef int (*stlib_int_threearg_t)(void *, void *, void *);
typedef void* (*stlib_voidp_onearg_t)(void* );
typedef void* (*stlib_voidp_twoarg_t)(void* , void*);

/* pointer to a generic print function that returns nothing and
 * takes a FILE pointer, and a void pointer as arguments */
typedef void (*stlib_generic_print_t)(FILE *, void *);

/* if "f" is a NULL, a direct comparison of ptrs x and y
 * is performed and the result returned is -1 if (x<y), 0 if (x==y),
 * and 1 if (x>y). If "f" is non-NULL, then we return the result
 * obtained by applying f to x and y.
 */
int generic_comparison(ty_comparison_function f,
		              const void *x, const void *y);


long dround(double x, dround_direction d);

/* convert n to characters in s */
void stlib_itoa(int n, char s[]); 

/* reverse string s in place */
void reverse(char s[]); 

/*
   function stlib_check_line_size

   This functions checks to make sure that every line contains
   less than or equal to <limit> characters, including the
   terminating newline character, but not including the terminating
   NULL character in the string representation. 
   If a line exceeding the limit is found,
   then the program is terminated with an error message.

	NOTE: file "fname" should be in an unopened state on entry
    to this function.
*/
void stlib_check_line_size(char *fname, int limit);

/* Open a file. Exit with an error message if the file cannot be opened with
 * the specified mode. Otherwise return the file pointer of the opened file.
 */

FILE *stlib_xfopen(const char *fname, const char *mode);

/*
	The following macro is for getting an entry of a square or
	rectangular matrix.  The macro yields m(i, j) where m has has n columns.
*/
#define stlib_MTX(m, i, j, n) ((m)[((i)*(n))+(j)])

/*
    function stlib_get_field

    This function reads a named field from an input file <fp> that is
    already opened for reading. First the function attempts to
    read the string <fname>, the field name, from the input file.
    If <fname> is not the next non-blank field in the input file,
    then the program aborts with an error message. Otherwise, the
    next non-blank field is read from <fp> using fscanf with <fmt>
    as the format string. Only one field field should be read with
    <fmt>.

	For example, suppose the input file contains the following text:

    name scooby
    age  8
    weight 75.5

    Then the following sequence of calls could read in the data from
    this file (after it has been opened as <fp>).

    The last argument should be a character buffer that is large
    enough to read the field specifier (or any erroneous string
    inserted in its place). E.g., it can be temporary buffer
    of length equal to the maximum line lenght.

    get_field(fp, "name", "%s", buf, tmpbuf);
    get_field(fp, "age", "%d", &age, tmpbuf);
    get_field(fp, "weight", "%d", &wt, tmpbuf);
*/
void stlib_get_field(FILE *fp, char *fname, char *fmt, void *data, char *buf);

#define stlib_rand32_MAX ((unsigned)0xffffffff)
unsigned stlib_rand32();

/* return a random integer between 0 and (N-1) inclusive */
#define stlib_random_nonneg(N) (stlib_rand32() % (N))

/* Get a random integer from 1 through N inclusive. N should be
   greater than or equal to 2 
*/
#define stlib_random_positive(N) (1+stlib_random_nonneg((N-1)))

/* Read a file into a string */
char *stlib_filetostring(FILE *fp);

/* Read a file into a character buffer */
int stlib_filetobuf(char *fname, char *s, int buflen);

/* Duplicate a string */
char *stlib_strdup(char *src);

/* Remove from "s" all appearances of characters that occur in the string
"characters_to_remove", and return the resulting string. The orginal string "s"
is unmodified. the malloc function is used to allocate space for the new string
that is returned.
*/
char *stlib_remove_characters(char *s, char *characters_to_remove);

/*******************************************************************************
Enable debugging output.
*******************************************************************************/
void stlib_debug_enable(void); 

/*******************************************************************************
Disable debugging output. Debugging output is disabled by default (i.e.,
upon program initialization).
*******************************************************************************/
void stlib_debug_disable(void);

/*******************************************************************************
If debugging output is enabled, then this function interprets its
variable argument list as arguments to printf, and follows the call
to printf with a flushing of standard output. If debugging output
is disabled, then this function produces no output or other side effects.
*******************************************************************************/
void stlib_debug(char *fmt, ...);

/*****************************************************************************
The following functions have been moved to module string_parsing, 10/04/2008.
char *firstfield(char *s);
char *getfield(char *s, int i);
char *nextfield(char *s);
*****************************************************************************/

#endif
