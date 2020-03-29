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
Utilities for parsing strings.
*******************************************************************************/
 

#include <ctype.h> 
#include <stdio.h> 
#include <string.h> 
#include "string_parsing.h"
#include "error.h"
#include "stlib.h"



boolean sp_is_numeric(char *s) {
    int i = 0;
    int digits = 0;
    int points = 0; 
    char c = '\0';

    if (s[0] == '-') {
        i++;
    }

    while (i < strlen(s)) {
        c = s[i];
        if (isdigit(c)) {
            digits++;
        } else if (c == '.') {
            points++;
        } else {
            return FALSE;
        }
        i++;
    }
    if ((digits == 0) || (points > 1)) {
            return FALSE;
    }
    return TRUE;
}


char *sp_first_field(char *s, char *skipped_space) {
  int i = 0;
  if (!isspace((int)(*s))) {
      return s;
  }
  while ((isspace((int)(*s)))&&((*s) != SP_NULL_CHAR)) {
      if (skipped_space != NULL) {
        skipped_space[i++] = (*s);
      }
      s++;
  }
  if (skipped_space != NULL) {
    skipped_space[i] = SP_NULL_CHAR;
  }
  return s;
}

char *sp_firstfield(char *s) {
    return sp_first_field(s, NULL);
}

char *sp_get_field(char *s, int i, char *skipped_space) {
    if (i < 0) {
        error_fatal("sp_get_field: invalid (negative) field index");
    }
    if (i == 0) {
        return sp_first_field(s, skipped_space);
    }
    else {
        return sp_next_field(s, skipped_space);
    }
}

char *sp_getfield(char *s, int i) {
    return sp_get_field(s, i, NULL);
}

char *sp_next_field(char *s, char *skipped_space) {
    int i = 0;
    if ((*s) == '\0') {
        if (skipped_space != NULL) {
            skipped_space[0] = SP_NULL_CHAR;
        }
        return s;
    }
    while ((!isspace((int)(*s)))&&((*s)!='\0')) {
        s++;
    }
    while ((isspace((int)(*s)))&&((*s)!='\0')) {
        if (skipped_space != NULL) {
            skipped_space[i++] = (*s);
        }
        s++;
    }
    if (skipped_space != NULL) {
        skipped_space[i] = SP_NULL_CHAR;
    }
    return s;
}


char *sp_nextfield(char *s) {
    return sp_next_field(s, NULL);
}


