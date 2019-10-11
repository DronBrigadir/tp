//
// Created by andrey on 01.10.2019.
//

#ifndef TASK1_PROJECT_INCLUDE_TEST_H_
#define TASK1_PROJECT_INCLUDE_TEST_H_

#include <stdio.h>
#include "matrix.h"

void test(FILE *in);
int parser(char **test_name, Matrix **a, Matrix **b, Matrix **answer, char *input);
int get_matrix_data(Matrix **matr, char **string_to_parse);
int check_answer(Matrix **l, Matrix**r, Matrix **answer, int parser_return_value);

#endif  // TASK1_PROJECT_INCLUDE_TEST_H_
