#ifndef _UNIT_TEST_H_
#define _UNIT_TEST_H_

#include <stdlib.h>

#include "unity/unity.h"


// unity 要求的必须用户提供的接口，留空即可。
void setUp(void);

void tearDown(void);


typedef void (*testFunc)(void);

struct TestEntry
{
    const char *name;
    testFunc func;
    struct TestEntry *next;
};

static struct TestEntry *testListHead = NULL;


#define R2FS_TEST(test_name)                                            \
    void test_name(void);                                               \
    static void __attribute__((constructor)) register_##test_name(void) \
    {                                                                   \
        struct TestEntry *node = malloc(sizeof(struct TestEntry));      \
        if (node)                                                       \
        {                                                               \
            node->name = #test_name;                                    \
            node->func = test_name;                                     \
            node->next = testListHead;                                  \
            testListHead = node;                                        \
        }                                                               \
    }                                                                   \
    void test_name(void)


void runAllTests(void);


#endif
