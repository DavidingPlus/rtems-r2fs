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
    printf("%p\n", key);
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
    printf("%p\n", key);

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

R2FS_TEST(ForeachTest)
{
    CacheIndexManager cim;

    int *k1 = malloc(sizeof(int));
    *k1 = 3;
    int *k2 = malloc(sizeof(int));
    *k2 = 4;

    TestEntry *e1 = malloc(sizeof(TestEntry));
    e1->id = 3;
    strcpy(e1->name, "Carol");
    TestEntry *e2 = malloc(sizeof(TestEntry));
    e2->id = 4;
    strcpy(e2->name, "Dave");

    cacheIndexManagerAdd(&cim, k1, e1);
    cacheIndexManagerAdd(&cim, k2, e2);

    int count = 0;
    CACHE_INDEX_MANAGER_FOREACH(&cim, entry)
    {
        TestEntry *v = (TestEntry *)entry->value;
        TEST_ASSERT_NOT_NULL(v);
        count++;
    }
    TEST_ASSERT_EQUAL_INT(2, count);

    free(e2);
    free(e1);
    free(k2);
    free(k1);
}
