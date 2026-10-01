#pragma once

#include <nn/types.h>

typedef u32 NnHttpCertId;
typedef u32 NnHttpInternalCaCertId;
typedef u32 NnHttpInternalClientCertId;

namespace nn {
namespace http {

enum RequestMethod
{
    REQUEST_METHOD_NONE,
    REQUEST_METHOD_GET,
    REQUEST_METHOD_POST,
    REQUEST_METHOD_HEAD,
    REQUEST_METHOD_PUT,
    REQUEST_METHOD_DELETE,
    REQUEST_METHOD_POST_NODATA,
    REQUEST_METHOD_PUT_NODATA
};

enum Status
{
    STATUS_CREATED,
    STATUS_INITIALIZED,
    STATUS_ENQUEUED_LSN,
    STATUS_IN_LSN,
    STATUS_ENQUEUED_COMM,
    STATUS_CONNECTING,
    STATUS_SENDING,
    STATUS_RECEIVING_HEADER,
    STATUS_RECEIVING_BODY,
    STATUS_RECEIVED,
    STATUS_FINISHED
};

enum PostDataType
{
    POST_DATA_TYPE_URLENCODE,
    POST_DATA_TYPE_MULTIPART,
    POST_DATA_TYPE_RAW
};

typedef NnHttpCertId CertId;
typedef NnHttpInternalCaCertId InternalCaCertId;
typedef NnHttpInternalClientCertId InternalClientCertId;

typedef u32 CertStoreId;
typedef s32 ConnectionHandle;

} // namespace http
} // namespace nn