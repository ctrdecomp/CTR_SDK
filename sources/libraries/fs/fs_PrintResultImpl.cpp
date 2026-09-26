// Filename: fs_PrintResultImpl.cpp
//
// Project: Horizon

#include <nn/Result.h>
#include <nn/fs/fs_Result.h>

#define BEGIN_GET_RESULT_DESCRIPTION_STRING_IMPL \
    const char* GetResultDescriptionStringImpl(nn::Result result) \
    { \
        (void)result; \
        if (0) {}

#define CASE_GET_RESULT_DESCRIPTION_STRING_IMPL(nameSpace, description) \
        else if (result.GetDescription() == nameSpace::description) return #description;

#define END_GET_RESULT_DESCRIPTION_STRING_IMPL \
        return 0; \
    }

namespace nn {
namespace fs {
namespace detail {


__weak const char* GetResultPrivateDescriptionStringImpl(nn::Result) { return 0; }

BEGIN_GET_RESULT_DESCRIPTION_STRING_IMPL
    if(const char *str = GetResultPrivateDescriptionStringImpl(result))
    {
        return str;
    }
END_GET_RESULT_DESCRIPTION_STRING_IMPL

void GetResultDescriptionStringImplKeeper()
{
    const char* (*volatile f)(nn::Result) = GetResultDescriptionStringImpl;
}

} // namespace detail
} // namespace fs
} // namespace nn
