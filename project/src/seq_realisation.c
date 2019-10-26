#include <stdlib.h>
#include <stdio.h>

#include "seq_realisation.h"
#include "utils.h"

int find_lis(int *buf, int buf_size, int **answer, int *answer_size) {
    if (buf_size <= 1) {
        return buf_size;
    }

    if (!buf) {
        return NULL_PTR;
    }

    int lis_length = -1;

    int *subsequence = (int*)malloc(buf_size * sizeof(int));
    if (!subsequence) {
        return ERR_MEM_ALLOC;
    }

    int *indexes = (int*)malloc(buf_size * sizeof(int));
    if (!indexes) {
        free(subsequence);
        return ERR_MEM_ALLOC;
    }

    for (int i = 0; i < buf_size; i++) {
        subsequence[i] = INT_MAX;
        subsequence[i] = INT_MAX;
    }

    subsequence[0] = buf[0];
    indexes[0] = 0;

    for (int i = 1; i < buf_size; i++) {
        indexes[i] = ceil_index(subsequence, 0, i, buf[i]);

        if (lis_length < indexes[i]) {
            lis_length = indexes[i];
        }
    }

    (*answer_size) = lis_length + 1;

    (*answer) = (int*)malloc ((*answer_size) * sizeof(int));
    if (!(*answer)) {
        free(subsequence);
        free(indexes);
        return ERR_MEM_ALLOC;
    }

    int pos = 0;
    while (indexes[pos] != lis_length) {
        pos++;
    }

    int j = ((*answer_size) - 1);
    (*answer)[j--] = buf[pos];

    int tmp = indexes[pos];
    if (tmp != 0) {
        for (int i = pos - 1; i >= 0; i--) {
            if (indexes[i] == (tmp - 1)) {
                (*answer)[j--] = buf[i];
                tmp--;
            }
        }
    }

    free(subsequence);
    free(indexes);
    return DEFAULT;
}

int ceil_index(int *subsequence, int l, int r, int key){
    if (!subsequence) {
        return NULL_PTR;
    }

    if (r <= l) {
        return WRONG_ARGS;
    }

    int mid = 0;
    int left = l;
    int right = r;
    int ceil_index = 0;
    int ceil_index_found = 0;

    for (mid = (left + right) / 2; left <= right && (ceil_index_found == 0); mid = (left + right) / 2) {
        if (subsequence[mid] > key) {
            right = mid - 1;
        }
        else if (subsequence[mid] == key) {
            ceil_index = mid;
            ceil_index_found = 1;
        }
        else if (mid + 1 <= right && subsequence[mid + 1] >= key) {
            subsequence[mid + 1] = key;
            ceil_index = mid + 1;
            ceil_index_found = 1;
        } else {
            left = mid + 1;
        }
    }

    if (ceil_index_found == 0) {
        if (mid == left) {
            subsequence[mid] = key;
            ceil_index = mid;
        }
        else {
            subsequence[mid + 1] = key;
            ceil_index = mid + 1;
        }
    }

    return ceil_index;
}