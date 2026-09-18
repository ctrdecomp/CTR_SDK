#pragma once

#include <nn/types.h>
#include <nn/os/os_Synchronization.h>
#include <nn/os/os_MemoryBlock.h>

namespace nn{
namespace os{

class HandleManager;

class TransferMemoryBlock : public MemoryBlockBase, public HandleObject
{
public:
    bool m_SpaceAllocated;
    s8 reseversed1[3];
    int reseversed2;

public:
    TransferMemoryBlock(): 
        m_SpaceAllocated(false) 
    {
    }

    ~TransferMemoryBlock()
    { 
        this->Finalize(); 
    }
    
    void Initialize(void* p, size_t size, bit32 myPermission = os::MEMORY_PERMISSION_NONE, bit32 otherPermission = os::MEMORY_PERMISSION_READ_WRITE);
    Result TryInitialize(void* p, size_t size, bit32 myPermission = os::MEMORY_PERMISSION_NONE, bit32 otherPermission = os::MEMORY_PERMISSION_READ_WRITE);
    void Finalize();
        
private:
    friend class HandleManager;
    Result Map(size_t size, bit32 otherPermission, bit32 myPermission);
    Result AttachAndMap(Handle handle, size_t size, bit32 otherPermission, bit32 myPermission);
    void Unmap();
};

}
}