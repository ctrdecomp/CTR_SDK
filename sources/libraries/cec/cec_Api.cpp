// Filename: cec_Api.cpp
//
// Project: Horizon

#include <ctype.h>
#include <string.h>

#include <nn/cec.h>
#include <nn/os.h>
#include <nn/dbg.h>
#include <nn/nstd.h>
#include <nn/init.h>
#include <nn/fnd.h>
#include <nn/fnd/fnd_ExpHeap.h>
#include <nn/os/os_Memory.h>

#include <nn/cec/CTR/cec_Api.h>

#include <nn/cec/CTR/cec_MessageBox.h>

using namespace nn::fnd;

namespace 
{
    nn::fnd::IAllocator* pAllocator = NULL;
    nn::fnd::ThreadSafeExpHeap s_ExpHeap;
    nn::fnd::ThreadSafeExpHeap::Allocator s_Allocator;
    bool s_IsSetBuffer = false;
}

namespace nn {
namespace cec {
namespace CTR {

u32 atol16(const u8 *str)
{
    NN_TASSERT_(str);
    u32 i = 0;
    u32 fig;
    const int base = 16;

    for (; *str != '\0'; ++str)
    {
        i *= base;
        if (*str >= '0' && *str <= '9')
        {
            fig = *str - '0';
        }
        else
        {
            fig = *str - ('a' - 0xa);
        }
        if (0 < fig && fig < base)
        {
            i += fig;
        }
    }

    return i;
}

void * os_malloc(size_t size, s32 alignment)
{
    if(pAllocator)
    {
        void* ptr = pAllocator->Allocate(size, alignment);
        return ptr;
    }
    return NULL;
}

void os_free(void *ptr)
{
    if (ptr == NULL)
    {
        NN_LOG_("os_free: ### ptr==NULL\n");
        return;
    }
    if(pAllocator)
    {
        pAllocator->Free(ptr);
    }
}

u32 Base64Str2CecTitleId(const u8* str)
{
    NN_TASSERT_(str);
    u32 retval = 0;
    u8 outCecTitleId[16] = {0};

    memcpy(outCecTitleId, str, sizeof(outCecTitleId));

    retval = atol16(str);
    return retval;
}

void FinalizeAllocFunc(void)
{
    if(s_IsSetBuffer)
    {
        s_Allocator = ThreadSafeExpHeap::Allocator();
        s_ExpHeap.Finalize();
        pAllocator = NULL;
        s_IsSetBuffer = false;
    }
}

void SetAllocFunc(nn::fnd::IAllocator& cecAllocFunc)
{
    pAllocator = &cecAllocFunc;
}

} // namespace CTR
} // namespace cec
} // namespace nn