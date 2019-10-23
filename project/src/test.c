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
            int check_return_value = check_answer(&a, &b, &answer, parser_return_value);

            if (check_return_value == MEM_ALLOC_ERR) {
                printf("Memory allocation error\n");
                return;
            } else if (check_return_value == TEST_PASSED) {
                printf("%s passed\n", test_name);
            } else if (check_return_value == TEST_FAILED) {
                printf("%s failed\n", test_name);
            }
        }
    }

    if (feof(in) == 0) {
        printf("Error reading from file\n");
    }
}

int parser(char **test_name, Matrix **a, Matrix **b,
           Matrix **answer, char *input) {
    char *separator = " |";

    // Получение наименования теста
    (*test_name) = strtok_r(input, separator, test_name);
    input += strlen(*test_name) + 1;
    if (!(*test_name)) {
        return WRONG_INPUT;
    }

    int get_data_return_value = get_matrix_data(a, &input);
    if (get_data_return_value == WRONG_INPUT) {
        return WRONG_INPUT;
    } else if (get_data_return_value == MEM_ALLOC_ERR) {
        return MEM_ALLOC_ERR;
    }

    get_data_return_value = get_matrix_data(b, &input);
    if (get_data_return_value == WRONG_INPUT) {
        free_matr(*a);
        return WRONG_INPUT;
    } else if (get_data_return_value == MEM_ALLOC_ERR) {
        free_matr(*a);
        return WRONG_INPUT;
    }

    get_data_return_value = get_matrix_data(answer, &input);
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

int get_matrix_data(Matrix **matr, char **string_to_parse) {
    char *separator = " |";

    // Получение количества строк матрицы
    int row;
    char *tmp;
    tmp = strtok_r(*string_to_parse, separator, &tmp);
    *string_to_parse += strlen(tmp) + 1;
    if (str_to_int(tmp, &row) == WRONG_INPUT) {
        return WRONG_INPUT;
    }

    // Если в тесте в ответе '0', то это значит,
    // что матрицы не могут быть умножены
    if (row == 0) {
        return MATR_CANNOT_BE_MULTIPLIED;
    }

    // Получение количества столбцов матрицы
    int col;
    tmp = strtok_r(*string_to_parse, separator, &tmp);
    *string_to_parse += strlen(tmp) + 1;
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
            tmp = strtok_r(*string_to_parse, separator, &tmp);
            *string_to_parse += strlen(tmp) + 1;
            if (str_to_int(tmp, &(*matr)->data[i][j]) == WRONG_INPUT) {
                free_matr(*matr);
                return WRONG_INPUT;
            }
        }
    }

    return DEFAULT;
}

int check_answer(Matrix **l, Matrix**r, Matrix **answer, int parser_return_value) {
    Matrix *result;
    int mult_return_value = matrix_multiplication(*l, *r, &result);

    free_matr(*l);
    free_matr(*r);

    if (mult_return_value == MEM_ALLOC_ERR) {
        if (parser_return_value != MATR_CANNOT_BE_MULTIPLIED) {
            free_matr(*answer);
        }
        return MEM_ALLOC_ERR;
    }

    if (parser_return_value == MATR_CANNOT_BE_MULTIPLIED) {
        if (mult_return_value == MATR_CANNOT_BE_MULTIPLIED) {
            return TEST_PASSED;
        } else if (mult_return_value == DEFAULT) {
            free_matr(result);
            return TEST_FAILED;
        }
    } else if (parser_return_value == DEFAULT) {
        if (mult_return_value == MATR_CANNOT_BE_MULTIPLIED) {
            free_matr(*answer);
            return TEST_FAILED;
        } else if (mult_return_value == DEFAULT) {
            int equal_return_value = is_matrix_equal(result, *answer);
            free_matr(result);
            free_matr(*answer);

            if (equal_return_value == WRONG_RESULT) {
                return TEST_FAILED;
            } else if (equal_return_value == DEFAULT) {
                return TEST_PASSED;
            }
        }
    }
}