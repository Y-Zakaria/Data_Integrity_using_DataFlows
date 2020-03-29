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

#include "error.h"
#include "stlib.h"

#define MAX_COLUMNS 256
#define MAX_LINE 2048

/* Columns are numbered starting at 0. Column output selection is
   done with an argument of the form n[.w], where n is the selected
   column  number (to be passed to the output), and .w provides
   an optimal minimum field width specification for right-justified
   printing of the column.

   Example:

   flcol 2.4 5 --> print the third input column with a minimum field width
   of 4 and the sixth column with the default minimum field width (0).
*/

int main(int argc, char **argv) {

    /* The ith entry of the following array is true if the associated
       column is to be passed to standard output.
    */
    int columns[MAX_COLUMNS];

    /* The ith entry of the following gives the minimum field width 
       of the corresponding column in the output. 
    */
    int widths[MAX_COLUMNS];

    int i = 0, column = 0, columns_printed = 0, current_column = 0, width = 0;
    char *delimeters = " \t";

    char line[MAX_LINE + 10];
    char *token = NULL;
    char *dot = NULL;

    for (i = 0; i < MAX_COLUMNS; i++) {
        columns[i] = 0;
    }

    for (i = 0; i < MAX_COLUMNS; i++) {
        widths[i] = 0;
    }


    /* Read the set of selected columns. */
    for (i = 1; i < argc; i++) {
        if (sscanf(argv[i], "%d", &column) != 1) {
            error_fatal("Invalid column specifier");
        }
        if (column < 0) {
            error_fatal("Negative column specifier");
        }
        if (column >= MAX_COLUMNS) {
            error_fatal("Invalid column specifier");
        }
        columns[column] = 1;
        dot = strchr(argv[i], '.');
        if (dot != NULL) {
            if (sscanf((dot + 1), "%d", &width) != 1) {
                error_fatal("Invalid field width specifier");
            }
            if (width < 0) {
                error_fatal("Negative width specifier");
            }
            widths[column] = width;
        }
    }

    while (read_line(line, MAX_LINE, stdin)) {
        token = strtok(line, delimeters);
        columns_printed = 0;
        current_column = 0;
        while (token != NULL) {
            if (columns[current_column]) {
                if (columns_printed > 0) {
                    printf(" ");
                }
                width = widths[current_column];
                printf("%*s", width, token);
                columns_printed++;
            }
            token = strtok(NULL, delimeters);
            current_column++;
        }
        printf("\n");
    }

    return 0;
}
