#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bignumber.h"

char *readline(FILE* fp) {
    int size = 16; // Устанавливает минимальный размер строки до 16 байт
    char *str = malloc(sizeof(char) * size);
    if (str == NULL) {
        printf("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }
    int c;
    size_t len = 0;
    while ((c = fgetc(fp)) && c != '\r' && c != '\n') {
        *(str + len) = c;
        ++len;
        if (len == size) {
            // Необходимо выделить больше памяти
            size = size + size / 2;
            str = (char *)realloc(str, sizeof(str) * size);
            if (str == NULL) {
                printf("Unable to allocate memory\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    *(str + len) = '\n';
    *(str + len + 1) = '\0';
    return str;
}
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("No file specified\n");
        exit(EXIT_FAILURE);
    }

    FILE* fp;
    if ((fp = fopen(argv[1], "r")) == NULL) {
        printf("Couldn't open file \"%s\".\n", argv[1]);
        exit(EXIT_FAILURE);
    }


    char* aString;
    char* bString;
    char operation;
    printf("Enter a = ");
    aString = readline(fp);
    printf("Enter b = ");
    bString = readline(fp);
    printf("\nEnter an operation\n");
    operation = fgetc(fp);
    bigNum a = BigNum(aString);
    bigNum b = BigNum(bString);
    printf("a = ");
    print(&a);
    printf("b = ");
    print(&b);
    if (operation == '+') {
        bigNum aSumB = sum(&a, &b);
        printf("a + b = ");
        print(&aSumB);
        DelBigNum(&aSumB);
    }
    if (operation == '-') {
        bigNum aSubB = sub(&a, &b);
        printf("a - b = ");
        print(&aSubB);
        DelBigNum(&aSubB);
    }
    if (operation == '*') {
        bigNum aMulB = mul(&a, &b);
        printf("a * b = ");
        print(&aMulB);
        DelBigNum(&aMulB);
    }
    if (operation == '/') {
        bigNum aDivB = bigNum_div(&a, &b);
        printf("a / b = ");
        print(&aDivB);
        DelBigNum(&aDivB);
    }
    free(aString);
    free(bString);
    DelBigNum(&a);
    DelBigNum(&b);
    return 0;
}
