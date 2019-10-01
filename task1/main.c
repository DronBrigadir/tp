#include <stdio.h>
#include "test.h"

int main() {
    FILE *test_file;
    test_file = fopen(TEST_FILE_PATH, "r");
    if (!test_file) {
        printf("Unable to open file: %s\n", TEST_FILE_PATH);
        return 1;
    }

    test(test_file);

    fclose(test_file);
    return 0;
}