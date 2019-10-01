//
// Created by andrey on 01.10.2019.
//

#ifndef TASK1_MATRIX_H
#define TASK1_MATRIX_H

struct Matrix {
    int **matr;
    int x_size;
    int y_size;
};

typedef struct Matrix Matrix;

int matrix_multiplication(Matrix *a, Matrix *b, Matrix *result);

#endif //TASK1_MATRIX_H
