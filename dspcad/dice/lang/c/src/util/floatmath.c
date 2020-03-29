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

/*
 * Module floatmath
 * @author Shuvra S. Bhattacharyya
 *
 */

#include <math.h>
#include <stdlib.h>
#include "error.h"
#include "stlib.h"
#include "floatmath.h"

struct _floatmath_vector {
    int length;
    double *value;
};

floatmath_vector *floatmath_vectorNew(int length, double *value) {
    int i;
    floatmath_vector *vector;

    if (length < 1) {
        error_fatal("vectorNew: invalid vector length of %d.", length);
    }
    vector = xmalloc(sizeof(floatmath_vector));
    vector->length = length;
    vector->value = xmalloc(length * sizeof(double));
    for (i = 0; i < length; i++) {
        vector->value[i] = value[i];
    }
    return vector;
}

double floatmath_vectorElement(floatmath_vector *vector, int index) {
    if ((index < 0) || (index >= vector->length)) {
        error_fatal("Vector index out of range (index = %f, limit = %f.",
                index, vector->length); 
    }
    return vector->value[index];
}

int floatmath_vectorLength(floatmath_vector *vector) {
    return vector->length;
}

/*
 * assuming a and b are positive integers
 * (in double format), return a mod b.
 *
 */
double floatmodulus(double a, double b)
{
  return a - (b*floor(a/b));
}



double euclid(double m, double n)
{
  double t;

  while (m > 0) {
    t = floatmodulus(n,m);
    n = m;
    m = t;
  }

  return n;
}


/* return the least common multiple of a and b */
double lcm(double a, double b)
{
  return (a * b / euclid(a,b));
}


/* reduce_fraction */
void reduce_fraction(double *num, double *denom)
{
  double gcd;
  gcd = euclid(*num, *denom);
  (*num) /= gcd;
  (*denom) /= gcd;
}

/* compute the gcd of the compnents of then n-vector a */
double vect_gcd(double *a, int n)
{
  int i;
  double g;

  g = a[0];
  for (i=1; i<n; i++) {
    g = euclid(a[i], g);
  }
  return g;
}



/* o

*/

