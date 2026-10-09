#pragma once

#include <nn/Result.h>
#include <nn/util/util_Result.h>
#include <nn/os/ARM/os_ExceptionHandler.h>
#include <nn/err/CTR/err_FatalErrTypes.h>

namespace nn {
namespace err {
namespace CTR {

namespace {
    const char PORT_NAME_ERR_F[] = "err:f";
} // namespace

    struct FatalErrInfo
    {
        bit8 type;
        u8 revisionHi;
        ushort revisionLo;
        nnResult result;
        uptr pc;
        bit32 processId;
        bit64 titleId;
        bit64 appTitleId;
        union Data
        {
            union Exception
            {
                u8 info[24];
                nn::os::ARM::ExceptionContext context;
            } exception;

            union Failure
            {
                char message[96];
            } failure;
        } data;
    };

    class FatalErr
    {
    public:
        Handle m_Session;
        
        FatalErr(Handle h){ m_Session = h; }
        Result Throw(err::CTR::FatalErrInfo& info);
    };

    void ThrowFatalErr(Result result, nnerrFatalErrType type, uint pc);
    void ThrowFatalErr(Result result, nnerrFatalErrType type);
    void ThrowFatalErr(Result res);
    void ThrowFatalErrAll(Result res);
} // namespace CTR
} // namespace err
} // namespace nn

#define NN_ERR_THROW_FATAL(result)                          \
    do                                                      \
    {                                                       \
        ::nn::Result resultLocal = (result);                \
        if (resultLocal.IsFailure())                        \
        {                                                   \
            ::nn::err::CTR::ThrowFatalErr(resultLocal);     \
        }                                                   \
    } while(0)


#define NN_ERR_THROW_FATAL_ALL(result)                      \
    do                                                      \
    {                                                       \
        ::nn::Result resultLocal = (result);                \
        if (resultLocal.IsFailure())                        \
        {                                                   \
            ::nn::err::CTR::ThrowFatalErrAll(resultLocal);  \
        }                                                   \
    } while (0)

#define NN_ERR_LOG_AND_PANIC_IF_FAILED(result)              \
    NN_ERR_THROW_FATAL_ALL(result)

