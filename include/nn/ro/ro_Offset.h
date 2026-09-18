#pragma once

#include <nn/types.h>

namespace nn { 
namespace ro {

template <typename T>
class OffsetPointer
{
private:
    uptr    m_Value;
public:
    operator T*()                  { return &**this; }
    operator const T*() const      { return &**this; }
    operator uptr() const          { return m_Value; }

    T* GetPointer(uptr baseAddr) const { return reinterpret_cast<T*>(m_Value + baseAddr); }
    T* GetPointer(const void* baseAddr) const { return GetPointer(reinterpret_cast<uptr>(baseAddr)); }
    void SetPointer(uptr addr) { m_Value = addr; }
    void SetPointer(int addr) { m_Value = static_cast<uptr>(addr); }
    void SetPointer(void* addr) { m_Value = reinterpret_cast<uptr>(addr); }
};

}
}