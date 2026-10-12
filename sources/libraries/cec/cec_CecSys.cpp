// Filename: cec_CecSys.cpp
//
// Project: Horizon

#include <nn/cec/CTR/cec_CecSys.h>
#include <nn/os/ipc/os_Message.h>
#include <nn/svc.h>

namespace nn {
namespace cec {
namespace CTR {
namespace detail {

Handle CecSys::s_Session;

Result CecSys::Open(u32 cecTitleId, u32 dataType, u32 option, size_t* filesize)
{
    MessageBuffer ipcMsg(GetMessageBuffer());
    ipcMsg.SetHeader(1, 3, 2, 0);
    ipcMsg.SetRaw(1, cecTitleId);
    ipcMsg.SetRaw(2, dataType);
    ipcMsg.SetRaw(3, option);
    ipcMsg.SetProcessIdHeader(4);


    Result ipcResult = SendSyncRequest(s_Session);
    if(ipcResult.IsFailure())
    {
        return ipcResult;
    }

    *filesize = ipcMsg.GetRaw<size_t>(2);

    return ipcMsg.GetRaw<Result>(1);
}

Result CecSys::Read(size_t* pReadLen, u8 pReadBuf[], size_t len)
{
    MessageBuffer ipcMsg(GetMessageBuffer());
    ipcMsg.SetHeader(2, 1, 2, 0);
    ipcMsg.SetRaw(1, len);
    ipcMsg.SetReceive(2, pReadBuf, sizeof(*pReadBuf) * len);


    Result ipcResult = SendSyncRequest(s_Session);
    if(ipcResult.IsFailure())
    {
        return ipcResult;
    }

    *pReadLen = ipcMsg.GetRaw<size_t>(2);

    return ipcMsg.GetRaw<Result>(1);
}

Result CecSys::ReadMessage(u32 cecTitleId, u8 in_or_out_box, const u8 pMessId[], size_t messIdLen, size_t* pReadLen, u8 pReadBuf[], size_t len)
{
    MessageBuffer ipcMsg(GetMessageBuffer());
    ipcMsg.SetHeader(3, 4, 4, 0);
    ipcMsg.SetRaw(1, cecTitleId);
    ipcMsg.SetRaw(2, in_or_out_box);
    ipcMsg.SetRaw(3, messIdLen);
    ipcMsg.SetRaw(4, len);
    ipcMsg.SetSend(5, pMessId, sizeof(*pMessId) * messIdLen);
    ipcMsg.SetReceive(7, pReadBuf, sizeof(*pReadBuf) * len);


    Result ipcResult = SendSyncRequest(s_Session);
    if(ipcResult.IsFailure())
    {
        return ipcResult;
    }

    *pReadLen = ipcMsg.GetRaw<size_t>(2);

    return ipcMsg.GetRaw<Result>(1);
}

Result CecSys::Write(const u8 pWriteBuf[], size_t len)
{
    MessageBuffer ipcMsg(GetMessageBuffer());
    ipcMsg.SetHeader(5, 1, 2, 0);
    ipcMsg.SetRaw(1, len);
    ipcMsg.SetSend(2, pWriteBuf, sizeof(*pWriteBuf) * len);


    Result ipcResult = SendSyncRequest(s_Session);
    if(ipcResult.IsFailure())
    {
        return ipcResult;
    }

    return ipcMsg.GetRaw<Result>(1);
}

Result CecSys::WriteMessageWithHmac(u32 cecTitleId, u8 in_or_out_box, u8 pMessId[], size_t messIdLen, const u8 pWriteBuf[], size_t len, const u8 pHmacKey[])
{
    MessageBuffer ipcMsg(GetMessageBuffer());
    ipcMsg.SetHeader(7, 4, 6, 0);
    ipcMsg.SetRaw(1, cecTitleId);
    ipcMsg.SetRaw(2, in_or_out_box);
    ipcMsg.SetRaw(3, messIdLen);
    ipcMsg.SetRaw(4, len);
    ipcMsg.SetSend(5, pWriteBuf, sizeof(*pWriteBuf) * len);
    ipcMsg.SetSend(7, pHmacKey, sizeof(*pHmacKey) * 32);
    ipcMsg.SetExchange(9, pMessId, sizeof(*pMessId) * messIdLen);


    Result ipcResult = SendSyncRequest(s_Session);
    if(ipcResult.IsFailure())
    {
        return ipcResult;
    }

    return ipcMsg.GetRaw<Result>(1);
}

Result CecSys::Delete(u32 cecTitleId, u32 dataType, u8 in_or_out_box, const u8 pMessId[], size_t messIdLen)
{
    MessageBuffer ipcMsg(GetMessageBuffer());
    ipcMsg.SetHeader(8, 4, 2, 0);
    ipcMsg.SetRaw(1, cecTitleId);
    ipcMsg.SetRaw(2, dataType);
    ipcMsg.SetRaw(3, in_or_out_box);
    ipcMsg.SetRaw(4, messIdLen);
    ipcMsg.SetSend(5, pMessId, sizeof(*pMessId) * messIdLen);
    

    Result ipcResult = SendSyncRequest(s_Session);
    if(ipcResult.IsFailure())
    {
        return ipcResult;
    }

    return ipcMsg.GetRaw<Result>(1);
}

Result CecSys::SetData(u32 cecTitleId, const u8 pData[], size_t len, u32 option)
{
    MessageBuffer ipcMsg(GetMessageBuffer());
    ipcMsg.SetHeader(9, 3, 2, 0);
    ipcMsg.SetRaw(1, cecTitleId);
    ipcMsg.SetRaw(2, len);
    ipcMsg.SetRaw(3, option);
    ipcMsg.SetSend(4, pData, sizeof(*pData) * len);


    Result ipcResult = SendSyncRequest(s_Session);
    if(ipcResult.IsFailure())
    {
        return ipcResult;
    }

    return ipcMsg.GetRaw<Result>(1);
}

Result CecSys::ReadData(u8 pReadBuf[], size_t len, u32 option, const u8 optionData[], size_t optionDataLen)
{
    MessageBuffer ipcMsg(GetMessageBuffer());
    ipcMsg.SetHeader(10, 3, 4, 0);
    ipcMsg.SetRaw(1, len);
    ipcMsg.SetRaw(2, option);
    ipcMsg.SetRaw(3, optionDataLen);
    ipcMsg.SetSend(4, optionData, sizeof(*optionData) * optionDataLen);
    ipcMsg.SetReceive(6, pReadBuf, sizeof(*pReadBuf) * len);


    Result ipcResult = SendSyncRequest(s_Session);
    if(ipcResult.IsFailure())
    {
        return ipcResult;
    }

    return ipcMsg.GetRaw<Result>(1);
}

Result CecSys::Start(u32 option)
{
    MessageBuffer ipcMsg(GetMessageBuffer());
    ipcMsg.SetHeader(11, 1, 0, 0);
    ipcMsg.SetRaw(1, option);


    Result ipcResult = SendSyncRequest(s_Session);
    if(ipcResult.IsFailure())
    {
        return ipcResult;
    }

    return ipcMsg.GetRaw<Result>(1);
}

Result CecSys::Stop(u32 option)
{
    MessageBuffer ipcMsg(GetMessageBuffer());
    ipcMsg.SetHeader(12, 1, 0, 0);
    ipcMsg.SetRaw(1, option);


    nn::Result ipcResult = SendSyncRequest(s_Session);
    if(ipcResult.IsFailure())
    {
        return ipcResult;
    }

    return ipcMsg.GetRaw<Result>(1);
}

Result CecSys::GetCecdState(u32* state)
{
    MessageBuffer ipcMsg(GetMessageBuffer());
    ipcMsg.SetHeader(14, 0, 0, 0);


    Result ipcResult = SendSyncRequest(s_Session);
    if(ipcResult.IsFailure())
    {
        return ipcResult;
    }

    *state = ipcMsg.GetRaw<u32>(2);

    return ipcMsg.GetRaw<Result>(1);
}

Result CecSys::GetChangeStateEventHandle(Handle* pEventHandle)
{
    MessageBuffer ipcMsg(GetMessageBuffer());
    ipcMsg.SetHeader(16, 0, 0, 0);


    Result ipcResult = SendSyncRequest(s_Session);
    if(ipcResult.IsFailure())
    {
        return ipcResult;
    }

    *pEventHandle = ipcMsg.GetHandle(3);

    return ipcMsg.GetRaw<Result>(1);
}

} // namespace detail
} // namespace CTR
} // namespace cec
} // namespace nn

