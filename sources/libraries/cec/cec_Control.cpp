// Filename: cec_CecControl.cpp
//
// Project: Horizon

#include <nn/cec.h>
#include <nn/dbg.h>
#include <nn/os/os_HandleManager.h>
#include <nn/os/os_Thread.h>
#include <nn/ndm.h>
#include <nn/ndm/CTR/ndm_Types.h>
#include <nn/init/init_Allocator.h>
#include <nn/CTR/CTR_ProgramId.h>

#include <nn/cfg.h>
#include <nn/cfg/CTR/cfg_DebugParam.h>

#include <nn/cec/CTR/cec_ControlSys.h>

#define DBG_PRINTF_ERR(format, args...)     NN_LOG_ERROR_(format, ## args)

namespace
{
bool isDebugMode = false;
}

namespace nn {
namespace cec {
namespace CTR {

bool CecControl::s_Initialized = false;
bool CecControl::s_NdmInitialized = false;
bool CecControl::s_NdmSuspended = false;
bool CecControl::s_EnterExclusiveState = false;

nn::os::CriticalSection CecControl::m_Cs = nn::WithInitialize();

CecControl::CecControl()
{
    Initialize();
}

CecControl::~CecControl()
{
    Finalize();
}

Result CecControl::Initialize()
{
    nn::Result  result = ResultSuccess();

    if(s_Initialized == false)
    {
        result = nn::cec::CTR::detail::InitializeCecControl();
        NN_UTIL_RETURN_IF_FAILED(result);
        nn::cec::CTR::detail::WaitForSessionValid();


        result = nn::ndm::Initialize() ;
        if(result.IsFailure())
        {
            DBG_PRINTF_ERR("nn::ndm::Initialize Failure\n");
            return result;
        }
        s_NdmInitialized = true;

        nn::cfg::CTR::Initialize();
        isDebugMode = nn::cfg::CTR::IsDebugMode();
        nn::cfg::CTR::Finalize();

        s_Initialized = true;
    }

    return result;
}

Result CecControl::Initialize(nn::fnd::IAllocator& cecAllocFunc)
{
    SetAllocFunc(cecAllocFunc);
    return CecControl::Initialize();
}

Result CecControl::Finalize()
{
    nn::Result result = ResultSuccess();

    if(s_Initialized)
    {
        if(s_NdmInitialized)
        {
            result = nn::ndm::Finalize();
            NN_UTIL_RETURN_IF_FAILED(result);
            s_NdmSuspended = false;
            s_NdmInitialized = false;
        }

        result = nn::cec::CTR::detail::FinalizeCecControl();
        NN_UTIL_RETURN_IF_FAILED(result);


        FinalizeAllocFunc();

        s_Initialized = false;
    }
    return result;
}

bool CecControl::IsInitialized()
{
    return s_Initialized || CecControlSys::IsInitializedSys();
}

Result CecControl::StartScanning(bool reset)
{
    Result result = ResultSuccess();
    if(IsInitialized())
    {
        nn::os::CriticalSection::ScopedLock locker(m_Cs);

        if(reset)
        {
            if(CecControlSys::IsInitializedSys() || isDebugMode)
            {
                result = detail::Start(14);
            }
            else
            {
                DBG_PRINTF_ERR("### Cannot reset.... \n");
                return ResultNotAuthorized();
            }
        }

        if(s_NdmInitialized)
        {
            if(s_NdmSuspended)
            {
                result = nn::ndm::Resume(nn::ndm::DN_CEC);
                if(result.IsFailure())
                {
                    DBG_PRINTF_ERR("### nn::ndm::Resume Failure .... \n");
                    NN_DBG_PRINT_TRESULT(result);
                    return result;
                }
                s_NdmSuspended = false;
            }
        }
        else
        {
            return ResultStateBusy();
        }
    }
    else
    {
        DBG_PRINTF_ERR("CecControl Not Initialized .... \n");
        result = ResultStateBusy();
    }
    return result;
}

Result CecControl::Suspend()
{
    nn::Result result = ResultSuccess();
    if(s_NdmInitialized)
    {
        if(!s_NdmSuspended)
        {
            nn::os::CriticalSection::ScopedLock locker(m_Cs);
            result = nn::ndm::Suspend(nn::ndm::DN_CEC);
            if(result.IsFailure())
            {
                DBG_PRINTF_ERR("### nn::ndm::Suspend Failure .... \n");
                NN_DBG_PRINT_TRESULT(result);
                return result;
            }
            nn::os::Thread::Sleep(nn::fnd::TimeSpan::FromMilliSeconds(16));
            s_NdmSuspended = true;
        }
    }
    else
    {
        result = ResultStateBusy();
    }
    return result;
}

Result CecControl::StopScanning(bool b_Immediate, bool b_Async)
{
    Result result = ResultSuccess();

    if(IsInitialized())
    {
        nn::os::CriticalSection::ScopedLock locker(m_Cs);

        if(s_NdmInitialized)
        {
            if(!s_NdmSuspended)
            {
                result = nn::ndm::Suspend( nn::ndm::DN_CEC );
                if(result.IsFailure())
                {
                    DBG_PRINTF_ERR("### nn::ndm::Suspend Failure .... \n");
                    NN_DBG_PRINT_TRESULT(result);
                    return result;
                }

                nn::os::Thread::Sleep(nn::fnd::TimeSpan::FromMilliSeconds(16));
                s_NdmSuspended = true;
            }
        }
        else
        {
            return ResultStateBusy();
        }

        if(b_Async)
        {
            if(b_Immediate)
            {
                result = detail::Stop(12);
            }
            else
            {
                result = detail::Stop(11);
            }
        }
        else
        {
            u32 state;
            int count = 0;

            nn::os::Event stateChangeEvent;
            result = GetChangeStateEventHandle(&stateChangeEvent.m_Handle);
            NN_RESULT_ASSERT_(result);

            if(b_Immediate)
            {
                result = detail::Stop(12);
            }
            else
            {
                result = detail::Stop(11);
            }

            while(stateChangeEvent.Wait(nn::fnd::TimeSpan::FromSeconds(1)) == false)
            {
                nn::cec::CTR::detail::GetCecdState(&state);
                if(state == 1)
                {
                    break;
                }
                if(count > 60)
                {
                    result = ResultStateBusy();
                    break;
                }
                count++;
            }
            stateChangeEvent.Finalize();
        }

    }
    else
    {
        DBG_PRINTF_ERR("CecControl Not Initialized .... \n");
        result = ResultStateBusy();
    }

    return result;
}

} // namespace CTR
} // namespace cec
} // namespace nn
