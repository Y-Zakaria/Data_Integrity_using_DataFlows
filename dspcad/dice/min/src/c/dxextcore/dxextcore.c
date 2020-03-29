/***************************************************************************
@ddblock_begin copyright

Copyright (c) 1999-2013
Maryland DSPCAD Research Group, The University of Maryland at College Park 
All rights reserved.

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
This programs prints the file name extension from a file name path. If
\ddcode{-f} is specified as the first argument, then the part of the
name after the first period is printed, else the part of the name
after the last period is printed. If there is no period, then nothing
is printed. The output is terminated by a newline.

The usage format of \ddcode{dxextcore} is as follows.

dxextcore.exe [-f] file

Examples:

\begin{verbatim}
(1) dxextcore.exe -f /users/smith/proj/x.tar.gz
(2) dxextcore.exe /users/smith/proj/x.tar.gz
(3) dxextcore.exe -f x.tar.gz
(4) dxextcore.exe x.tar.gz
(5) dxextcore.exe -f x.txt
(6) dxextcore.exe x.txt
\end{verbatim}

\begin{verbatim}
These produce the following lines of output, respectively
\ddcodeblock_begin{}
(1) tar.gz
(2) gz
(3) tar.gz
(4) gz
(5) txt
(6) txt
\end{verbatim}

This script replaces an earlier python script dxext.py. This script is
part of dice-min package as dicelang-c package is dependent on this
script.
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PROG "dxextcore"
#define ERROR_ARG_COUNT "error: arg count"
#define ERROR_INVALID_ARG "error: invalid first argument"
#define FALSE 0
#define TRUE 1

int main(int argc, char **argv) {
    int first_period = FALSE;
    char *path = NULL;
    int last = -1;
    int first = -1;
    int start = -1;
    int end = -1;
    int i = 0;

    if ((argc < 2) || (argc > 3)) { 
      fprintf(stderr, "%s %s\n", PROG, ERROR_ARG_COUNT);
      exit(1);
    }

    /* Process the first argument if there are two arguments. */
    if (argc == 3) {
      if (strcmp(argv[1], "-f") == 0) {
        first_period = TRUE;
      } else { 
        fprintf(stderr, "%s %s\n", PROG, ERROR_INVALID_ARG);
        exit(1);
      }
    }

    /* Process the last argument. */
    if (argc == 2) {
      path = argv[1];
    } else {
      path = argv[2];
    }

    /***************************************************************************
    Start at the end of the file path. Stop as soon as we have worked back 
    through the beginning of the string or we see a path separator ('/').  
    Keep track of the last and first period that we have seen so far.
    ***************************************************************************/
    for (i = strlen(path) - 1; (i >= 0) && (path[i] != '/'); i--) {
        if (path[i] == '.') {
            if (last < 0) {
                last = i;
            } 
            first = i;   
        }        
    } 

    if (first >= 0) {
      if (first_period) {
	start = first + 1;
      } else {
	start = last + 1;
      }

      end = strlen(path) - 1;
    }
        
    /* Print the result */
    for (i = start; i <= end; i++) {
        putchar(path[i]);
    }
    printf("\n");

    return 0;
} 
