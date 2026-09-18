// Filename: rocrt_Control.cpp
//
// Project: Horizon
//
// Custom, filename not confirmed. 

#include <nn/ro.h>
#include <nn/module.h>
#include <nn/version.h>
#include <nn/ro/ro_Info.h>

extern "C"
{
    extern __weak bit8 Image$$CODE_INIT_ARRAY$$Base[];
    extern __weak bit8 Image$$CODE_INIT_ARRAY$$Limit[];
    extern __weak bit8 Image$$RO_STATIC_INIT$$Base[];
    extern __weak bit8 Image$$RO_STATIC_INIT$$Limit[];
}

namespace{

#if defined(NN_BUILD_DEBUG) || defined(NN_BUILD_DEVELOPMENT)
    NN_MAKE_MODULE(sDebugIndicator,  "NINTENDO", "DEBUG");
#endif
    NN_MAKE_MODULE(s_SdkVersion,      "NINTENDO", NN_CURRENT_SDK_VERSION);
    NN_MAKE_MODULE(s_FirmwareVersion, "NINTENDO", NN_CURRENT_FIRMWARE_VERSION);

void* s_pWorkArea;

const CodeRegion s_InitArray =
{
    reinterpret_cast<uptr>(Image$$CODE_INIT_ARRAY$$Base),
    reinterpret_cast<uptr>(Image$$CODE_INIT_ARRAY$$Limit)
};

const CodeRegion s_StaticInitArray =
{
    reinterpret_cast<uptr>(Image$$RO_STATIC_INIT$$Base),
    reinterpret_cast<uptr>(Image$$RO_STATIC_INIT$$Limit)
};

asm void ReferSymbol(const void* sym, ...)
{
    bx  lr
}

}

extern "C"{


extern __weak detail::EitLinkNode nnroEitNode_;

Result nnroControlObject_(void* param, ObjectControl c){
    switch(c)
    {
    case OBJECT_CONTROL_GET_EIT_NODE:
    {
        detail::EitLinkNode** ppNode = reinterpret_cast<detail::EitLinkNode**>(param);
        *ppNode = &nnroEitNode_;
    }
    break;

    case OBJECT_CONTROL_SET_WORK_AREA:
    {
        s_pWorkArea = param;
    }
    break;

    case OBJECT_CONTROL_GET_WORK_AREA:{
        void** ppWork = reinterpret_cast<void**>(param);
        *ppWork = s_pWorkArea;
    }
    break;

    case OBJECT_CONTROL_GET_INIT_ARRAY:
    {
        reinterpret_cast<CodeRegion*>(param)->begin = s_InitArray.begin;
        reinterpret_cast<CodeRegion*>(param)->end = s_InitArray.end;
    }
    break;

    case OBJECT_CONTROL_GET_STATIC_INIT_ARRAY:
    {
        reinterpret_cast<CodeRegion*>(param)->begin = s_StaticInitArray.begin;
        reinterpret_cast<CodeRegion*>(param)->end = s_StaticInitArray.end;
    }
    default:

#if defined(NN_BUILD_DEBUG) || defined(NN_BUILD_DEVELOPMENT)
        ReferSymbol(s_DebugIndicator);
#endif
        ReferSymbol(s_SdkVersion);
        ReferSymbol(s_FirmwareVersion);
        return ResultUnknownObjectControl();
    }

    return nn::ResultSuccess();

}

}