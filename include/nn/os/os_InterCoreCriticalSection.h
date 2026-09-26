#pragma once

#include <nn/os/os_CriticalSection.h>
#include <nn/os/ARM/os_MemoryBarrier.h>

namespace nn{
namespace os{

typedef CriticalSection InterCoreCriticalSection;

}
}