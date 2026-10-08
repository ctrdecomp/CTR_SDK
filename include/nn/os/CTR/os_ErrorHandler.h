#pragma once

#include <nn/Result.h>

namespace nn{
namespace os{
namespace CTR{
namespace detail{

void HandleInternalError(Result result, const char* fileName, int lineNo);
void HandleInternalError(Result result);
void SetInternalErrorHandlingMode(bool preferFatal);

}
}
}
}
