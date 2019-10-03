//
// Created by andrey on 01.10.2019.
//

#include <string.h>
#include "matrix.h"
#include "utils.h"
#include "malloc.h"

int matrix_multiplication(Matrix *a, Matrix *b, Matrix **result) {
    if (a->num_col != b->num_row) {
        return MATR_CANNOT_BE_MULTIPLIED;
    }

    if (init_matr(result, a->num_row, b->num_col) == MEM_ALLOC_ERR) {
        return MEM_ALLOC_ERR;
    }

    for (size_t i = 0; i < a->num_row; i++)
    {
        for (size_t j = 0; j < b->num_col; j++)
        {
            (*result)->data[i][j] = 0;
            for (size_t k = 0; k < a->num_col; k++)
                (*result)->data[i][j] += a->data[i][k] * b->data[k][j];
        }
    }

    return DEFAULT;
}

int init_matr(Matrix **matrix, int num_row, int num_col) {
    (*matrix) = (Matrix*)malloc(sizeof(Matrix));
    if ((*matrix) == NULL) {
        return MEM_ALLOC_ERR;
    }
    (*matrix)->num_col = num_col;
    (*matrix)->num_row = num_row;

    (*matrix)->data = (int**)malloc((*matrix)->num_row * sizeof(int*));
    if ((*matrix)->data == NULL) {
        free(*matrix);
        return MEM_ALLOC_ERR;
    }
    int is_alloc_err = DEFAULT;
    for (size_t i = 0; i < (*matrix)->num_row; i++) {
        (*matrix)->data[i] = (int*)malloc((*matrix)->num_col * sizeof(int));
        if ((*matrix)->data[i] == NULL) {
            is_alloc_err = MEM_ALLOC_ERR;
            break;
        }
    }
    if (is_alloc_err == MEM_ALLOC_ERR) {
        free_matr(*matrix);
        return MEM_ALLOC_ERR;
    }

    return DEFAULT;
}

void free_matr(Matrix *matrix) {
    for (size_t i = 0; i < matrix->num_row; i++) {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}

int is_matrix_equal (Matrix *l, Matrix *r) {
    if (l->num_row != r->num_row || l->num_col != r->num_col) {
        return WRONG_RESULT;
    }
    for (size_t i = 0; i < l->num_row; i++) {
        for (size_t j = 0; j < l->num_col; j++) {
            if (l->data[i][j] != r->data[i][j]) {
                return WRONG_RESULT;
            }
        }
    }

    return DEFAULT;
}