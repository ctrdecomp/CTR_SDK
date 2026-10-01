#pragma once

#include <nn/types.h>
#include <nn/cec/CTR/cec_Cec.h>
#include <nn/cec/CTR/cec_CecAPI.h>
#include <nn/cec/CTR/cec_Message.h>
#include <nn/cec/CTR/cec_MessageBox.h>
#include <nn/cec/CTR/cec_Control.h>

namespace nn{
namespace cec{
namespace CTR{
namespace 
{
    const bit32 FILEOPT_READ         = (1<<1);
    const bit32 FILEOPT_WRITE        = (1<<2);
    const bit32 FILEOPT_READWRITE    = FILEOPT_READ|FILEOPT_WRITE;
    const bit32 FILEOPT_MKDIR        = (1<<3);
    const bit32 FILEOPT_NOCHECK      = (1<<4);
    const bit32 FILEOPT_DUMP         = (1<<30);
} // namespace

typedef enum CecBoxDataType
{
    BOXDATA_TYPE_START = 100,
    BOXDATA_TYPE_ICON,
    BOXDATA_TYPE_NAME_1 = 110,
    BOXDATA_TYPE_END = 200
}BoxDataType;

enum CecFileType
{
    FILETYPE_MESSAGE_BOX_LIST = 1,
    FILETYPE_MESSAGE_BOX_INFO = 2,
    FILETYPE_INBOX_INFO       = 3,
    FILETYPE_OUTBOX_INFO      = 4,
    FILETYPE_OUTBOX_INDEX     = 5,
    FILETYPE_INBOX_MESSAGE    = 6,
    FILETYPE_OUTBOX_MESSAGE   = 7,
    FILETYPE_CEC_BASE_DIR    = 10,
    FILETYPE_MESSAGE_BOX_DIR    = 11,
    FILETYPE_MESSAGE_INBOX_DIR  = 12,
    FILETYPE_MESSAGE_OUTBOX_DIR = 13,

    FILETYPE_END,

    FILETYPE_BOXDATA_START = 100,
        
    FILETYPE_BOXDATA_ICON,
    FILETYPE_BOXDATA_NAME_1 = 110,
    FILETYPE_BOXDATA_NAME_2,
    FILETYPE_BOXDATA_NAME_3,
    FILETYPE_BOXDATA_NAME_4,
    FILETYPE_BOXDATA_TEXT_1 = 120,
    FILETYPE_BOXDATA_TEXT_2,
    FILETYPE_BOXDATA_TEXT_3,
    FILETYPE_BOXDATA_TEXT_4,
    FILETYPE_BOXDATA_DATA_1 = 130,
    FILETYPE_BOXDATA_DATA_2,
    FILETYPE_BOXDATA_DATA_3,
    FILETYPE_BOXDATA_DATA_4,
    FILETYPE_BOXDATA_FLAG_1 = 140,
    FILETYPE_BOXDATA_FLAG_2,
    FILETYPE_BOXDATA_FLAG_3,
    FILETYPE_BOXDATA_FLAG_4,
    FILETYPE_BOXDATA_PROGRAM_ID = 150,

    FILETYPE_BOXDATA_END = 200,

    FILETYPE_ANY               = 0xff
};

u32 atol16(const u8 *str);

void os_free(void *ptr);
void* os_malloc(size_t size, s32 alignment = 4);

u32 Base64Str2CecTitleId(const u8* str);

void FinalizeAllocFunc();
void SetAllocFunc(nn::fnd::IAllocator& cecAllocFunc);

} // namespace CTR
} // namespace cec
} // namespace nn