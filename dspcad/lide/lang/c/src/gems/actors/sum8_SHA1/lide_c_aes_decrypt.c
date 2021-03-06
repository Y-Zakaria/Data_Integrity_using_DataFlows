/*******************************************************************************
@ddblock_begin copyright

Copyright (c) 1997-2018
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
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <openssl/opensslconf.h>
#include <stddef.h>
#include <openssl/aes.h>

#include "lide_c_fifo.h"
#include "lide_c_aes_decrypt.h"
#include "lide_c_util.h"

/*******************************************************************************
AES Functions DEFINITION
*******************************************************************************/

/* Print Encrypted and Decrypted data packets */
void print_data_decrypt(const char *tittle, const void *data, int len)
{
    printf("%s : ", tittle);
    const unsigned char *p = (const unsigned char *)data;
    int i = 0;

    for (; i < len; ++i)
        printf("%02X ", *p++);

    printf("\n");
}

/*******************************************************************************
AES STRUCTURE DEFINITION
*******************************************************************************/

struct _lide_c_aes_decrypt_context_struct
{
#include "lide_c_actor_context_type_common.h"

    /* variable. */

    /* Input ports. */
    lide_c_fifo_pointer in;

    /* Output port. */
    lide_c_fifo_pointer out;
};

/*******************************************************************************
IMPLEMENTATIONS OF INTERFACE FUNCTIONS.
*******************************************************************************/

lide_c_aes_decrypt_context_type *lide_c_aes_decrypt_new(
    lide_c_fifo_pointer in,
    lide_c_fifo_pointer out)
{

    lide_c_aes_decrypt_context_type *context = NULL;

    context = lide_c_util_malloc(sizeof(lide_c_aes_decrypt_context_type));
    context->enable =
        (lide_c_actor_enable_function_type)lide_c_aes_decrypt_enable;
    context->invoke =
        (lide_c_actor_invoke_function_type)lide_c_aes_decrypt_invoke;
    context->in = in;
    context->out = out;
    return context;
}

boolean lide_c_aes_decrypt_enable(
    lide_c_aes_decrypt_context_type *context)
{
    boolean result = FALSE;

    result = (lide_c_fifo_population(context->in) >= 1) &&
             (lide_c_fifo_population(context->out) <
              lide_c_fifo_capacity(context->out));

    printf("test decrypt enable : %d \n", result);

    return result;
}

void lide_c_aes_decrypt_invoke(lide_c_aes_decrypt_context_type *context)
{
    int aes_input = 4;

    /* AES key for Encryption and Decryption */
    const static unsigned char aes_key[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                            0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

    /* Init vector */
    unsigned char iv[AES_BLOCK_SIZE];
    memset(iv, 0x00, AES_BLOCK_SIZE);

    /* Buffers for Encryption and Decryption */
    unsigned char dec_out[sizeof(aes_input)];
    unsigned char enc_out[((sizeof(aes_input) + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE];

    /* AES-128 bit CBC Encryption */
    AES_KEY dec_key;

    /* AES-128 bit CBC Decryption */
    memset(iv, 0x00, AES_BLOCK_SIZE);                            // don't forget to set iv vector again, else you can't decrypt data properly
    AES_set_decrypt_key(aes_key, sizeof(aes_key) * 8, &dec_key); // Size of key is in bits
    AES_cbc_encrypt(enc_out, dec_out, sizeof(aes_input), &dec_key, iv, AES_DECRYPT);

    /* Printing and Verifying */
    print_data_decrypt("\n Decrypted", dec_out, sizeof(dec_out));
}

void lide_c_aes_decrypt_terminate(
    lide_c_aes_decrypt_context_type *context)
{
    free(context);
}
