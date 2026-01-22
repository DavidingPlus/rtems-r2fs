#ifndef _UNIT_TEST_H_
#define _UNIT_TEST_H_

#include <stdlib.h>

#include "unity/unity.h"


#define MAX_TESTS 1000


// unity 要求的必须用户提供的接口，留空即可。
void setUp(void);

void tearDown(void);


typedef void (*testFunc)(void);

struct TestEntry
{
    const char *name;
    testFunc func;
};

extern struct TestEntry testArray[MAX_TESTS];
extern int testCount;


#define R2FS_TEST(test_name)                                                  \
    void test_name(void);                                                     \
    static void __attribute__((unused)) register_##test_name(void)            \
    {                                                                         \
        if (testCount < MAX_TESTS)                                            \
        {                                                                     \
            testArray[testCount].name = #test_name;                           \
            testArray[testCount].func = test_name;                            \
            testCount++;                                                      \
        }                                                                     \
    }                                                                         \
    static void __attribute__((constructor)) _call_register_##test_name(void) \
    {                                                                         \
        register_##test_name();                                               \
    }                                                                         \
    void test_name(void)


void runAllTests(void);


#endif
