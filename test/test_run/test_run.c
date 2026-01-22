#include "r2fs_test.h"


R2FS_TEST(TestRun1)
{
    TEST_PASS();
}

R2FS_TEST(TestRun2)
{
    TEST_ASSERT_EQUAL(1 + 1, 2);
}
