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
Other Constributors: Bishnupriya Bhattacharya
*******************************************************************************/

#include <stdio.h>
#include "lklst.h"


/* container for one element of a linked list */
typedef struct lklst_node {
  void *data;              /* pointer to the data associated with
			      the element */
  struct lklst_node *next; /* pointer to next element of the list */
} lklst_node;

typedef lklst_node *lklst_nodeptr;

/* A linked list */
struct lklst {
lklst_nodeptr head;      /* ptr to first element of the list */
lklst_nodeptr iter_pos;  /* The iter_pos is used when iterating
			    through the list. During an iteration
			    through the list, it is initialized
			    (with lklst_iter_init)
			    to start at the beginning of the
			    list, and it advances successively
			    to the next element with each
			    call to lklst_iter_next) */
int length; 		 /* the length of the list */
};

lklst_ptr lklst_allocate() {
  lklst_ptr l = allocate_scalar(lklst);
  lklst_init(l);
  return l;
}

void lklst_init(lklst_ptr l) {
	l->head=NULL;
	l->iter_pos = NULL;
	l->length = 0;
}

void lklst_insert(lklst_ptr l, void *d) {
  lklst_nodeptr p = allocate_scalar(lklst_node); 
  p->data = d;
  p->next = l->head;
  l->head = p;
  l->length++;
}

void lklst_append(lklst_ptr l, void *d) {

  lklst_nodeptr p = l->head, prev=NULL, n;

  if (p == NULL) {
    lklst_insert(l, d);
    return;
  }

  while (p!=NULL) {
    prev = p;
    p = p->next;
  }

  /* prev points to the last element in the list */
  n = allocate_scalar(lklst_node); 
  n->data = d;
  n->next = NULL;
  prev->next = n;
  l->length++;

}

void *lklst_iter_init(lklst_ptr l) {
  if (l->head == NULL) return NULL;
  else {
    l->iter_pos = l->head;
    return l->head->data;
  }
}

void *lklst_iter_next(lklst_ptr l) {
  if (l->iter_pos == NULL) return NULL;
  else {
    l->iter_pos = l->iter_pos->next;
    if (l->iter_pos == NULL) return NULL;
    else return l->iter_pos->data;
  }
}

void lklst_traverse(lklst_ptr l, ty_generic_onearg f) {
  void *p=lklst_iter_init(l);
  while (p!=NULL) {
    f(p);
    p = lklst_iter_next(l);
  }
}

boolean lklst_isempty(lklst_ptr l) { 
  return((l->head)==NULL);
}

int lklst_length(lklst_ptr l) {

  return l->length;

}

/* return a pointer to the first data item in the list
 * that matches "d". If the comparison function is NULL,
 * we return the first data item whose address matches "d".
 * If the item is not found, NULL is returned.
 */
void *lklst_find(lklst_ptr l, const void *d, ty_comparison_function cmp) {

  void *p=lklst_iter_init(l);
  while (p!=NULL) {
    if (generic_comparison(cmp, p, d)==0) return p;
    p = lklst_iter_next(l);
  }
  return NULL;
}

/* This routine allows a portion of the data to be compared. The user supplies
 * only portion of the data in "d", and the "ex" function pointer, which the
 * routine uses to extract the relevant portion from the data present in the
 * list, and then uses the "cmp" function pointer routine for comparison.
 * If "ex" is NULL, then this behaves the same as lklst_find(), if the user
 * supplies the entire data in "d".
 */
void *lklst_extract_and_find(lklst_ptr l, const void *d,
                             lklst_extract_t ex,
                             lklst_cmp_t cmp) {

  void *p=lklst_iter_init(l);
  if (ex == NULL)
    return lklst_find(l, d, cmp);
  while (p!=NULL) {
    if (generic_comparison(cmp, ex(p), d)==0) return p;
    p = lklst_iter_next(l);
  }
  return NULL;
}

/* delete the first item that matched "d". See "lklst_find" for an
 * explanation of the "cmp" argument. If the item is found
 * and deleted successfully then TRUE is returned, otherwise
 * FALSE is returned.
 */
boolean lklst_delete(lklst_ptr l, const void *d, ty_comparison_function cmp) {

  lklst_nodeptr p = l->head, prev=NULL;
  
  while (p!=NULL) {
    if (generic_comparison(cmp, (const void*)(p->data), d)==0)  {
	    if (prev==NULL) {
		/* delete the head of the list */
		l->head = l->head->next;
	    } else {
		prev->next = p->next;
	    }
	    free(p);
	    l->length--;
	    return TRUE;
    } else {
	    prev = p;
	    p = p->next;
    }
  }
    return FALSE;
}

boolean lklst_extract_and_delete(lklst_ptr l, const void *d, 
				 lklst_extract_t ex,
				 ty_comparison_function cmp) 
{

  lklst_nodeptr p = l->head, prev=NULL;
  
  if (ex == NULL)
    return lklst_delete(l, d, cmp);

  while (p!=NULL) {
    if (generic_comparison(cmp, (const void*)(ex(p->data)), d)==0)  {
	    if (prev==NULL) {
		/* delete the head of the list */
		l->head = l->head->next;
	    } else {
		prev->next = p->next;
	    }
	    free(p);
	    l->length--;
	    return TRUE;
    } else {
	    prev = p;
	    p = p->next;
    }
  }
    return FALSE;
}

void *lklst_extract_node(lklst_ptr l, int pos) {

  lklst_nodeptr p, prev;
  void *d;
  int i;

  if ((pos < 0) || (pos > l->length))
    error("lklst_extract_node: invalid pos argument - negative or too big");

  for (i=1, p=l->head, prev=NULL; i<pos; i++, prev=p, p=p->next) 
    ;
  
  if (prev == NULL) {
  /* the head node is to be extracted */
    l->head = p->next;
  } else  {
  /* not head node */
    prev->next = p->next;
  }
  d = p->data;
  free(p);
  l->length--;
  return(d);

}

void lklst_traverse_extract_apply(lklst_ptr l,
				  int npairs,
                                  lklst_extract_t ex, ty_generic_onearg f,
                                  ...)
{
  
  void *p=lklst_iter_init(l);
  void *q;
  va_list ap;
  lklst_extract_t ex1[10];
  ty_generic_onearg f1[10];
  int i;

  if (npairs < 1)
    error("lklst_traverse_extract_apply: at least one pair of routines have to be supplied");
  else if (npairs > 1) {
    va_start(ap, f);
    for (i = 0; i < (npairs - 1); i++) {
      ex1[i] = va_arg(ap, lklst_extract_t);
      f1[i] = va_arg(ap, ty_generic_onearg);
    }
    va_end(ap);
  }

  while (p!=NULL) {
    if (ex) {
      q = ex(p);
      if (f) f(q);
    } else
      if (f) f(p);

    /* now see how many more pairs of functions have been supplied */
    for (i = 0; i < (npairs - 1); i++) {
      if (ex1[i]) {
	q = (ex1[i])(p);
	if (f1[i]) (f1[i])(q);
      } else 
	if (f1[i]) (f1[i])(p);
    }
    p = lklst_iter_next(l);
  }

}

void lklst_traverse_extract_print(lklst_ptr l,
				  int npairs,
				  FILE *fp,
                                  lklst_extract_t ex, stlib_generic_print_t f,
                                  ...)
{
  
  void *p=lklst_iter_init(l);
  void *q;
  va_list ap;
  lklst_extract_t ex1[10];
  stlib_generic_print_t f1[10];
  int i;

  if (npairs < 1)
    error("lklst_traverse_extract_print: at least one pair of routines have to be supplied");
  else if (npairs > 1) {
    va_start(ap, f);
    for (i = 0; i < (npairs - 1); i++) {
      ex1[i] = va_arg(ap, lklst_extract_t);
      f1[i] = va_arg(ap, stlib_generic_print_t);
    }
    va_end(ap);
  }

  while (p!=NULL) {
    if (ex) {
      q = ex(p);
      if (f) f(fp, q);
    } else
      if (f) f(fp, p);

    /* now see how many more pairs of functions have been supplied */
    for (i = 0; i < (npairs - 1); i++) {
      if (ex1[i]) {
	q = (ex1[i])(p);
	if (f1[i]) (f1[i])(fp, q);
      } else 
	if (f1[i]) (f1[i])(fp, p);
    }
    p = lklst_iter_next(l);
  }
}

