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


/*******************************************************************************
This is an ADT to represent a named, typed attribute. 
This ADT is under construction.
*******************************************************************************/
 

#include <stdio.h>

#include "stlib.h"
#include "attribute.h"

/*******************************************************************************
Structure for an attribute ADT instance.
*******************************************************************************/

struct _dlc_attribute {
    char *name;
	int type;
    void *value;
};

dlc_attribute_pointer dlc_attribute_new(void) {
    dlc_attribute_pointer attribute = NULL;

    attribute = allocate_scalar(dlc_attribute);
    attribute->name = NULL;
    attribute->type = DLC_ATTR_UNDEFINED;
    attribute->value = NULL;
    return attribute;
}

void dlc_attribute_set_name(dlc_attribute_pointer attribute, char *name) {
    attribute->name = name;
    
}

void dlc_attribute_set_type(dlc_attribute_pointer attribute, int type) {
    attribute->type = type;
}
