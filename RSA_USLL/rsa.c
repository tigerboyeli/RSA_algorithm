#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

// Greatest Common Divisor
int gcd(int a, int b)
{
    int temp;
    while (b != 0)
    {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to calculate (a ^ b) % c
unsigned long long modulo(unsigned long long a, unsigned long long b, unsigned long long c)
{
    unsigned long long x=1, y=a;
    while(b > 0)
    {
        if(b%2 == 1)
        {
            x=(x*y)%c;
        }
        y = (y*y)%c;
        b /= 2;
    }
    return x%c;
}

// Returns true if n is prime
int isPrime(unsigned long long n)
{
    if (n <= 1 || n == 4) return 0;
    if (n <= 3) return 1;
    int i;
    for (i=2; i<=sqrt(n); i++)
    {
        if (n%i == 0)
            return 0;
    }
    return 1;
}

// Generates public and private keys
void generateKeys(unsigned long long *p, unsigned long long *q,unsigned long long  *e, unsigned long long *d, unsigned long long *n)
{
    // Generate two random prime numbers
   srand(time(NULL));
    *p = rand() % (ULONG_LONG_MAX - 100) + 100;
    while (!isPrime(*p))
    {
        *p = rand() % (ULONG_LONG_MAX - 100) + 100;
    }

    *q = rand() % (ULONG_LONG_MAX - 100) + 100;
    while (!isPrime(*q))
    {
        *q = rand() % (ULONG_LONG_MAX - 100) + 100;
    }

    // Calculate n
    *n = (*p) * (*q);

    // Calculate totient
    unsigned long long phi = (*p-1) * (*q-1);

    // Generate public key
    *e = 2;
    while (*e < phi)
    {
        if (gcd(*e, phi) == 1)
            break;
        else
            (*e)++;
    }

    // Generate private key
    *d = modulo(*e, 1, phi);
}

// Encrypts message
unsigned long long encryptMessage(unsigned long long message, int e, unsigned long long n)
{
    return modulo(message, e, n);
}

// Decrypts message
unsigned long long decryptMessage(unsigned long long ciphertext, unsigned long long d, unsigned long long n)
{
    return modulo(ciphertext, d, n);
}

int main()
{
    unsigned long long p, q, d, n;
    unsigned long long  e;

    // on Genere la cle public and prive
    generateKeys(&p, &q, &e, &d, &n);

    // j affiche toute les valeurs des cle et nombre premiers  pour pouvoir si on a le bon resultat
    printf("p %llu\n", p);
    printf("q %llu\n", q);
    printf("d %llu\n", d);
    printf("n %llu\n", n);
    printf("e %llu\n", e);


    unsigned long long message = 1234;
    printf("Message: %llu\n", message);
    

    // Encrypt the message (avec e et n qui constitue ensemble la cle public) 
    unsigned long long ciphertext = encryptMessage(message, e, n);
    printf("Ciphertext: %llu\n", ciphertext);

    // Decrypt the message (avec d et n qui constitue ensemble la cle prive) 
    unsigned long long decryptedMessage = decryptMessage(ciphertext, d, n);
    printf("Decrypted Message: %llu\n", decryptedMessage);
    
return 0;
}
