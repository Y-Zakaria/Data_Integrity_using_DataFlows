#ifndef _hashlst_h_
#define _hashlst_h_

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
Hash table ADT. The KEY, and associated DATA can be of any data type, and can
vary from bucket to bucket. The implementation of the collision chain of
buckets is done through a linked list

Author: Bishnupriya Bhattacharyya
*******************************************************************************/
 

#include "hashtbl.h"

typedef struct hashtbl hashlst;
typedef hashlst *hashlst_ptr;

/***************************************************************
type definition for functions that the user needs to supply to
configure the hash table.
****************************************************************/ 

typedef unsigned (*hashlst_generic_hash_function_t)(const void *);
/* define a type for a generic hash function, which accepts as an
 * argument, a pointer to void *. If we need to pass more than one
 * argument to the hash function, put them in a structure, and pass
 * a pointer to that. Returns an index into the hash table array.
 */

typedef int (*hashlst_cmp_t)(const void *, const void *);
/* function pointer to a comparison function for comparing 2 keys in the hash
 * table
 */

typedef void (*hashlst_prnt_key_t)(FILE *, const void *);
/* function pointer for printing a hash table key */

typedef void (*hashlst_prnt_data_t)(FILE *, const void *);
/* function pointer for printing the data associated with a hash table key */

/**************************************************************
routines that the user can call to access the ADT interface
***************************************************************/

/* make a new hash table. Arguments are function pointers for each such entry
 * in the hash table header. Depending on the data structure for the bucket
 * chains, different kinds of initializations maybe necessary during 
 * allocation. The necessary parameters are passed through "init_params". 
 */
hashlst_ptr hashlst_allocate(const unsigned size,
                       		  hashlst_generic_hash_function_t hash,
                        	  hashlst_cmp_t cmp,
                        	  hashlst_prnt_key_t prntkey
		        	  );

/* initialize a hash table. */
void hashlst_init(hashlst_ptr tab);

/* add an element to a hash table. Also supply 1 function pointer to print
 * the DATA
 */
void hashlst_add_element(hashlst_ptr tab, void *key, void *data,
			 hashlst_prnt_data_t prnt_data);

/* find an element in a hash table, matching by "key" */ 
void *hashlst_find_element(const hashlst_ptr tab, void *key);

/* delete one node of a bucket from a hash table, whose key matches "key" */
boolean hashlst_del_element(hashlst_ptr tab, void *key);

/* print a hash table */
void hashlst_print(FILE *, const hashlst_ptr tab);

#endif
