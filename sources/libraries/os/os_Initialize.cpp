// Filename: os_Initialize.cpp
//
// Project: Horizon

#include <nn/os.h>

NN_DBG_DECLARE_GET_RESULT_DESCRIPTION_STRING_IMPL_KEEPER(os)
NN_DBG_DECLARE_GET_RESULT_DESCRIPTION_STRING_IMPL_KEEPER(fnd)

namespace nn{
namespace os{
    
void Initialize()
{
    WaitableCounter::Initialize();
    detail::InitializeSharedMemory();
    detail::InitializeStackMemory();
    
    NN_DBG_USE_GET_RESULT_DESCRIPTION_STRING_IMPL_KEEPER(os);
    NN_DBG_USE_GET_RESULT_DESCRIPTION_STRING_IMPL_KEEPER(fnd);

    detail::InitializeThreadEnvrionment();
}

}
}

extern "C" {

void nnosInitialize()
{
    nn::os::Initialize();
}

}
