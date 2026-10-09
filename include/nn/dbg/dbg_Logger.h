#pragma once

#include <nn/types.h>
#include <nn/dbg/dbg_DebugString.h>

namespace nn { 
namespace dbg { 
namespace detail {

class Logger
{
private:
    static u32 s_UpperLevel;
    static u32 s_LowerLevel;
    static u8 s_ShowFlag;
    static bool s_Initialized;
public:
    static void PrintLog(const u32 level, const char8* funcName, const char8* fileName,
        const int line, const char8* fmt, ...);

    static size_t MakeFuncName(const char8* src, char8* dest, size_t length);

    static char8* s_LevelStrings[];
};

} // namespace detail
} // namespace dbg
} // namespace nn

#ifndef NN_LOG_BASE_
#define NN_LOG_BASE_(level, ...)                                     \
    ::nn::dbg::detail::Logger::PrintLog(::nn::dbg::detail::Logger::LEVEL_ ## level,   \
        NN_FUNCTION, NN_FILE_NAME, __LINE__, __VA_ARGS__)
#endif

#ifdef NN_SWITCH_DISABLE_DEBUG_PRINT_FOR_SDK
#define NN_LOG_ERROR_(...)  NN_LOG_BASE_(DEBUG, __VA_ARGS__)
#else
#define NN_LOG_ERROR_(...) ((void)0)
#endif

