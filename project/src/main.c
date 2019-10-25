#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "seq_realisation.h"

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        printf("Please pass arguments to the program\nUse: program path_to_file\n");
        return WRONG_ARGS;
    }

    char *file_path = argv[1];

    int *buf = NULL;
    int size = 0;

    int return_value = read_from_file(&buf, &size, file_path);
    if (return_value == CANT_OPEN_FILE) {
        printf("Wrong path to file\n");
        return CANT_OPEN_FILE;
    } else if (return_value == ERR_MEM_ALLOC) {
        printf("Memory allocation error\n");
        return ERR_MEM_ALLOC;
    } else if (return_value == ERR_CONTENT_FILE) {
        printf("The file must contain only numbers\n");
        return ERR_CONTENT_FILE;
    }

    printf("Sequentially realisation:\n");
    int *res = NULL;
    int res_size = 0;
    return_value = find_lis(buf, size, &res, &res_size);
    if (return_value == ERR_MEM_ALLOC) {
        free(buf);
        printf("Memory allocation error\n");
        return ERR_MEM_ALLOC;
    }

    printf("size: %d\n", res_size);
    for (int i = 0; i < res_size; i++) {
        printf("%d ", res[i]);
    }

    free(res);
    free(buf);
    return 0;
}