#pragma once

#include <nn/assert.h>
#include <nn/util/util_NonCopyable.h>

#include <nn/os/os_HandleObject.h>
#include <nn/os/os_Event.h>
#include <nn/os/os_Mutex.h>
#include <nn/os/os_Semaphore.h>
#include <nn/os/os_SharedMemory.h>
#include <nn/os/os_TransferMemoryBlock.h>

namespace nn {
namespace os {

class HandleManager
{
    HandleManager();
    ~HandleManager();
public:
    static void AttachHandle(HandleObject* obj, Handle handle)
    {
        obj->SetHandle(handle);
    }

    static Handle DetachHandle(HandleObject* obj)
    {
        Handle handle = obj->GetHandle();
        obj->ClearHandle();
        return handle;
    }

    static void AttachSharedMemoryHandle(SharedMemoryBlock* sharedMemory, Handle handle, size_t size, bool readOnly)
    {
        sharedMemory->AttachAndMap(handle, size, readOnly);
    }

    static Result AttachTransferMemoryBlockHandle(TransferMemoryBlock* pMemBlock, Handle handle, size_t size,
        bit32 otherPermission = os::MEMORY_PERMISSION_NONE, bit32 myPermission = os::MEMORY_PERMISSION_READ_WRITE);
};

inline Result HandleManager::AttachTransferMemoryBlockHandle(TransferMemoryBlock* pMemBlock, Handle handle, size_t size,
    bit32 otherPermission = os::MEMORY_PERMISSION_NONE, bit32 myPermission = os::MEMORY_PERMISSION_READ_WRITE)
{
    return pMemBlock->AttachAndMap(handle, size, otherPermission, myPermission);
}

}
}