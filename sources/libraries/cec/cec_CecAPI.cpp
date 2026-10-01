// Filename: cec_CecAPI.cpp
//
// Project: Horizon

#include <nn/svc.h>
#include <nn/srv.h>
#include <nn/Result.h>
#include <nn/Handle.h>
#include <nn/os/os_HandleManager.h>
#include <nn/os/os_Thread.h>
#include <nn/cec.h>

#include <nn/cec/CTR/cec_Api.h>
#include <nn/cec/CTR/cec_CecSys.h>
#include <nn/dbg/dbg_PrintResult.h>

NN_DBG_DECLARE_GET_RESULT_DESCRIPTION_STRING_IMPL_KEEPER(cec)

using namespace nn;
using namespace nn::cec::CTR;

namespace nn {
namespace cec {
namespace CTR {
namespace detail {

Result InitializeCecControl()
{
    NN_DBG_USE_GET_RESULT_DESCRIPTION_STRING_IMPL_KEEPER(cec);

    Result result = nn::srv::Initialize();
    NN_UTIL_RETURN_IF_FAILED(result);

    result = nn::srv::GetServiceHandle(&Cec::s_Session, PORT_NAME_CEC);

    return result;
}

Result FinalizeCecControl()
{
    if (!detail::Cec::s_Session.IsValid())
    {
        return ResultNotInitialized();
    }

    return nn::svc::CloseHandle(Cec::s_Session);
}

Result InitializeCecControlSys()
{
    Result result = nn::srv::Initialize();
    NN_UTIL_RETURN_IF_FAILED(result);

    result = nn::srv::GetServiceHandle(&CecSys::s_Session, PORT_NAME_CEC_SYS);

    return result;
}

Result FinalizeCecControlSys()
{
    if (!detail::CecSys::s_Session.IsValid())
    {
        return ResultNotInitialized();
    }

    return nn::svc::CloseHandle(CecSys::s_Session);
}

Result WaitForSessionValid()
{
    while (!detail::Cec::s_Session.IsValid())
    {
        nn::os::Thread::Sleep(nn::fnd::TimeSpan::FromMilliSeconds(10));
    }

   return ResultSuccess();
}

Result Open(u32 cecTitleId, u32 dataType, u32 option, size_t* filesize)
{
    if(detail::CecSys::s_Session.IsValid())
    {
        return detail::CecSys::Open(cecTitleId, dataType, option, filesize);
    }
    else if(detail::Cec::s_Session.IsValid())
    {
        return detail::Cec::Open(cecTitleId, dataType, option, filesize);
    }

    return ResultNotInitialized();
}

Result Read(size_t* pReadLen, u8 pReadBuf[], size_t len)
{
    if(detail::CecSys::s_Session.IsValid())
    {
        return detail::CecSys::Read(pReadLen, pReadBuf, len);
    }
    else if(detail::Cec::s_Session.IsValid())
    {
        return detail::Cec::Read(pReadLen, pReadBuf, len);
    }

    return ResultNotInitialized();
}

Result Write(const u8 pWriteBuf[], size_t len)
{
    if(detail::CecSys::s_Session.IsValid())
    {
        return detail::CecSys::Write(pWriteBuf, len);
    }
    else if(detail::Cec::s_Session.IsValid())
    {
        return detail::Cec::Write(pWriteBuf, len);
    }

    return ResultNotInitialized();
}

Result WriteMessageWithHmac(u32 cecTitleId, u8 in_or_out_box, u8 pMessId[], size_t messIdLen, const u8 pWriteBuf[], size_t len ,const u8 pHmac[])
{
    if(detail::CecSys::s_Session.IsValid())
    {
        return detail::CecSys::WriteMessageWithHmac(cecTitleId, in_or_out_box, pMessId, messIdLen, pWriteBuf, len , pHmac);
    }
    else if(detail::Cec::s_Session.IsValid())
    {
        return detail::Cec::WriteMessageWithHmac(cecTitleId, in_or_out_box, pMessId, messIdLen, pWriteBuf, len , pHmac);
    }

    return ResultNotInitialized();
}

Result Delete(u32 cecTitleId, u32 dataType, u8 in_or_out_box, const u8 pMessId[], size_t messIdLen)
{
    if(detail::CecSys::s_Session.IsValid())
    {
        return detail::CecSys::Delete(cecTitleId, dataType, in_or_out_box, pMessId, messIdLen);
    }
    else if(detail::Cec::s_Session.IsValid())
    {
        return detail::Cec::Delete(cecTitleId, dataType, in_or_out_box, pMessId, messIdLen);
    }

    return ResultNotInitialized();
}

Result Start(u32 option)
{
    if(detail::CecSys::s_Session.IsValid())
    {
        return detail::CecSys::Start(option);
    }
    else if(detail::Cec::s_Session.IsValid())
    {
        return detail::Cec::Start(option);
    }

    return ResultNotInitialized();
}

Result Stop(u32 option)
{
    if(detail::CecSys::s_Session.IsValid())
    {
        return detail::CecSys::Stop(option);
    }
    else if(detail::Cec::s_Session.IsValid())
    {
        return detail::Cec::Stop(option);
    }

    return ResultNotInitialized();
}

Result SetData(u32 cecTitleId, const u8 pData[], size_t len, u32 option)
{
    if(detail::CecSys::s_Session.IsValid())
    {
        return detail::CecSys::SetData(cecTitleId, pData, len, option);
    }
    else if(detail::Cec::s_Session.IsValid())
    {
        return detail::Cec::SetData(cecTitleId, pData, len, option);
    }

    return ResultNotInitialized();
}

Result ReadData(u8 pReadBuf[], size_t len, u32 option , const u8 optionData[], size_t optionDataLen)
{
    if(detail::CecSys::s_Session.IsValid())
    {
        return detail::CecSys::ReadData(pReadBuf, len, option , optionData, optionDataLen);
    }
    else if(detail::Cec::s_Session.IsValid())
    {
        return detail::Cec::ReadData(pReadBuf, len, option, optionData, optionDataLen);
    }

    return ResultNotInitialized();
}

Result GetChangeStateEventHandle(Handle* pEventHandle)
{
    if(detail::CecSys::s_Session.IsValid())
    {
        return detail::CecSys::GetChangeStateEventHandle(pEventHandle);
    }
    else if(detail::Cec::s_Session.IsValid())
    {
        return detail::Cec::GetChangeStateEventHandle(pEventHandle);
    }

    return ResultNotInitialized();
}

Result GetCecdState(u32* state)
{
    if(detail::CecSys::s_Session.IsValid())
    {
        return detail::CecSys::GetCecdState(state);
    }
    else if(detail::Cec::s_Session.IsValid())
    {
        return detail::Cec::GetCecdState(state);
    }

    return ResultNotInitialized();
}

} // namespace detail
} // namespace CTR
} // namespace cec
} // namespace nn