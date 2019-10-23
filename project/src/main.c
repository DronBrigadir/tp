#include <stdio.h>
#include "test.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        printf("Please add the test path to the main arguments\n");
        return EMPTY_ARGS;
    }

    FILE *test_file;
    char *file_path = argv[1];
    test_file = fopen(file_path, "r");
    if (!test_file) {
        printf("Unable to open file: %s\n", file_path);
        return UNABLE_TO_OPEN_FILE;
    }

    test(test_file);

    fclose(test_file);
    return DEFAULT;
}
