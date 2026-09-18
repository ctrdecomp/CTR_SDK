#pragma once

#include <nn/os/os_Types.h>
#include <nn/os/os_ErrorHandlerSelect.h>
#include <nn/util/util_NonCopyable.h>
#include <nn/svc.h>
#include <nn/Assert.h>

namespace nn {
namespace os {

class HandleObject : util::ADLFireWall::NonCopyable<HandleObject>
{
public:
    Handle m_Handle;
public:
    HandleObject()
    {
    }

    ~HandleObject()
    {
        Close();
    }

    Handle GetHandle() const{ return m_Handle; }
    bool IsValid() const{ return m_Handle.IsValid(); }
    void SetHandle(nn::Handle handle);

    void Close()
    {
        if (IsValid())
        {
            nn::svc::CloseHandle(m_Handle);
            m_Handle = Handle();
        }
    }
    void Finalize()
    {
        Close();
    }

    void ClearHandle()
    {
        m_Handle = Handle();
    }
};

inline void HandleObject::SetHandle(nn::Handle handle)
{
    NN_TASSERTMSG_(!IsValid(), "current handle(=%08X) is active\n", mHandle.GetPrintableBits());
    NN_TASSERT_(handle.IsValid());
    m_Handle = handle;
}

}
}