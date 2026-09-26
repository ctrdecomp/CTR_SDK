// Filename: fnd_PrintResultImpl.cpp
//
// Project: Horizon

#include <nn/Result.h>
#include <nn/fnd/fnd_Result.h>

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
namespace fnd {
namespace detail {


BEGIN_GET_RESULT_DESCRIPTION_STRING_IMPL
    CASE_GET_RESULT_DESCRIPTION_STRING_IMPL(::nn::fnd, DESCRIPTION_INVALID_NODE)
    CASE_GET_RESULT_DESCRIPTION_STRING_IMPL(::nn::fnd, DESCRIPTION_ALREADY_LISTED)
    CASE_GET_RESULT_DESCRIPTION_STRING_IMPL(::nn::fnd, DESCRIPTION_OUT_OF_RANGE)
    CASE_GET_RESULT_DESCRIPTION_STRING_IMPL(::nn::fnd, DESCRIPTION_OBSOLETE_RESULT)
END_GET_RESULT_DESCRIPTION_STRING_IMPL

void GetResultDescriptionStringImplKeeper()
{
    const char* (*volatile f)(nn::Result) = GetResultDescriptionStringImpl;
}

} // namespace detail
} // namespace fnd
} // namespace nn
