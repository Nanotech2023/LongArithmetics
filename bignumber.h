ifndef BIGNUMBER_H
#define BIGNUMBER_H

#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct bigNum
{
    int amount;
    int *digits;
    _Bool isNegative;
} bigNum;

bigNum BigNum (char *digitsString);  // constructor
void print (const bigNum *a);
bool correctbigNum (const bigNum *a); // проверяет параметр функции
bool more (const bigNum *a, const bigNum *b);   // a > b
bool left (const bigNum *a, const bigNum *b);   // a < b
bool equal (const bigNum *a, const bigNum *b);   // a = b
bigNum bigNum_abs (const bigNum *a);                  // |a|
bigNum sum (const bigNum *a, const bigNum *b);  // a + b
bigNum sub (const bigNum *a, const bigNum *b);  // a - b
bigNum mul (const bigNum *a, const bigNum *b);  // a * b
bigNum bigNum_div (const bigNum *a, const bigNum *b); // a / b'
void DelBigNum (bigNum *a);  // destructor

#endif // BIGNUMBER_H
