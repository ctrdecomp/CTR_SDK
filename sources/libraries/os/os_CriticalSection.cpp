// Filename: os_CriticalSection.cpp
//
// Project: Horizon

#include <nn/os/os_CriticalSection.h>
#include <nn/dbg/dbg_Break.h>
#include <nn/Assert.h>

namespace nn{
namespace os{

void CriticalSection::EnterImpl()
{
    for(;;)
    {
        if(*m_Counter > 0)
        {
            if(TryEnterImpl())
            {
                break;
            }
        }

        this->m_Counter.DecrementAndWaitIfLessThan(0);
    }
}

}
}
