#ifndef _lklst_h
#define _lklst_h

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
Linked list ADT
Author: Shuvra S. Bhattacharyya

Other contributors: Bishnupriya Bhattacharya 
*******************************************************************************/

#include "stlib.h"

struct lklst;
typedef struct lklst lklst;
typedef struct lklst *lklst_ptr;

/*********************************************************
type definitions of functions that the user needs to supply
**********************************************************/

typedef void*(*lklst_extract_t)(const void *);
typedef int(*lklst_cmp_t)(const void *f1, const void *f2);

/*********************************************************
functions that the user can call
**********************************************************/

lklst_ptr lklst_allocate();
/* Allocate a new linked list and initialize it to be empty */

void lklst_init(lklst_ptr l);  
/* initialize the list (to be empty) */

void *lklst_iter_init(lklst_ptr l);
/* Initialize a new iteration through the list */

void *lklst_iter_next(lklst_ptr l); 
/* return the next list element when iterating through the list */

void lklst_traverse(lklst_ptr l, ty_generic_onearg f);
/* Apply f() to each data item pointed to by each element
 * of the list (starting at the head of the list at
 * progressing to the tail)
 */

void lklst_traverse_extract_apply(lklst_ptr l, 
				  int npairs,
				  lklst_extract_t ex, ty_generic_onearg f,
 				  ...);
/* Apply f() to a portion of each data item, pointed to by each element
 * of the list (starting at the head of the list and
 * progressing to the tail). Use "ex" function to extract a portion of the data.
 * It is also possible to extract different portions of each data item, and 
 * apply different functions to them. The argument list is kept variable for
 * this purpose. Pairs of extract routines, and applying functions have to be
 * supplied. npairs specifies how manu such pairs are supplied. The default
 * value of npairs is 1. For e.g., if the fucntion is called as
 * lklst_traverse_extract_apply(l, 2, ex1, f1, ex2, f2)
 * then for each data item in l, a portion is extracted by "ex1", and "f1" is
 * applied on the extraceted portion, then another portion is extracted with
 * "ex2", and "f2" is applied on that. If any of the extracting functions is
 * NULL, then the the corresponding applying routine is applied on the entire 
 * data.
 */

void lklst_traverse_extract_print(lklst_ptr l, 
				  int npairs,
				  FILE *fp,
				  lklst_extract_t ex, stlib_generic_print_t f,
 				  ...);
/* This is the same as lklst_traverse_extract_apply, only difference is that
 * the applying function is a printing routine, which will take an additional
 * FILE * argument.
 */

void lklst_insert(lklst_ptr l, void *d);
/* Insert a new element at the beginning of the list */

void lklst_append(lklst_ptr l, void *d);
/* Append a new element at the end of the list */

void *lklst_find(lklst_ptr l, const void *d, lklst_cmp_t cmp);
/* return a pointer to the first data item in the list
 * that matches "d". If the comparison function is NULL,
 * we return the first data item whose address matches "d".
 * If the item is not found, NULL is returned.
 */

void *lklst_extract_and_find(lklst_ptr l, const void *d, 
			     lklst_extract_t ex,
			     lklst_cmp_t cmp);
/* This routine allows a portion of the data to be compared. The user supplies
 * only portion of the data in "d", and the "ex" function pointer, which the
 * routine uses to extract the relevant portion from the data present in the
 * list, and then uses the "cmp" function pointer routine for comparison.
 * If "ex" is NULL, then this behaves the same as lklst_find(), if the user
 * supplies the entire data in "d".
 */

boolean lklst_delete(lklst_ptr l, const void *d, ty_comparison_function cmp);
/* delete the first item that matched "d". See "lklst_find" for an
 * explanation of the "cmp" argument. If the item is found
 * and deleted successfully then TRUE is returned, otherwise
 * FALSE is returned.
 */

boolean lklst_extract_and_delete(lklst_ptr l, const void *d, 
				 lklst_extract_t ex,
				 ty_comparison_function cmp);
/* delete the first item, a portion of which matched "d". See 
 * "lklst_extract_and_find" for an explanation of the "ex" argument. If the 
 * item is found and deleted successfully then TRUE is returned, otherwise
 * FALSE is returned. if ex = NULL, then the behavior is the same as 
 * lklst_delete.
 */

void *lklst_extract_node(lklst_ptr l, int pos);
/* delete the node at position "pos" in the list, and return the data contained
 * in that node
 */

boolean lklst_isempty(lklst_ptr l);
/* Return true if and only if list "l" is empty */

int lklst_length(lklst_ptr l);
/* return the length of the list */

#endif
