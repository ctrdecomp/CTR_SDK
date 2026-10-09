#pragma once

#include <nn/fnd/detail/fnd_DetailHeap.h>

namespace nn{
namespace fnd{
namespace detail{

void NNSi_FndFinalizeHeap(NNSiFndHeapHead* pHeadHd);
void NNSi_FndInitHeapHead(ExpHeapImpl* pHeapHd,u32 signature,void *heapStart,void *heapEnd,ushort optFlag);

static bool sRootListInitialized = false;
static NNSFndList sRootList;

}
}
}

