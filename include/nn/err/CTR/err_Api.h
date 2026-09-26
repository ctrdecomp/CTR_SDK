#pragma once

#include <nn/Result.h>
#include <nn/util/util_Result.h>
#include <nn/os/ARM/os_ExceptionHandler.h>
#include <nn/err/CTR/err_FatalErrTypes.h>

namespace nn{
namespace err{
namespace CTR{
namespace{
    const char PORT_NAME_ERR_F[] = "err:f";
}

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

    void ThrowFatalErr(Result result, uptr pc);
    void ThrowFatalErr(Result result, nnerrFatalErrType type, uptr pc);
    void ThrowFatalErrAll(Result, uptr pc);

namespace detail
{
    template <bool(*IsTarget)(Result), void(*TargetFunc)(Result, uptr)>
    inline void CallIf(Result r, uptr pc)
    {
        if (IsTarget(r))
        {
            TargetFunc(r, pc);
        }
    }

    template <bool(*IsTarget)(Result), void(*TargetFunc)(Result, const char*, int, uptr)>
    inline void CallIf(Result r, const char* fileName, int lineno, uptr pc)
    {
        if (IsTarget(r))
        {
            TargetFunc(r, fileName, lineno, pc);
        }
    }

    inline bool IsResultFailure(Result r) {return r.IsFailure(); }
    inline bool IsResultFatal  (Result r) {return r.GetLevel() == ::Result::LEVEL_FATAL; }

}
} 
}
}



    #define NN_ERR_CTR_ERR_API_H_CALL_IF(result, test, f) \
        ::nn::err::CTR::detail::CallIf \
            < ::nn::err::CTR::detail::test, \
            ::nn::err::CTR::f >(result, __current_pc())

    #define NN_ERR_CTR_ERR_API_H_CALL_IF2(result, test, f) \
        ::nn::err::CTR::detail::CallIf \
            < ::nn::err::CTR::detail::test, \
            ::nn::err::CTR::f >(result, NN_FILE_NAME, __LINE__, __current_pc())

    #define NN_ERR_THROW_FATAL_IF_FATAL_ONLY(result) \
        NN_ERR_CTR_ERR_API_H_CALL_IF(result, IsResultFatal, ThrowFatalErrAll)

    #define NN_ERR_THROW_FATAL(result) \
        NN_ERR_CTR_ERR_API_H_CALL_IF(result, IsResultFailure, ThrowFatalErr)

    #define NN_ERR_THROW_FATAL_ALL(result) \
        NN_ERR_CTR_ERR_API_H_CALL_IF(result, IsResultFailure, ThrowFatalErrAll)

    #ifndef NN_SWITCH_DISABLE_DEBUG_PRINT_FOR_SDK
        #define NN_ERR_LOG_AND_PANIC_IF_FAILED(result) \
            NN_ERR_CTR_ERR_API_H_CALL_IF2(result, IsResultFailure, LogAndPanic)

    #else

        #define NN_ERR_LOG_AND_PANIC_IF_FAILED(result) \
            NN_ERR_CTR_ERR_API_H_CALL_IF(result, IsResultFailure, LogAndPanic)

    #endif

#define NN_ERR_THROW_FATAL_IF_FATAL_ONLY(result) \
    NN_UTIL_PANIC_IF_FAILED(result)

#define NN_ERR_THROW_FATAL(result) \
    NN_UTIL_PANIC_IF_FAILED(result)

#define NN_ERR_THROW_FATAL_ALL(result) \
    NN_UTIL_PANIC_IF_FAILED(result)

#define NN_ERR_LOG_AND_PANIC_IF_FAILED(result) \
    NN_UTIL_PANIC_IF_FAILED(result)