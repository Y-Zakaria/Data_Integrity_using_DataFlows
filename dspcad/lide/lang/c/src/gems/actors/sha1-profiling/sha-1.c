#include <stdio.h>
#include <string.h>

#include "sha-1.h"


struct SHA1Context
{
    unsigned char chunk[64];      /* 512-bit message blocks      */
    unsigned int h[5];

}SHA1Context;

void sha1_padding_msg(unsigned char *msg, int length){
    int i = 0;
    int l =strlen(msg);
    SHA1Context.chunk[l] = 0x80;
    for (i=l+1; i<60; i++){ // zeros + high lenght 32-bits
        SHA1Context.chunk[i] = 0;
    }
    SHA1Context.chunk[60] = length*8 >> 24;
    SHA1Context.chunk[61] = length*8 >> 16;
    SHA1Context.chunk[62] = length*8 >> 8;
    SHA1Context.chunk[63] = length*8;
    i = 0;
    while (l--){
        SHA1Context.chunk[i++] = *msg;
        msg++;
    }
    
}

void sha1_print_hash_hex(unsigned char* hh, int size){
    int i;
    for(i=0;i<size;i++){
        printf("%02X ",hh[i]);
    }
    printf("\n");

}

void sha1_compute_inter_hashs(void){
    unsigned int a,b,c,d,e;
    unsigned int f = 0, k = 0;

    unsigned int w[80];

    unsigned int temp = 0;
    int i= 0;

    /** break chunk into sixteen 32-bit big-endian words w[i], 0 ≤ i ≤ 15 **/
    for (i = 0; i < 16; i++){
        w[i] = SHA1Context.chunk[i * 4] << 24;
        w[i] |= SHA1Context.chunk[i * 4 + 1] << 16;
        w[i] |= SHA1Context.chunk[i * 4 + 2] << 8;
        w[i] |= SHA1Context.chunk[i * 4 + 3];
    }
    /** extends words to 80 32-bit words **/
    for (i = 16; i < 80; i++){
        w[i] = (w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16]);
        w[i] = leftrotate(w[i],1);
    }
    a = SHA1Context.h[0];
    b = SHA1Context.h[1];
    c = SHA1Context.h[2];
    d = SHA1Context.h[3];
    e = SHA1Context.h[4];
    /** main loop **/
    for (i = 0; i < 80; i++){
        if ( i >= 0 && i <= 19){
            f = (b & c) | ((~b) & d);
            k = 0x5A827999;
        }
        else if ( i >= 20 && i <= 39){
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;
        }
        else if ( i >= 40 && i <= 59){
            f = (b & c) | (b & d) | (c & d); 
            k = 0x8F1BBCDC;
        }
        else if ( i >= 60 && i <= 79){
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
        }
        temp = leftrotate(a,5) + f + e + k + w[i];
        e = d;
        d = c;
        c = leftrotate(b,30);
        b = a;
        a = temp;
    }
    //Add this chunk's hash to result so far:
    SHA1Context.h[0] += a;
    SHA1Context.h[1] += b;
    SHA1Context.h[2] += c;
    SHA1Context.h[3] += d;
    SHA1Context.h[4] += e;

}

void sha1(unsigned char *m, int length, unsigned char *hh){
    int i,j = 0;
    int index = HS-1;

    SHA1Context.h[0] = 0x67452301;
    SHA1Context.h[1] = 0xEFCDAB89;
    SHA1Context.h[2] = 0x98BADCFE;
    SHA1Context.h[3] = 0x10325476;
    SHA1Context.h[4] = 0xC3D2E1F0;

    if (length < 64)
    {
        sha1_padding_msg(m, length);
        sha1_compute_inter_hashs();
    }
    else
    {
        for(i = length; i>=64; i-=64){
            for(j =0; j<64; j++)
            {
                SHA1Context.chunk[j] = m[j];
            }
            sha1_compute_inter_hashs();
        }
        
        sha1_padding_msg((m + length - i) ,length);
        sha1_compute_inter_hashs();
    }  

    /** calculate the final hash 160-bit **/ 
    for (i = 4; i>=0; i--){
        for (j = 0; j<4; j++){
                hh[index] = SHA1Context.h[i] >> 8*j;  
                index -= 1; 
        }
    }

    //sha1_print_hash_hex(hh,HS);
}

int sha1_verify_integrity(unsigned char *hh, unsigned char *ref){
    int i = 0;
    for(i =0; i<HS; i++){
        if (hh[i] != ref[i]){
            printf("%02X %02X\n", hh[i], ref[i]);
            printf("Integrity issue: data received is corrupted!\n");
            sha1_print_hash_hex(hh,HS);
            return 0;
        }
    }
    return 1;
}
