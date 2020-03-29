/***************************************************************************
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
***************************************************************************/

/*******************************************************************************
This program filters selected words, based on a given word list file, from 
standard input.

Usage: dxrmwcore.exe <file>

Here, <file> is the word list file.

Naming: (r)e(m)oving (w)ords --- (core) utility for an associated wrapper
script

All words in standard input are passed to standard output except that those
that match words in the word list file. Words in the word list file and
standard input are separated by white space.
*******************************************************************************/
 
#include <stdio.h>
#include <string.h>

#include "lklst.h"
#include "error.h"
#include "stlib.h"

/* The name of this program. */
#define PRG "dxrmw"

/* Maximum input line size. */
#define MAX_LINE 1024

/*******************************************************************************
Delimeters that separate input fields. 
*******************************************************************************/
#define FIELD_DELIMETERS " \t\n\r"

int comparison_function(const void *f1, const void *f2) {
    int result = 0;
    char *s1 = NULL;
    char *s2 = NULL;

    s1 = (char *)f1;
    s2 = (char *)f2;

    result = strcmp(s1, s2);
    return result;
}

int main(int argc, char **argv) {
    char buffer[MAX_LINE + 10];
    char *token = NULL;
    char *delimeters = FIELD_DELIMETERS;
    FILE *word_list_file=NULL;
    lklst_ptr list = NULL;
    char *new_string = NULL;

    if (argc != 2) {
        error_fatal("%s error: arg count\n", PRG);
    }
    word_list_file = stlib_xfopen(argv[1], "r");

    list = lklst_allocate();
    
    while (read_line(buffer, MAX_LINE, word_list_file)) {
        token = strtok(buffer, delimeters);
        while (token != NULL) {
            if (lklst_find(list, token, comparison_function) != NULL) {
                error_fatal("%s error: duplicate word '%s'\n", PRG, token);
            }
            new_string = xmalloc(strlen(token) + 1); 
            strcpy(new_string, token);
            lklst_insert(list, new_string);
            token = strtok(NULL, delimeters);
        }
    }
    fclose(word_list_file);

    while (read_line(buffer, MAX_LINE, stdin)) {
        token = strtok(buffer, delimeters);
        while (token != NULL) {
            if (lklst_find(list, token, comparison_function) == NULL) {
                printf("%s\n", token);
            }
            token = strtok(NULL, delimeters);
        }
    }

    /* Make sure all output is written before we exit. */
    fflush(stdout);

    return 0;
}
