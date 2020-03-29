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
dx (dicemin) rm (remove) cr (carriage returns) core

Usage: dxrmcrcore <input_file> <filtered_file>

This program filters out all carriage return characters from the specified
file, and places the resulting filtered text in the specified output
file. The input file is left unchanged. A status message indicating
the number of characters removed is printed to standard output.
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

/* Suppress the "core" for conciseness. */
#define PROG "dxrmcr"

int main(int argc, char **argv) {
    int c;
    int count = 0;
    FILE *input = NULL;
    FILE *output = NULL;
        
    if (argc != 3) {
        fprintf(stderr, "%s error: arg count", PROG);
        exit(1);
    }
    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");
    if (input == NULL) {
        fprintf(stderr, "%s error: Could not open input file '%s'.\n", 
                PROG, argv[1]);
        exit(1);
    }
    if (output == NULL) {
        fprintf(stderr, "%s error: Could not open output file '%s'.\n", 
                PROG, argv[1]);
        exit(1);
    }

    while ((c = fgetc(input))!=EOF) {
        if (c == '\r') {
            count++;
            fputc('\n', output);
        } else {
            fputc(c, output);    
        }
    }

    fclose(input);
    fclose(output);

    printf("%s: %d carriage return character(s) replaced from '%s'.\n", 
            PROG, count, argv[1]);

    return 0;
}

