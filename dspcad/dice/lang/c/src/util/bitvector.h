#ifndef _bitvector_h
#define _bitvector_h

/****************************************************************************
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
******************************************************************************/

/*******************************************************************************
This is an ADT to represent a list of bits, and have the data available in a 
relatively user friendly way

Author: Robert Ricketts   
*******************************************************************************/

#include "stlib.h"

#define LONGBITS (CHAR_BIT*sizeof(unsigned long int))

typedef struct _bitVector bitVector;


/*
bitVector_allocate(int n)
 
allocates space for a bitVector of size n
it returns a pointer to the created bitVector

note: this is now the only function that does memory 
allocation for a new bitVector

*/
bitVector * bitVector_allocate(int );



/*
bitVector_resize(bitVector * bits,int length);	

this function takes in bitVector bits, and resizes the data field
if the new size is smaller than the old size, then data beyond the new size 
is lost

return values:
	NULL on an error
	bits otherwise

*/


bitVector * bitVector_resize(bitVector * ,int );


/*
bitVector_deallocate(bitVector * bits)
 
frees the space used by bits
*/
void bitVector_deallocate(bitVector * bits);



/*
bitVector_size(bitVector * bits)
 
returns the size of bitVector bits


*/
int bitVector_size(bitVector *);


/*
bitVector_get(bitVector * bits, int n)
 
returns the nth bit in the bitVector bits 

returns -1 if n >bits->size
*/
unsigned int bitVector_get(bitVector *, int);


/*
bitVector_set(bitVector * bits, int n, int v)
 
sets the nth bit in the bitVector bits to be v

return values:
	-1 on an error
	0 otherwise

*/
int bitVector_set(bitVector * , int,int);

/*
bitVector_flip(bitVector * bits, int n)
 
flips the nth bit in the bitVector bits 

return values:
	-1 on an error
	0 otherwise
*/
int bitVector_flip(bitVector * , int);



/*
bitVector_copy(bitVector from, bitVector * to)

this writes the contents of bitVector from into the bitVector to

if to is larger than from, only the first portion of from will be overwritten

if to is smaller than from, an error will be raised

return values:
	NULL on an error
	to otherwise

*/
bitVector * bitVector_copy(bitVector*,bitVector * );



/*
bitVector_equals(bitVector * bits1, bitVector * bits2);

this compares two bitVectors for equality,
the return value:
1 if they are equal
0 if they are not

*/
int bitVector_equals(bitVector *, bitVector *);


/*
bitVector_zero(bitVector * bits)

sets all the data in bits to be 0

return values:
	NULL on an error
	bits otherwise
*/
bitVector * bitVector_zero(bitVector *);

/*
bitVector * bitVector_and(bitVector * a,bitVector * b,bitVector * out);

performs a logical and on vectors a and b, and stores the result in out

return values:
	NULL on an error
	out otherwise

*/
bitVector * bitVector_and(bitVector *,bitVector *,bitVector *);



/*
bitVector * bitVector_or(bitVector * a,bitVector * b,bitVector * out);

performs a logical or on vectors a and b, and stores the result in out

return values:
	NULL on an error
	out otherwise

*/
bitVector * bitVector_or(bitVector *,bitVector *,bitVector *);



/*
bitVector * bitVector_not(bitVector * a,bitVector * out);

performs a logical not on vectors a, and stores the result in out

return values:
	NULL on an error
	out otherwise

*/
bitVector * bitVector_not(bitVector *,bitVector *);



/*
bitVector * bitVector_xor(bitVector * a,bitVector * b,bitVector * out);

performs a logical exclusive-or on vectors a and b, and stores the result in out

return values:
	NULL on an error
	out otherwise

*/
bitVector * bitVector_xor(bitVector *,bitVector *,bitVector *);




/*
bitVector_random(bitVector * bits,double weight)

this sets all the data in bits to be random values

the parameter weight defines the probability that a given bit
will have a value of 1. the value should be in the range 0.0 to 1.0 

return values:
	NULL on an error
	bits otherwise

*/
bitVector * bitVector_random(bitVector *,double);





/*
bitVector_fromDoubles(double * data,bitVector * dest)
 

this function reads through the array of doubles data
and sets the value of each bit to be that as the same one
in data

return values:
	NULL on an error
	dest otherwise


*/
bitVector * bitVector_fromDoubles(double* ,bitVector *);



/*
bitVector_fromBytes(char* data, bitVector * dest)

this function reads through the array of 1-byte integers data
and sets the value of each bit to be that as the same one as
in data

please note: the input is an array of 1-byte integers, not a string.
so the values should be '\0' or '\1'
if an input conatains other values, (such as '0' or '1'), 
the array will be invalid

to do a conversion from a string to a bitVector, use bitVector_fromString()

return values:
	NULL on an error
	dest otherwise

*/
bitVector * bitVector_fromBytes(char* , bitVector *);

/*
bitVector_toString(bitVector * bits, char* str, int len)

modifies the string str so that it contains the data of bits
len signifies the maximum number of characters that will be written to str.

return values:
	NULL on an error
	str otherwise
*/

char * bitVector_toString(bitVector * bits, char* str, int len);

/*
bitVector_fromString(bitVector * bits, char * str)

this function takes in a string of characters converts them into a bitVector,
and stores the result in bits.

this method ignores all characters in the string except for '0', '1', and '\0'
'0

return values:
	NULL on an error
	bits otherwise
*/

bitVector * bitVector_fromString(bitVector *, char *);



/*
bitVector_shift(bitVector * a,int amount, bitVector * out);

this performs a logical right shift of amount on bitVector a
and stores the result in out

to perform a left shift, simply pass in a negative value for amount

return values:
	NULL on an error
	out otherwise

*/
bitVector * bitVector_shift(bitVector * ,int , bitVector * );


/*
bitVector_subVector(bitVector * a, int begin, int length, bitVector * out);

this function takes a subsection of the bitVector a and stores it into out
this subvector starts at begin and has size length

return values:
	NULL on an error
	out otherwise

*/

bitVector * bitVector_subVector(bitVector * , int, int , bitVector * );

/*
bitVector_concatenate(bitVector * left, bitVector * right,bitVector*dest);

this function concatenates the bitVector left to the bitVector right
and stores the result in dest

return values:
	NULL on an error
	dest otherwise

*/

bitVector * bitVector_concatenate(bitVector *, bitVector *,bitVector*);







#endif
