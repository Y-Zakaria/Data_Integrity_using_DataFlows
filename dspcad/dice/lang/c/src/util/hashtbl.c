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
Hash table ADT. The KEY can be of any data type, but has to be uniform for a
single hash table. The DATA can be of any data type, and a single hash table
can have different data types associated with a KEY.  The implementation of the
collision chain of buckets is user specified. 

Author: Bishnupriya Bhattacharya
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "hashtbl.h"


/*********************************************************************
                        internal function
**********************************************************************/

static void _printData(FILE *, void *);
static void _printKEY(FILE *, void *);
static void *_extract_key(const void *);

/*********************************************************************
                        hash table data structure
*********************************************************************/


/* the hash table has a header, which contains a pointer to the actual
 * hash table array. Each element of the array is a pointer to void.
 * This will allow the chain of buckets in each array location to be
 * maintained as any arbitrary data structure - linked lists, heaps,
 * even another hash table ! Each operation that needs to be performed
 * on the hash table will depend on the actual implementation of the
 * bucket chain. Hence the header contains a function pointer
 * corresponding to each such operation. For e.g., the function pointer
 * "allocate_table_of_chains" allocates memory for the hash table array
 * depending on the implementation of the bucket chains, and the function
 * pointer "init_chain" appropriately initializes the data structure for each
 * bucket chain.
 * In addition, the header contains pointers to a generic hash function,
 * and a generic comparison function. The data contained in each bucket
 * can again be of arbitrary type (void *). Hence, the necessity of the
 * generic hash and comparison function.
 * Note that our design allows any implementation to be adopted for the
 * bucket chains, but that implementation has to be uniform over all the
 * bucket chains. If we want each bucket chain to have an arbitrary
 * implementation, (e.g. one bucket chain is implemented by a linked list,
 * another bucket chain by a hash table, etc.), then we will need to create
 * another structure "bucket_chain", make each hash table entry a pointer
 * to "bucket_chain", and move out the function pointers corresponding to
 * each hash table operation to the "bucket_chain" structure. We did not go for
 * the later, since usually, in practice, a uniform implementation is adopted
 * over all the bucket chains.
 */

struct hashtbl {

        void **table;
        /* the actual hash table array. Each array entry is a pointer to void,
         * and can point to any arbitrary data structure.
         */
        int size;
        /* maximum # elements in the table */
        int num_elements;
        /* # elements currently in the table */
        hashtbl_generic_hash_function_t hash;
        /* the hash function */
        hashtbl_cmp_t cmp;
        /* the comparison function. This returns 0 if the 2 arguments are
         * equal.
         */
	hashtbl_prnt_key_t prnt_key;
        hashtbl_allocate_t allocate_table_of_chains;
        /* function pointer that allocates memory for the table of size "size".
         * If the bucket chain is implemented as a linked list, say, then table
         * will be allocated with 'size" entries, each of which is a pointer to
         * void *. each row of the table will be allocated memory for the
         * header of the linked list.
         * Depending on the bucket chain implementation, allocating memory for
         * bucket chains may need some parameters. For, e.g. for a hash table
         * implementation, the size of the table, the hash function, etc. have
         * to be supplied as parameters through the "init_params" argument. For
         * a linked list implementation, "init_params" will be NULL.
         */
        hashtbl_init_t init_chain;
        /* Function pointer that does the proper initialization for a
         * particular bucket chain implementation.
         */
        /* The following pointers to functions correspond to each operation
         * that we wish to perform on the hash table
         */
        hashtbl_add_t add_to_chain;
        /* add an element to the chain. For a linked list implementation,
         * this will point to a function that accepts as arguments a linked
         * list, and the data to be inserted, and adds that data to the list
         */
        hashtbl_find_t find_in_chain;
        /* find the bucket in the "chain" whose key matches "data". Comparison
         * is done with the function pointer "cmp"
         */
        hashtbl_del_t del_from_chain;
        /* delete a bucket from the "chain", whose key matches "data".
         * Comparison is done with the function pointer "cmp". Returns
         * TRUE if successfully deleted, else returns FALSE.
         */
        hashtbl_isbucket_empty_t is_empty_chain;
	/* check if the bucket chain is empty */
        hashtbl_print_t print_chain;
        /* traverses the chain, and prints the data in each bucket by the
         * generic "print_function".
         */

};

/* This represents a single hash table node, i.e. the entry in each bucket.
 * Contains a KEY,the associated DATA, and 1 function pointer for printing the 
 * DATA
 */

typedef struct hashtbl_node_t {
        void *key;
        void *data;
	hashtbl_prnt_data_t prnt_data;
} hashtbl_node;


/* allocate a new hash table */
hashtbl_ptr hashtbl_allocate(
                       const unsigned size,
                       hashtbl_generic_hash_function_t hash,
                       hashtbl_cmp_t cmp,
                       hashtbl_prnt_key_t prnt_key,
		       hashtbl_allocate_t alloc,
                       const void * init_params,
                       hashtbl_init_t init,
                       hashtbl_add_t add,
                       hashtbl_find_t find,
                       hashtbl_del_t del,
		       hashtbl_isbucket_empty_t empty,
                       hashtbl_print_t print)

{
        hashtbl_ptr t;
	unsigned sizet;

        if (!size) sizet = 127;
	else sizet = size;

	/* allocate memory for the hash table header */
        t = allocate_scalar(hashtbl); 
        t->size = sizet;
        t->num_elements = 0;
        t->hash = hash;
        t->cmp = cmp;
	t->prnt_key = prnt_key;
        t->allocate_table_of_chains = alloc;
        t->init_chain = init;
        t->add_to_chain = add;
        t->find_in_chain = find;
        t->del_from_chain = del;
        t->is_empty_chain = empty;
        t->print_chain = print;
	/* now allocate memory for the hash table itself. uses the 
	* function pointer "allocate_table_of_chains"
	*/
        t->table = (void **)(*t->allocate_table_of_chains)(sizet, init_params);
        return t;
}

/* initalize the hash table. For each array entry, initialize the bucket chain
 * through the "init_chain" function pointer
 */
void hashtbl_init(hashtbl_ptr tab)
{
    int i;

    for (i = 0; i < tab->size; i++)
        (*tab->init_chain)(tab->table[i]);
}

/* add an element to the hash table. Compute the index into the hash table
 * array with the hash function, and store the data in that bucket chain, 
 * using the function pointer to "add_to_chain". Also store the function
 * pointers used for printing.
 */
void hashtbl_add_element(hashtbl_ptr tab, void *key, void *data,
			 hashtbl_prnt_data_t prnt_data) {

        int tab_index = ((*tab->hash)(key)) % tab->size;
        void *chain = tab->table[tab_index];
	/* wrap the key, and data in a bucket, then insert */
	hashtbl_node *n = allocate_scalar(hashtbl_node);
	n->key = key;
    	n->data = data;
	n->prnt_data = prnt_data;
        (*tab->add_to_chain)(chain, (void *)n);
        
}

/* find an element in the hash table. Returns a pointer to the data, whose 
 * key matches "key", else returns NULL. Uses the function pointer
 * "find_in_chain".
 */
void *hashtbl_find_element(const hashtbl_ptr tab, void *key)
{
          
        void *chain;
        int tab_index;
	    hashtbl_node *n;

        if (!tab) return NULL;

        tab_index = ((*tab->hash)(key)) % tab->size;
        chain = tab->table[tab_index];

        n = (hashtbl_node *)((*tab->find_in_chain)(chain, key, _extract_key, tab->cmp)); 

	/* find_in_chain will return a bucket (i.e. a hashtbl_node) in case of
	 * a match. the user wants the data associated with the bucket.
	 * Extract that and return
	 */
	
	if (n)
		return(n->data);
	else
		return(NULL);
}

/* delete a bucket from the hash table, whose key matches "data". Uses the
 * function pointer "del_from_chain".
 */
boolean hashtbl_del_element(hashtbl_ptr tab, void *key)
{
 
        void *chain;
        int tab_index;
	boolean result;

        if (!tab) return FALSE;

        tab_index = ((*tab->hash)(key)) % tab->size;
        chain = tab->table[tab_index];


        result = ((*tab->del_from_chain)(chain, key, _extract_key, tab->cmp)); 
	return(result);
}

/* print the hash table. Uses the "print_chain" function pointer for printing 
 * the contents of each bucket chain. The data in each bucket is printed
 * through the function pointer "print" provided as a parameter
 */
void hashtbl_print(FILE *fp, const hashtbl_ptr tab)
{

        void *chain;
        int i;

        if (!tab || tab->size == 0)
                return;
                
        for (i = 0; i < tab->size; i++) {
                chain = tab->table[i];
		(*tab->print_chain)(chain, 
				    fp,
				    NULL, _printKEY,
				    _extract_key, tab->prnt_key,
				    NULL, _printData);

        }
}

/* This function compares 2 buckets. The key fields are extracetd, and then 
 * passed on to the user defined comparison function for comparing 2 keys.
 * The pointer to the user defined comparison function is obtained from the
 * global variable _cmp2keys. It is not possible to pass this pointer as an
 * argument, because cmp2buckets will ultimately be called from the routine 
 * which traverses the buckets corresponding to a particular implementation of
 * the buckets. For a linked list implementation, lklst_find() will ultimately
 * call cmp2buckets. Passing a pointer to cmp2keys to lklst_find() implies that
 * the linked list knows that it is being used to implement a bucket chain,
 * which is not possible.
 */

void _printData(FILE *fp, void *bucket)
{
	hashtbl_node *b1 = (hashtbl_node *)bucket;
	fprintf(fp, " DATA: ");
	if (b1->prnt_data)
		(b1->prnt_data)(fp, b1->data);

}

void _printKEY(FILE *fp, void *bucket)
{
	fprintf(fp, " KEY: ");
}

void *_extract_key(const void *bucket) {
	return (void *)(((hashtbl_node *)bucket)->key);
}

