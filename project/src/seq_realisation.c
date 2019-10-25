#include <stdlib.h>

#include "seq_realisation.h"
#include "utils.h"

int find_lis(int *src, int size_src, int **dest, int *size_dest) {
    if (!src) {
        return NULL_PTR;
    }

    int *prev = (int*)malloc(size_src * sizeof(int));
    if (!prev) {
        return ERR_MEM_ALLOC;
    }

    int *d = (int*)malloc(size_src * sizeof(int));
    if (!d) {
        free(prev);
        return ERR_MEM_ALLOC;
    }

    for (int i = 0; i < size_src; i++) {
        d[i] = 1;
        prev[i] = -1;
        for (int j = 0; j < i; j++) {
            if (src[j] < src[i] && d[j] + 1 > d[i]) {
                d[i] = d[j] + 1;
                prev[i] = j;
            }
        }
    }

    int pos = 0;
    int length = d[0];
    for (int i = 0; i < size_src; i++) {
        if (d[i] > length) {
            pos = i;
            length = d[i];
        }
    }

    *size_dest = 0;
    int tmp = pos;
    while (tmp != -1) {
        tmp = prev[tmp];
        (*size_dest)++;
    }

    (*dest) = (int*)malloc((*size_dest) * sizeof(int));
    if (!(*dest)) {
        free(d);
        free(prev);
        return ERR_MEM_ALLOC;
    }

    int i = 0;
    while (pos != -1) {
        (*dest)[i++] = src[pos];
        pos = prev[pos];
    }

    for (int i = 0; i < ((*size_dest) / 2); i++) {
        tmp = (*dest)[i];
        (*dest)[i] = (*dest)[(*size_dest) - 1 - i];
        (*dest)[(*size_dest) - 1 - i] = tmp;
    }

    free(d);
    free(prev);
    return DEFAULT;
}