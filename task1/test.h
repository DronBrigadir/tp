//
// Created by andrey on 01.10.2019.
//

#ifndef TASK1_TEST_H
#define TASK1_TEST_H

#define TEST_FILE_PATH "/home/andrey/tp/task1/test_file.txt"
#define MAX_BUFFER 1024
#define NUMBER_OF_ARGS 7
#define WRONG_INPUT -1
#define MATR_CANNOT_BE_MULTIPLIED -2

#include <stdio.h>
#include "matrix.h"

void test(FILE *in);
int parser(char *test_name, Matrix *a, Matrix *b, char *output, char *input);
int checkStrOutput (char *result, char *output);
int checkMatrOutput (Matrix *result, char *output);

#endif //TASK1_TEST_H
