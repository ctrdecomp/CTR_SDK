#pragma once

#include <nn/Result.h>
#include <nn/http/http_ConnectionIpc.h>

namespace nn {
namespace http {

Result Initialize(uptr bufferAddress = 0, size_t bufferSize = 0);
Result Finalize();

} // namespace http
} // namespace nn

