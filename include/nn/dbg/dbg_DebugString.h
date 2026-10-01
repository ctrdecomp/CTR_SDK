#pragma once

#include <nn/Result.h>
#include <cstdarg>

namespace nn{
namespace dbg{
namespace detail{
    void Printf(const char* fmt, ...);
    void TPrintf(const char* fmt, ...);
    void VPrintf(const char* fmt, ::std::va_list arg);
    void TVPrintf(const char* fmt, ::std::va_list arg);
    void PutString(const char* text, s32 length);
    void PutString(const char* text);
}
}
}

extern "C"{
    void nndbgDetailPrintf(const char* fmt, ...);
    void nndbgDetailTPrintf(const char* fmt, ...);
    void nndbgDetailVPrintf(const char* fmt, va_list arg);
    void nndbgDetailTVPrintf(const char* fmt, va_list arg);
    void nndbgDetailPutString(const char* text, s32 length);

    int nndbgAssertionFailureHandler(bool print, const char* filename, int lineno, const char* fmt, ...);
    int nndbgTAssertionFailureHandler(bool print, const char* filename, int lineno, const char* fmt, ...);
}

#if !defined(NN_SWITCH_DISABLE_DEBUG_PRINT_FOR_SDK) || !defined(NN_SWITCH_DISABLE_ASSERT_WARNING_FOR_SDK)
    #ifdef __cplusplus
            #define NN_LOG_(...)           (void)nn::dbg::detail::Printf(__VA_ARGS__)
            #define NN_SLOG_(...)          (void)nn::dbg::detail::Printf(__VA_ARGS__)
            #define NN_TLOG_(...)          (void)nn::dbg::detail::TPrintf(__VA_ARGS__)
        #else
            #define NN_LOG_(...)           (void)nndbgDetailPrintf(__VA_ARGS__)
            #define NN_LOGV_(fmt, arg)     (void)nndbgDetailVPrintf((fmt), (arg))
            #define NN_PUT_(text, length)  (void)nndbgDetailPutString((text), (length))
    #endif
#else

#define NN_LOG_(exp, ...)
#define NN_SLOG_(exp, ...)
#define NN_TLOG_(exp, ...)

#endif