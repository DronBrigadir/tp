//
// Created by andrey on 01.10.2019.
//

#ifndef TASK1_UTILS_H
#define TASK1_UTILS_H

// Структура входных данных тесте (каждый тест находится на одной строке):
// название_теста количество_строк_А количество_столбцов_А данные_для_А (каждое число указывается через '|')
// количество_строк_В количество_столбцов_В данные_для_В (каждое число указывается через '|')
// выходные данные: если матрицы не могут быть перемножены, то: выходное значние '0'.
// Если матрицы могут быть перемножены,
// то: количество_строк_С количество_столбцов_С данные_для_С (каждое число указывается через '|').
// где С - матрица, полученная в результате умножения А и В
// В конце строки каждого теста нужно добавить '|'

#define TEST_FILE_PATH "/home/andrey/tp/task1/project/tests/test.txt"
#define MAX_BUFFER 1024
#define WRONG_INPUT -1
#define MATR_CANNOT_BE_MULTIPLIED -2
#define MEM_ALLOC_ERR -3
#define WRONG_RESULT -4
#define UNABLE_TO_OPEN_FILE -5
#define DEFAULT 0

int str_to_int(char* src, int *tmp);
int is_str_contains_letter (char *str);

#endif //TASK1_UTILS_H