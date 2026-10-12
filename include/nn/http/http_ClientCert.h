#pragma once

#include <nn/Result.h>
#include <nn/util/util_NonCopyable.h>
#include <nn/http/http_Types.h>

namespace nn {
namespace http {

class Connection;

class ClientCert : private nn::util::ADLFireWall::NonCopyable<ClientCert>
{
    friend class Connection;
public:
    ClientCert();

    virtual ~ClientCert ();

private:
    bool m_isInitialized;
    u8 padding[3];
    CertId m_certId;

    bool IsValid(){return m_isInitialized;}
};

} // namespace http
} // namespace nn

