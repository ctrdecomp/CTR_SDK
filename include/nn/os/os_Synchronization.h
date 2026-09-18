#pragma once

#include <nn/os/os_HandleObject.h>

namespace nn{
namespace os{

class WaitObject : public HandleObject
{
public:
    nn::Result WaitOneImpl(s64);
    void WaitOne();
    bool WaitOne(nn::fnd::TimeSpan timeout);
protected:
    WaitObject()
    {
    }
    ~WaitObject()
    {
    }
};

inline nn::Result WaitObject::WaitOneImpl(s64 nanoSecondsTimeout)
{
    s32 dummy;
    Handle handle = GetHandle();
    return nn::svc::WaitSynchronizationN(&dummy, &handle, 1, false, nanoSecondsTimeout);
}

inline void WaitObject::WaitOne()
{ 
    NN_OS_ERROR_IF_FAILED(WaitOneImpl(WAIT_INFINITE)); 
}

inline bool WaitObject::WaitOne(nn::fnd::TimeSpan timeout)
{
    nn::Result result = WaitOneImpl(timeout.GetNanoSeconds());
    NN_OS_ERROR_IF_FAILED(result);
    return result.GetDescription() != nn::Result::DESCRIPTION_TIMEOUT;
}

}
}