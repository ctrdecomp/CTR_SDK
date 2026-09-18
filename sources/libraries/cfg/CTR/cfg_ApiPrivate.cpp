// Filename: cfg_ApiPrivate.cpp
//
// Project: Horizon

#include <nn/cfg/CTR/cfg_DebugParam.h>
#include <nn/cfg/CTR/cfg_Api.h>
#include <nn/cfg/CTR/cfg_DetailApi.h>
#include <nn/cfg/CTR/cfg_IpcUser.h>
#include <nn/os/CTR/os_Environment.h>
#include <nn/Result.h>
#include <nn/err/CTR/err_Api.h>

namespace nn {
namespace cfg {
namespace CTR {
namespace detail {

Result GetDebugParam(DebugParamCfgData* debugParam)
{
    Result result;
    return GetConfig(debugParam, 4, CFG_KEY_USER_DEBUG_DATA);
}

}

u8 GetFsLatencyEmulationParam()
{
    nn::Result res;
    DebugParamCfgData debugParam;
    if (!os::IsRunOnDevelopmentHardWare())
    {
        return 0U;
    }

    detail::_IPCPortType portType;
    res = detail::InitializeProperPort(&portType);
    if (res.IsFailure()) 
    {
        NN_ERR_THROW_FATAL_ALL(res);
        NN_TLOG_("[cfg] Application is not permitted to use cfg.\n");
    }

    NN_ERR_THROW_FATAL_ALL(detail::GetDebugParam(&debugParam));

    detail::FinalizeProperPort(portType);

    return debugParam.fsLatencyParam;
}

bool IsDebugMode()
{
    Result res;
    DebugParamCfgData debugParam;

    if (!os::IsRunOnDevelopmentHardWare())
    {
        return false;
    }

    detail::_IPCPortType portType;
    res = detail::InitializeProperPort(&portType);
    if (res.IsFailure()) 
    {
        NN_ERR_THROW_FATAL_ALL(res);
        NN_TLOG_("[cfg] Application is not permitted to use cfg.\n");
    }

    NN_ERR_THROW_FATAL_ALL(detail::GetDebugParam(&debugParam));

    detail::FinalizeProperPort(portType);
    if(debugParam.param.flags1 & 1)
    {
        return true;
    } 
    else
    {
        return false;
    }
}

}
}
}