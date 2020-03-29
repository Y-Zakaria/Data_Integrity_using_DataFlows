#ifndef _floatmath_h
#define _floatmath_h

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
Header file for utilities that work with floating point values.
Author: Shuvra S. Bhattacharyya 
*******************************************************************************/
 
/* A vector of floating point (double-typed) numbers. */
typedef struct _floatmath_vector floatmath_vector;

/* Return a new vector with a given length, and a given sequence (array) of 
   element values. 
*/
floatmath_vector *floatmath_vectorNew(int, double *);

/* Given an index and a vector, return the element of the vector at that index.
Vectors are indexed starting at zero.
*/
double floatmath_vectorElement(floatmath_vector *, int);

/* Given a vector, return the length (number of elements in) the vector. 
*/
int floatmath_vectorLength(floatmath_vector *);

/* Return the remainder when dividing the first number by the second. */
double floatmodulus(double, double);

/* Using Euclid's algorithm, return the greatest common divisor of two 
numbers.
*/
double euclid(double, double);

/* Return the least common multiple of two numbers. */
double lcm(double, double);

/* Given pointers to the numerator and denominator of a fraction, modify
the numerator and denominator so that the fraction is in reduced form.
*/
void reduce_fraction(double *, double *);

/* Given an array of floating point numbers and the length of the array,
return the greatest common divisor of all elements in the array.
*/
double vect_gcd(double *, int);

#endif
