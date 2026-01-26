#include "r2fs_test.h"

#include "cache/cache_lru_replacer.h"


R2FS_TEST(CLRInitEmptyTest)
{
    CacheLruReplacer lru;

    cacheLruReplacerInit(&lru);


    TEST_ASSERT_FALSE(cacheLruReplacerCanReplace(&lru));


    cacheLruReplacerDestroy(&lru);
}

R2FS_TEST(CLRAddPopTest1)
{
    CacheLruReplacer lru;
    int k1 = 1;

    cacheLruReplacerInit(&lru);


    cacheLruReplacerAdd(&lru, &k1);

    TEST_ASSERT_TRUE(cacheLruReplacerCanReplace(&lru));

    void *key = cacheLruReplacerPop(&lru);
    TEST_ASSERT_EQUAL_INT(k1, *(int *)key);
    TEST_ASSERT_EQUAL_PTR(&k1, key);

    TEST_ASSERT_FALSE(cacheLruReplacerCanReplace(&lru));


    cacheLruReplacerDestroy(&lru);
}

R2FS_TEST(CLRAddPopTest2)
{
    CacheLruReplacer lru;
    int k1 = 1, k2 = 2, k3 = 3;

    cacheLruReplacerInit(&lru);


    cacheLruReplacerAdd(&lru, &k1);
    cacheLruReplacerAdd(&lru, &k2);
    cacheLruReplacerAdd(&lru, &k3);

    TEST_ASSERT_EQUAL_INT(*(int *)cacheLruReplacerPop(&lru), k1);
    TEST_ASSERT_EQUAL_PTR(cacheLruReplacerPop(&lru), &k2);
    TEST_ASSERT_EQUAL_INT(*(int *)cacheLruReplacerPop(&lru), k3);


    cacheLruReplacerDestroy(&lru);
}

R2FS_TEST(CLRAddPopRefreshTest1)
{
    CacheLruReplacer lru;
    int k1 = 1, k2 = 2, k3 = 3;

    cacheLruReplacerInit(&lru);


    cacheLruReplacerAdd(&lru, &k1);
    cacheLruReplacerAdd(&lru, &k2);
    cacheLruReplacerAdd(&lru, &k3);

    cacheLruReplacerAccess(&lru, &k1);

    TEST_ASSERT_EQUAL_PTR(cacheLruReplacerPop(&lru), &k2);
    TEST_ASSERT_EQUAL_INT(*(int *)cacheLruReplacerPop(&lru), k3);
    TEST_ASSERT_EQUAL_INT(*(int *)cacheLruReplacerPop(&lru), k1);


    cacheLruReplacerDestroy(&lru);
}

R2FS_TEST(CLRAddPopRefreshTest2)
{
    CacheLruReplacer lru;
    int k1 = 1, k2 = 2, k3 = 3;

    cacheLruReplacerInit(&lru);


    cacheLruReplacerAdd(&lru, &k1);
    cacheLruReplacerAdd(&lru, &k2);
    cacheLruReplacerAdd(&lru, &k3);

    cacheLruReplacerAccess(&lru, &k1);
    cacheLruReplacerAccess(&lru, &k2);

    TEST_ASSERT_EQUAL_INT(*(int *)cacheLruReplacerPop(&lru), k3);
    TEST_ASSERT_EQUAL_INT(*(int *)cacheLruReplacerPop(&lru), k1);
    TEST_ASSERT_EQUAL_PTR(cacheLruReplacerPop(&lru), &k2);


    cacheLruReplacerDestroy(&lru);
}

R2FS_TEST(CLRRemoveTest1)
{
    CacheLruReplacer lru;
    int k1 = 1, k2 = 2, k3 = 3;

    cacheLruReplacerInit(&lru);


    cacheLruReplacerAdd(&lru, &k1);
    cacheLruReplacerAdd(&lru, &k2);
    cacheLruReplacerAdd(&lru, &k3);

    cacheLruReplacerRemove(&lru, &k2);

    TEST_ASSERT_EQUAL_INT(*(int *)cacheLruReplacerPop(&lru), k1);
    TEST_ASSERT_EQUAL_INT(*(int *)cacheLruReplacerPop(&lru), k3);


    cacheLruReplacerDestroy(&lru);
}

R2FS_TEST(CLRRemoveTest2)
{
    CacheLruReplacer lru;
    int k1 = 1, k2 = 2;

    cacheLruReplacerInit(&lru);


    cacheLruReplacerAdd(&lru, &k1);
    cacheLruReplacerAdd(&lru, &k2);

    cacheLruReplacerRemove(&lru, &k1);

    TEST_ASSERT_EQUAL_INT(*(int *)cacheLruReplacerPop(&lru), k2);


    cacheLruReplacerDestroy(&lru);
}

R2FS_TEST(CLRRemoveTest3)
{
    CacheLruReplacer lru;
    int k1 = 1, k2 = 2;

    cacheLruReplacerInit(&lru);


    cacheLruReplacerAdd(&lru, &k1);
    cacheLruReplacerAdd(&lru, &k2);

    cacheLruReplacerRemove(&lru, &k2);

    TEST_ASSERT_EQUAL_INT(*(int *)cacheLruReplacerPop(&lru), k1);


    cacheLruReplacerDestroy(&lru);
}

R2FS_TEST(CLRCanReplaceTest)
{
    CacheLruReplacer lru;
    int k1 = 1, k2 = 2;

    cacheLruReplacerInit(&lru);


    cacheLruReplacerAdd(&lru, &k1);
    cacheLruReplacerAdd(&lru, &k2);

    TEST_ASSERT_TRUE(cacheLruReplacerCanReplace(&lru));

    cacheLruReplacerPop(&lru);
    TEST_ASSERT_TRUE(cacheLruReplacerCanReplace(&lru));

    cacheLruReplacerPop(&lru);
    TEST_ASSERT_FALSE(cacheLruReplacerCanReplace(&lru));


    cacheLruReplacerDestroy(&lru);
}
