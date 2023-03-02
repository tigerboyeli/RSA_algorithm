#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BASE 10
#define BITS_PER_DIGIT 4
#define MAX_LENGTH 1000

typedef struct {
    int sign;
    int size;
    int *tab;
} bignum;

bignum str2bignum(char *str)
{
    int len = strlen(str);
    bignum result;
    result.sign = 1;
    result.size = len;
    result.tab = malloc(len * sizeof(int));

    int i;
    for (i = 0; i < len; i++)
    {
        result.tab[i] = str[len - i - 1] - '0';
    }

    return result;
}

bignum add(bignum a, bignum b)
{
    bignum result;
    int size = (a.size > b.size) ? a.size : b.size;
    result.tab = malloc(size * sizeof(int));
    int carry = 0;
    int i;
    for (i = 0; i < size; i++)
    {
        int digit = carry;
        if (i < a.size)
        {
            digit += a.tab[i];
        }
        if (i < b.size)
        {
            digit += b.tab[i];
        }
        result.tab[i] = digit % BASE;
        carry = digit / BASE;
    }
    if (carry)
    {
        result.tab = realloc(result.tab, (size + 1) * sizeof(int));
        result.tab[size] = carry;
        size++;
    }
    result.size = size;
    result.sign = 1;
    return result;
}

bignum sub(bignum a, bignum b)
{
    bignum result;
    int size = (a.size > b.size) ? a.size : b.size;
    result.tab = malloc(size * sizeof(int));
    int borrow = 0;
    int i;
    for (i = 0; i < size; i++)
    {
        int digit = a.tab[i] - borrow;
        if (i < b.size)
        {
            digit -= b.tab[i];
        }
        if (digit < 0)
        {
            digit += BASE;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }
        result.tab[i] = digit;
    }
    while (size > 1 && !result.tab[size - 1])
    {
        size--;
    }
    result.size = size;
    result.sign = (size == 1 && !result.tab[0]) ? 1 : (a.sign == b.sign ? 1 : -1);
    return result;
}

bignum mult(bignum a, bignum b) {
  bignum result;
  result.sign = a.sign * b.sign;
  result.size = a.size + b.size;

  result.tab = malloc(result.size * sizeof(int));
  if (result.tab == NULL) {
    printf("%s", "ERROR_MESSAGE");
    exit(1);
  }

  for (int i = 0; i < result.size; i++) {
    result.tab[i] = 0;
  }

  for (int i = 0; i < a.size; i++) {
    for (int j = 0; j < b.size; j++) {
      result.tab[i + j] += a.tab[i] * b.tab[j];
    }
  }

  // Perform the carry operation on each digit
  for (int i = 0; i < result.size - 1; i++) {
    result.tab[i + 1] += result.tab[i] / 10;
    result.tab[i] = result.tab[i] % 10;
  }

  // Remove the leading zeros
  while (result.tab[result.size - 1] == 0 && result.size > 1) {
    result.size--;
  }

  return result;
}



bignum remainder(bignum a, bignum b) {
  // Inefficient algorithm for finding the remainder
  bignum result;
  result.sign = 1;
  result.size = a.size;
  result.tab = (int *) malloc(result.size * sizeof(int));
  
  int carry = 0;
  for (int i = a.size - 1; i >= 0; i--) {
    carry = carry * BASE + a.tab[i];
    result.tab[i] = carry / b.tab[0];
    carry = carry % b.tab[0];
  }
  
  return result;
}

// Additional functions can be implemented in a similar manner.


bignum addmod(bignum a, bignum b, bignum n) {
    bignum res = {0};
    res.size = (a.size > b.size) ? a.size + 1 : b.size + 1;
    res.tab = malloc(res.size * sizeof(int));
    memset(res.tab, 0, res.size * sizeof(int));
    for (int i = 0; i < res.size - 1; i++) {
        res.tab[i] += (i < a.size ? a.tab[i] : 0) + (i < b.size ? b.tab[i] : 0);
        res.tab[i + 1] = res.tab[i] / BASE;
        res.tab[i] %= BASE;
    }
    while (res.size > 1 && res.tab[res.size - 1] == 0) {
        res.size--;
    }
    return res;
}

bignum multmod(bignum a, bignum b, bignum n) {
    bignum res = {0};
    res.size = a.size + b.size;
    res.tab = malloc(res.size * sizeof(int));
    memset(res.tab, 0, res.size * sizeof(int));
    for (int i = 0; i < a.size; i++) {
        for (int j = 0; j < b.size; j++) {
            res.tab[i + j] += a.tab[i] * b.tab[j];
            res.tab[i + j + 1] += res.tab[i + j] / BASE;
            res.tab[i + j] %= BASE;
        }
    }
    while (res.size > 1 && res.tab[res.size - 1] == 0) {
        res.size--;
    }
    return res;
}





bignum expmod(bignum a, bignum b, bignum n) {
  bignum result;
  result.sign = 1;
  result.size = 1;
  result.tab = (int *) malloc(result.size * sizeof(int));
  result.tab[0] = 1;
  while (b.size > 1 || b.tab[0] > 0) {
    if (b.tab[0] & 1) {
      result = multmod(result, a, n);
    }
    b = div2(b);
    a = multmod(a, a, n);
  }
  return result;
}

int millerrabin(bignum a, int t) {
  bignum n_minus_1 = sub(a, 1);
  bignum d = n_minus_1;
  int s = 0;
  while (d.tab[0] % 2 == 0) {
    d = div2(d);
    s++;
  }
  for (int i = 0; i < t; i++) {
    bignum x = expmod(rand() % (a.tab[0] - 1) + 1, d, a);
    if (x.tab[0] == 1 || x.tab[0] == n_minus_1.tab[0]) {
      continue;
    }
    for (int j = 0; j < s - 1; j++) {
      x = multmod(x, x, a);
      if (x.tab[0] == n_minus_1.tab[0]) {
        break;
      }
    }
    if (x.tab[0] != n_minus_1.tab[0]) {
      return 0;
    }
  }
  return 1;
}

bignum genrandom(int length) {
  int size = length / BITS_PER_DIGIT + 1;
  bignum result;
  result.sign = 1;
  result.size = size;
  result.tab = (int *) malloc(result.size * sizeof(int));
  for (int i = 0; i < size; i++) {
    result.tab[i] = rand() % BASE;
  }
  return result;
}

bignum genrandomprime(int length) {
  bignum result = genrandom(length);
  while (!millerrabin(result, 5)) {
    result = add(result, 1);
  }
  return result;
}


