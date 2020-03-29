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
#include "lklst.h"

#define MAX_LINE_LENGTH 1024

/*****************************************************************************
The name of this program, for prefixing error messages.
*****************************************************************************/
#define PROGRAM_NAME  "process-results"

/*****************************************************************************
The number of header lines in the input format.
*****************************************************************************/
#define HEADER_LINE_COUNT 2

/*****************************************************************************
The starting text of the first line of the footer in the input format.
*****************************************************************************/
#define FOOTER_START "Tests completed on:"

/*****************************************************************************
The output file for detailed output.
*****************************************************************************/
FILE *detail = NULL;
 

/*****************************************************************************
The input format is assumed to be as follows

<HEADER_LINE_COUNT lines of header lines>
<TEST SEPARATOR LINE>
<TEST RESULT LABEL>
<TEST PATH>
[Possibly a 'differ' string to indicate a test failure]
<TEST SEPARATOR LINE>
<ERROR RESULT LABEL>
<TEST PATH>
[Possibly a 'differ' string to indicate a test failure]
...
<FOOTER_LINE_COUNT lines of footer lines>
 
*****************************************************************************/
 

/*****************************************************************************
The following constants pertain to return values associated with function
read_and_compare_line.
*****************************************************************************/
#define READ_MATCH 1
#define READ_NO_MATCH 2
#define READ_EOF 3



/*****************************************************************************
Read a line from standard input into the given buffer, and compare beginning
of the line to the given string <s>. Return an appropriate named constant
depending on whether end of file is encountered immediately upon
trying to read; a line is read, and the beginning of the line matches <s>;
a line is read, and the beginning of the line does not match <s>.
*****************************************************************************/
int read_and_compare_line(char *s, char *buffer) {
    if (read_line(buffer, MAX_LINE_LENGTH, stdin) == 0) {
        return READ_EOF;
    }
    if (strncmp(s, buffer, strlen(s)) == 0) {
        return READ_MATCH;
    }
    return READ_NO_MATCH;
}

/*****************************************************************************
This function is meant to passed through a function pointer argument
to lklst_traverse. This is why the argument is a generic pointer.
*****************************************************************************/
void print_path(void *path) {
    fprintf(detail, "%s\n", (char*)path);
}

/*****************************************************************************
Return if there is a match; return false if there is no match or if
end of file is encountered.
*****************************************************************************/
void read_expected_match(char *s, char *buffer) {
    if (read_line(buffer, MAX_LINE_LENGTH, stdin) == 0) {
        error_fatal("%s: end of file encountered when expecting '%s'", 
                PROGRAM_NAME, s);
    }
    if (strncmp(s, buffer, strlen(s)) == 0) {
        return;
    }
    error_fatal("%s: Missing '%s'", PROGRAM_NAME, s);
}

/*****************************************************************************
Usage: process-results <file name>

Input is taken from standard input.
Detailed output is sent to the output file <file name>.
Summary output is sent to standard output.
 
*****************************************************************************/
int main(int argc, char **argv) {
    char *test_separator = "--------";
    char *error_comparison_label = "Error comparison result";
    char *test_label = "Test result for";
    int i = 0;
    char buffer[MAX_LINE_LENGTH + 16];
    char path_buffer[MAX_LINE_LENGTH + 16];
    boolean done_reading_results = FALSE;
    boolean skip_test_separator = FALSE;
    int test_count = 0;
    lklst_ptr test_failure_list = NULL;
    lklst_ptr error_failure_list = NULL;
    char *path_name = NULL;
    char summary_string[MAX_LINE_LENGTH];

    if (argc != 2) {
        error_fatal("%s: arg count", PROGRAM_NAME);
    }

    detail = stlib_xfopen(argv[1], "w");

    for (i = 0; i < HEADER_LINE_COUNT; i++) {
        if (read_line(buffer, MAX_LINE_LENGTH, stdin) == 0) {
            error_fatal("%s: missing header line", PROGRAM_NAME);
        }
    }

    test_failure_list = lklst_allocate();
    error_failure_list = lklst_allocate();

    while (!done_reading_results) {
        if ((!skip_test_separator) && 
                read_and_compare_line(test_separator, buffer) != READ_MATCH) {
            done_reading_results = TRUE;
        } else {
            test_count++;
            skip_test_separator = FALSE; 
            /* Read the next test result. */
            read_expected_match(test_label, buffer);
            if (read_line(path_buffer, MAX_LINE_LENGTH, stdin) == 0) {
                error_fatal("%s: missing path for test", PROGRAM_NAME);
            }
            switch (read_and_compare_line(test_separator, buffer)) {
            case READ_EOF:
                error_fatal("%s: missing test separator or error (diff) report",
                        PROGRAM_NAME);
                break;
            case READ_NO_MATCH:
                path_name = allocate_array(strlen(path_buffer) + 1, char);
                strcpy(path_name, path_buffer);
                lklst_append(test_failure_list, path_name);
                read_expected_match(test_separator, buffer);
                break;
            default:
                break;
            }

            /* Read the next error comparison result. */
            read_expected_match(error_comparison_label, buffer);
            if (read_line(path_buffer, MAX_LINE_LENGTH, stdin) == 0) {
                error_fatal("%s: missing path for error output comparison", 
                        PROGRAM_NAME);
            }
            switch (read_and_compare_line(test_separator, buffer)) {
            case READ_EOF:
                error_fatal("%s: premature EOF in error output comparison", 
                        PROGRAM_NAME);
                break;
            case READ_NO_MATCH:
                if (strncmp(buffer, FOOTER_START, strlen(FOOTER_START)) == 0) {
                    done_reading_results = TRUE; 
                } else {
                    path_name = allocate_array(strlen(path_buffer) + 1, char);
                    strcpy(path_name, path_buffer);
                    lklst_append(error_failure_list, path_name);
                }
                /* FIXME: add path to list. */
                break;
            case READ_MATCH:
                /* Skip the separator on the next outer loop iteration since
                   we have already read it in. 
                */
                skip_test_separator = TRUE;
                break;
            }
        } /* else */
    } /* while */

    /* Display the results. */
    sprintf(summary_string, 
            "Test count: %d; test failures: %d; error output mismatches: %d\n",
            test_count, lklst_length(test_failure_list), 
            lklst_length(error_failure_list));
    printf("%s", summary_string);
    fprintf(detail, "%s", summary_string);
    if (lklst_length(test_failure_list) > 0) {
        fprintf(detail, "Test failures: \n");
        lklst_traverse(test_failure_list, print_path);
    }
    if (lklst_length(error_failure_list) > 0) {
        fprintf(detail, "Error comparison failures: \n");
        lklst_traverse(error_failure_list, print_path);
    }

    return 0;
}

