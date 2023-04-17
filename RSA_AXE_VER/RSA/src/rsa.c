#include "rsa.h"

/**
 * @brief allocate memory for keys structure
 * 
 * @return t_keys* key
 */
t_keys* initialize_keys(void){
    t_keys* keys = malloc(sizeof(t_keys)); 
    return keys;
}

void generate_rsa_keys(t_keys* keys){
    srand(time(NULL));  // Seed the random number generator

    // Choose two distinct prime numbers, p and q.
    uint64_t p = generate_prime();
    uint64_t q = generate_prime();

    // Calculate the product of the two primes (modulus), N = p * q.
    uint64_t N = p * q;

    // Calculate the totient of N, which is given by φ(N) = (p-1) * (q-1).
    uint64_t phi = (p - 1) * (q - 1);

    // Choose an integer e such that 1 < e < φ(N), and e is coprime to φ(N).
    uint64_t e, gcd_val;
    do {
        e = rand() % (phi - 2) + 2;  // Generate a random number between 2 and phi-1
        gcd_val = gcd(e, phi);
    } while(gcd_val != 1);

    // Calculate the modular inverse of e modulo φ(N), denoted as d, such that d * e ≡ 1 (mod φ(N)).
    uint64_t d = mod_inverse(e, phi);


    keys->private_key.d = d;
    keys->private_key.N = N;

    keys->public_key.e = e;
    keys->public_key.N = N;

}

// ciphertext = (plaintext ^ e) mod N
// Function to encrypt a message using RSA
void rsa_encrypt(char* plaintext, uint64_t len, t_public_key public_key, uint64_t *ciphertext){
    for(uint64_t i = 0; i < len; i++) {
        ciphertext[i] = mod_exp(plaintext[i], public_key.e, public_key.N);
    }
}


// plaintext = (ciphertext ^ d) mod N
// Function to decrypt a message using RSA
void rsa_decrypt(uint64_t *ciphertext, uint64_t len, t_private_key private_key, char *plaintext){
    for(uint64_t i = 0; i < len; i++) {
        plaintext[i] = mod_exp(ciphertext[i], private_key.d, private_key.N);
    }
}

// Print the ciphertext
void print_ciphertext(uint64_t *ciphertext, uint64_t len){
    printf("Ciphertext: '");
    for(uint64_t i = 0; i < len; i++) {
        printf("%ld ", ciphertext[i]);
    }
    printf("'\n");
}

void print_rsa_keys(t_keys* keys){
    puts("Public key:");
    printf("\tModulus  : %ld\n", keys->public_key.N);
    printf("\tExponent : %ld\n\n", keys->public_key.e);
    puts("Private key:");
    printf("\tModulus  : %ld\n", keys->private_key.N);
    printf("\tExponent : %ld\n", keys->private_key.d);
}

void free_keys(t_keys* keys){
    free(keys);
}

// ---- ----

t_key_bytes *public_key_to_bytes(t_keys *keys){
    t_key_bytes* key_bytes = (t_key_bytes*)malloc(sizeof(t_key_bytes));
    if (key_bytes == NULL) return NULL;

    // Calculate the size of the byte array
    size_t byte_array_size = sizeof(uint64_t) * 2;  // because modulus and exponent

    // Allocate memory for the byte array
    key_bytes->byte_array = (uint8_t*)malloc(byte_array_size);
    if (key_bytes->byte_array == NULL) {
        free(key_bytes);
        return NULL;
    }

    key_bytes->length = byte_array_size;

    rsa_to_bytes(&keys->public_key, key_bytes->byte_array);
    return key_bytes;
}

t_public_key bytes_to_public_key(const t_key_bytes* byte_array)
{
    // Convert the modulus and exponent values from network byte order
    uint64_t n_network, e_network;
    memcpy(&n_network, byte_array->byte_array, sizeof(uint64_t));
    memcpy(&e_network, byte_array->byte_array + sizeof(uint64_t), sizeof(uint64_t));
    uint64_t n = ntohll(n_network);
    uint64_t e = ntohll(e_network);

    // Create the public key struct
    t_public_key key = {n, e};

    return key;
}

void print_public_key_bytes(const t_key_bytes *bytes) {
    for (size_t i = 0; i < bytes->length; i++) {
        printf("%02x ", bytes->byte_array[i]);
    }
    printf("\n");
}

void free_public_key_bytes(t_key_bytes *key_bytes){
    free(key_bytes->byte_array);
    free(key_bytes);
}

// ---- ----

uint8_t* uint64_array_to_bytes(const uint64_t * array, uint64_t array_size){
    uint8_t * byte_array = (uint8_t*)malloc(array_size * sizeof(uint64_t));

    for (size_t i = 0; i < array_size; i++) {
        // Convert the array element to network byte order
        uint64_t element_network = htonll(array[i]);

        // Copy the array element to the byte array
        memcpy(byte_array + i * sizeof(uint64_t), &element_network, sizeof(uint64_t));
    }
    return byte_array;
}

uint64_t *bytes_to_uint64_array(uint8_t* byte_array, size_t byte_array_size){
    if (byte_array_size % sizeof(uint64_t) != 0) {
        // The byte array size must be a multiple of the uint64_t size
        return NULL;
    }

    printf("print array\n");
    print_bytes(byte_array, byte_array_size);

    size_t array_size = byte_array_size / sizeof(uint64_t);

    printf("array size: %ld - %ld\n", array_size, byte_array_size);

    uint64_t* array = (uint64_t*)malloc(array_size * sizeof(uint64_t));
    if (array == NULL) return NULL;

    for (size_t i = 0; /*i < array_size*/ i * sizeof(uint64_t) < byte_array_size; i++) {
        // Copy the byte array element to the array element
        memcpy(&array[i], byte_array + i * sizeof(uint64_t), sizeof(uint64_t));

        // Convert the array element to host byte order
        array[i] = ntohll(array[i]);
    }
    return array;
}

void print_bytes(const uint8_t *byte_array, size_t length) {
    for (size_t i = 0; i < length; i++) {
        printf("%02x ", byte_array[i]);
    }
    printf("\n");
}

// ---- ----

// not optimised for large numbers
uint64_t is_prime(uint64_t number) {
    if (number == 2 || number == 3)
        return 1;
    if (number < 2 || number % 2 == 0)
        return 0;

    for (uint64_t i = 0; i < 10; i++) {
        uint64_t a = rand() % (number - 3) + 2;
        if (!miller_rabin(number, a))
            return 0;
    }
    return 1;
}

uint64_t miller_rabin(uint64_t n, uint64_t a) {
    uint64_t r = 0, d = n - 1;
    while (d % 2 == 0) {
        r++;
        d /= 2;
    }
    uint64_t x = powmod(a, d, n);
    if (x == 1 || x == n - 1)
        return 1;

    for (uint64_t i = 0; i < r - 1; i++) {
        x = powmod(x, 2, n);
        if (x == n - 1)
            return 1;
    }

    return 0;
}

uint64_t generate_prime(void) {
    uint64_t p;
    do {
        // Generate a random number between 1000 and 1999
        p = rand() % 1000 + 1000;
    } while (!is_prime(p));
    return p;
}

// Function to find the greatest common divisor of two integers
uint64_t gcd(uint64_t a, uint64_t b) {
    uint64_t temp;
    while(b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to find the modular inverse of a number
uint64_t mod_inverse(uint64_t a, uint64_t m) {
    uint64_t m0 = m, t, q;
    int64_t x0 = 0, x1 = 1;

    if (m == 1)
        return 0;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0)
        x1 += m0;

    return x1;
}

// Function to calculate the modular exponentiation of a number
uint64_t mod_exp(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp = exp / 2;
    }
    return result;
}

//unsigned long long powmod(unsigned long long base, unsigned long long exponent, unsigned long long modulus);
uint64_t powmod(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t result = 1;

    while (e > 0) {
        if (e & 1) {
            result = (result * b) % m;
        }
        e >>= 1;
        b = (b * b) % m;
    }

    return result;
}

void rsa_to_bytes(t_public_key *public_key, uint8_t *byte_array) {
    // Convert the N and e values to network byte order
    uint64_t n_network = htonll(public_key->N);
    uint64_t e_network = htonll(public_key->e);

    // Copy the N and e values to the byte array
    memcpy(byte_array, &n_network, sizeof(uint64_t));
    memcpy(byte_array + sizeof(uint64_t), &e_network, sizeof(uint64_t));
}

/**
 * @brief Converts a 64-bit integer from host byte order to network byte order.
 * @note htonll: "host to network long long"
 * 
 * @param x             64-bit integer host byte order
 * @return uint64_t     64-bit integer network byte order
 */
uint64_t htonll(uint64_t x)
{
    uint64_t result = 0;
    result |= (x & 0x00000000000000ff) << 56;
    result |= (x & 0x000000000000ff00) << 40;
    result |= (x & 0x0000000000ff0000) << 24;
    result |= (x & 0x00000000ff000000) << 8;
    result |= (x & 0x000000ff00000000) >> 8;
    result |= (x & 0x0000ff0000000000) >> 24;
    result |= (x & 0x00ff000000000000) >> 40;
    result |= (x & 0xff00000000000000) >> 56;
    return result;
}

/**
 * @brief Converts a 64-bit integer from network byte order to host byte order.
 * @note ntohll: "network to host long long"
 * 
 * @param x             64-bit integer network byte order
 * @return uint64_t     64-bit integer host byte order
 */
uint64_t ntohll(uint64_t x)
{
    uint64_t result = 0;
    for (int i = 0; i < 8; i++) {
        result |= ((x >> (8 * i)) & 0xff) << (56 - 8 * i);
    }
    return result;
}
