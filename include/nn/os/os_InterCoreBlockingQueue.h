#pragma once

#include <nn/os/os_BlockingQueue.h>
#include <nn/os/os_InterCoreCriticalSection.h>

namespace nn{ 
namespace os{
namespace detail{

typedef BlockingQueue InterCoreBlockingQueue;

} // namespace os
} // namespace nn