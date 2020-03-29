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
#include "stlib.h"
#include "error.h"
#include "string_parsing.h"

/*****************************************************************************
The maximum number of characters in a line of input
*****************************************************************************/
#define MAX_LINE 1000

/*****************************************************************************
The starting character of the configuration file header
*****************************************************************************/
#define HEADER_START ';'

/*****************************************************************************
Text filtering modes (corresponding to the <mode> argument to the program).
*****************************************************************************/
#define MODE_IN 0
#define MODE_OUT 1
 

/*****************************************************************************
Usage: extract-text <config-file> <mode>

This program extracts text from a file based on matching occurences of
<start-string> and <stop-string>. If <mode> is "out" ("in"), then any text
outside (inside) such matching occurences is passed to standard output, while
the rest is suppressed. Mode specifiers are case-sensitive. When passing
text between start- and stop-string delimeted blocks, lines that begin
with <prefix> have the <prefix> removed before passing the
text to standard output. <start-string>, <stop-string>, and <prefix>
are specified in the specified configuration file (<config-file>.

To be recognized in the input as a delimiter, a start string or
stop string must be separated from any adjacent text by white space.

The delimeters themseleves are suppressed from output.

Configuration file format:
Line 1: <header>
Line 2: <start-string>
Line 3: <stop-string>
Line 4: <prefix>

Line 1 should begin with the character ";". The purpose of this
line to is allow for brief documentation about the purpose and contents
of the configuration file.

Lines 2-4 should each contain a single input field. If no prefix is desired,
then "$none" should be entered on line 3 (without the quotes).

Example configuration file:
----------------------------
; dicedoc configuration file for bash and sh scripts
@ddbegin
@ddend
#
----------------------------
*****************************************************************************/
int main(int argc, char **argv) {
    char buffer[MAX_LINE + 10];
    char skipped_space[MAX_LINE + 10];
    char field_buffer[MAX_LINE + 10];
    char prefix[MAX_LINE + 10];
    char *next_field = NULL;
    char *current_field = NULL;
    boolean line_completed = FALSE;
    boolean suppress_input = FALSE;
    boolean prefix_present = TRUE;
    FILE *configuration = NULL;

    /* delimiters[0] stores the comment start delimeter, and delimeters[1]
       stores the comment end delimeter.
    */
    char delimeters[2][MAX_LINE + 10];
    int configuration_index = 0;
    int delimeter_index = 0;   
    char *delimeter = NULL;
    int field_index = 0;
    int mode = -1;


    if (argc != 3) {
        error_fatal("extract-text: arg count");
    } 

    /* Read the configuration file. */
    configuration = stlib_xfopen(argv[1], "r");
    if (!read_line(buffer, MAX_LINE, configuration)) {
        error_fatal("Missing header line in configuration file");
    }
    if (buffer[0] != HEADER_START) {
        error_fatal("Configuration header must start with '%c'", HEADER_START);
    }
    while ((configuration_index < 2) && 
            read_line(buffer, MAX_LINE, configuration)) {
        if (sscanf(buffer, "%s", delimeters[configuration_index]) != 1) {
            error_fatal("Incomplete configuration file");
        }
        configuration_index++;
    }
    if (!read_line(buffer, MAX_LINE, configuration)) {
        error_fatal("Missing prefix line in configuration file");
    }
    if (sscanf(buffer, "%s", prefix) != 1) {
        error_fatal("Missing prefix in configuration file");
    }
    if (strcmp(prefix, "$none") == 0) {
        prefix[0] = SP_NULL_CHAR;
        prefix_present = FALSE;
    } else {
        prefix_present = TRUE;
    }
    fclose(configuration);

    /* extract the input filtering mode */
    if (strcmp(argv[2], "in") == 0) {
        suppress_input = TRUE;
        mode = MODE_IN;
    } else if (strcmp(argv[2], "out") == 0) {
        suppress_input = FALSE;
        mode = MODE_OUT;
    } else {
        error_fatal("extract-text: invalid mode");
    }

    delimeter = delimeters[delimeter_index];
    while (read_line(buffer, MAX_LINE, stdin)) {
        line_completed = FALSE;
        field_index = 0;
        current_field = buffer;
        do {
            next_field = sp_get_field(current_field, field_index, 
                    skipped_space);
            current_field = next_field;
            if (!suppress_input) {
                printf("%s", skipped_space);
            }
            if (strlen(current_field) == 0) {
                line_completed = TRUE;
            } else {
                if (sscanf(current_field, "%s", field_buffer) != 1) {
                    error_fatal("Internal error: field expected.");
                }
                if (strcmp(field_buffer, delimeter) == 0) {
                    delimeter_index = (delimeter_index + 1) % 2;
                    delimeter = delimeters[delimeter_index];
                    suppress_input = (!suppress_input);
                } else if ((field_index == 0) && (mode == MODE_IN) && 
                        (prefix_present == TRUE) &&
                        (strcmp(prefix, field_buffer) == 0)) {
                    /* do nothing --- block the prefix */
                } else if (!suppress_input) {
                    printf("%s", field_buffer);
                } 
            }
            field_index++;
        } while (!line_completed);
        if (!suppress_input) {
            putchar('\n');
        }
    }

    fclose(stdout); 
    return 0;
}
