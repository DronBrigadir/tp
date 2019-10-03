//
// Created by andrey on 01.10.2019.
//

#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "test.h"
#include "matrix.h"
#include "utils.h"

void test(FILE *in) {
    char str[MAX_BUFFER];

    while (fgets(str, MAX_BUFFER, in)) {
        printf("%s", str);

        Matrix *a, *b;
        char *test_name, *output;

        int return_value = parser(&test_name, &a, &b, &output, str);
        if (return_value == WRONG_INPUT) {
            printf("Wrong input\n");
            return;
        } else if (return_value == MEM_ALLOC_ERR) {
            printf("Memory allocation error\n");
            return;
        } else if (return_value == DEFAULT) {
            Matrix *result_matr;
            return_value = matrix_multiplication(a, b, &result_matr);
            free_matr(a);
            free_matr(b);
            if (return_value == MEM_ALLOC_ERR) {
                printf("Memory allocation error\n");
                return;
            } else if (return_value == MATR_CANNOT_BE_MULTIPLIED) {
                if (check_str_output("Matrix_cannot_be_multiplied\n", output) == DEFAULT) {
                    printf("%s passed\n", test_name);
                } else {
                    printf("%s failed\n", test_name);
                }
            } else if (return_value == DEFAULT) {
                int answer = check_matr_output(result_matr, output);
                free_matr(result_matr);
                if (answer == MEM_ALLOC_ERR) {
                    printf("Memory allocation error\n");
                    return;
                } else if (answer == WRONG_INPUT) {
                    printf("Wrong input\n");
                    return;
                } else if (answer == WRONG_RESULT) {
                    printf("%s failed\n", test_name);
                } else if (answer == DEFAULT) {
                    printf("%s passed\n", test_name);
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

int parser(char **test_name, Matrix **a, Matrix **b, char **output, char *input) {
    char *separator = " ";

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

    // Получение данных для матрицы А TODO(): make function, that will read
    for (size_t i = 0; i < (*a)->num_row; i++) {
        for (size_t j = 0; j < (*a)->num_col; j++) {
            tmp = strtok(NULL, separator);
            if (tmp == NULL) {
                free_matr(*a);
                return WRONG_INPUT;
            } else {
                if (str_to_int(tmp, &(*a)->data[i][j]) == WRONG_INPUT) {
                    free_matr(*a);
                    return WRONG_INPUT;
                }
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
            if (tmp == NULL) {
                free_matr(*a);
                free_matr(*b);
                return WRONG_INPUT;
            } else {
                if (str_to_int(tmp, &(*b)->data[i][j]) == WRONG_INPUT) {
                    free_matr(*a);
                    free_matr(*b);
                    return WRONG_INPUT;
                }
            }
        }
    }

    // Получение ответа
    (*output) = strtok(NULL, separator);
    if (*output == NULL) {
        free_matr(*a);
        free_matr(*b);
        return WRONG_INPUT;
    }

    return DEFAULT;
}

int check_str_output (char *result, char *output) {
    if (strlen(result) != strlen(output)) {
        return WRONG_RESULT;
    } else {
        for (size_t i = 0; i < strlen(result); i++) {
            if (*result != *output) {
                return WRONG_RESULT;
            }
            result++;
            output++;
        }

        return DEFAULT;
    }
}

int check_matr_output (Matrix *result, char *output) {
    char *separator = "|";

    // Получение количества строк для матрицы
    int row;
    char *tmp = strtok(output, separator);
    if (str_to_int(tmp, &row) == WRONG_INPUT) {
        return WRONG_INPUT;
    }

    // Получение количества столбцов для матрицы
    int col;
    tmp = strtok(NULL, separator);
    if (str_to_int(tmp, &col) == WRONG_INPUT) {
        return WRONG_INPUT;
    }

    // Выделение памяти для матрицы
    Matrix *answer;
    if (init_matr(&answer, row, col) == MEM_ALLOC_ERR) {
        return MEM_ALLOC_ERR;
    }

    // Получение данных для матрицы
    for (size_t i = 0; i < answer->num_row; i++) {
        for (size_t j = 0; j < answer->num_col; j++) {
            tmp = strtok(NULL, separator);
            if (tmp == NULL) {
                free_matr(answer);
                return WRONG_INPUT;
            } else {
                if (str_to_int(tmp, &(answer->data[i][j])) == WRONG_INPUT) {
                    free_matr(answer);
                    return WRONG_INPUT;
                }
            }
        }
    }

    if (result->num_row != answer->num_row || result->num_col != answer->num_col) {
        free_matr(answer);
        return WRONG_RESULT;
    }
    for (size_t i = 0; i < result->num_row; i++) {
        for (size_t j = 0; j < result->num_col; j++) {
            if (result->data[i][j] != answer->data[i][j]) {
                free_matr(answer);
                return WRONG_RESULT;
            }
        }
    }

    free_matr(answer);
    return DEFAULT;
}