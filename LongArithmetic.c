#include <stdio.h>
#include <stdlib.h>

#include "bignumber.h"

const int BASE = 1e4;
const int BASE_LENGTH = 4;
const char *digitFormat = "%.4d";   // format output

bool isNumber (char *string, int strLength) // Проверяет строку перед преобразованием в bigNumber
{
    if (strLength == 0)
    {
        return false;
    }
    if (isdigit(string[0]) == false)   // Проверяет потенциальные минусы
    {
        if (string[0] != '-' || strLength == 1)   // '-' не является числом
        {
            return false;
        }
    }

    int i;
    for (i = 1; i < strLength; ++i)
    {
        if (isdigit(string[i]) == false)
        {
            return false;
        }
    }
    return true;
}

void print(const bigNum* bigOne)
{
    if (bigOne -> amount == 0)   // Пустые переменные имеют сумму .amount = 0
    {
        printf("I can't to print it!\n");
        return;
    }
    int i;
    if (bigOne -> isNegative == true)
    {
        printf("-");
    }
    printf("%d", bigOne -> digits[bigOne -> amount - 1]);
    for (i = bigOne -> amount - 2; i >= 0; --i)
    {
        printf(digitFormat, bigOne -> digits[i]);
    }
    printf("\n");
}

bigNum BigNum (char *digitsString)  // Constructor
{
    bigNum datBig;   // Новое число bigNum
    int digitsLen = strlen(digitsString) - 1;   // digitsString содержит '\n' в конце так, что digitsLen = strlen() - 1


    if (digitsString[0] == '-')  // Контрольный знак
    {
        datBig.isNegative = digitsString[1] != 0;
        digitsString++;
        digitsLen--;
    }
    else
    {
        datBig.isNegative = false;
    }

    int i, pos = 0, decPoint = 1;      // Преобразует текущие символы в .digits[i]
    datBig.digits = (int *) malloc(sizeof(int) * (digitsLen / BASE_LENGTH + 5));    // Выделяет память на массив цифр
    if (datBig.digits == NULL)
    {
        printf("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }
    datBig.digits[pos] = 0;

    int newDigit = (digitsLen - 1) % BASE_LENGTH;
    for (i = digitsLen - 1; i >= 0; --i)
    {
        if (i % BASE_LENGTH == newDigit && i != digitsLen - 1)   // Следующая цифра
        {
            ++pos;
            datBig.digits[pos] = 0;
            decPoint = 1;
        }
        if (isdigit(digitsString[i]))
        {
            if (!isNumber(digitsString, digitsLen))
            {
                if (digitsLen != -1) // Пустые числа bigNum создаются с использованием "" в виде digitsString
                {
                    printf("This isn't a number!\n");
                }
                datBig.amount = 0;
                datBig.isNegative = false;
                datBig.digits = (int *) malloc(sizeof(int));
                return datBig;
            }
            datBig.digits[pos] = datBig.digits[pos] + (digitsString[i] - '0') * decPoint;
            decPoint*= 10;
        }
    }

    int newAmount = pos + 1;    // Стирает ведущие нули
    while (newAmount > 1 && datBig.digits[newAmount - 1] == 0)
    {
        --newAmount;
    }
    datBig.amount = newAmount;
    return datBig;
}


bool correctbigNum (const bigNum *a)
{
    if (a -> amount <= 0)
    {
        return false;
    }
    if (a -> amount > 1 && a -> digits[a -> amount - 1] == 0)
    {
        return false;
    }
    return true;
}

bool more (const bigNum *a, const bigNum *b)       // a > b
{
    if (!correctbigNum(a) || !correctbigNum(b))
    {
        printf("Invalid bigNum number used");
        exit(EXIT_FAILURE);
    }

    _Bool bothNegative = false;

    if (!(a -> isNegative) && b -> isNegative)
    {
        return true;
    }
    if (a -> isNegative && !(b -> isNegative))
    {
        return false;
    }
    if (a -> isNegative && b -> isNegative)
    {
        bothNegative = true;
    }

    if (a -> amount != b -> amount)
    {
        return (a -> amount > b -> amount) ^ bothNegative;
    }

    int i;
    for (i = a -> amount - 1; i >= 0; --i)
    {
        if (a -> digits[i] != b -> digits[i])
        {
            return (a -> digits[i] > b -> digits[i]) ^ bothNegative;
        }
    }
    return false;
}

bool less (const bigNum *a, const bigNum *b)       // a < b
{
    if (!correctbigNum(a) || !correctbigNum(b))
    {
        printf("Invalid bigNum number used");
        exit(EXIT_FAILURE);
    }

    _Bool bothNegative = false;

    if (!(a -> isNegative) && b -> isNegative)
    {
        return false;
    }
    if (a -> isNegative && !(b -> isNegative))
    {
        return true;
    }
    if (a -> isNegative && b -> isNegative)
    {
        bothNegative = true;
    }

    if (a -> amount != b -> amount)
    {
        return (a -> amount < b -> amount) ^ bothNegative;
    }

    int i;
    for (i = a -> amount - 1; i >= 0; --i)
    {
        if (a -> digits[i] != b -> digits[i])
        {
            return (a -> digits[i] < b -> digits[i]) ^ bothNegative;
        }
    }
    return false;
}

bool equal (const bigNum *a, const bigNum *b)
{
    if (!correctbigNum(a) || !correctbigNum(b))
    {
        printf("Invalid bigNum number used");
        exit(EXIT_FAILURE);
    }

    if (a -> isNegative ^ b -> isNegative)
    {
        return false;
    }

    if (a -> amount != b -> amount)
    {
        return false;
    }

    int i;
    for (i = a -> amount - 1; i >= 0; --i)
    {
        if (a -> digits[i] != b -> digits[i])
        {
            return false;
        }
    }
    return true;
}

bigNum bigNum_abs (const bigNum *a)
{
    if (!correctbigNum(a))
    {
        printf("Invalid bigNum number used");
        exit(EXIT_FAILURE);
    }

    bigNum newBig;
    newBig.digits = (int *) malloc(sizeof(int) * a -> amount);
    if (newBig.digits == NULL)
    {
        printf("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }
    int i;
    for (i = 0; i < a -> amount; ++i)     // Создаёт копию  a
    {
        newBig.digits[i] = a -> digits[i];
    }
    newBig.amount = a -> amount;
    newBig.isNegative = false;
    return newBig;
}

bigNum sum (const bigNum *a, const bigNum *b)      // a + b
    {
    if (!correctbigNum(a) || !correctbigNum(b))
    {
        printf("Invalid bigNum number used");
        exit(EXIT_FAILURE);
    }

    bigNum newBig = BigNum("");

    if (a -> isNegative && b -> isNegative)   //-|a| + - |b| <=> -|a + b|
    {
        newBig.isNegative = true;
    }
    else if (a -> isNegative)      // -|a| + b <=> b - |a|
    {
        bigNum absA = bigNum_abs(a);
        newBig = sub(b, &absA);
        DelBigNum(&absA);
        return newBig;
    }
    else if (b -> isNegative)     // a + (-|b|) <=> a - |b|
    {
        bigNum absB = bigNum_abs(b);
        newBig = sub(a, &absB);
        DelBigNum(&absB);
        return newBig;
    }
    else   // |a| + |b|
    {
        newBig.isNegative = false;
    }

    int i, r = 0;   //Здесь и далее r - остаток от деления на BASE значений, которые больше, чем BASE
    if (a -> amount > b -> amount)
    {
        newBig.amount = a -> amount;
    }
    else {
        newBig.amount = b -> amount;
    }

    newBig.digits = (int *) calloc(newBig.amount + 2, sizeof(int));
    if (newBig.digits == NULL)
    {
        printf("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < newBig.amount || r != 0; ++i)
    {
        newBig.digits[i] = r;
        if (i < a -> amount)
        {
            newBig.digits[i] += a -> digits[i];
        }
        if (i < b -> amount)
        {
            newBig.digits[i] += b -> digits[i];
        }

        if (newBig.digits[i] >= BASE)
        {
            newBig.digits[i]-= BASE;
            r = 1;
        }
        else {
            r = 0;
        }
    }
    if (newBig.digits[newBig.amount] != 0)
    {
        newBig.amount++;
    }
    return newBig;
}

bigNum sub (const bigNum *a, const bigNum *b)     // a - b
    {
    if (!correctbigNum(a) || !correctbigNum(b))
    {
        printf("Invalid bigNum number used");
        exit(EXIT_FAILURE);
    }

    bigNum newBig = BigNum("");

    if (a -> isNegative && b -> isNegative)   // -|a| - (-|b|) <=> |b| - |a|
    {
        bigNum absA = bigNum_abs(a);
        bigNum absB = bigNum_abs(b);
        newBig = sub(&absB, &absA);
        DelBigNum(&absA);
        DelBigNum(&absB);
        return newBig;
    }
    else if (a -> isNegative)      // -|a| - |b| <=> -(|a| + |b|)
    {
        bigNum absA = bigNum_abs(a);
        newBig = sum(&absA, b);
        DelBigNum(&absA);
        newBig.isNegative = true;
        return newBig;
    }
    else if (b -> isNegative)  // a - (-|b|) <=> a + |b|
    {
        bigNum absB = bigNum_abs(b);
        newBig = sum(a, &absB);
        DelBigNum(&absB);
        return newBig;
    }
    else   // just |a| - |b|
    {
        if (more(a, b) || equal(a, b))
        {
            newBig.isNegative = false;
        }
        else
        {
            newBig = sub(b, a);
            newBig.isNegative = true;
            return newBig;
        }
    }

    int i;
    newBig.amount = a -> amount;
    newBig.digits = (int *) realloc(newBig.digits, sizeof(int) * newBig.amount + 2);
    if (newBig.digits == NULL)
    {
        printf("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < newBig.amount; ++i)     // Создаёт копию a
    {
        newBig.digits[i] = a -> digits[i];
    }
    newBig.digits[newBig.amount] = 0;
    newBig.digits[newBig.amount + 1] = 0;

    for (i = 0; i < newBig.amount; ++i)
    {
        if (i < b -> amount)
        {
            newBig.digits[i] -= b -> digits[i];
        }
        if (newBig.digits[i] < 0)
        {
            newBig.digits[i]+= BASE;
            --newBig.digits[i + 1];
        }

    }

    int amount = newBig.amount;
    while (amount && newBig.digits[amount] == 0)
    {
        amount--;
    }
    newBig.amount = amount + 1;

    return newBig;
}

bigNum mul (const bigNum *a, const bigNum *b)      // a * b
{
    if (!correctbigNum(a) || !correctbigNum(b))
    {
        printf("Invalid bigNum number used");
        exit(EXIT_FAILURE);
    }

    bigNum newBig = BigNum("");

    if (a -> isNegative ^ b -> isNegative)    // Меняет знак
    {
        if (!(a -> amount == 1 && a -> digits[0] == 0) && !(b -> amount == 1 && b -> digits[0] == 0))   // != 0
        {
            newBig.isNegative = true;
        }
    }

    newBig.digits = (int *) calloc (a -> amount + b -> amount + 2, sizeof(int));
    if (newBig.digits == NULL)
    {
        printf("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }

    int i, j;
    for (i = 0; i < a -> amount; ++i)
    {
        int r = 0;
        for (j = 0; j < b -> amount || r != 0; ++j)
        {
            if (j >= b -> amount)
            {
                 newBig.digits[i + j] += r;
            }
            else
            {
                newBig.digits[i + j] += a -> digits[i] * b -> digits[j] + r;
            }
            r = newBig.digits[i + j] / BASE;
            newBig.digits[i + j] -= r * BASE;
        }
    }
    int amount = a -> amount + b -> amount;
    while (amount > 0 && newBig.digits[amount] == 0)
    {
        amount--;
    }
    newBig.amount = amount + 1;
    return newBig;
}

bigNum bigNum_div (const bigNum *a, const bigNum *b)    // a / b
{
    if (!correctbigNum(a) || !correctbigNum(b))
    {
        printf("Invalid bigNum number used");
        exit(EXIT_FAILURE);
    }

    bigNum newBig = BigNum("");

    if (b -> amount == 1 && b -> digits[0] == 0)   // Деление на нуль
    {
        printf("Division by zero");
        exit(EXIT_FAILURE);
    }
    if (a -> isNegative ^ b -> isNegative)    // Меняет знак
    {
        if (!(a -> amount == 1 && a -> digits[0] == 0))  // != 0
        {
            newBig.isNegative = true;
        }
    }

    bigNum absA = bigNum_abs(a);
    bigNum absB = bigNum_abs(b);
    if (less(&absA, &absB))
    {
        newBig.amount = 1;
        newBig.digits[0] = 0;
        return newBig;
    }
    DelBigNum(&absA);
    DelBigNum(&absB);

    newBig.digits = (int *) calloc(a -> amount + 1, sizeof(int));
    if (newBig.digits == NULL)
    {
        printf("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }

    int *currValue;  // Часть числа a, которая удовлетворяет условию BASE * b >= currValue
    currValue = (int *) calloc(a -> amount + 1, sizeof(int));
    if (currValue == NULL)
    {
        printf("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }
    int i;
    for (i = a -> amount - 1; i >= 0; --i)
    {
        int j;
        for (j = a -> amount; j >= 1; --j)  // currValue *= BASE
        {
            currValue[j] = currValue[j - 1];
        }
        currValue[0] = a -> digits[i];

        int leftBound = 0, rightBound = BASE; // Двоичный поиск max(x) такой, что b * x <= currValue
        int x = 0, retX = 0;
        int *bMulX1, *bMulX2;
        int **currMul, **retMul;
        bMulX1 = (int *) calloc (b -> amount + 1, sizeof(int));
        bMulX2 = (int *) calloc (b -> amount + 1, sizeof(int));
        if (bMulX1 == NULL || bMulX2 == NULL)
        {
            printf("Unable to allocate memory");
            exit(EXIT_FAILURE);
        }
        currMul = &bMulX1;
        retMul = &bMulX2;

        while (leftBound <= rightBound)
        {
            x = (leftBound + rightBound) >> 1;
            int r = 0;               // Умножение
            for (j = 0; j < b -> amount; ++j)
            {
                (*currMul)[j] = b -> digits[j] * x + r;
                r = (*currMul)[j] / BASE;
                (*currMul)[j] %= BASE;
            }
           (*currMul)[j] = r;

            _Bool compare = true; // b * x <= currValue
            for (j = b -> amount; j >= 0; --j)     // Сравнение
            {
                if ((*currMul)[j] < currValue[j])
                {
                    break;
                }
                else if ((*currMul)[j] > currValue[j])
                {
                    compare = false;
                    break;
                }
            }
            if (compare)
            {
                retX = x;
                int *temp = *currMul; // замена currMul и retMul
                *currMul = *retMul;
                *retMul = temp;
                leftBound = x + 1;
            }
            else
            {
                rightBound = x - 1;
            }
        }
        newBig.digits[i] = retX;
        for (j = 0; j <= b -> amount; ++j)     // currValue -= bMulX;
        {
            currValue[j] -= (*retMul)[j];
            if (currValue[j] < 0)
            {
                currValue[j] += BASE;
                --currValue[j + 1];
            }

        }
        free(bMulX1);
        free(bMulX2);
    }
    free(currValue);

    int amount = a -> amount;
    while (amount >= 0 && newBig.digits[amount] == 0)
    {
        amount--;
    }
    newBig.amount = amount + 1;
    return newBig;
}

void DelBigNum(bigNum *a)
{
    a -> digits = (int *) realloc(NULL, sizeof(int));
    free(a -> digits);
}
