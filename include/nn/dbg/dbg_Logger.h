#pragma once

#include <nn/types.h>
#include <nn/dbg/dbg_DebugString.h>

namespace nn { 
namespace dbg { 
namespace detail {

#define NN_LOGGER_LEVEL_DEBUG 0
#define NN_LOGGER_LEVEL_INFO 1
#define NN_LOGGER_LEVEL_WARN 2
#define NN_LOGGER_LEVEL_ERROR 3
#define NN_LOGGER_LEVEL_FATAL 4
#define NN_LOGGER_LEVEL_FORCE 5

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
    ::nn::dbg::detail::Logger::PrintLog(NN_LOGGER_LEVEL_ ## level,   \
        NN_FUNCTION, NN_FILE_NAME, __LINE__, __VA_ARGS__)
#endif

#if 0
#define NN_LOG_DEBUG(...)  NN_LOG_BASE_(DEBUG, __VA_ARGS__)
#else
#define NN_LOG_DEBUG(...)  ((void)0)
#endif

#if 0
#define NN_LOG_INFO(...)  NN_LOG_BASE_(INFO, __VA_ARGS__)
#else
#define NN_LOG_INFO(...)  ((void)0)
#endif

#if 0
#define NN_LOG_WARN(...)  NN_LOG_BASE_(WARN, __VA_ARGS__)
#else
#define NN_LOG_WARN(...)  ((void)0)
#endif

#if 0
#define NN_LOG_ERROR(...)  NN_LOG_BASE_(ERROR, __VA_ARGS__)
#else
#define NN_LOG_ERROR(...)  ((void)0)
#endif

#if 0
#define NN_LOG_FATAL(...)  NN_LOG_BASE_(FATAL, __VA_ARGS__)
#else
#define NN_LOG_FATAL(...)  ((void)0)
#endif

#if 0
#define NN_LOG_FORCE(...)  NN_LOG_BASE_(FORCE, __VA_ARGS__)
#else
#define NN_LOG_FORCE(...)  ((void)0)
#endif
