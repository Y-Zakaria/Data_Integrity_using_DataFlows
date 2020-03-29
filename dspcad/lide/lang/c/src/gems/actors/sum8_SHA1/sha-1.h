#ifndef _sha_1_h
#define _sha_1_h

#define HS 20 //taille en octets du hash pour SHA1

#define leftrotate(a, n_bits) ( (a << n_bits) | a >> (32 - n_bits))

void sha1_padding_msg(unsigned char *msg, int length);

void sha1_print_hash_hex(unsigned char* hh, int size);

void sha1_compute_inter_hashs(void);

void sha1(unsigned char *m, int length, unsigned char *hh);

int sha1_verify_integrity(unsigned char *hh, unsigned char *ref);

#endif