#include "unit_test.h"

void setUp(void) {}

void tearDown(void) {}

void runAllTests(void)
{
    UNITY_BEGIN();

    struct TestEntry *node = testListHead;
    while (node)
    {
        printf("[ RUN      ] %s\n", node->name);
        RUN_TEST(node->func);
        printf("[       OK ] %s\n", node->name);
        node = node->next;
    }

    UNITY_END();
}
