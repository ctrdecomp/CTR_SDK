// Filename: os_ExceptionHandler.cpp
//
// Project: Horizon

#include <nn/types.h>
#include <nn/os.h>
#include <nn/os/ARM/os_ExceptionHandler.h>
#include <nn/os/CTR/os_ThreadLocalRegion.h>

namespace nn {
namespace os {
namespace ARM {

void SetUserExceptionHandler(UserExceptionHandler pHandler, uptr stackBottom, ExceptionBuffer* pExceptionBuffer);

void SetUserExceptionHandler(UserExceptionHandler pHandler, uptr stackBottom)
{
    SetUserExceptionHandler(pHandler, stackBottom, NULL);
}

void SetUserExceptionHandler(UserExceptionHandler pHandler, uptr stackBottom, ExceptionBuffer* pExceptionBuffer)
{
    CTR::ThreadLocalRegion* pTlr = detail::GetMainThreadThreadLocalRegion();
    NN_TASSERT_(pTlr != NULL);

    pTlr->handlerAddress              = reinterpret_cast<uptr>(pHandler);
    pTlr->handlerStackBottomAddress   = stackBottom;
    pTlr->exceptionBufferAddress      = reinterpret_cast<uptr>(pExceptionBuffer);
}

void SetUserExceptionHandlerLocal(UserExceptionHandler pHandler, uptr stackBottom, ExceptionBuffer* pExceptionBuffer)
{
    os::CTR::ThreadLocalRegion* pTlr = os::CTR::GetThreadLocalRegion();

    pTlr->handlerAddress  = reinterpret_cast<uptr>(pHandler);
    pTlr->handlerStackBottomAddress = stackBottom;
    pTlr->exceptionBufferAddress = reinterpret_cast<uptr>(pExceptionBuffer);
}

}
}
}

