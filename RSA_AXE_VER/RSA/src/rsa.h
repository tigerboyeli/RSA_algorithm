#ifndef _H_RSA_
#define _H_RSA_

#include <string.h>
#include <stdio.h>

// For sqrt
#include <math.h>

// Use malloc & free
#include <stdlib.h>

// For uint8_t, ...
#include <stdint.h>

// For rand
#include <time.h>

typedef struct public_key
{
    uint64_t N; // modulus
    uint64_t e; // public exponent
} t_public_key;

typedef struct private_key
{
    uint64_t N; // modulus
    uint64_t d; // private exponent
} t_private_key;

typedef struct keys
{
    t_public_key public_key;
    t_private_key private_key;
} t_keys;

typedef struct key_bytes
{
    uint8_t *byte_array;
    size_t length;
} t_key_bytes;

// ----

t_keys *initialize_keys(void);
void generate_rsa_keys(t_keys *keys);

void rsa_encrypt(char *plaintext, uint64_t len, t_public_key public_key, uint64_t *ciphertext);
void rsa_decrypt(uint64_t *ciphertext, uint64_t len, t_private_key private_key, char *plaintext);
void print_ciphertext(uint64_t *ciphertext, uint64_t len);

void print_rsa_keys(t_keys *keys);
void free_keys(t_keys *keys);

// ----

t_key_bytes *public_key_to_bytes(t_keys *keys);
t_public_key bytes_to_public_key(const t_key_bytes *byte_array);
void print_public_key_bytes(const t_key_bytes *bytes);
void free_public_key_bytes(t_key_bytes *key_bytes);

// ----

uint8_t* uint64_array_to_bytes(const uint64_t * array, uint64_t array_size);
uint64_t *bytes_to_uint64_array(uint8_t* byte_array, size_t byte_array_size);
void print_bytes(const uint8_t *byte_array, size_t length);

// ---- ----

uint64_t is_prime(uint64_t number);
uint64_t miller_rabin(uint64_t n, uint64_t a);
uint64_t generate_prime(void);

uint64_t gcd(uint64_t a, uint64_t b);
uint64_t mod_inverse(uint64_t a, uint64_t m);
uint64_t mod_exp(uint64_t base, uint64_t exp, uint64_t mod);

uint64_t powmod(uint64_t b, uint64_t e, uint64_t m);

void rsa_to_bytes(t_public_key *public_key, uint8_t *byte_array);

uint64_t htonll(uint64_t x);
uint64_t ntohll(uint64_t x);

#endif