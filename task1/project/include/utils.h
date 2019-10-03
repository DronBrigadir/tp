//
// Created by andrey on 01.10.2019.
//

#ifndef TASK1_UTILS_H
#define TASK1_UTILS_H

// Структура входных данных:
// название_теста количество_строк_А количество_столбцов_А данные_для_А(каждое число указывается через пробел)
// количество_строк_В количество_столбцов_В данные_для_В(каждое число указывается через пробел)
// выходные данные(либо сообщение о том, что матрицы нельзя перемножить,
// либо: количество_строк_С|количество_столбцов_С|данные_для_С(каждое число указывается через '|',
// в конце также нужно добавить '|'),
// где С - матрица, полученная в результате умножения А и В

#define TEST_FILE_PATH "/home/andrey/tp/task1/test_file.txt"
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