#pragma once

#include <nn/Result.h>

namespace nn  {
namespace cfg {

    enum Description
    {
        DESCRIPTION_NOT_VERIFIED                        = 1,
        DESCRIPTION_VERIFICATION_FAILED                 = 2,
        DESCRIPTION_INVALID_NTR_SETTING                 = 3,
        DESCRIPTION_ALREADY_LATEST_VERSION              = 4,
        DESCRIPTION_MOUNT_CONTENT_FAILED                = 5,
        DESCRIPTION_INVALID_TARGET                      = 6,
        DESCRIPTION_OBSOLETE_RESULT                     = 1023
    };

NN_DEFINE_RESULT_CONST(ResultCancelRequested, Result::LEVEL_PERMANENT, Result::SUMMARY_CANCELLED, Result::MODULE_NN_CFG, Result::DESCRIPTION_CANCEL_REQUESTED);
NN_DEFINE_RESULT_CONST(ResultNotAuthorized, Result::LEVEL_PERMANENT, Result::SUMMARY_WRONG_ARGUMENT, Result::MODULE_NN_CFG, Result::DESCRIPTION_NOT_AUTHORIZED);
NN_DEFINE_RESULT_CONST(ResultInvalidHandle, Result::LEVEL_PERMANENT, Result::SUMMARY_INVALID_STATE, Result::MODULE_NN_CFG, Result::DESCRIPTION_INVALID_HANDLE);
NN_DEFINE_RESULT_CONST(ResultAlreadyInitialized, Result::LEVEL_STATUS, Result::SUMMARY_INVALID_STATE, Result::MODULE_NN_CFG, Result::DESCRIPTION_ALREADY_INITIALIZED);

} // namespace cfg
} // namespace nn

