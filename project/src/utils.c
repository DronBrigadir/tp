#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

int read_from_file(int **buf, int *size, char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        return CANT_OPEN_FILE;
    }

    int num_of_spaces = 0;
    char tmp;

    while ((tmp = fgetc(file)) != EOF) {  // Подсчет количества пробелов в файле
        if (tmp == ' ') {
            num_of_spaces++;
        }
    }

    *size = num_of_spaces + 1;  // Количество чисел = число разделителей между ними + 1
    (*buf) = (int*)malloc((*size) * sizeof(int));
    if (!(*buf)) {
        fclose(file);
        return ERR_MEM_ALLOC;
    }

    fseek(file, 0, SEEK_SET);  // Устанавливаем указатель положения в файле на начало
    int i = 0;
    int return_value = 0;

    do {
        return_value = fscanf(file, "%d", &((*buf)[i]));
        if (return_value == 0) {  // В случае, если встретилась не цифра
            free(*buf);
            fclose(file);
            return ERR_CONTENT_FILE;
        }
        i++;
    } while (return_value != EOF);

    fclose(file);
    return DEFAULT;
}