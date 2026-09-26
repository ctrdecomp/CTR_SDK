#pragma once

#include <nn/WithInitialize.h>
#include <nn/os/os_SimpleLock.h>
#include <nn/os/os_Types.h>
#include <nn/hardware/hardware_RegAccess.h>

namespace nn { 
namespace os {
class CriticalSection : private nn::util::ADLFireWall::NonCopyable<CriticalSection>
{
private:
    SimpleLock m_Lock;
    WaitableCounter m_Counter;
    u32 m_ThreadUniqueValue;
    s32 m_LockCount;

public:

    CriticalSection() : m_ThreadUniqueValue(GetInvalidThreadUniqueValue()), m_LockCount(-1) 
    {
    }
    CriticalSection(const nn::WithInitialize&) { this->Initialize(); }

    void Initialize();
    void Enter();
    void Leave();
    void Initialize();
    bool TryEnter();

    Result TryInitialize()
    {
        this->Initialize();
        return ResultSuccess();
    }

    void Finalize(){ this->m_LockCount = -1;}
    ~CriticalSection() 
    { 
    }

    class ScopedLock;

    void OnLocked()
    {
        this->m_ThreadUniqueValue = GetThreadUniqueValue();
    }

    bool LockedByCurrentThread() const
    {
        return GetThreadUniqueValue() == m_ThreadUniqueValue;
    }
private:
    static uptr GetThreadUniqueValue()
    {
        uptr v;
        HW_GET_CP15_THREAD_ID_USER_READ_ONLY(v);
        return v;
    }

    static uptr GetInvalidThreadUniqueValue()
    {
        return static_cast<uptr>(-1);
    }

    bool IsInitialized() const
    {
        return this->m_LockCount >= 0;
    }
};
    
NN_UTIL_DETAIL_DEFINE_SCOPED_LOCK(CriticalSection, Enter(), Leave());

}
}