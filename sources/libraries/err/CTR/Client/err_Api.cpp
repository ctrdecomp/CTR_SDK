// Filename: err_Api.cpp
//
// Project: Horizon

#include <nn/err/CTR/err_Api.h>
#include <nn/dbg/dbg_Break.h>
#include <nn/dbg/dbg_DebugString.h>
#include <nn/os/os_Result.h>
#include <nn/os/os_Thread.h>
#include <nn/os/os_CriticalSection.h>
#include <nn/os/os_LightEvent.h>
#include <nn/version.h>

using namespace nn::dbg::detail;

namespace nn{
namespace err{
namespace CTR{
namespace{
    
static FatalErrInfo s_FatalErrInfo;
Handle s_FatalErrSession = INVALID_HANDLE_VALUE;
os::CriticalSection s_Lock = nn::WithInitialize();


Result InitializeFatalErrSession()
{
    Result res;
    if(!s_FatalErrSession.IsValid())
    {
        res = svc::ConnectToPort(&s_FatalErrSession, FATAL_ERR_PORT_NAME);
        if(!res.IsSuccess())
        {
            return res;
        }
    }
    return ResultSuccess();
}

void FinalizeFatalErrSession()
{
    if(s_FatalErrSession.IsValid())
    {
        svc::CloseHandle(s_FatalErrSession);
        s_FatalErrSession = INVALID_HANDLE_VALUE;
    }
}

void Throw(FatalErrInfo& info)
{
    for(;;)
    {
        nn::os::CriticalSection::ScopedLock lock(s_Lock);
            
        Result sessionResult = InitializeFatalErrSession();
        if (sessionResult.IsSuccess())
        {
            svc::GetProcessId(&info.processId, PSEUDO_HANDLE_CURRENT_PROCESS);

            FatalErr fe(s_FatalErrSession);
            fe.Throw(info);
                
            FinalizeFatalErrSession();
        }
        else if (sessionResult == nn::os::ResultOverPortCapacity())
        {
            if (info.type == NN_ERR_FATAL_TYPE_CARD_EJECTION)
            {
                nn::os::Thread::Sleep(nn::fnd::TimeSpan::FromMilliSeconds(5));
                continue;
            }
        }
        else
        {
            NN_DBG_PRINT_TRESULT(sessionResult);
        }
            
        break;
    }
        

    if (info.type != 2 && info.type != 5)
    {
        nn::os::LightEvent  infiniteLock(true);
        infiniteLock.Wait();
    }
}

}

void ThrowFatalErr(Result result, nnerrFatalErrType type, uint pc)
{
    NN_TLOG_("----\n");
    NN_TLOG_("Fatal Error occured. (pc=0x%08x)\n", pc);
    NN_DBG_PRINT_TRESULT(result);
    {
        FatalErrInfo& fei = s_FatalErrInfo;
        s_FatalErrInfo.revisionHi = 0;
        s_FatalErrInfo.revisionLo = 0; // DEPENDS ON SDK VERSION
        s_FatalErrInfo.type = type;
        s_FatalErrInfo.result = result;
        s_FatalErrInfo.pc = pc;
        Throw(s_FatalErrInfo);
    }
}

void ThrowFatalErr(Result result, uptr pc)
{
    Result::Level lev = result.Result::GetLevel();
    if(lev == Result::LEVEL_INFO || lev == Result::LEVEL_STATUS)
        return;
    ThrowFatalErr(result, NN_ERR_FATAL_TYPE_SYSTEM_COMMON, pc);
}

void ThrowFatalErrAll(Result result, uptr pc)
{
    return ThrowFatalErr(result, NN_ERR_FATAL_TYPE_SYSTEM_COMMON, pc);
}

}
}
}

