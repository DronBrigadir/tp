//
// Created by andrey on 01.10.2019.
//

#include <malloc.h>
#include "test.h"
#include "matrix.h"

void test(FILE *in) {
    char str[MAX_BUFFER];

    while (fgets(str, MAX_BUFFER, in) != NULL) {
        printf("%s", str);

        Matrix *a;
        Matrix *b;
        char *test_name;
        char *output;

        if (parser(test_name, a, b, output, str) == WRONG_INPUT) {
            printf("Wrong input\n");
            return;
        } else {
            Matrix *result_matr;
            if (matrix_multiplication(a, b, result_matr) == MATR_CANNOT_BE_MULTIPLIED) {
                if (checkStrOutput("Matrix cannot be multiplied", output)) {
                    printf("%s passed\n", test_name);
                } else {
                    printf("%s failed\n", test_name);
                }
            } else {
                if (checkMatrOutput(result_matr, output)) {
                    printf("%s passed\n", test_name);
                } else {
                    printf("%s failed\n", test_name);
                }
            }
        }
    }

    if (feof(in) != 0) {
        printf("Tests passed\n");
    } else {
        printf("Error reading from file\n");
    }
}

int parser(char *test_name, Matrix *a, Matrix *b, char *output, char *input) {
}

int checkStrOutput (char *result, char *output) {
}

int checkMatrOutput (Matrix *result, char *output) {
}