
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
Miscellaneous DLC utilities.

Author: Shuvra S. Bhattacharyya
*******************************************************************************/

#include <stdio.h>
#include <time.h>
#include "stlib.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "error.h"


int _stlib_debug_on = FALSE;

/* convert (in-place) all uppercase alphabetic characters
 * to lowercase. Leaves all other characters unchanged.
 */
void strlower(char *s) {
  char *p;
  for (p=s; (*p)!='\0'; p++) (*p) = tolower(*p);
}

boolean blankstr(char *s) {
  while (*s != '\0') {
    if (!isspace((int)(*s))) return FALSE;
    s++;
  }
  return TRUE;
}

/* This function uses the system clock to set
   the random-number-generator seed. The seed
   that was used is returned. The seed is set
   to be the number of seconds that have elapsed
   so far in the present day relative to 
   Greenwich Mean Time (GMT). */
int randomly_set_rand_seed()
{
  time_t t1;
  struct tm *tmptr;
  int tmp;

  t1 = time(NULL);
  tmptr = gmtime(&t1);  
  tmp = ((tmptr->tm_hour) * 3600) + ((tmptr->tm_min) * 60) +
        tmptr->tm_sec;
  srand((unsigned)tmp);
  return tmp;

    

} /* randomly_set_rand_seed */

int read_line(char *buffer, int limit, FILE *fp) {
    char *newline_ptr = NULL, *p = NULL;

    /* We Need to allow fgets one extra space to store a trailing newline
       if it exits.
    */
    if (fgets(buffer, limit + 1, fp) == NULL) {
        return 0;
    }

    /* Find the newline character on the line if it exists. */
    newline_ptr = strchr(buffer, '\n');

    if (newline_ptr == NULL) {
        /* Do not complain if the last line of the file is not terminated
           by a newline.
        */
        if (!feof(fp)) {
            fprintf(stderr,
                    "Error: line too long. Limit is %d characters \n", limit);
            exit(1);
        }
    }

    /* Remove trailing newline */
    if (newline_ptr != NULL) {
        (*newline_ptr) = '\0';
    }

    /* If the trailing newline is preceding by a carriage return, then
       remove the carriage return (for Windows format files).
    */
    if (newline_ptr > buffer) {
        p = (newline_ptr - 1);
        if ((*p) == '\r') {
            (*p) = '\0';
        }
    } 

    return 1;    
} /* read_line */

/* substitute first occurence of str1 with str2 in fp
 * write result to stdout (file pointed to by fp) is unmodified */
void strsub(char *str1, char *str2, FILE *fp) {
    char buf[512];
    char *p;
    char l1[512], *l2;
    boolean found = FALSE;

    while (read_line(buf, 512, fp)) {
        if ((p=strstr(buf, str1))!=NULL) {
            if (!found) {
                strncpy(l1, buf, p-buf);
                l1[p-buf] = '\0';
                l2 = p + strlen(str1);
                printf("%s%s%s\n", l1, str2, l2);
                found = TRUE;
            } else  {
                puts(buf);
            }
        } else {
            puts(buf);
        }
    }
}




/* 
 * THIS APPEARS TO BE AN IMPROPERLY CONSTRUCTED FUNTION,
 * IN PARTICULAR, THE CAST TO (DOUBLE*) IS STRANGE ...
 * 
 *
 * round a double value to the nearest long integer.
 * if the fractional part is 0.5; use the
 * rounding direction parameter to determine
 * which way to round
 */ 
long dround(double x, dround_direction d) {
	/*
	double integ; 
	double frac = modf(x, &integ);
	*/
	long integ;
	double frac = modf(x, (double *)&integ);
	
	if (frac < 0.5) return integ;
	else if (frac > 0.5) return (integ+1);
	else {
            if (d==dround_up) return (integ+1);
	    else return integ;
        }
}

int generic_comparison(ty_comparison_function f,
		              const void *x, const void *y)
{
	if (f==NULL) {
		if (x<y) return -1;
		else if (x>y) return 1;
		else return 0;
	}
	else return f(x,y);
}

/* reverse string s in place */
void reverse(char s[]) {

        int c,i, j;

        for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
                c = s[j];
                s[i] = s[j];
                s[j] = c;
        }
}  

/* convert n to characters in s */
void stlib_itoa(int n, char s[]) {

        int i, sign;

        if ((sign = n) < 0)    /* record sign */
                n = -n;         /* make n positive */
        i = 0;
        do {    /* generates digits in reverse order */
                s[i++] = n % 10 + '0';  /* get next digit */
        } while ((n /= 10) > 0);        /* delete it */
        if (sign < 0)
                s[i++] = '-';
        s[i] = '\0';
        reverse(s);
} 

/* Open a file. Exit with an error message if the file cannot
   be opened with the specified mode ("r", "w", etc. as in
   coventional fopen). Otherwise return the file pointer of
   the opened file.
*/
FILE *stlib_xfopen(const char *fname, const char *mode) {
  FILE *fp;

  if ((fp=fopen(fname, mode))==NULL) {
        error_fatal("could not open file named '%s' with mode '%s'", 
                                fname, mode);
  }
  return fp;
}

void *xmalloc(size_t size)
{
   void *p;

   if ((p=malloc(size))!=NULL)
       return(p);
   else
       error_fatal("xmalloc error: insufficient memory");
   return NULL;  /*NOTREACHED*/
}

void *xcalloc(size_t nelem,size_t elsize)
{
   void *p;

   if ((p=calloc(nelem,elsize))!=NULL)
        return(p);
   else
       error_fatal("xcalloc error: insufficient memory");
   return NULL;  /*NOTREACHED*/
}

void *xrealloc(void *pointer, int new_size) {
    pointer = realloc(pointer, new_size);
    if (pointer == NULL) {
        error_fatal("xrealloc error: out of memory");
    }
    return pointer;
}


/*
   function stlib_check_line_size

   This functions checks to make sure that every line contains
   less than or equal to <limit> characters, including the
   terminating newline character, but not including the terminating
   NULL character in the string representation. 
   If a line exceeding the limit is found,
   then the program is terminated with an error message.
*/
void stlib_check_line_size(char *fname, int limit)
{
	FILE *fp;
    char *buf; 

    if (limit < 1) error_fatal("check_line_size: invalid line limit");
    buf=allocate_array(limit+15, char);
    fp = stlib_xfopen(fname, "r");
    while (fgets(buf, limit+10, fp)!=NULL) {
      if (strlen(buf) > limit) 
        error_fatal("File '%s': maximum line size exceeds %d", 
                    fname, limit);
    }
    free(buf);
    fclose(fp);
}


/*
    function stlib_get_field

    This function reads a named field from an input file <fp> that is
    already opened from reading. First the function attempts to
    read the string <fname>, the field name, from the input file.
    If <fname> is not the next non-blank field in the input file,
    then the program aborts with an error message. Otherwise, the
    next non-blank field is read from <fp> using fscanf with <fmt>
    as the format string. Only one field field should be read with
    <fmt>.

    The last argument should be a character buffer that is large
    enough to read the field specifier (or any erroneous string
    inserted in its place). E.g., it can be temporary buffer
    of length equal to the maximum line lenght.

	For example, suppose the input file contains the following text:

    name scooby
    age  8
    weight 75.5

    Then the following sequence of calls could read in the data from
    this file (after it has been opened as <fp>).

    get_field(fp, "name", "%s", buf, tmpbuf);
    get_field(fp, "age", "%d", &age, tmpbuf);
    get_field(fp, "weight", "%d", &wt, tmpbuf);
*/
void stlib_get_field(FILE *fp, char *fname, char *fmt, void *data,
                     char *buf) {

  if (fp==NULL) error_fatal("get_field: unopened input file");
  if (fscanf(fp, "%s", buf)!=1) 
    error_fatal("'%s' field expected", fname);
  if (strcmp(buf, fname)!=0)
    error_fatal("'%s' field expected", fname);
  if (fscanf(fp, fmt, data)!=1) 
    error_fatal("invalid data for '%s' field", fname);

} /* stlib_get_field */



/*
   function stlib_rand32()

   This function generates a random unsigned 32 bit quantity.

*/
unsigned stlib_rand32() {
  unsigned tmp, num;

  /* check for machine-dependent compatibility */
  if (RAND_MAX < 0x7fff) 
    error_fatal("stlib_rand32: rand() range too small");
  if (sizeof(unsigned) < 4)
    error_fatal("stlib_rand32: unsigned range too small");

  /* Shift out the low order bits, as in some implementations of
     rand(), these are not very random.
  */
  tmp = rand() >> 3;
  num = tmp & 0xfff; 
  tmp = rand() >> 3;
  tmp = tmp & 0xfff;
  tmp = (tmp << 12);
  num = num | tmp;
  tmp = rand() >> 3;
  tmp = tmp & 0xff;
  tmp = (tmp << 24);
  num = num | tmp;
  return num;
} /* stlib_rand32 */


/*
   stlib_filetostring

   Read a file into a string. Storage for the string is allocated
   in this routine and a pointer to the string is returned.
   The file pointed to by <fp> is closed at the end of this routine.
   It is assumed to be open for writing on entry.
*/

char* stlib_filetostring(FILE *fp) {

    int i=0; char c;

    char *buf = allocate_array(100, char);
    int buflen = 100;

    while ((c=getc(fp))!=EOF) {
      if (i==(buflen-1)) {
        buflen += 100;
        if ((buf = realloc(buf, buflen))==NULL) 
            error_fatal("stlib_filetostring: out of memory");
      }
      buf[i++]=c;
    }
    buf[i]='\0';
    fclose(fp);
    return buf;
} /* stlib_filetostring */

/*
   filetobuf

   Read a file into a string. <s> is a buffer of size <buflen>.
   Return value:
        1   Everything OK
        0   Entire file could not fit into <s>; only first
            <buflen>-1 characters have been read.
*/

int stlib_filetobuf(char *fname, char *s, int buflen) {

    int i=0; char c; FILE *fp;

    if ((fp=fopen(fname, "r"))==NULL) return (-1);
    while ((c=getc(fp))!=EOF) {
      if (i==(buflen-1)) {
        s[i]='\0';
        fclose(fp);
        return 0;
      }
      else {
        s[i++]=c;
      }
    }
    s[i]='\0';
    fclose(fp);
    return 1;
} /* filetostring */


char *stlib_remove_characters(char *s, char *characters_to_remove) {
    char *result = NULL;
    int i = 0;
    char next_character = '\0';
    int j = 0;

    result = allocate_array(strlen(s) + 1, char);
    for (i = 0; i < strlen(s); i++) {
        next_character = s[i];
        if (strchr(characters_to_remove, next_character) == NULL) {
            result[j] = next_character;
           j++;
        }
    }
    result[j] = '\0';
    return result; 
}

/* stilib_strdup

    This function returns a copy of a string. Memory for
    the copy is allocated internally, and can be deallocated
    using "free." NULL is returned if there isn't sufficient memory.

*/
char *stlib_strdup(char *src) {
    char *p = allocate_array(strlen(src)+1, char);
    if (p==NULL) return NULL;
    strcpy(p, src);
    return p;
}

void stlib_debug_enable(void) {
    _stlib_debug_on = TRUE;
}

void stlib_debug_disable(void) {
    _stlib_debug_on = FALSE;
}

void stlib_debug(char *fmt, ...) {
    va_list args;

    if (_stlib_debug_on) {
        va_start(args, fmt);
        vfprintf(stdout, fmt, args);
        fflush(stdout);
        va_end(args);
    }
}


