#include <gtest/gtest.h>

extern "C" {
    #include "seq_realisation.h"
}

TEST(LisTest, BaseTest) {
    int buf[] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
    int size = sizeof(buf) / sizeof (int);
    int *res = NULL;
    int res_size = 0;
    find_lis(buf, size, &res, &res_size);
    ASSERT_EQ(res_size, 6);
    ASSERT_EQ(res[0], 0);
    free(res);
}