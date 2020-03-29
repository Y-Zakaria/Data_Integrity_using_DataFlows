#ifndef _attribute_h
#define _attribute_h

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
Header file for a named, typed attribute.

This ADT is under construction.

@author Shuvra S. Bhattacharyya 
*******************************************************************************/
 

 
/*******************************************************************************
Attribute types

Positive-valued attributes indicate built-in attributes. Negative
values can be used in application-specific ways for user-defined attributes.
*******************************************************************************/
#define DLC_ATTR_UNDEFINED 0
#define DLC_ATTR_INT 1
#define DLC_ATTR_FLOAT 2
#define DLC_ATTR_DOUBLE 3
#define DLC_ATTR_STRING 4
#define DLC_ATTR_CHAR 5

/*******************************************************************************
A vector provides access to a generic vector data type, which can 
store single- or multi-dimensional arrays of selected primitive types.
*******************************************************************************/
#define DLC_ATTR_VECTOR 6

/* An attribute */ 
typedef struct _dlc_attribute dlc_attribute;

/* A pointer to an attribute . */
typedef dlc_attribute *dlc_attribute_pointer;

/*******************************************************************************
INTERFACE FUNCTIONS
*******************************************************************************/

/*******************************************************************************
Allocate and return a pointer to a new attribute. 
*******************************************************************************/
dlc_attribute_pointer dlc_attribute_new(void);

/*******************************************************************************
Set the name of a given attribute to a given string value.
*******************************************************************************/
void dlc_attribute_set_name(dlc_attribute_pointer attribute, char *name);

/*******************************************************************************
Set the type of a given attribute to a given integer value.
*******************************************************************************/
void dlc_attribute_set_type(dlc_attribute_pointer attribute, int type);

/*******************************************************************************
Set a given attribute so that its value points to the object or primitive data
value pointed to by a given pointer p. Memory for the value pointed to must be
managed by the client. Changes by the client to the value pointed to by 
p will in general affect the value that is referenced by the attribute.
*******************************************************************************/
 
#endif
