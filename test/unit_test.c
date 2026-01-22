#include "unit_test.h"


struct TestEntry testArray[MAX_TESTS] = {};

int testCount = 0;


void setUp(void) {}

void tearDown(void) {}

void runAllTests(void)
{
    UNITY_BEGIN();

    for (int i = 0; i < testCount; i++)
    {
        printf("[ RUN      ] %s\n", testArray[i].name);
        RUN_TEST(testArray[i].func);
        printf("[       OK ] %s\n", testArray[i].name);
    }

    UNITY_END();
}
