#pragma once

#include <nn/Handle.h>
#include <nn/Result.h>
#include <nn/types.h>
#include <nn/http/http_Types.h>

namespace nn {
namespace http {

class ConnectionIpc
{
public:
    ConnectionIpc(Handle session): 
        m_Session(session) 
    {
    }

    Result InitializeGeneralSession(Handle hSharedMemory, size_t size);
    Result CreateConnection(const char8 url[], size_t urlLen, RequestMethod reqMethod, ConnectionHandle* handleCourier);
    Result DestroyConnection(ConnectionHandle handle);
    Result CancelConnection(ConnectionHandle handle);
    Result GetConnectionProgress(ConnectionHandle handle, size_t* receivedSizeCourier, size_t* contentSizeCourier);
    Result InitializeConnectionSession(ConnectionHandle handle);
    Result StartConnectionAsync(ConnectionHandle handle);
    Result ReadBody(ConnectionHandle handle, u8 bodyCourier[], size_t bodyCourierLen);
    Result ReadBodyWithTimeout(ConnectionHandle handle, u8 bodyCourier[], size_t bodyCourierLen, s64 timeout);
    Result SetProxyDefault(ConnectionHandle handle);
    Result AddHeaderField(ConnectionHandle handle, const char8 label[], size_t labelLen, const char8 value[], size_t valueLen);
    Result SetLazyPostDataSetting(ConnectionHandle handle, PostDataType dataType);
    Result SendPostDataRaw(ConnectionHandle handle, const u8 value[], size_t valueLen);
    Result SendPostDataRawWithTimeout(ConnectionHandle handle, const u8 value[], size_t valueLen, s64 timeout);
    Result GetHeaderField(ConnectionHandle handle, const char8 label[], size_t labelLen, char8 valueCourier[], size_t valueCourierLen, u32* fieldLenCourier);
    Result GetHeaderAll(ConnectionHandle handle, char8 headerCourier[], size_t headerCourierLen, u32* allHeaderLenCourier);
    Result GetResultCode(ConnectionHandle handle, s32* resultCodeCourier );
    Result GetResultCodeWithTimeout(ConnectionHandle handle, s32* resultCodeCourier, s64 timeout );
    Result SetRootCA(ConnectionHandle handle, const u8 rootCaData[], size_t rootCaDataLen );
    Result SetInternalRootCA(ConnectionHandle handle, InternalCaCertId inCaCertName);
    Result GetConnectionStatus(ConnectionHandle handle, Status* statusCourier);
    Result GetConnectionSslError(ConnectionHandle handle, s32* resultCodeCourier);
    Result SetVerifyOption(ConnectionHandle handle, u32 verifyOption);
    Result SetLazyPostDataSettingWithSize(ConnectionHandle handle, PostDataType dataType, size_t dataLen);

private:
    Handle m_Session;
};

} // namespace http
} // namespace nn