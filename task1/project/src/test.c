//
// Created by andrey on 01.10.2019.
//

#include <malloc.h>
#include <string.h>
#include "test.h"
#include "matrix.h"
#include "utils.h"

void test(FILE *in) {
    char str[MAX_BUFFER];

    while (fgets(str, MAX_BUFFER, in)) {
        Matrix *a, *b, *answer;
        char *test_name;

        int parser_return_value = parser(&test_name, &a, &b, &answer, str);

        if (parser_return_value == WRONG_INPUT) {
            printf("Wrong input\n");
            return;
        } else if (parser_return_value == MEM_ALLOC_ERR) {
            printf("Memory allocation error\n");
            return;
        } else {
            Matrix *result;
            int mult_return_value = matrix_multiplication(a, b, &result);

            free_matr(a);
            free_matr(b);

            if (mult_return_value == MEM_ALLOC_ERR) {
                if (parser_return_value != MATR_CANNOT_BE_MULTIPLIED) {
                    free_matr(answer);
                }
                printf("Memory allocation error\n");
                return;
            }

            if (parser_return_value == MATR_CANNOT_BE_MULTIPLIED) {
                if (mult_return_value == MATR_CANNOT_BE_MULTIPLIED) {
                    printf("%s passed\n", test_name);
                } else if (mult_return_value == DEFAULT) {
                    printf("%s failed\n", test_name);
                    free_matr(result);
                }
            } else if (parser_return_value == DEFAULT) {
                if (mult_return_value == MATR_CANNOT_BE_MULTIPLIED) {
                    printf("%s failed\n", test_name);
                } else if (mult_return_value == DEFAULT) {
                    int equal_return_value = is_matrix_equal(result, answer);
                    free_matr(result);

                    if (equal_return_value == WRONG_RESULT) {
                        printf("%s failed\n", test_name);
                    } else if (equal_return_value == DEFAULT) {
                        printf("%s passed\n", test_name);
                    }
                }

                free_matr(answer);
            }
        }
    }

    if (feof(in) == 0) {
        printf("Error reading from file\n");
    }
}

int parser(char **test_name, Matrix **a, Matrix **b, Matrix **answer, char *input) {
    char *separator = " |";

    // Получение наименования теста
    (*test_name) = strtok(input, separator);
    if (!(*test_name)) {
        return WRONG_INPUT;
    }

    // Получение количества строк матрицы А
    int row_a;
    char *tmp;
    tmp = strtok(NULL, separator);
    if (str_to_int(tmp, &row_a) == WRONG_INPUT) {
        return WRONG_INPUT;
    }

    // Получение количества столбцов матрицы А
    int col_a;
    tmp = strtok(NULL, separator);
    if (str_to_int(tmp, &col_a) == WRONG_INPUT) {
        return WRONG_INPUT;
    }

    // Выделение памяти для матрицы А
    if (init_matr(a, row_a, col_a) == MEM_ALLOC_ERR) {
        return MEM_ALLOC_ERR;
    }

    // Получение данных для матрицы А
    for (size_t i = 0; i < (*a)->num_row; i++) {
        for (size_t j = 0; j < (*a)->num_col; j++) {
            tmp = strtok(NULL, separator);
            if (str_to_int(tmp, &(*a)->data[i][j]) == WRONG_INPUT) {
                free_matr(*a);
                return WRONG_INPUT;
            }
        }
    }

    // Получение количества строк для матрицы В
    int row_b;
    tmp = strtok(NULL, separator);
    if (str_to_int(tmp, &row_b) == WRONG_INPUT) {
        free_matr(*a);
        return WRONG_INPUT;
    }

    // Получение количества столбцов для матрицы В
    int col_b;
    tmp = strtok(NULL, separator);
    if (str_to_int(tmp, &col_b) == WRONG_INPUT) {
        free_matr(*a);
        return WRONG_INPUT;
    }

    // Выделение памяти для матрицы В
    if (init_matr(b, row_b, col_b) == MEM_ALLOC_ERR) {
        free_matr(*a);
        return MEM_ALLOC_ERR;
    }

    // Получение данных для матрицы В
    for (size_t i = 0; i < (*b)->num_row; i++) {
        for (size_t j = 0; j < (*b)->num_col; j++) {
            tmp = strtok(NULL, separator);
            if (str_to_int(tmp, &(*b)->data[i][j]) == WRONG_INPUT) {
                free_matr(*a);
                free_matr(*b);
                return WRONG_INPUT;
            }
        }
    }

    // Получение количества строк матрицы ответа
    int row_answer;
    tmp = strtok(NULL, separator);
    if (str_to_int(tmp, &row_answer) == WRONG_INPUT) {
        free_matr(*a);
        free_matr(*b);
        return WRONG_INPUT;
    }

    if (row_answer == 0) {
        return MATR_CANNOT_BE_MULTIPLIED;
    }

    // Получение количества столбцов матрицы ответа
    int col_answer;
    tmp = strtok(NULL, separator);
    if (str_to_int(tmp, &col_answer) == WRONG_INPUT) {
        free_matr(*a);
        free_matr(*b);
        return WRONG_INPUT;
    }

    // Выделение памяти для матрицы ответа
    if (init_matr(answer, row_answer, col_answer) == MEM_ALLOC_ERR) {
        free_matr(*a);
        free_matr(*b);
        return MEM_ALLOC_ERR;
    }

    // Получение данных для матрицы ответа
    for (size_t i = 0; i < (*answer)->num_row; i++) {
        for (size_t j = 0; j < (*answer)->num_col; j++) {
            tmp = strtok(NULL, separator);
            if (str_to_int(tmp, &(*answer)->data[i][j]) == WRONG_INPUT) {
                free_matr(*a);
                free_matr(*b);
                free_matr(*answer);
                return WRONG_INPUT;
            }
        }
    }

    return DEFAULT;
}