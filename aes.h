#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

#define KEY_128_BITS (size_t) 16
#define KEY_192_BITS (size_t) 24
#define KEY_256_BITS (size_t) 32

#define BLOCK_SIZE_128_BITS (uint8_t) 16 // 128 bits = 16 bytes

typedef struct aes_key
{
    uint8_t *key;
    size_t key_size;
} t_aes_key;

// --- Key functions ---

t_aes_key* generate_aes_key(size_t key_size);
void print_aes_key(t_aes_key *key);
void free_aes_key(t_aes_key *key);

// --- Encrypt functions ---

void aes_encrypt(unsigned char *plaintext, t_aes_key *key, uint8_t *ciphertext);
void main_encrypt(uint8_t *state, uint8_t *expanded_key, uint8_t nbr_rounds);

void create_round_key(uint8_t *expanded_key, uint8_t *round_key);
void add_round_key(uint8_t* state, const uint8_t* round_key);
void sub_bytes(uint8_t* state);
void shift_rows(uint8_t *state);
void shift_row(uint8_t *state, uint8_t nbr);

void mix_columns(uint8_t *state);
void mix_column(uint8_t *column);

// --- Decrypt functions ---

void aes_decrypt(uint8_t *ciphertext, t_aes_key *key, unsigned char *plaintext);
void main_decrypt(uint8_t *state, uint8_t *expanded_key, uint8_t nbr_rounds);

void sub_bytes_inv(uint8_t* state);
void shift_rows_inv(uint8_t *state);
void shift_row_inv(uint8_t *state, uint8_t nbr);

void mix_columns_inv(uint8_t *state);
void mix_column_inv(uint8_t *column);

// --- General functions ---

void rotate(uint8_t *word);
void core(uint8_t *word, uint32_t iteration);
void key_expansion(const t_aes_key* key, uint8_t* expanded_key);

uint8_t set_number_round(uint8_t key_size);

uint8_t gf_mul(uint8_t a, uint8_t b);
void generate_sbox_inv(void);