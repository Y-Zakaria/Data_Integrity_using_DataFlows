#ifndef _hashtbl_h_
#define _hashtbl_h_

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
buckets is user specified.

Author: Bishnupriya Bhattacharya
*******************************************************************************/

#include "stlib.h"

struct hashtbl;
typedef struct hashtbl hashtbl;
typedef struct hashtbl *hashtbl_ptr;

/***************************************************************
type definition for functions that the user needs to supply to 
configure the hash table. 
****************************************************************/

/* The following function pointers are necessary to allow the hash table
 * to accept a generic KEY, and the asociated DATA data types
 */

typedef unsigned (*hashtbl_generic_hash_function_t)(const void *);
/* define a type for a generic hash function, which accepts as an
 * argument, a pointer to void *. If we need to pass more than one
 * argument to the hash function, put them in a structure, and pass
 * a pointer to that. Returns an index into the hash table array.
 */

typedef int (*hashtbl_cmp_t)(const void *, const void *);
/* function pointer to a comparison function for comparing 2 keys in the hash table
 */

typedef void (*hashtbl_prnt_key_t)(FILE *, void *);
/* function pointer for printing a hash table key */

typedef void (*hashtbl_prnt_data_t)(FILE *, void *);
/* function pointer for printing the data associated with a hash table key */

/* the following function pointers are necessay to allow the bucket chain in
 * the hash table to be implemented in a generic fashion by the user. e.g. the
 * user can choose a linked list implementation of the bucket chains
 */

typedef void *(*hashtbl_ex_t)(const void *data);
/* an extraction routine that extracts something from the bucket. The bucket
 * chain implementation will not supply this function, but needs to know this 
 * type signature, so that it can support the find_in_chain routine, where this
 * function pointer has to be suplied as a parameter.
 */

typedef void *(*hashtbl_allocate_t)(int, const void *);
/* the memory allocate for the hash table will depend on how the bucket chain
 * is implemented. This is a function pointer which allocates the table memory
 * according to the bucket chain implementation. The 1st argument is the size
 * of the table, and the 2nd argument is any initialization parameters necessary
 */

typedef void (*hashtbl_init_t)(void *);
/* Function pointer that does the proper initialization for a
 * particular bucket chain implementation. The 1st argument is the bucket chain.
 */

typedef void (*hashtbl_add_t)(void *, void *);
/* function pointer to add a bucket (2nd argument) to a bucket chain (1st 
 * argument)
 */

typedef void *(*hashtbl_find_t)(const void *, const void *, hashtbl_ex_t, hashtbl_cmp_t);
/* function pointer for finding the bucket in the "chain" (1st argument), whose
 * key matches "data" (2nd argument). Comparison is done with the function 
 * pointer "cmp" (4th argument), after extracting the key with the "ex" function
 * pointer (3rd argument).
 */

typedef boolean (*hashtbl_del_t)(void *, const void *, hashtbl_ex_t, hashtbl_cmp_t);
/* function pointer for deleting a bucket from the "chain" (1st argument), 
 * whose key matches "data" (2nd argument). Comparison is done with the 
 * function pointer "cmp" (4th argument), after extracting the key with the 
 * "ex" function pointer (3rd argument).  Returns TRUE if successfully deleted,
 *  else returns FALSE.
 */

typedef boolean (*hashtbl_isbucket_empty_t)(const void *);
/* function pointer for checking if a bucket chain is empty. The chain is
 * supplied as the 1st argument
 */

typedef void (*hashtbl_print_t)(const void *, 
				FILE *fp,
				hashtbl_ex_t dummy, stlib_generic_print_t prt_KEY,
				hashtbl_ex_t ex_key, stlib_generic_print_t prt_key,
 				hashtbl_ex_t ex_data, stlib_generic_print_t prt_data);
/* function pointer that traverses the chain (1st argument), extracts the key 
 * from the bucket by ex_key() function, prints the key by prt_key() function, 
 * and then does the same for the data with ex_data(), and prt_data(). The first
 * pairs of arguments are necessary to print the word "KEY: ". The printing is
 * done to the file pointer "fp".
 */

/**************************************************************
routines that the user can call to access the ADT interface 
***************************************************************/

/* make a new hash table. Arguments are function pointers for each such entry
 * in the hash table header. Depending on the data structure for the bucket
 * chains, different kinds of initializations maybe necessary during 
 * allocation. The necessary parameters are passed through "init_params". 
 */
hashtbl_ptr hashtbl_allocate(const unsigned size,
                       hashtbl_generic_hash_function_t hash,
                       hashtbl_cmp_t cmp,
                       hashtbl_prnt_key_t prntkey,
		       hashtbl_allocate_t alloc,
                       const void * init_params,
		       hashtbl_init_t init,
		       hashtbl_add_t add,
		       hashtbl_find_t find,
		       hashtbl_del_t del,
		       hashtbl_isbucket_empty_t empty,
		       hashtbl_print_t print
		       );

/* initialize a hash table. */
void hashtbl_init(hashtbl_ptr tab);

/* add an element to a hash table. Also supply 1 function pointer to print
 * the DATA
 */
void hashtbl_add_element(hashtbl_ptr tab, void *key, void *data, 
			 hashtbl_prnt_data_t prnt_data);

/* find an element in a hash table, matching by "key" */
void *hashtbl_find_element(const hashtbl_ptr tab, void *key);

/* delete one node of a bucket from a hash table, whose key matches "key" */
boolean hashtbl_del_element(hashtbl_ptr tab, void *key);

/* print a hash table to file pointer fp */
void hashtbl_print(FILE *fp, const hashtbl_ptr tab);

#endif
