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
            if (test_name) {
                printf("%s: ", test_name);
            }
            printf("Wrong input\n");
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

    int get_data_return_value = get_matrix_data(a);
    if (get_data_return_value == WRONG_INPUT) {
        return WRONG_INPUT;
    } else if (get_data_return_value == MEM_ALLOC_ERR) {
        return MEM_ALLOC_ERR;
    }

    get_data_return_value = get_matrix_data(b);
    if (get_data_return_value == WRONG_INPUT) {
        free_matr(*a);
        return WRONG_INPUT;
    } else if (get_data_return_value == MEM_ALLOC_ERR) {
        free_matr(*a);
        return WRONG_INPUT;
    }

    get_data_return_value = get_matrix_data(answer);
    if (get_data_return_value == WRONG_INPUT) {
        free_matr(*a);
        free_matr(*b);
        return WRONG_INPUT;
    } else if (get_data_return_value == MEM_ALLOC_ERR) {
        free_matr(*a);
        free_matr(*b);
        return MEM_ALLOC_ERR;
    } else if (get_data_return_value == MATR_CANNOT_BE_MULTIPLIED) {
        return MATR_CANNOT_BE_MULTIPLIED;
    }

    return DEFAULT;
}

int get_matrix_data(Matrix **matr) {
    char *separator = " |";

    // Получение количества строк матрицы
    int row;
    char *tmp;
    tmp = strtok(NULL, separator);
    if (str_to_int(tmp, &row) == WRONG_INPUT) {
        return WRONG_INPUT;
    }

    // Если в тесте в ответе '0', то это значит, что матрицы не могут быть умножены
    if (row == 0) {
        return MATR_CANNOT_BE_MULTIPLIED;
    }

    // Получение количества столбцов матрицы
    int col;
    tmp = strtok(NULL, separator);
    if (str_to_int(tmp, &col) == WRONG_INPUT) {
        return WRONG_INPUT;
    }

    // Выделение памяти для матрицы
    if (init_matr(matr, row, col) == MEM_ALLOC_ERR) {
        return MEM_ALLOC_ERR;
    }

    // Получение данных для матрицы
    for (size_t i = 0; i < (*matr)->num_row; i++) {
        for (size_t j = 0; j < (*matr)->num_col; j++) {
            tmp = strtok(NULL, separator);
            if (str_to_int(tmp, &(*matr)->data[i][j]) == WRONG_INPUT) {
                free_matr(*matr);
                return WRONG_INPUT;
            }
        }
    }

    return DEFAULT;
}