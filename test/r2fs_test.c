#include "r2fs_test.h"


struct TestEntry testArray[R2FS_MAX_TESTS] = {};

int testCount = 0;


void setUp(void) {}

void tearDown(void) {}

void runAllTests(void)
{
    UNITY_BEGIN();

    int failedCases = 0;
    for (int i = 0; i < testCount; i++)
    {
        printf(R2FS_COLOR_GREEN "[ RUN      ] %s" R2FS_COLOR_RESET "\n", testArray[i].name);

        // 记录当前测试失败样例的总数。
        failedCases = Unity.TestFailures;

        RUN_TEST(testArray[i].func);

        // 如果失败数增加了，说明这个测试失败。
        if (Unity.TestFailures > failedCases)
        {
            printf(R2FS_COLOR_RED "[   FAIL   ] %s" R2FS_COLOR_RESET "\n", testArray[i].name);
        }
        else
        {
            printf(R2FS_COLOR_GREEN "[       OK ] %s" R2FS_COLOR_RESET "\n", testArray[i].name);
        }
    }

    UNITY_END();
}
