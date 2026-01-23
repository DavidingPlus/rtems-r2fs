#include "r2fs_test.h"

#include "cache/cache_index_manager.h"


typedef struct
{
    int id;
    char name[32];
} TestEntry;


R2FS_TEST(AddGetTest)
{
    CacheIndexManager cim;

    int *key = malloc(sizeof(int));
    *key = 1;

    TestEntry *entry = malloc(sizeof(TestEntry));
    entry->id = 1;
    strcpy(entry->name, "Alice");

    cacheIndexManagerAdd(&cim, key, entry);

    TestEntry *e = (TestEntry *)cacheIndexManagerGet(&cim, key);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(1, e->id);
    TEST_ASSERT_EQUAL_STRING("Alice", e->name);

    free(entry);
    free(key);
}

R2FS_TEST(RemoveTest)
{
    CacheIndexManager cim;

    int *key = malloc(sizeof(int));

    TestEntry *entry = malloc(sizeof(TestEntry));
    entry->id = 2;
    strcpy(entry->name, "Bob");

    cacheIndexManagerAdd(&cim, key, entry);

    TestEntry *removed = (TestEntry *)cacheIndexManagerRemove(&cim, key);

    TEST_ASSERT_NOT_NULL(removed);
    TEST_ASSERT_EQUAL_INT(2, removed->id);
    TEST_ASSERT_EQUAL_STRING("Bob", removed->name);

    free(entry);
    free(key);
}

R2FS_TEST(AddDupKeyTest)
{
    CacheIndexManager cim;

    int *key = malloc(sizeof(int));
    *key = 5;

    TestEntry *v1 = malloc(sizeof(TestEntry));
    v1->id = 5;
    strcpy(v1->name, "Eve");

    cacheIndexManagerAdd(&cim, key, v1);

    // 再添加同样 key 应该触发 assert
    TestEntry *v2 = malloc(sizeof(TestEntry));
    v2->id = 6;
    strcpy(v2->name, "Frank");

    // assert 本来会导致程序崩溃，但是被这条语句阻止了。
    TEST_IGNORE_MESSAGE("Expect assertion failure when adding duplicate key");
    cacheIndexManagerAdd(&cim, key, v2);

    free(v2);
    free(v1);
    free(key);
}
