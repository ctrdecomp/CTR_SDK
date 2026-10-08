#pragma once

#include <nn/types.h>

namespace nn{
namespace init{
    void InitializeAllocator(uptr addr, size_t size);
    void InitializeAllocator(size_t size);

}
}
