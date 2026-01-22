#ifndef _UNIT_TEST_H_
#define _UNIT_TEST_H_

#include <stdlib.h>

#include "unity/unity.h"


#define MAX_TESTS 1000


// unity 要求的必须用户提供的接口，留空即可。
void setUp(void);

void tearDown(void);


/*
========================================================================
单元测试自动注册（RTEMS）

在桌面 Linux 等系统上，测试函数通常可以通过构造函数或
动态内存在启动时自动注册到链表。但在 RTEMS 这类嵌入式系统：

1. 构造函数 (__attribute__((constructor))) 不可靠，Init Task 执行时可能根本未被调用。
2. 动态节点依赖 malloc，堆可能未初始化或空间不足，无法安全保证自动注册。因此无法使用动态扩展的链表。

因此，使用静态数组 + 注册表方式：
- 节点在编译期生成，安全可靠；
- Init Task 只需一次遍历注册表即可生成链表；
- 支持多文件自动注册，同时不依赖堆或 constructor。

当然，问题在于有测试函数数量的上限，这个其实问题不大。
========================================================================
*/
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
