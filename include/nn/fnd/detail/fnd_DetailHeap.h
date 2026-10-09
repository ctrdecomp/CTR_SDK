#pragma once

#include <nn/fnd/detail/fnd_DetailHeapHead.h>
#include <nn/fnd/fnd_ExpHeap.h>

#define NN_OS_EXPHEAP_ALLOC_DIR_FRONT 0
#define NN_OS_EXPHEAP_ALLOC_DIR_REAR  1
#define NN_OS_EXPHEAP_ALLOC_MODE_FIRST 0
#define NN_OS_EXPHEAP_ALLOC_MODE_NEAR 1

namespace nn{
namespace fnd{
namespace detail{
    typedef nn::fnd::detail::NNSiFndExpHeapHead* Heap;

    ushort SetGroupIDForHelp(Heap heap, ushort groupId);
    ushort SetAllocModeForHeap(Heap heap, ushort mode);
    bool UseMarginOfAlignmentForHeap(Heap heap, bool reuse);
    void* AllocFromHeap(Heap heap, size_t size, s32 alignment);
    void* AllocUsedBlockFromFreeBlock(NNSiFndExpHeapHead* pEHHead, NNSiFndExpHeapMBlockHead* pMBHeadFree,void* mblock,u32 size,u16 direction);
    void FreeToHeap(Heap heap, void* p);
    Heap CreateHeap(Heap heapHandle, void* startAddr, u32 size, ushort optFlag);
    NNSiFndHeapHead* FindContainHeap(NNSFndList* pList, void* memBlock);
}
}
}

