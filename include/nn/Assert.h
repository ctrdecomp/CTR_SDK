#pragma once

#include <nn/Handle.h>
#include <nn/os/CTR/MPCore/os_MemoryMap.h>
#include <nn/dbg/dbg_Break.h>
#include <nn/dbg/dbg_DebugString.h>
#include <nn/dbg/dbg_PrintResult.h>

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <new>
#include <limits>
#include <string.h>

#define NN_FILE_NAME            __MODULE__
#define NN_FUNCTION             __PRETTY_FUNCTION__

#if defined(NN_SWITCH_DISABLE_DEBUG_PRINT_FOR_SDK) || defined(NN_SWITCH_DISABLE_ASSERT_WARNING_FOR_SDK)

/* POINTER */
    #define NN_ANY_TO_UPTR(ptr)         ((uptr)((const void*)(ptr)))
    #define NN_IS_VALID_POINTER(ptr)    ( (NN_OS_ADDR_NULL_TRAP_END <= NN_ANY_TO_UPTR(ptr)) \
                                        && (NN_ANY_TO_UPTR(ptr) < NN_OS_ADDR_SPACE_END) )
    #define NN_ASSERTMSG_(exp, ...)                                                  \
                    (void) ((exp) || nndbgAssertionFailureHandler(true, NN_FILE_NAME, __LINE__, __VA_ARGS__))

    #define NN_ASSERTMSG_WITH_RESULT_(exp, result, ...) \
            (void) ((exp) || nnResultFailureHandler(static_cast< ::nn::Result>(result), NN_FILE_NAME, __LINE__, __VA_ARGS__))

    #define NN_TASSERTMSG_(exp, ...) \
        (void) ((exp) || nndbgTAssertionFailureHandler(true, NN_FILE_NAME, __LINE__, __VA_ARGS__))

    #define NN_TASSERTMSG_WITH_RESULT_(exp, result, ...) \
        (void) ((exp) || nnResultTFailureHandler(static_cast< ::nn::Result>(result), NN_FILE_NAME, __LINE__, __VA_ARGS__))

    #define NN_TASSERT_(exp)                     NN_TASSERTMSG_(exp, "%s", #exp)
    #define NN_TASSERT_WITH_RESULT_(exp, result) NN_TASSERTMSG_WITH_RESULT_((exp), (result), "%s", #exp)
    #define NN_RESULT_TASSERT_(exp)              NN_TASSERTMSG_WITH_RESULT_((exp).IsSuccess(), (exp), "\"%s\" is Failure.", #exp)
    #define NN_NULL_TASSERT_(exp)                NN_TASSERTMSG_((exp) != NULL, "%s must not be NULL", #exp)
    #define NN_ALIGN_TASSERT_(exp, align)        NN_TASSERTMSG_(((uptr)(exp)) % (align) == 0, "%s(=0x%08x) must be %d byte aligned.", #exp, (exp), align)
    #define NN_MIN_TASSERT_(exp, min)            NN_TASSERTMSG_((exp) >= (min), "%s(=%d) must be >= %s(=%d).", #exp, (exp), #min, min)
    #define NN_MAX_TASSERT_(exp, max)            NN_TASSERTMSG_((exp) <= (max), "%s(=%d) must be <= %s(=%d).", #exp, (exp), #max, max)
    #define NN_MINMAX_TASSERT_(exp, min, max)    NN_TASSERTMSG_((exp) >= (min) && (exp) <= (max), "%s(=%d) must be >= %s(=%d) and <= %s(=%d).", #exp, (exp), #min, min, #max, max)
    #define NN_EQUAL_TASSERT_(exp, equ)          NN_TASSERTMSG_((exp) == (equ), "%s(=%d) must be == %s(=%d).", #exp, (exp), #equ, equ)
    #define NN_NOT_EQUAL_TASSERT_(exp, equ)      NN_TASSERTMSG_((exp) != (equ), "%s(=%d) must be != %s(=%d).", #exp, (exp), #equ, equ)
    #define NN_POINTER_TASSERT_(p)               NN_TASSERTMSG_(NN_IS_VALID_POINTER(p), "%s(=0x%08X) is invalid pointer", #p, (p))
    #define NN_THIS_TASSERT_()                   NN_POINTER_TASSERT_(this)
    #define NN_FLOAT_TASSERT_(exp)               NN_TASSERTMSG_((-FLT_MAX <= (exp) && (exp) <= FLT_MAX), "Floating Point Value Error\n"#exp" is infinite or nan.")
    #define NN_TASSERT_RESULT_(exp)              NN_RESULT_TASSERT_(exp)

    #define NN_TPANIC_(...) \
        (void) (NN_TLOG_("Panic: %s:%d\n", NN_FILE_NAME, __LINE__), \
                NN_TLOG_(__VA_ARGS__), \
                NN_TLOG_("\n"), \
                nndbgPanic())

    #define NN_PANIC_(...) \
        (void) (NN_LOG_("Panic: %s:%d\n", NN_FILE_NAME, __LINE__), \
                NN_LOG_(__VA_ARGS__), \
                NN_LOG_("\n"), \
                nndbgPanic())
    
    #define NN_TWARNING_(exp, ...) (void) ((exp) || (nndbgDetailTPrintf(__VA_ARGS__), 0))
        
    #define NN_WARNING_(exp, ...) (void) ((exp) || (nndbgDetailPrintf(__VA_ARGS__), 0))

    #define NN_PANIC_IF_FALSE(result) \
        do \
        { \
            if (!(result)) \
            { \
                NN_PANIC_("Failed condition."); \
            } \
        } while(0)

    #define NN_TPANIC_IF_FALSE_(result) \
        do \
        { \
            if (!(result)) \
            { \
                NN_TPANIC_("Failed condition."); \
            } \
        } while(0)

    #define NN_PANIC_WITH_RESULT_(result, ...) \
        ((void)nnResultTPanicHandler(static_cast< ::nn::Result>(result), NN_FILE_NAME, __LINE__, __VA_ARGS__), NN_PANIC_(__VA_ARGS__))

    #define NN_PANIC_IF_NULL_(exp)              NN_PANIC_IF_FALSE(exp)

    #define NN_PANIC_IF_FAILED_(result) \
        do { \
            ::nn::Result nn_result_try_result = (result); \
            if (nn_result_try_result.IsFailure()) \
            { \
                NN_TPANIC_WITH_RESULT_(nn_result_try_result, "\"%s\" is Failure.", #result); \
            } \
        } while (0)

    #define NN_TPANIC_WITH_RESULT_(result, ...) \
        ((void)nnResultTPanicHandler(static_cast< ::nn::Result>(result), NN_FILE_NAME, __LINE__, __VA_ARGS__), NN_TPANIC_(__VA_ARGS__))

    #define NN_TPANIC_IF_NULL_(exp)              NN_TPANIC_IF_FALSE_(exp)

    #define NN_TPANIC_IF_FAILED_(result) \
        do { \
            ::nn::Result nn_result_try_result = (result); \
            if (nn_result_try_result.IsFailure()) \
            { \
                NN_TPANIC_WITH_RESULT_(nn_result_try_result, "\"%s\" is Failure.", #result); \
            } \
        } while (0)

    #define NN_ASSERT_(exp)                     NN_ASSERTMSG_(exp, "%s", #exp)
    #define NN_ASSERT_WITH_RESULT_(exp, result) NN_ASSERTMSG_WITH_RESULT_((exp), (result), "%s", #exp)
    #define NN_RESULT_ASSERT_(exp)              NN_ASSERTMSG_WITH_RESULT_((exp).IsSuccess(), (exp), "\"%s\" is Failure.", #exp)
    #define NN_NULL_ASSERT_(exp)                NN_ASSERTMSG_((exp) != NULL, "%s must not be NULL", #exp)
    #define NN_ALIGN_ASSERT_(exp, align)        NN_ASSERTMSG_(((uptr)(exp)) % (align) == 0, "%s(=0x%08x) must be %d byte aligned.", #exp, (exp), align)
    #define NN_MIN_ASSERT_(exp, min)            NN_ASSERTMSG_((exp) >= (min), "%s(=%d) must be >= %s(=%d).", #exp, (exp), #min, min)
    #define NN_MAX_ASSERT_(exp, max)            NN_ASSERTMSG_((exp) <= (max), "%s(=%d) must be <= %s(=%d).", #exp, (exp), #max, max)
    #define NN_MINMAX_ASSERT_(exp, min, max)    NN_ASSERTMSG_((exp) >= (min) && (exp) <= (max), "%s(=%d) must be >= %s(=%d) and <= %s(=%d).", #exp, (exp), #min, min, #max, max)
    #define NN_EQUAL_ASSERT_(exp, equ)          NN_ASSERTMSG_((exp) == (equ), "%s(=%d) must be == %s(=%d).", #exp, (exp), #equ, equ)
    #define NN_NOT_EQUAL_ASSERT_(exp, equ)      NN_ASSERTMSG_((exp) != (equ), "%s(=%d) must be != %s(=%d).", #exp, (exp), #equ, equ)
    #define NN_POINTER_ASSERT_(p)               NN_ASSERTMSG_(NN_IS_VALID_POINTER(p), "%s(=0x%08X) is invalid pointer", #p, (p))
    #define NN_THIS_ASSERT_()                   NN_POINTER_ASSERT_(this)
    #define NN_FLOAT_ASSERT_(exp)               NN_ASSERTMSG_((-FLT_MAX <= (exp) && (exp) <= FLT_MAX), "Floating Point Value Error\n"#exp" is infinite or nan.")
    #define NN_ASSERT_RESULT_(exp)              NN_RESULT_ASSERT_(exp)

#else

    #define NN_ANY_TO_UPTR(ptr)         ((uptr)((const void*)(ptr)))
    #define NN_IS_VALID_POINTER(ptr)    ( (NN_OS_ADDR_NULL_TRAP_END <= NN_ANY_TO_UPTR(ptr)) \
                                        && (NN_ANY_TO_UPTR(ptr) < NN_OS_ADDR_SPACE_END) )

    #define NN_POINTER_ASSERT_(p)
    #define NN_ASSERT_WITH_RESULT_(exp, result)

    #define NN_TASSERTMSG_(exp, ...)
    #define NN_TASSERT_(exp)                     NN_TASSERTMSG_((exp), "%s", #exp)
    #define NN_TASSERTMSG_WITH_RESULT_(exp, result, ...)
    #define NN_RESULT_TASSERT_(exp)              NN_TASSERTMSG_WITH_RESULT_((exp).IsSuccess(), (exp), "\"%s\" is Failure.", #exp)
    #define NN_NULL_TASSERT_(exp)                NN_TASSERTMSG_((exp) != NULL, "%s must not be NULL", #exp)
    #define NN_ALIGN_TASSERT_(exp, align)        NN_TASSERTMSG_(((uptr)(exp)) % (align) == 0, "%s(=0x%08x) must be %d byte aligned.", #exp, (exp), align)
    #define NN_MIN_TASSERT_(exp, min)            NN_TASSERTMSG_((exp) >= (min), "%s(=%d) must be >= %s(=%d).", #exp, (exp), #min, min)
    #define NN_MAX_TASSERT_(exp, max)            NN_TASSERTMSG_((exp) <= (max), "%s(=%d) must be <= %s(=%d).", #exp, (exp), #max, max)
    #define NN_MINMAX_TASSERT_(exp, min, max)    NN_TASSERTMSG_((exp) >= (min) && (exp) <= (max), "%s(=%d) must be >= %s(=%d) and <= %s(=%d).", #exp, (exp), #min, min, #max, max)
    #define NN_EQUAL_TASSERT_(exp, equ)          NN_TASSERTMSG_((exp) == (equ), "%s(=%d) must be == %s(=%d).", #exp, (exp), #equ, equ)
    #define NN_NOT_EQUAL_TASSERT_(exp, equ)      NN_TASSERTMSG_((exp) != (equ), "%s(=%d) must be != %s(=%d).", #exp, (exp), #equ, equ)
    #define NN_POINTER_TASSERT_(p)               NN_TASSERTMSG_(NN_IS_VALID_POINTER(p), "%s(=0x%08X) is invalid pointer", #p, (p))
    #define NN_THIS_TASSERT_()                   NN_POINTER_TASSERT_(this)
    #define NN_FLOAT_TASSERT_(exp)               NN_TASSERTMSG_((-FLT_MAX <= (exp) && (exp) <= FLT_MAX), "Floating Point Value Error\n"#exp" is infinite or nan.")
    #define NN_TASSERT_RESULT_(exp)              NN_RESULT_TASSERT_(exp)

    #define NN_TPANIC_(...)                      nndbgPanic()
    #define NN_TPANIC_WITH_RESULT_(result, ...) \
        ((void)nnResultTPanicHandler(static_cast< ::nn::Result>(result), NN_FILE_NAME, __LINE__, __VA_ARGS__), NN_TPANIC_(__VA_ARGS__))

    #define NN_TPANIC_IF_FALSE_(exp)    (void) ((exp) || (NN_TPANIC_("Failed condition."), 0))
    #define NN_TPANIC_IF_NULL_(exp)
    #define NN_TPANIC_IF_FAILED_(result)

    #define NN_TWARNING_(exp, ...)
    #define NN_WARNING_(exp, ...)

    #define NN_ASSERTMSG_(exp, ...)
    #define NN_ASSERT_(exp)                     NN_ASSERTMSG_((exp), "%s", #exp)
    #define NN_ASSERTMSG_WITH_RESULT_(exp, result, ...)
    #define NN_RESULT_ASSERT_(exp)              NN_ASSERTMSG_WITH_RESULT_((exp).IsSuccess(), (exp), "\"%s\" is Failure.", #exp)
    #define NN_NULL_ASSERT_(exp)                NN_ASSERTMSG_((exp) != NULL, "%s must not be NULL", #exp)
    #define NN_ALIGN_ASSERT_(exp, align)        NN_ASSERTMSG_(((uptr)(exp)) % (align) == 0, "%s(=0x%08x) must be %d byte aligned.", #exp, (exp), align)
    #define NN_MIN_ASSERT_(exp, min)            NN_ASSERTMSG_((exp) >= (min), "%s(=%d) must be >= %s(=%d).", #exp, (exp), #min, min)
    #define NN_MAX_ASSERT_(exp, max)            NN_ASSERTMSG_((exp) <= (max), "%s(=%d) must be <= %s(=%d).", #exp, (exp), #max, max)
    #define NN_MINMAX_ASSERT_(exp, min, max)    NN_ASSERTMSG_((exp) >= (min) && (exp) <= (max), "%s(=%d) must be >= %s(=%d) and <= %s(=%d).", #exp, (exp), #min, min, #max, max)
    #define NN_EQUAL_ASSERT_(exp, equ)          NN_ASSERTMSG_((exp) == (equ), "%s(=%d) must be == %s(=%d).", #exp, (exp), #equ, equ)
    #define NN_NOT_EQUAL_ASSERT_(exp, equ)      NN_ASSERTMSG_((exp) != (equ), "%s(=%d) must be != %s(=%d).", #exp, (exp), #equ, equ)
    #define NN_POINTER_ASSERT_(p)               NN_ASSERTMSG_(NN_IS_VALID_POINTER(p), "%s(=0x%08X) is invalid pointer", #p, (p))
    #define NN_THIS_ASSERT_()                   NN_POINTER_ASSERT_(this)
    #define NN_FLOAT_ASSERT_(exp)               NN_ASSERTMSG_((-FLT_MAX <= (exp) && (exp) <= FLT_MAX), "Floating Point Value Error\n"#exp" is infinite or nan.")
    #define NN_ASSERT_RESULT_(exp)              NN_RESULT_ASSERT_(exp)

    #define NN_PANIC_(...)                      nndbgPanic()
    #define NN_PANIC_WITH_RESULT_(result, ...) \
        ((void)nnResultPanicHandler(static_cast< ::nn::Result>(result), NN_FILE_NAME, __LINE__, __VA_ARGS__), NN_PANIC(__VA_ARGS__))

#endif

