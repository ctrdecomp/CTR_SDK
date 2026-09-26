// Filename: cfg_PrintResultImpl.cpp
//
// Project: Horizon

#include <nn/Result.h>
#include <nn/cfg/CTR/cfg_Result.h>

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
namespace cfg {
namespace detail {

BEGIN_GET_RESULT_DESCRIPTION_STRING_IMPL
    CASE_GET_RESULT_DESCRIPTION_STRING_IMPL(::nn::cfg, DESCRIPTION_NOT_VERIFIED)
    CASE_GET_RESULT_DESCRIPTION_STRING_IMPL(::nn::cfg, DESCRIPTION_VERIFICATION_FAILED)
    CASE_GET_RESULT_DESCRIPTION_STRING_IMPL(::nn::cfg, DESCRIPTION_INVALID_NTR_SETTING)
    CASE_GET_RESULT_DESCRIPTION_STRING_IMPL(::nn::cfg, DESCRIPTION_ALREADY_LATEST_VERSION)
    CASE_GET_RESULT_DESCRIPTION_STRING_IMPL(::nn::cfg, DESCRIPTION_MOUNT_CONTENT_FAILED)
    CASE_GET_RESULT_DESCRIPTION_STRING_IMPL(::nn::cfg, DESCRIPTION_OBSOLETE_RESULT)
END_GET_RESULT_DESCRIPTION_STRING_IMPL

void GetResultDescriptionStringImplKeeper()
{
    const char* (*volatile f)(nn::Result) = GetResultDescriptionStringImpl;
}

} // namespace detail
} // namespace cfg
} // namespace nn