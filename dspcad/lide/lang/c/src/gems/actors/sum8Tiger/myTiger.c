#include <stdio.h>
#include <string.h>
#include "myTiger.h"



struct TigerContext
{
    unsigned char chunk[64];      /* 512-bit message blocks      */
    word64 h[3];

}TigerContext;


void tiger_padding_msg(unsigned char *msg, int length){
    int i = 0;
    int l = strlen(msg);
    TigerContext.chunk[l] = 0x80;
    for (i=l+1; i<56; i++){ // zeros
        TigerContext.chunk[i] = 0;
    }
    TigerContext.chunk[56] = length*8;
    TigerContext.chunk[57] = length*8 >> 8;
    TigerContext.chunk[58] = length*8 >> 16;
    TigerContext.chunk[59] = length*8 >> 24;
    for (i=60; i<64; i++) //high lenght 32-bits  / to be optimized
        TigerContext.chunk[i] = 0;
    i = 0;
    while (l--){
        TigerContext.chunk[i++] = *msg;
        msg++;
    }
    
}

void tiger_compute_inter_hashs(void){
    word64 a,b,c,aa,bb,cc;

    word64 x[8];

    int i = 0;


    /** break chunk into 8 64-bit little-endian words x[i], 0 ≤ i ≤ 7 **/
    for (i = 0; i < 8; i++){
        x[i] = (word64) TigerContext.chunk[i * 8];
        x[i] |= ((word64) TigerContext.chunk[i * 8 + 1]) << 8;
        x[i] |= ((word64) TigerContext.chunk[i * 8 + 2]) << 16;
        x[i] |= ((word64) TigerContext.chunk[i * 8 + 3]) << 24;
        x[i] |= ((word64) TigerContext.chunk[i * 8 + 4]) << 32;
        x[i] |= ((word64) TigerContext.chunk[i * 8 + 5]) << 40;
        x[i] |= ((word64) TigerContext.chunk[i * 8 + 6]) << 48;
        x[i] |= ((word64) TigerContext.chunk[i * 8 + 7]) << 56;
    }
    
    a = TigerContext.h[0];
    b = TigerContext.h[1];
    c = TigerContext.h[2];

    // save_abc
    aa = a;
    bb = b;
    cc = c;

    passe(a,b,c,5)
    key_schedule
    passe(c,a,b,7)
    key_schedule
    passe(b,c,a,9)
    key_schedule



    // feedforward 
      a ^= aa;
      b -= bb;
      c += cc;

    TigerContext.h[0] = a;
    TigerContext.h[1] = b;
    TigerContext.h[2] = c;

    for (i = 0; i<64; i++)
        TigerContext.chunk[i] = 0;

}


void tiger(unsigned char *m, int length, word64 *hh){
    TigerContext.h[0] = 0x0123456789ABCDEFLL;
    TigerContext.h[1] = 0xFEDCBA9876543210LL;
    TigerContext.h[2] = 0xF096A5B4C3B2E187LL;

    int i = 0;
    int j = 0;

 /** message processing **/
    if (length < 64)
    {
        tiger_padding_msg(m, length);
        tiger_compute_inter_hashs();
    }
    else
    {
        for(i = length; i>=64; i-=64){
            for(j =0; j<64; j++)
            {
                TigerContext.chunk[j] = m[j];
            }
            tiger_compute_inter_hashs();
        }
        
        tiger_padding_msg((m + length - i) ,length);
        tiger_compute_inter_hashs();
    }      

    /** calculate the final hash 192-bit **/
    // print_hash_hex(&TigerContext.h[0]);
    // print_hash_hex(&TigerContext.h[1]);
    // print_hash_hex(&TigerContext.h[2]);
    // printf("\n");

    hh[0] = TigerContext.h[0];
    hh[1] = TigerContext.h[1];
    hh[2] = TigerContext.h[2];
} 

int tiger_verify_integrity(word64 *hh, word64 *ref){
    int i = 0;
    for(i =0; i<3; i++){
        if (hh[i] != ref[i]){
            print_hash_hex(&hh[i]);
            print_hash_hex(&ref[i]);
            printf("\nIntegrity issue: data received is corrupted!\n");
            return 0;
        }
        print_hash_hex(&hh[i]);
    }
    printf("\n");
    return 1;
}
