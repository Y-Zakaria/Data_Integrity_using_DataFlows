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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ARG_LEN 1024

/*****************************************************************************
Print an error message and exit.
*****************************************************************************/
void cvperror(char *message) {
    fprintf(stderr, "%s\n", message);
    exit(1);
}

/*****************************************************************************
Usage: convertpath <path>

Example: convertpath "C:/my name/mydir"/proj1\test1

Example output: "C:/my name/mydir/proj1/test1"

This program converts dos-style paths into unix-style paths, replacing
backwards slashes with forward ones throughout the path.

The output is quoted only if the argument contains one or more space 
characters
*****************************************************************************/
int main(int argc, char **argv) {

    char *arg = NULL;
    char result[MAX_ARG_LEN+1];
    int arg_index = 0;
    int result_index = 0;
    char result_char = '\0';
    int space_found = 0;
    
    if (argc != 2) {
        cvperror("Arg count");
    }
    arg = argv[1];
    if (strlen(arg) > MAX_ARG_LEN) {
        cvperror("Argument is too long");
    }
    for (arg_index = 0; arg_index < strlen(arg); arg_index++) {
        switch (arg[arg_index]) {
        case '"':
            /* Do not echo the quote. */
            result_char = '\0';  
            break;
        case '\\':
            result_char = '/';
            break;
        case ' ':
            space_found = 1;
            result_char = ' ';
            break;
        default:
            result_char = arg[arg_index];
            break; 
        }
        if (result_char != '\0') {
            result[result_index++] = result_char;
        }
    }
    result[result_index] = '\0';
    if (space_found) {
        printf("\"");
    }
    printf("%s", result);
    if (space_found) {
        printf("\"");
    }
    printf("\n");
    return 0;
}


