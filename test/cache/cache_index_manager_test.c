#include "r2fs_test.h"

#include "cache/cache_index_manager.h"


typedef struct
{
    int id;
    char name[32];
} TestEntry;


R2FS_TEST(CIMInitTest)
{
    CacheIndexManager cim;

    cacheIndexManagerInit(&cim);

    cacheIndexManagerDestroy(&cim);
}

R2FS_TEST(CIMAddGetTest)
{
    CacheIndexManager cim;
    int key = 1;

    cacheIndexManagerInit(&cim);


    TestEntry entry;
    entry.id = 1;
    strcpy(entry.name, "Alice");

    cacheIndexManagerAdd(&cim, &key, &entry);

    TestEntry *e = (TestEntry *)cacheIndexManagerGet(&cim, &key);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(1, e->id);
    TEST_ASSERT_EQUAL_STRING("Alice", e->name);


    cacheIndexManagerDestroy(&cim);
}

R2FS_TEST(CIMRemoveTest)
{
    CacheIndexManager cim;
    int key = 2;

    cacheIndexManagerInit(&cim);


    TestEntry entry;
    entry.id = 2;
    strcpy(entry.name, "Bob");

    cacheIndexManagerAdd(&cim, &key, &entry);

    TestEntry *removed = (TestEntry *)cacheIndexManagerRemove(&cim, &key);

    TEST_ASSERT_NOT_NULL(removed);
    TEST_ASSERT_EQUAL_INT(2, removed->id);
    TEST_ASSERT_EQUAL_STRING("Bob", removed->name);


    cacheIndexManagerDestroy(&cim);
}

R2FS_TEST(CIMAddDupKeyTest)
{
    CacheIndexManager cim;
    int key = 3;

    cacheIndexManagerInit(&cim);


    TestEntry v1;
    v1.id = 3;
    strcpy(v1.name, "Eve");

    cacheIndexManagerAdd(&cim, &key, &v1);

    // 再添加同样 key 应该触发 assert
    TestEntry v2;
    v2.id = 3;
    strcpy(v2.name, "Frank");

    // assert 本来会导致程序崩溃，但是被这条语句阻止了。
    TEST_IGNORE_MESSAGE("Expect assertion failure when adding duplicate key");
    cacheIndexManagerAdd(&cim, &key, &v2);


    cacheIndexManagerDestroy(&cim);
}
