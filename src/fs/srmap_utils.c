#include "srmap_utils.h"


/**
 * @brief 返回 lpa 的反向映射在 srmap 中的 <lpa, idx>，用过指针出参返回。
 */
static void srmapUtilsGetSrmapPosOfLpa(uint32_t lpa, uint32_t *srmapBlkLpa, uint32_t *idx);

/**
 * @brief 封装 SrmapCache 不命中时，从 lpa 读取 Srmap Block 到缓存的过程。
 */
static BlockBuffer *srmapUtilsGetSrmapBlk(SrmapUtils *this, uint32_t lpa);


void srmapUtilsInit(SrmapUtils *this, struct file_system_manager *fsManager)
{
}

void srmapUtilsDestroy(SrmapUtils *this)
{
}

void srmapUtilsWriteSrmapOfData(SrmapUtils *this, uint32_t dataLpa, uint32_t ino, uint32_t blkoff)
{
}

void srmapUtilsWriteSrmapOfNode(SrmapUtils *this, uint32_t nodeLpa, uint32_t nid)
{
}

void srmapUtilsWriteDirtySrmapSync(SrmapUtils *this)
{
}

void srmapUtilsClearCache(SrmapUtils *this)
{
}


static void srmapUtilsGetSrmapPosOfLpa(uint32_t lpa, uint32_t *srmapBlkLpa, uint32_t *idx)
{
}

static BlockBuffer *srmapUtilsGetSrmapBlk(SrmapUtils *this, uint32_t lpa)
{
}
