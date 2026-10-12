// Filename: os_LightEvent.cpp
//
// Project: Horizon

#include <nn/os/os_LightEvent.h>
#include <nn/svc.h>

namespace nn{
namespace os{

void LightEvent::Initialize(bool pIsManualReset)
{
    m_Lock.Initialize();
    *m_Counter = pIsManualReset ? NOT_RESETED_MANUAL: NOT_RESETED_AUTO;
}

void LightEvent::ClearSignal()
{
    if(*m_Counter == RESETED_MANUAL)
    {
        SimpleLock::ScopedLock lock(m_Lock);
        *m_Counter = NOT_RESETED_MANUAL;
    }
    else if(*m_Counter == RESETED_AUTO)
    {
        *m_Counter = NOT_RESETED_AUTO;
    }
}

void LightEvent::Wait()
{
    for(;;)
    {
        switch (*m_Counter)
        {
        case NOT_RESETED_MANUAL:
            m_Counter.WaitIfLessThan(0);
            return;
        case RESETED_MANUAL:
            return;
        case NOT_RESETED_AUTO:
            break;
        case RESETED_AUTO:
            if (m_Counter->CompareAndSwap (RESETED_AUTO, NOT_RESETED_AUTO) == RESETED_AUTO)
            {
                return;
            }
            break;
        }
        m_Counter.WaitIfLessThan (0);
    }
}

void LightEvent::Signal()
{
    if(*m_Counter == NOT_RESETED_AUTO)
    {
        *m_Counter = RESETED_AUTO;
        m_Counter.Signal(1);
    }
    else if(*m_Counter == NOT_RESETED_MANUAL)
    {
        SimpleLock::ScopedLock lock(this->m_Lock);
        *m_Counter = RESETED_MANUAL;
        m_Counter.SignalAll();
    }
}

bool LightEvent::TryWait()
{
    if(*m_Counter == RESETED_MANUAL)
    {
        return true;
    }
    else
    {
        return m_Counter->CompareAndSwap(RESETED_AUTO, NOT_RESETED_AUTO) == RESETED_AUTO;
    }
}

} // namespace os
} // namespace nn

