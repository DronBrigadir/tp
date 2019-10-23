//
// Created by andrey on 02.10.2019.
//

#include <string.h>
#include <stdlib.h>
#include "utils.h"

int str_to_int(char* src, int *tmp) {
    if (src == NULL) {
        return WRONG_INPUT;
    } else {
        if (is_str_contains_letter(src)) {
            return WRONG_INPUT;
        } else {
            *tmp = atoi(src);
            return DEFAULT;
        }
    }
}

int is_str_contains_letter(char *str) {
    if (str)

    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return 1;
        }
    }
    return 0;
}
