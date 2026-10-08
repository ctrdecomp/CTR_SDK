// Filename: dbg_Logger.cpp
//
// Project: Horizon

#include <nn/types.h>
#include <nn/dbg/dbg_Logger.h>
#include <nn/nstd.h>
#include <nn/os.h>
#include <cstring>
#include <cstdio>
#include <algorithm>

namespace nn {
namespace dbg {
namespace detail {

namespace {

#ifndef NN_SWITCH_DISABLE_DEBUG_PRINT_FOR_SDK
    size_t SafeTVSNPrintf(char* dst, size_t bufferLength, const char* fmt, va_list vlist)
    {
        size_t length = std::min<size_t>(nn::nstd::TVSNPrintf(dst, bufferLength, fmt, vlist), bufferLength);
        return length;
    }

    size_t SafeTSNPrintf(char* dst, size_t bufferLength, const char* fmt, ...)
    {
        va_list vlist;
        va_start(vlist, fmt);
        size_t length = std::min<size_t>(nn::nstd::TVSNPrintf(dst, bufferLength, fmt, vlist), bufferLength);
        va_end(vlist);
        return length;
    }

    char* GetBaseName(const char* path)
    {
        char* pFileNameTop = (char8*)std::strrchr(path, '\\');
        if (!pFileNameTop)
        {
            return const_cast<char*>(path);
        }
        return pFileNameTop + 1;
    }
#endif

} // namespace

char8* Logger::s_LevelStrings[] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL",
    "FORCE"
};

u32 Logger::s_UpperLevel = 4;
u32 Logger::s_LowerLevel = 2;
bool Logger::s_Initialized = false;
u8 Logger::s_ShowFlag = 0;

size_t Logger::MakeFuncName(const char8* src, char8* dest, size_t length)
{
    char8* pSrcTop = const_cast<char8*>(src);
    char8* pSrcSigHead = ::std::strrchr(pSrcTop, '(');

    char8* pSrcLast = (s_ShowFlag & 2) ? pSrcTop + ::std::strlen(src) : pSrcSigHead;

    if(!(s_ShowFlag & 1))
    {
        pSrcTop = pSrcSigHead;
        for(int count = 0;pSrcTop != src && count < 3; pSrcTop--)
        {
            count = (*pSrcTop == ':') ? count + 1 : count;
        }
        pSrcTop += 2;
    }
    size_t copyLength = ::std::min<size_t>(pSrcLast - pSrcTop, length - 1);
    ::std::memcpy(dest, pSrcTop, copyLength);
    dest[copyLength] = '\0';
    return copyLength;
}

void Logger::PrintLog(const u32 level, const char8* funcName, const char8* fileName, const int line,
    const char8* fmt, ...)
{
#ifndef NN_SWITCH_DISABLE_DEBUG_PRINT_FOR_SDK
    if(!s_Initialized)
    {
        s_LowerLevel = 2;
    }

    if(level != 5 && (level < s_LowerLevel || s_UpperLevel < level))
    {
        return;
    }

    char8 buffer[256];
    size_t len = 0;

    {
        len += SafeTSNPrintf(&buffer[len], sizeof(buffer) - len, "[%s] ", s_LevelStrings[level]);
        
        len += MakeFuncName(funcName, &buffer[len], 256 - len - 3);

        ::std::strncat(&buffer[len], " : ", sizeof(buffer) - len - 3);
        len += 3;
    }

    {
        va_list vlist;
        va_start(vlist, fmt);
        len += SafeTVSNPrintf(&buffer[len], sizeof(buffer) - len, fmt, vlist);
        va_end(vlist);

        for (; len > 0 && buffer[len - 1] == '\n'; --len)
        {
            buffer[len - 1] = '\0';
        }
    }

    {
        char8* pFileNameTop;
        if(!(s_ShowFlag & 4))
        {
            pFileNameTop = GetBaseName(fileName);
        }
        else
        {
            pFileNameTop = (char8*)fileName;
        }
        len += SafeTSNPrintf(&buffer[len], sizeof(buffer) - len, " (%s:%d)",  pFileNameTop, line);
    }

    len = std::min<size_t>(len, sizeof(buffer) - 2);
    buffer[len++] = '\n';
    buffer[len] = '\0'; 

    nn::dbg::detail::PutString(buffer, len);
#else
    NN_UNUSED_VAR(level);
    NN_UNUSED_VAR(funcName);
    NN_UNUSED_VAR(fileName);
    NN_UNUSED_VAR(line);
    NN_UNUSED_VAR(fmt);
#endif
}

} // namespace detail
} // namespace dbg
} // namespace nn
