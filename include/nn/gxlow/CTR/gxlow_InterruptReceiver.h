#pragma once

#include <nn/drivers/gxlow/CTR/gxlow_InterruptTable.h>
#include <nn/gxlow/CTR/detail/gxlow_DisplaySwapInfoPad.h>
#include <nn/gxlow/CTR/detail/gxlow_InterruptRelayQueue.h>
#include <nn/os/os_HandleManager.h>

namespace nn{
namespace gxlow{
namespace CTR{
namespace detail{

class SharedWorkMem
{
public:
    void Initialize(Handle hSharedMemory)
    {
        nn::os::HandleManager::AttachSharedMemoryHandle(&this->m_SharedMemory, hSharedMemory, 0x1000, false);
    }
    uptr GetBufferForRelayQueue(s32 index)
    {
        uptr addr = m_SharedMemory.GetAddress() + OFFSET_RELAY_QUEUE + index * InterruptRelayQueueBase::QUEUE_BODY_SIZE;
        return addr;
    }
    uptr GetBufferForCmdReqQueue(s32 index)
    {
        uptr addr = m_SharedMemory.GetAddress() + OFFSET_CMDREQ_QUEUE + index * CmdReqQueueBase::QUEUE_BODY_SIZE;
        return addr;
    }
    uptr GetBufferForDisplaySwapInfoPad(s32 index)
    {
        uptr addr = m_SharedMemory.GetAddress() + OFFSET_SWAP_INFO_PAD + index * DisplaySwapInfoPadBase::PAD_BODY_SIZE;
        return addr;
    }
private:
    static const size_t WORK_MEMORY_SIZE     = 0x1000;
    static const u32    OFFSET_RELAY_QUEUE   = 0x0000;
    static const u32    OFFSET_SWAP_INFO_PAD = 0x0200;
    static const u32    OFFSET_CMDREQ_QUEUE  = 0x0800;

    nn::os::SharedMemoryBlock m_SharedMemory;
};
}

class InterruptRelayQueueRx : public detail::InterruptRelayQueueBase
{
public:
    void SuppressPdcEvents(bool enable);
    Result TryDequeue(nngxlowInterrupt* pSrc);
};

class InterruptReceiver : public drivers::gxlow::CTR::InterruptTable
{
private:
    enum HandlerWaitStatus
    {
        RECEIVER_NOT_WAITING,
        RECEIVER_WAITING,
        RECEIVER_ANY_HANDLER_DONE
    };

    os::Event m_RxEvent;
    InterruptRelayQueueRx m_RelayQ;
    detail::SharedWorkMem m_SharedWorkMem;
    CmdReqQueueTx m_CmdReqQ;
    DisplaySwapInfoPadTx m_SwapInfoPad;
    os::LightEvent m_AnyHandlerDoneEvent;
    os::Thread m_ReceiverThread;
    s8 m_GspContextIndex;
    bool m_IsFirstConnection;
    util::SizedEnum1<HandlerWaitStatus> m_HandlerWaitStatus;
    bool m_FinalizeRequest;
    os::StackBuffer<0x1000> m_ThreadStack;
public:
    InterruptReceiver()
    {
    }

    void CallHandlerFunc(s32 index);
    void Initialize();
    void Finalize();
    CmdReqQueueTx& GetCmdReqQueue(){ return m_CmdReqQ; }
    DisplaySwapInfoPadTx& GetSwapInfoPad(){ return m_SwapInfoPad; }
    bool IsFirstConnection(){ return m_IsFirstConnection; }
    static void ReceiverThreadFunc(uptr arg);
    void SuppressPdcEvents(bool enable){ m_RelayQ.SuppressPdcEvents(enable); }
    void WaitAnyHandlerDone();
}; 

}
}
}