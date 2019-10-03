//
// Created by andrey on 01.10.2019.
//

#ifndef TASK1_MATRIX_H
#define TASK1_MATRIX_H

struct Matrix {
    int **data;
    int num_row;
    int num_col;
};

typedef struct Matrix Matrix;

int matrix_multiplication(Matrix *a, Matrix *b, Matrix **result);
int init_matr(Matrix **matrix, int num_row, int num_col);
void free_matr(Matrix *matrix);

#endif //TASK1_MATRIX_H
