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
This is an ADT to represent a list of bits, and have the data 
available in a relatively user friendly way.

@author Robert Ricketts   
*******************************************************************************/

#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "bitvector.h"

/*

definition of the structure type for bitVector

*/

/*#define BITS_PER_UNIT 8
  typedef unsigned char dataUnit;
*/

#define BITS_PER_UNIT 32
typedef unsigned int dataUnit;

struct _bitVector{
	int size;
	dataUnit * data;
	
	
} ;

/*
sweep zeros out the portion of the data array that extends past bits->size
this is not intended for external use

*/
static void sweep(bitVector * bits){
	int i;	
	int index, subindex;
	int endExtra = (1+bits->size/BITS_PER_UNIT)*BITS_PER_UNIT;
	for(i=bits->size;i<endExtra;i++){
		index=i/BITS_PER_UNIT;
		subindex = BITS_PER_UNIT-i%BITS_PER_UNIT-1;
		(bits->data)[index]&=~(0x1<<subindex);
		
	}
	
	
}



unsigned int bitVector_get(bitVector * bits, int position ){
	int index;
	int subindex;
	
	index = position/BITS_PER_UNIT;
	subindex = BITS_PER_UNIT-position%BITS_PER_UNIT-1;
    if(position>=bits->size){
		error_warn("cannot get: bitVector index out of bounds");
		return -1;
	}
	
	return (((bits->data)[index])>>(subindex))&1;
	
		
}

int bitVector_size(bitVector * bits){
	
	return bits->size;
	
		
}

int bitVector_set(bitVector * bits, int position,int value){
	unsigned int one;
	int index;
	int subindex;
    
    if((position>=bits->size)||(position<0)){
		error_warn("cannot set: bitVector index out of bounds");
		return -1;
	}
	
	one =1;
	index = position/BITS_PER_UNIT;
	subindex = BITS_PER_UNIT-position%BITS_PER_UNIT-1;
	switch(value){
		case 0:
			((bits->data)[index])&=~(one<<subindex);
		break;
		case 1:
			((bits->data)[index])|=(one<<subindex);
		break;
		default:
		/*error statement*/
		error_warn("invalid operand");
		return -1;
	}
	return 0;
}
int bitVector_flip(bitVector * bits, int position){
	unsigned int one;
	int index;
	int subindex;
	if((position>=bits->size)||(position<0)){
		error_warn("cannot flip: bitVector index out of bounds");
		return -1;
	}
	
	one =1;
	index = position/BITS_PER_UNIT;
	subindex = BITS_PER_UNIT-position%BITS_PER_UNIT-1;
	((bits->data)[index])^=(one<<subindex);
	return 0;
}


int bitVector_equals(bitVector * bits1,bitVector * bits2){
	int i; 
    if(bits1->size!=bits2->size){
		return 0;	
	}
	
	for(i=bits1->size/BITS_PER_UNIT+1;i>=0;i--){
		if(bits1->data[i]!= bits2->data[i]){
			return 0;	
		}
	}
	return 1;
	
}
bitVector * bitVector_copy(bitVector *oldBits, bitVector * newBits){
	if( newBits->size<oldBits->size){
		error_warn("not enough space to copy bitVector");	
		return NULL;
	}
	sweep(oldBits);
	memcpy(newBits->data,oldBits->data,sizeof(dataUnit)*
		(1+(oldBits->size)/BITS_PER_UNIT));
		
	sweep(newBits);	
	return newBits;
	
}
bitVector * bitVector_randomize(bitVector *bits,double weight){
	int maxIndex, subindex, index;
	if(bits==NULL){
		error_warn("invalid operand in bitVector_randomize()");
		return NULL;
	}
	bitVector_zero(bits);	
	
	maxIndex = 1+bits->size/BITS_PER_UNIT;
	
	
	
	for(index =0;index<maxIndex;index++){
		
		for(subindex =0;(subindex<BITS_PER_UNIT);subindex++){
			if((rand()%1000)/1000.0<weight){
				bits->data[index]+=1;	
				
			}
			
			bits->data[index]<<=1;
			
			
		}
		
	}
	/*
	for(index =0;index<bits->size;index++){
		if((rand()%1000)/1000.0<weight){
				bitVector_set(bits,index,1);	
				
		}	
	}
	*/
	sweep(bits);
	return bits;
}

bitVector * rightShift(bitVector * a,int amount, bitVector * out){	
	int byteShift;
	int bitShift1;
	int bitShift2;
	int source, dest;
	
	/*this is a very inefficient way to do this*/
	bitVector* temp = bitVector_allocate(out->size);
	/*
	int source=0,dest=amount;
	if(amount<0){
		source=-amount;
		dest=0;	
	}
	for(;source<a->size&&dest<out->size;source++,dest++){
		bitVector_set(temp,dest,bitVector_get(a,source));
	}
	bitVector_copy(temp,out);
	*/
	
	
	byteShift = amount/BITS_PER_UNIT;
	bitShift1 = amount%BITS_PER_UNIT;
	bitShift2 = BITS_PER_UNIT-bitShift1;
	
	temp->data[byteShift]=a->data[0]>>bitShift1;
	source=1;
    dest=byteShift+1;
	
	
	for(;source<=a->size/BITS_PER_UNIT&&dest<=out->size/BITS_PER_UNIT;source++,dest++){
		temp->data[dest]=(dataUnit)( a->data[source]>>bitShift1) |((bitShift2!=32)?(dataUnit) (a->data[source-1]<<bitShift2):0);
		
		
	}
	bitVector_copy(temp,out);

	bitVector_deallocate(temp);	
	sweep(out);
	
	return out;
	
}

bitVector * leftShift(bitVector * a,int amount, bitVector * out){	
	int byteShift;
	int bitShift1;
	int bitShift2;	
	int source, dest;
	
	bitVector* temp = bitVector_allocate(out->size);
	
	byteShift = amount/BITS_PER_UNIT;
	bitShift1 = amount%BITS_PER_UNIT;
	bitShift2 = BITS_PER_UNIT-bitShift1;	
	
	source=byteShift;
    dest=0;
	
	
	for(;source<(a->size/BITS_PER_UNIT)&&(dest<out->size/BITS_PER_UNIT);source++,dest++){
		temp->data[dest]=(a->data[source]<<bitShift1) |((bitShift2!=32)?( a->data[source+1]>>bitShift2):0);
		
		
	}
	temp->data[dest]=a->data[source]<<bitShift1;
	bitVector_copy(temp,out);

	bitVector_deallocate(temp);	
	sweep(out);
	
	return out;
	
}
bitVector * bitVector_shift(bitVector * a,int amount, bitVector * out){
	if(amount==0){
		return bitVector_copy(a,out);	
	}
	if(amount<0){
		return leftShift(a,-amount,out);	
	}else{
		return rightShift(a,amount,out);
	}	
}


bitVector * bitVector_subVector(bitVector * a, int begin, int length, 
		bitVector * out){
	if((begin<0)||(begin>=a->size)||(length<0)||(length+begin>a->size)){
		error_warn("invalid range for bitVector_subVector");	
		return NULL;
	}
			
	bitVector_shift(a,-begin,out);
	
	bitVector_resize(out,length);
	
	return out;
	
}

bitVector * bitVector_concatenate(
		bitVector * left,
		bitVector * right,
		bitVector * dest){
	bitVector * temp1;
	bitVector * temp2;
	if(dest->size<left->size+right->size){
		error_warn("not enough space to concatenate bitVectors");
		return NULL;
	}
	temp1 =bitVector_allocate(left->size+right->size);	
	
	bitVector_copy(left,temp1);
	temp2 =bitVector_allocate(left->size+right->size);	
	bitVector_shift(right,left->size,temp2);
	bitVector_or(temp1,temp2,dest);
	bitVector_deallocate(temp1);
	bitVector_deallocate(temp2);
	return dest;
	
			
}

bitVector * bitVector_allocate(int length){
	bitVector * bits = xmalloc(sizeof(bitVector));
	
	bits->data = xcalloc(sizeof(dataUnit)*(1+length/BITS_PER_UNIT),1);
	
	bits->size = length;
	return bits;
}

bitVector * bitVector_resize(bitVector * bits,int length){
	if(length>bits->size){	
		bits->data = realloc(bits->data,sizeof(dataUnit)*(1+length/BITS_PER_UNIT));
		if(bits->data == NULL){
			error_fatal("insufficient memory");
		}
	}
	
	
	bits->size = length;
	sweep(bits);
	return bits;
}

void bitVector_deallocate(bitVector * bits){
	
	
	free(bits->data);
	free(bits);
}

bitVector * bitVector_and(bitVector * a,bitVector * b,bitVector *out){
	int maxIndex;
	int index;
	/* if the vectors are of different lengths,
	   go up to the length of the smallest one*/
	
	maxIndex=MIN( 1+a->size/BITS_PER_UNIT,1+b->size/BITS_PER_UNIT);
	maxIndex=MIN( 1+out->size/BITS_PER_UNIT,maxIndex);

	for(index=0;index<maxIndex;index++){
		(out->data)[index]=(a->data)[index]&(b->data)[index];	
		
	}
	sweep(out);
	return out;
	
	
}
bitVector * bitVector_or(bitVector *a,bitVector *b,bitVector *out){
	int maxIndex;
	int index;
	
	maxIndex=MIN( 1+a->size/BITS_PER_UNIT,1+b->size/BITS_PER_UNIT);
	maxIndex=MIN( 1+out->size/BITS_PER_UNIT,maxIndex);

	
	for(index=0;index<maxIndex;index++){
		(out->data)[index]=(a->data)[index]|(b->data)[index];	
		
	}
		
	sweep(out);
	return out;
}
bitVector * bitVector_not(bitVector *a,bitVector *out){

	int maxIndex;	
	int index;
	
    maxIndex=MIN( 1+a->size/BITS_PER_UNIT,1+out->size/BITS_PER_UNIT);
	
	for(index=0;index<maxIndex;index++){
		(out->data)[index]=~(a->data)[index];	
		
	}
	sweep(out);
	return out;
	
}
bitVector * bitVector_xor(bitVector *a,bitVector *b,bitVector *out){
	int maxIndex;
	int index;

	maxIndex=MIN( 1+a->size/BITS_PER_UNIT,1+b->size/BITS_PER_UNIT);
	maxIndex=MIN( 1+out->size/BITS_PER_UNIT,maxIndex);

	for(index=0;index<maxIndex;index++){
		(out->data)[index]=(a->data)[index]^(b->data)[index];	
		
	}
	sweep(out);
	return out;
}

bitVector * bitVector_zero(bitVector * bits){
	int i; 
	for(i=bits->size/BITS_PER_UNIT;i>=0;i--){
		bits->data[i]=0;
	}
	return bits;
}


bitVector * bitVector_fromDoubles(double * data, bitVector * bits){
	int length;
	/*unsigned int one;	*/
	int index ;
	
    length = bits->size;
	/*one =1;*/
    
	for(index =0;index<length;index++){
		bitVector_set(bits,index,data[index]);
		
		
	}
	return bits;
	
	
}
bitVector * bitVector_fromBytes(char * data, bitVector * bits){
		
	int length;	
	/*unsigned int one;*/
	int index ;
    
    length = bits->size;
	/*one =1;*/
	
	for(index =0;index<length;index++){
		bitVector_set(bits,index,data[index]);		
	}
	return bits;
	
}



char * bitVector_toString(bitVector * bits, char* str, int len){
	int a;
    char *start, *end;
	strcpy(str,"\0");
	
    start = str;
    end = start + len;
	for(a=0;a<(bits->size)&&a*2<len;a++){
		start += snprintf(start, end-start ,"%d ",bitVector_get(bits,a));
	}
    
	return str;

}

/*snprintf related statement should be revised; take bitVector_toString as a reference */
char * bitVector_toHex(bitVector * bits, char* str, int len){
	int a;
	int x;
	char *start, *end;
	/*sprintf(str,"\0");
	*/
	strcpy(str,"\0");
    start = str;
    end = start + len;
	
	for(a=0;a<(bits->size)&&a<len;a+=4){
		x=bitVector_get(bits,a)*8+bitVector_get(bits,a+1)*4+
			bitVector_get(bits,a+2)*2+bitVector_get(bits,a+3);
		/*snprintf(str,len,"%s%x",str, bits->data[a]);*/
		/*snprintf(str,len,"%s%x",str, x);*/
        start += snprintf(start, end-start ,"%x",x);
		
	}
	return str;

}

bitVector * bitVector_fromString(bitVector * bits, char *str){
	int bitPos =0;
	int strPos =0;
	
	while((bitPos<bits->size)&&(str[strPos]!=0)){
		if(str[strPos]=='0'){
			bitVector_set(bits,bitPos,0);
			bitPos++;
		}else if(str[strPos]=='1'){
			bitVector_set(bits,bitPos,1);
			bitPos++;
		}	
		strPos++;
		
	} 	
	return bits;
	
}






