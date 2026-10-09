#pragma once

#if defined(_WIN32) || defined(WIN32) || defined(__VC32__)
#   define GL_APICALL __declspec(dllimport)
#elif defined (__ARMCC_VERSION)
#   define GL_APICALL
#elif defined (__SYMBIAN32__) && defined (__GCC32__)
#   define GL_APICALL __declspec(dllexport)
#elif defined (__GNUC__)
#   define GL_APICALL
#endif

#if !defined (GL_APICALL)
#   error Unsupported platform!
#endif

#define GL_APIENTRY

#if defined(_WIN32) || defined(WIN32)
typedef __int64 GLint64EXT;
typedef unsigned __int64 GLuint64EXT;
#else
typedef signed long long GLint64EXT;
typedef unsigned long long GLuint64EXT;
#endif

# define GL_APICALL

