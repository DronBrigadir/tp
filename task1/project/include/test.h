//
// Created by andrey on 01.10.2019.
//

#ifndef TASK1_TEST_H
#define TASK1_TEST_H

#include <stdio.h>
#include "matrix.h"

void test(FILE *in);
int parser(char **test_name, Matrix **a, Matrix **b, char **output, char *input);
int check_str_output (char *result, char *output);
int check_matr_output (Matrix *result, char *output);

#endif //TASK1_TEST_H