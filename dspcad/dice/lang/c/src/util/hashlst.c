/******************************************************************
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

Author: Bishnupriya Bhattacharya
*******************************************************************************/

 

#include <stdlib.h>
#include <stdio.h>
#include "hashlst.h"
#include "lklst.h"


typedef void *(*hashlst_ex_t)(const void *);

/*******************************************************************
 routines to implement the bucket chain as a linked list 
*******************************************************************/

/* allocates an array of size "size", making each entry a pointer to the header
 * of a linked list
 */
void *hashlst_bucket_allocate_table_of_lists(const int size, const void *init_params)
{
        lklst **table;
        int i;

        if (init_params)
                error("List implementation of bucket chain does not require\
                       any initialization parameters");

        /* allocate memory for an array of lenghth "size", where each entry
         * is a pointer to the header of a linked list
         */
        table = allocate_array(size, lklst_ptr);

        /* for each array entry, actually allocate memeory for the linked list
         * header
         */
        for (i = 0; i < size; i++)
                table[i] = lklst_allocate();

        return (void *)table;
}

/* initialize the linked list. Call the "lklst_init" function */
void hashlst_bucket_init_list(void *chain)
{
        lklst_init((lklst *)chain);
}

/* add a bucket to the linked list, call the "lklst_insert" function */
void hashlst_bucket_add_to_list(void *chain, void *bucket)
{
        lklst_insert((lklst *)chain, bucket);
}

/* find an element in the linked list, whose key matches that present in bucket.
 * call lklst_find().
 */
void *hashlst_bucket_find_in_list(const void *chain, const void *key, 
				  hashlst_ex_t ex, hashlst_cmp_t cmp)
{
        return lklst_extract_and_find((lklst *)chain, key, ex, cmp);
}

/* delete an element from the linked list, whose key matches that in bucket.
 * Call the "lklst_delete" function
 */

boolean hashlst_bucket_del_from_list(void *chain, const void *key,
				  hashlst_ex_t ex, hashlst_cmp_t cmp)
{
	return lklst_extract_and_delete((lklst *)chain, key, ex, cmp);
}
		
boolean hashlst_bucket_is_empty_list(const void *chain)
{
	return lklst_isempty((lklst *)chain);
}

/* print the data contained in the list, with the function pointer parameter
 * "print_func". Call the "lklst_traverse" function with the "print_func"
 * parameter
 */
void hashlst_bucket_print_list(const void *chain, 
		FILE *fp,
		hashlst_ex_t ex_dummy, stlib_generic_print_t print_KEY,
		hashlst_ex_t ex_key, stlib_generic_print_t print_key,
		hashlst_ex_t ex_data, stlib_generic_print_t print_data)
{
        lklst_traverse_extract_print((lklst *)chain, 3, fp,
				     ex_dummy, print_KEY,
				     ex_key, print_key,
				     ex_data, print_data);
}

/**************************************************************
  routines that the user can call
**************************************************************/

/* allocate a new hash table */
hashlst_ptr hashlst_allocate(
                       const unsigned size,
                       hashlst_generic_hash_function_t hash,
                       hashlst_cmp_t cmp,
                       hashlst_prnt_key_t prnt_key
                       )

{
	hashlst_ptr t = (hashlst_ptr)hashtbl_allocate(size,
				hash,
				cmp,
				(hashtbl_prnt_key_t)prnt_key,
				*hashlst_bucket_allocate_table_of_lists,
				NULL,
				*hashlst_bucket_init_list,
				*hashlst_bucket_add_to_list,
				*hashlst_bucket_find_in_list,
				*hashlst_bucket_del_from_list,
				*hashlst_bucket_is_empty_list,
				*hashlst_bucket_print_list);
	return t;
}

void hashlst_init(hashlst_ptr tab) {
	hashtbl_init(tab);
	return;
}

/* add an element to a hash table */
void hashlst_add_element(hashlst_ptr tab, void *key, void *data,
			 hashlst_prnt_data_t prnt_data) {
	hashtbl_add_element(tab, key, data, (hashtbl_prnt_data_t)prnt_data);
	return;
}

/* find an element in a hash table */
void *hashlst_find_element(const hashlst_ptr tab, void *key) {
	return (void *)hashtbl_find_element(tab, key);
}

/* delete one node of a bucket from a hash table */
   boolean hashlst_del_element(hashlst_ptr tab, void *key) {
	return hashtbl_del_element(tab, key);
}

/* print a hash table */
void hashlst_print(FILE *fp, const hashlst_ptr tab) {
	hashtbl_print(fp, tab);
}

