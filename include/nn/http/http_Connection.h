#pragma once

#include <nn/Result.h>
#include <nn/Handle.h>
#include <nn/http/http_Types.h>
#include <nn/http/http_ConnectionIpc.h>
#include <nn/http/http_ClientCert.h>
#include <nn/util/util_NonCopyable.h>
#include <nn/fnd/fnd_TimeSpan.h>

namespace nn {
namespace http {

class Connection : private nn::util::ADLFireWall::NonCopyable<Connection>
{
public:
    explicit Connection();
    explicit Connection(const char* pUrl, RequestMethod method = REQUEST_METHOD_GET, bool isUseDefaultProxy = true);

    virtual ~Connection();

    Result Initialize(const char* pUrl, RequestMethod method = REQUEST_METHOD_GET, bool isUseDefaultProxy = true);
    Result Finalize();

    Result ConnectAsync();
    Result Cancel();

    Result GetStatus(Status* pStatusBuf) const;
    Result GetProgress(size_t* pReceivedLen, size_t* pContentLen) const;

    Result Read(u8* pBodyBuf, size_t bufLen);
    Result Read(u8* pBodyBuf, size_t bufLen, const nn::fnd::TimeSpan& timeout);

    Result GetHeaderField(const char* pLabel, char* pFieldBuf, size_t bufSize, size_t* pFieldLengthCourier = NULL) const;
    Result GetHeaderField(const char* pLabel, char* pFieldBuf, size_t bufSize, const nn::fnd::TimeSpan& timeout, size_t* pFieldLengthCourier = NULL) const;
    Result GetHeaderAll(char* pHeaderBuf, size_t bufSize, size_t* pLengthCourier = NULL) const;
    Result GetHeaderAll(char* pHeaderBuf, size_t bufSize, const nn::fnd::TimeSpan& timeout, size_t* pLengthCourier = NULL) const;
    Result GetStatusCode(s32* pStatusCodeCourier) const;
    Result GetStatusCode(s32* pStatusCodeCourier, const nn::fnd::TimeSpan& timeout) const;

    Result AddHeaderField(const char* pLabel, const char* pValue);

    Result SetLazyPostDataSetting();
    Result SetLazyPostDataSetting(PostDataType dataType);
    Result SendPostDataRaw(const void* pValue, size_t valueSize);
    Result SendPostDataRaw(const void* pValue, size_t valueSize, const nn::fnd::TimeSpan& timeout);
    Result SetRootCa(const u8* pCertData, size_t certDataSize);
    Result SetRootCa(InternalCaCertId inCaCertName);
    Result SetClientCert(InternalClientCertId inClientCertName);
    Result SetVerifyOption(u32 verifyOption);

    Result GetSslError(s32* pResultCodeBuf) const;

    Result NotifyFinishSendPostData();
private:
    Result SetProxyDefault();
};

} // namespace http
} // namespace nn

