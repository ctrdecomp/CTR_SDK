// Filename: applet_API.cpp
//
// Project: Horizon

#include <nn/applet.h>
#include <nn/err.h>
#include <nn/fs.h>
#include <nn/srv.h>
#include <nn/applet/CTR/applet_Connect.h>
#include <nn/applet/CTR/applet_Info.h>
#include <nn/applet/CTR/applet_TimeoutChecker.h>
#include <nn/applet/CTR/applet_ClientThread.h>
#include <nn/applet/CTR/applet_InitialParamaters.h>
#include <nn/applet/CTR/applet_Ipc.h>
#include <nn/applet/CTR/applet_Result.h>
#include <nn/camera/CTR/camera_API.h>
#include <nn/gxlow/CTR/gxlow_SystemUse.h>
#include <nn/gxlow/CTR/gxlow_Management.h>
#include <nn/gxlow/CTR/gxlow_Result.h>
#include <nn/dsp/CTR/MPCore/dsp_Api.h>
#include <nn/os/os_HandleManager.h>

#include <nn/dbg/dbg_DebugString.h>

#include <nn/gx/CTR/gx_CTR.h>

/* PI */

namespace 
{
    const size_t WRAP_SIZE = 16;
}

/* Application Thread */

namespace
{
    AppletId                  selfAppletId;
    AppletAttr                selfAppletAttr;
    nn::os::Event             eventForCont;
    nn::os::Mutex             appletMutex;
    nn::os::Event             eventForMesg;
    nn::os::Event             eventForAbort;
    bool                      isClientThreadEnd;
}

namespace nn {
namespace applet {
namespace CTR {
namespace detail {
namespace{
    nn::os::Thread            clientThread;
    nn::os::StackBuffer<4096> clientStack;
    AppletReceiveCallback     mReceiveCallback;
    uptr                      mReceiveCallbackParam;
    nn::os::LightEvent        waitCont;
}

void ThreadFunc(int param);

void InitializeClientThread(s32 threadPriority)
{
    waitCont.Initialize(true);
    isClientThreadEnd = false;
    clientThread.Start(ThreadFunc, 0, clientStack, threadPriority);
}

void FinalizeClientThread()
{
    isClientThreadEnd = true;
    eventForMesg.Signal();
    clientThread.Join();
    clientThread.Finalize();
    waitCont.Finalize();
}

void SetReceiveCallback(AppletReceiveCallback callback,uptr parameter)
{
    mReceiveCallback = callback;
    mReceiveCallbackParam = parameter;
}

void WaitForControlEvent()
{
    waitCont.Wait();
}

bool TryWaitForControlEvent()
{
    return waitCont.TryWait();
}

void ClearControlEvent()
{
    waitCont.ClearSignal();
}

void ThreadFunc(int param)
{
    NN_UNUSED_VAR(param);

    Handle handles[3];

    handles[0] = eventForMesg.GetHandle();
    handles[1] = eventForCont.GetHandle();
    handles[2] = eventForAbort.GetHandle();

    while (!isClientThreadEnd)
    {
        s32 index;

        Result result = nn::svc::WaitSynchronizationN(&index, handles, 3, false, nn::os::WAIT_INFINITE);

        NN_ERR_THROW_FATAL(result);

        if (isClientThreadEnd)
        {
            break;
        }

        if (index == 2)
        {
            if (waitCont.TryWait())
            {
                WaitBySleep(10);
                eventForAbort.Signal();
                continue;
            }

            eventForAbort.ClearSignal();

            SetMessageCommand(COMMAND_WAKEUP_BY_CANCEL);
            waitCont.Signal();
        }
        else if (index == 1)
        {
            if (waitCont.TryWait())
            {
                WaitBySleep(10);
                eventForCont.Signal();
                continue;
            }

            eventForCont.ClearSignal();

            bool bSignal = true;

            if (mReceiveCallback)
            {
                bSignal = mReceiveCallback(mReceiveCallbackParam);
            }

            if (bSignal)
            {
                waitCont.Signal();
            }
        }
        else if (index == 0)
        {
            if (waitCont.TryWait())
            {
                WaitBySleep(10);
                eventForMesg.Signal();
                continue;
            }

            eventForMesg.ClearSignal();

            AppletNotification notification;

            detail::LockAndConnect();

            result = APPLET::InquireNotification(GetId(), &notification);

            detail::DisconnectAndUnlock();

            if (!result.IsSuccess())
            {
                continue;
            }

            switch (notification)
            {
            case NOTIFICATION_HOME_BUTTON_1:
            case NOTIFICATION_HOME_BUTTON_2:
            {
                if (detail::GetAbsoluteHomeButtonState() == HOME_BUTTON_NONE)
                {
                    detail::SetAbsoluteHomeButtonState((notification == NOTIFICATION_HOME_BUTTON_1)
                            ? HOME_BUTTON_SINGLE_PRESSED : HOME_BUTTON_DOUBLE_PRESSED);
                }

                bool bSignal = true;

                if (mReceiveCallback)
                {
                    bSignal = mReceiveCallback(mReceiveCallbackParam);
                }

                if (bSignal)
                {
                    SetMessageCommand((notification == NOTIFICATION_HOME_BUTTON_1)
                            ? COMMAND_HOME_BUTTON_SINGLE : COMMAND_HOME_BUTTON_DOUBLE);

                    waitCont.Signal();
                }
            }
            break;

            case NOTIFICATION_SLEEP_QUERY:
            case NOTIFICATION_SLEEP_CANCELED_BY_OPEN:
            case NOTIFICATION_SLEEP_ACCEPTED:
            case NOTIFICATION_AWAKE:
            {
                switch (notification)
                {
                case NOTIFICATION_SLEEP_QUERY:
                    detail::SetSleepSysState(SLEEP_SYS_STATE_QUERY);
                    break;

                case NOTIFICATION_SLEEP_CANCELED_BY_OPEN:
                    detail::SetSleepSysState(SLEEP_SYS_STATE_CANCELED);
                    break;

                case NOTIFICATION_SLEEP_ACCEPTED:
                    detail::SetSleepSysState(SLEEP_SYS_STATE_ACCEPTED);
                    break;

                case NOTIFICATION_AWAKE:
                    detail::SetSleepSysState(SLEEP_SYS_STATE_AWAKE);
                    break;
                }

                if (mReceiveCallback)
                {
                    (void)mReceiveCallback(mReceiveCallbackParam);
                }
            }
            break;

            case NOTIFICATION_SHUTDOWN:
            {
                SetShutdownCallbackFlag();
                detail::SetShutdownState(SHUTDOWN_STATE_RECEIVED);
                SetOrderToCloseState(ORDER_TO_CLOSE_STATE_RECEIVED);

                if (mReceiveCallback)
                {
                    (void)mReceiveCallback(mReceiveCallbackParam);
                }
            }
            break;

            case NOTIFICATION_POWER_BUTTON_CLICK:
            {
                SetPowerButtonCallbackFlag();
                SetPowerButtonState(POWER_BUTTON_STATE_CLICK);

                if (mReceiveCallback)
                {
                    (void)mReceiveCallback(mReceiveCallbackParam);
                }
            }
            break;

            case NOTIFICATION_POWER_BUTTON_CLEAR:
            {
                SetPowerButtonState(POWER_BUTTON_STATE_NONE);
            }
            break;

            case NOTIFICATION_TRY_SLEEP:
            {
                detail::LockAndConnect();

                NN_TLOG_("applet_API: SleepSystem\n");

                result = detail::APPLET::SleepSystem(WAKEUP_TRIGGER_SHELL_OPEN);

                NN_ERR_THROW_FATAL(result);

                detail::DisconnectAndUnlock();
            }
            break;

            case NOTIFICATION_ORDER_TO_CLOSE:
            {
                SetOrderToCloseState(ORDER_TO_CLOSE_STATE_RECEIVED);
            }
            break;

            default:
                NN_PANIC_("applet_API: unknown notification\n");
            }
        }
    }
}
} // namespace detail
} // namespace CTR
} // namespace applet
} // namespace nn

/* Application Info */

namespace nn{
namespace applet{
namespace CTR{
namespace{
    bool                        isAppletMode = false;
    bool                        isActive = false;
    u32                         messageCommand = COMMAND_NONE;
    HomeButtonState             absoluteHomeButtonState = HOME_BUTTON_NONE;
    SleepSysState               sleepSysState = SLEEP_SYS_STATE_NONE;
    ShutdownState               shutdownState = SHUTDOWN_STATE_NONE;
    PowerButtonState            powerButtonState = POWER_BUTTON_STATE_NONE;
    OrderToCloseState           orderToCloseState = ORDER_TO_CLOSE_STATE_NONE;
    bool                        isToCallPowerButtonCallback = false;
    bool                        isToCallShutdownCallback = false;
    bool                        isReceivedWakeupByCancelFlag = false;
    TransitionType              prevTransition = TRANSITION_NONE;
    SleepNotificationState      sleepNotificationState = NOTIFY_NONE;
    HomeButtonState             homeButtonState = HOME_BUTTON_NONE;
    bool                        isExpectedToJumpToHomeMenu = false;
}

CTR::AppletAttr GetAttribute()
{
    return selfAppletAttr;
}

CTR::AppletAttr GetAppletType()
{
    return GetAttribute() & 7;
}

void SetAttribute(CTR::AppletAttr attribute)
{
    attribute = attribute;
}

bool IsSystemApplet()
{
    return selfAppletAttr & 7 == 2;
}

bool IsApplication()
{
    return selfAppletAttr & 7 == 0;
}

bool IsInfoAccess()
{
    return selfAppletAttr & 7 == 6;
}

bool IsToCallShutdownCallback()
{
    return isToCallShutdownCallback;
}

void SetHomeButtonState(CTR::HomeButtonState state)
{
    homeButtonState = state;
}

CTR::HomeButtonState GetHomeButtonState()
{
    return homeButtonState;
}

void SetExpectationToJumpToHome(bool flag)
{
    isExpectedToJumpToHomeMenu = flag;
}

bool IsExpectedToJumpToHomeMenu()
{
    return isExpectedToJumpToHomeMenu;
}

CTR::AppletId GetId()
{
    return selfAppletId;
}

void SetId(CTR::AppletId aptId)
{
    selfAppletId = aptId;
}

u32 GetMessageCommand()
{
    return messageCommand;
}

void SetMessageCommand(u32 message)
{
    messageCommand = message;
}

SleepNotificationState GetSleepNoticationState()
{
    return sleepNotificationState;
}

void SetSleepNotificationState(SleepNotificationState state)
{
    sleepNotificationState = state;
}

TransitionType GetTransitionType()
{
    return prevTransition;
}

void SetTransitionType(TransitionType type)
{
    prevTransition = type;
}

void SetShutdownCallbackFlag()
{
    isToCallShutdownCallback = true;
}

void ClearShutdownCallbackFlag()
{
    isToCallShutdownCallback = false;
}

bool IsToShutdownCallback()
{
    return isToCallShutdownCallback;
}

void SetPowerButtonCallbackFlag()
{
    isToCallPowerButtonCallback = 1;
}

bool IsToCallPowerButtonCallback()
{
    return isToCallPowerButtonCallback;
}

void ClearPowerButtonCallbackFlag()
{
    isToCallPowerButtonCallback = 0;
}

void SetReceivedWakeupByCancelFlag()
{
    isReceivedWakeupByCancelFlag = true;
}

bool IsReceivedWakeupByCancel()
{
    return isReceivedWakeupByCancelFlag;
}

void SetOrderToCloseState(OrderToCloseState state)
{
    orderToCloseState = state;
}

namespace detail{

CTR::HomeButtonState GetAbsoluteHomeButtonState()
{
    return CTR::absoluteHomeButtonState;
}

void SetAbsoluteHomeButtonState(CTR::HomeButtonState state)
{
    CTR::absoluteHomeButtonState = state;
}

void ClearAbsoluteHomeButtonState()
{
    CTR::absoluteHomeButtonState = HOME_BUTTON_NONE;
}

CTR::SleepSysState GetSleepSysState()
{
    return CTR::sleepSysState;
}

void SetSleepSysState(CTR::SleepSysState state)
{
    CTR::sleepSysState = state;
}

bool IsActive()
{
    return CTR::isActive;
}

void SetActive()
{
    CTR::isActive = true;
}

void SetInactive()
{
    CTR::isActive = false;
}

CTR::PowerButtonState GetPowerButtonState()
{
    return CTR::powerButtonState;
}

void SetPowerButtonState(CTR::PowerButtonState state)
{
    CTR::powerButtonState = state;
}

CTR::OrderToCloseState GetOrderToCloseState()
{
    return CTR::orderToCloseState;
}

void ClearSleepSysState()
{
    CTR::sleepSysState = SLEEP_SYS_STATE_NONE;
}

void SetShutdownState(CTR::ShutdownState state)
{
    CTR::shutdownState = state;
}

bool IsAppletMode()
{
    return CTR::isAppletMode;
}

}
}
}
}

/* Application Connecting */

namespace nn{
namespace applet{
namespace CTR{
namespace detail{
namespace
{
    const char* portName = PORT_NAME_USER;
}

void SetPortName(const char* name)
{
    if(portName == NULL)
    {
        portName = name;
    }
}

Result InitializePort(Handle* pSession)
{
    SetPortName(PORT_NAME_USER);
    if(pSession->IsValid())
    {
        return ResultAlreadyInitialized();
    }
    return srv::GetServiceHandle(pSession,portName);
}

Result FinalizePort(Handle* pSession)
{
    if (!pSession->IsValid())
    {
        return ResultNotInitialized();
    }

    Result result = nn::svc::CloseHandle(*pSession);
    *pSession = INVALID_HANDLE_VALUE;
    return result;
}

void Lock()
{
    if(appletMutex.IsValid())
    {
        appletMutex.Lock();
    }
}

void Unlock()
{
    if(appletMutex.IsValid())
    {
        appletMutex.Unlock();
    }
}

Result Connect()
{
    Result res = InitializePort(&APPLET::s_Session);
    NN_ERR_THROW_FATAL(res);
    return res;
}

Result Disconnect()
{
    Result res = FinalizePort(&APPLET::s_Session);
    NN_ERR_THROW_FATAL(res);
    return res;
}

void LockAndConnect()
{
    Lock();
    Connect();
}

void DisconnectAndUnlock()
{
    Disconnect();
    Unlock();
}

void InitializeMutex(nn::Handle handle)
{
    nn::os::HandleManager::AttachHandle(&appletMutex, handle);
}

}
}
}
}

/* Application Parameters */

namespace nn{
namespace applet{
namespace CTR {
namespace detail {
namespace {
    bool              isInitialParamValid = false;
    AppletId          initialSenderId;
    u8                initializeParamBuffer[4096];
    s32               initialParamBufferSize;
    AppletWakeupState initialWakeupState;
}

u8* GetInitialParamBuffer()
{
    return initializeParamBuffer;
}

void SetInitialParamSenderId(AppletId id)
{
    initialSenderId = id;
}

void SetInitialParamSenderSize(s32 size)
{
    initialParamBufferSize = size;
}

void SetInitialParamValid()
{
    isInitialParamValid = true;
}

void SetInitialWakeupState(WakeupState state)
{
    initialWakeupState = state;
}

}
}
}
}

namespace{
    bool                        isInitialized = false;
    bool                        isGpuRightGiven = false;
    bool                        isDspSleeping   = false;
    nn::fnd::TimeSpan           sleepSpan;

    class ExitHandler : public NotificationHandler
    {
    public:
        virtual void HandleNotification(bit32 message)
        {
            NN_TLOG_("**** Exit handle id=%x \n", GetId());
        }
    };

    ExitHandler exitHandler;
}

namespace nn{
namespace applet{
namespace CTR{

nn::Handle HANDLE_NONE = 0;

bool IsInitialized()
{
    return isInitialized;
}

namespace detail{
namespace{

inline bool DisableSleepForTransition()
{
    bool isSleep = CTR::IsEnableSleep();
    if(isSleep) DisableSleep(true);
    return isSleep;
}

inline bool EnableSleepForTransition()
{
    bool isSleep = CTR::IsEnableSleep();
    if(!isSleep) EnableSleep(true);
    return isSleep;
}

inline void RestoreSleepForTransition(bool e)
{
    if(e)
        EnableSleepForTransition();
    else
        DisableSleepForTransition();
}

bool s_IsVramSaved = false;

inline Result SaveVramSysArea()
{
    s_IsVramSaved = true;
    return gxlow::CTR::SaveVramSysArea();
}

} // namespace

/* Misc Funcs */

/* JumpToHomeMenu_Func */

class JumpToHomeMenu_Func
{
public:
    static Result PrepareCore();
};

Result JumpToHomeMenu_Func::PrepareCore()
{
    return APPLET::PrepareToJumpToHomeMenu();
}

/* StartApplicationApplet_Func */

class StartApplicationApplet_Func
{
public:
    static u32* pLaunchInfo;
    static u8* pParam;
    static u32* pHmacBuf;
    static size_t paramSize;
    static size_t hmacBufSize;
    static AppletId id;
};

u32* StartApplicationApplet_Func::pLaunchInfo;
u8* StartApplicationApplet_Func::pParam;
u32* StartApplicationApplet_Func::pHmacBuf;
size_t StartApplicationApplet_Func::paramSize;
size_t StartApplicationApplet_Func::hmacBufSize;
AppletId StartApplicationApplet_Func::id;

/* StartLibraryApplet_Func */

class StartLibraryApplet_Func
{
public:
    static Result PrepareCore();
    static Result StartCore();

    static AppletId id;
    static u8* pParam;
    static size_t paramSize;
    static Handle* pHandle;
};

AppletId StartLibraryApplet_Func::id;
u8* StartLibraryApplet_Func::pParam;
size_t StartLibraryApplet_Func::paramSize;
Handle* StartLibraryApplet_Func::pHandle;

Result StartLibraryApplet_Func::PrepareCore()
{
    return APPLET::PrepareToStartLibraryApplet(id);
}

Result StartLibraryApplet_Func::StartCore()
{
    return APPLET::StartLibraryApplet(id, pParam, paramSize, *pHandle);
}

/* StartSystemApplet_Func */

class StartSystemApplet_Func
{
public:
    static Result PrepareCore();
    static Result StartCore();

    static AppletId id;
    static u8* pParam;
    static size_t paramSize;
    static Handle* pHandle;
};

AppletId StartSystemApplet_Func::id;
u8* StartSystemApplet_Func::pParam;
size_t StartSystemApplet_Func::paramSize;
Handle* StartSystemApplet_Func::pHandle;

Result StartSystemApplet_Func::PrepareCore()
{
    return APPLET::PrepareToStartSystemApplet(id);
}

Result StartSystemApplet_Func::StartCore()
{
    return APPLET::StartSystemApplet(id, pParam, paramSize, *pHandle);
}

/* StartNewestHomeMenuApplet_Func */

class StartNewestHomeMenuApplet_Func
{
public:
    static u8* pParam;
    static size_t paramSize;
    static Handle* pHandle;
};

u8* StartNewestHomeMenuApplet_Func::pParam;
size_t StartNewestHomeMenuApplet_Func::paramSize;
Handle* StartNewestHomeMenuApplet_Func::pHandle;

/* StartResidentApplet_Func */

class StartResidentApplet_Func
{
public:
    static u8* pParam;
    static size_t paramSize;
    static Handle* pHandle;
};

u8* StartResidentApplet_Func::pParam;
size_t StartResidentApplet_Func::paramSize;
Handle* StartResidentApplet_Func::pHandle;

/* PreloadResidentApplet_Func */

class PreloadResidentApplet_Func
{
public:
    static AppletId id;
};

AppletId PreloadResidentApplet_Func::id;

/* PreloadLibraryApplet_Func */

class PreloadLibraryApplet_Func
{
public:
    static Result CancelCore();
    static Result PrepareCore();

    static AppletId id;
    static bool isApplicationEnd;
};

AppletId PreloadLibraryApplet_Func::id;
bool PreloadLibraryApplet_Func::isApplicationEnd;

Result PreloadLibraryApplet_Func::CancelCore()
{
    return APPLET::CancelLibraryApplet(isApplicationEnd);
}

Result PreloadLibraryApplet_Func::PrepareCore()
{
    return APPLET::PreloadLibraryApplet(id);
}

/* CloseApplication_Func */

class CloseApplication_Func
{
public:
    static Result CloseCore();
    static Result PrepareCore();

    static bool isToJumpHomeOrSystem;
    static u8* pParam;
    static size_t paramSize;
    static Handle* pHandle;
};

bool CloseApplication_Func::isToJumpHomeOrSystem;
u8* CloseApplication_Func::pParam;
size_t CloseApplication_Func::paramSize;
Handle* CloseApplication_Func::pHandle;

Result CloseApplication_Func::PrepareCore()
{
    return APPLET::PrepareToCloseApplication(isToJumpHomeOrSystem);
}

Result CloseApplication_Func::CloseCore()
{
    return APPLET::CloseApplication(pParam, paramSize, *pHandle);
}

/* JumpApplication_Func */

class JumpApplication_Func
{
public:
    static u8* pParam;
    static u32* pHmacBuf;
    static size_t paramSize;
    static size_t hmacBufSize;
    static Handle* pHandle;
};

u8* JumpApplication_Func::pParam;
u32* JumpApplication_Func::pHmacBuf;
size_t JumpApplication_Func::paramSize;
size_t JumpApplication_Func::hmacBufSize;

/* Result Function for `_func` classes. */

Result ExecFunctionTillSuccess(Result (*function)(), fnd::TimeSpan timeout = WAIT_INFINITE)
{
    os::Tick startTick = os::Tick::GetSystemCurrent();

    Result result;

    for(;;)
    {
        LockAndConnect();

        result = function();

        DisconnectAndUnlock();

        if (result.IsSuccess())
            break;

        if (result.GetDescription() == nn::Result::DESCRIPTION_BUSY || 
            result.GetDescription() == DESCRIPTION_APPLET_TRANSITION_BUSY || 
            result.GetDescription() == DESCRIPTION_ALREADY_LISTED)
        {
            os::Thread::Sleep(sleepSpan);
            continue;
        }

        break;
    }

    return result;
}

/* Rights */

void AssignGpuRight(bool flag)
{
    Result res;
    if(flag)
    {
        isGpuRightGiven = true;
        res = gxlow::CTR::AcquireGpuRight();
        NN_ERR_THROW_FATAL(res);
    }
    else
    {
        if (!isGpuRightGiven)
        {
            return;
        }
        isGpuRightGiven = false;
        res = gxlow::CTR::ReleaseGpuRight();
        if(res == nn::gxlow::CTR::ResultNotRegistered())
        {
            return;
        }
        else if(res == nn::os::ResultInvalidHandle())
        {
            NN_TLOG_("applet_API: Warning: Release GPU right despite no gx init.\n");
        }
        else
        {
            NN_ERR_THROW_FATAL(res);
        }
    }
}

void AssignDspRight(bool flag)
{
    if(flag)
    {
        if(isDspSleeping)
        {
            dsp::CTR::WakeUp();
            isDspSleeping = false;
        }
    }
    else
    {
        if(dsp::CTR::IsComponentLoaded())
        {
            dsp::CTR::Sleep();
            isDspSleeping = true;
        }
    }
}

void AssignCameraRight(bool flag)
{
    if(flag)
    {
        camera::CTR::detail::LeaveApplication();
    }
    else
    {
        camera::CTR::detail::ArriveApplication();
    }
}

/* Initialization */

Result $Sub$$Initialize(AppletAttr appletAttr)
{
    if (!detail::IsAppletMode())
    {
        isGpuRightGiven = true;
        appletAttr = AppletAttr(appletAttr & ~7);
        nn::srv::RegisterNotificationHandler(&exitHandler, 0x100);
        Result result = detail::InitializeConnect(0x300, appletAttr, 0xF);
        if (result.IsFailure())
        {
            return result;
        }
    }
    return ResultSuccess();
}

Result InitializeConnect(AppletId appletId, AppletAttr appletAttr, s32 threadPriority)
{
    if (isInitialized)
    {
        return nn::applet::CTR::ResultAlreadyInitialized();
    }
    isInitialized = true;

    Connect();
    {
        Handle handle;
        AppletAttr attrDecided;
        bit32 miscState;
        Result res = APPLET::GetLockHandle(&handle, appletAttr, &attrDecided, &miscState);
    
        if (!res.IsSuccess())
        {
            Disconnect();
            return res;
        }
        InitializeMutex(handle);
        
        appletAttr = attrDecided;
        
        SetPowerButtonState((miscState & MISC_STATE_POWER_BUTTON) ? POWER_BUTTON_STATE_CLICK: POWER_BUTTON_STATE_NONE);
        SetOrderToCloseState((miscState & MISC_STATE_SHUTDOWN_PROCESSING) ? ORDER_TO_CLOSE_STATE_RECEIVED: ORDER_TO_CLOSE_STATE_NONE);
    }
    Disconnect();
    
    SetId(appletId);
    SetAttribute(appletAttr);

    if (IsInfoAccess())
        return ResultSuccess();
    LockAndConnect();
    {
        SetActive();
        nn::Handle handleForCont;
        nn::Handle handleForMesg;
        
        Result res = APPLET::Initialize(GetId(), GetAttribute(), &handleForMesg, &handleForCont);
        NN_ERR_THROW_FATAL(res);
        
        InitializeWrapper();
        InitializeClientThread(threadPriority);
    }
    DisconnectAndUnlock();

    if (!IsApplication())
    {
        gxlow::CTR::SetAppletMode();
    }
    return ResultSuccess();
}

/* Reply Sleep */

void ReplySleepQueryToManager(QueryReply reply)
{
    Result res;
    LockAndConnect();
    res = APPLET::ReplySleepQuery(GetId(), reply);
    NN_ERR_THROW_FATAL(res);
    DisconnectAndUnlock();
}

void ReplySleepNotificationCompleteToManager()
{
    Result res;
    LockAndConnect();
    res = APPLET::ReplySleepNotificationComplete(GetId());
    NN_ERR_THROW_FATAL(res);
    DisconnectAndUnlock();
}

void Enable(bool isSleepEnable)
{
    NN_TASSERTMSG_(!nn::gxlow::CTR::IsInitialized(), "%s must be called before initializing graphics library\n", NN_FUNCTION );
    NN_TASSERTMSG_(!nn::camera::CTR::detail::IsInitialized(), "%s must be called before initializing camera\n", NN_FUNCTION );
    NN_TASSERTMSG_(!nn::dsp::CTR::IsComponentLoaded(), "%s must be called before loading dspcomponent\n", NN_FUNCTION );
    if(isSleepEnable)
    {
        EnableSleep(false);
    }

    LockAndConnect();
    Result result = APPLET::Enable(GetAttribute());
    NN_ERR_THROW_FATAL(result);
    DisconnectAndUnlock();
    if(nn::applet::CTR::IsApplication() && !(GetAttribute(), & *(AppletAttr*)0x20))
    {
        AppletId id;
        s32 size;
        SetTransitionType(TRANSITION_ENABLE_APPLET);
        WakeupState state = WaitForStarting(&id,GetInitialParamBuffer(),0x1000,&size);
        SetInitialParamSenderId(id);
        SetInitialParamSenderSize(size);
        SetInitialParamValid();
        SetInitialWakeupState(state);
    }
}

/* Get Applet Things */

AppletId GetHomeMenuAppletId()
{
    AppletPos pos;
    AppletId id1; 
    AppletId id2; 
    AppletId id3; 
    LockAndConnect();
    Result res = APPLET::GetAppletManInfo(POS_NONE, &pos, &id1, &id2, &id3);
    NN_ERR_THROW_FATAL(res);
    DisconnectAndUnlock();
    return id2;
}

void GetAppletManInfo(AppletPos requestPos,AppletPos *pCurrentPos,AppletId *pRequestedId,AppletId *pHomeMenuId,AppletId *pCurrentId)
{
    AppletPos currentPos; AppletId requestedId; AppletId homeMenuId; AppletId currentId; Result result;
    LockAndConnect();
    result = APPLET::GetAppletManInfo(requestPos, &currentPos, &requestedId, &homeMenuId, &currentId);
    NN_ERR_THROW_FATAL(result);
    DisconnectAndUnlock();
    if (pCurrentPos)  *pCurrentPos  = currentPos;
    if (pRequestedId) *pRequestedId = requestedId;
    if (pHomeMenuId)  *pHomeMenuId  = homeMenuId;
    if (pCurrentId)   *pCurrentId   = currentId;
}

bool GetAppletInfo(AppletId appletId, ProgramId* pProgramId, nn::fs::MediaType* pMediaType, bool* pIsUsed, bool* pIsPreloaded, AppletAttr* pAttr)
{
    Result res;
    ProgramId programId;
    nn::fs::MediaType mediaType;
    bool isUsed;
    bool isPreloaded;
    AppletAttr appletAttr;

    detail::LockAndConnect();
    res = APPLET::GetAppletInfo(appletId, &programId, &mediaType, &isUsed, &isPreloaded, &appletAttr);
    detail::DisconnectAndUnlock();

    if (res.IsSuccess())
    {
        if (pProgramId)
            *pProgramId = programId;
        if (pMediaType)
            *pMediaType = mediaType;
        if (pIsUsed)
            *pIsUsed = isUsed;
        if (pIsPreloaded)
            *pIsPreloaded = isPreloaded;
        if (pAttr)
            *pAttr = appletAttr;
        return true;
    }
    return false;
}

/* APT Registers */

bool IsRegistered(AppletId id)
{
    bool isRegistered;

    LockAndConnect();
    Result res = APPLET::IsRegistered(id, &isRegistered);
    NN_ERR_THROW_FATAL(res);
    DisconnectAndUnlock();
    return isRegistered;
}

bool WaitForRegister(AppletId appletId, nn::fnd::TimeSpan span)
{
    TimeoutChecker checker(span);

    while(!detail::IsRegistered(appletId))
    {
        if (checker.Check())
        {
            return false;
        }
        WaitBySleep(10);
    }
    return true;
}

/* Sending Applet Parameters */

Result TrySend(AppletId receiverId, u32 command, const u8* pParam, size_t paramSize, Handle handle)
{
    bool isFinalize = (command & 0x10000) ? true: false;
    if(paramSize > 0x1000)
        NN_TPANIC_("Too long parameter buffer size");
    if((pParam == NULL) || (paramSize == 0)){
        pParam = 0;
        paramSize = 0;
    }
    Result res;
    LockAndConnect();
    res = APPLET::SendParameter(GetId(), receiverId, command, pParam, paramSize, handle);
    if (res.IsSuccess())
    {
        if (isFinalize)
        {
            FinalizeClientThread();
        }
    }
    DisconnectAndUnlock();
    return res;
}

Result Send(AppletId receiverId, u32 command, const u8* pParam, size_t paramSize, nn::Handle handle, nn::fnd::TimeSpan timeout)
{
    TimeoutChecker checker(timeout);
    Result res;
    for(;;)
    {
        res = TrySend( receiverId, command, pParam, paramSize, handle );
        if (res.IsSuccess())
        {
            break;
        }
        else if (res == ResultNotEmpty())
        {
            if (checker.Check())
            {
                break;
            }
        }
        else
        {
             break;
        }

        WaitBySleep(10);
    }

    return res;
}

/* Receiving APT Parameters */

Result TryReceive(AppletId *pSenderId,u32 *pCommand,u8 *pParam,size_t paramSize,s32 *pReadLen, Handle *pHandle,bool isTry)
{
    Result res;
    if(!isTry)
    {
        WaitForControlEvent();
    }
    else
    {
        if(!TryWaitForControlEvent())
        {
            return ResultNoData();
        }
    }
    if(!GetMessageCommand())
    {
        AppletId dummyId;
        pSenderId = (!pSenderId)? &dummyId: pSenderId;

        u32 dummyCommand;
        pCommand = (!pCommand)? &dummyCommand: pCommand;

        u8 dummyParam;
        paramSize = (!pParam)? 0: paramSize;
        pParam = (paramSize==0)? &dummyParam: pParam;

        s32 dummyLen;
        pReadLen = (!pReadLen)? &dummyLen: pReadLen;

        nn::Handle dummyHandle = nn::Handle();
        pHandle = (!pHandle)? &dummyHandle: pHandle;
        LockAndConnect();
        res = APPLET::ReceiveParameter(pSenderId, GetId(), pCommand, pParam, paramSize, pReadLen, pHandle);
        DisconnectAndUnlock();
        if(dummyHandle.IsValid()) svc::CloseHandle(dummyHandle);
    }
    else
    {
        *pCommand = GetMessageCommand();
        SetMessageCommand(0);
        ClearControlEvent();
        res = ResultSuccess();
        if(pSenderId) *pSenderId = 0;
        if(pReadLen) *pReadLen = 0;
        if(pHandle) *pHandle = HANDLE_NONE;
    }
    ClearControlEvent();
    return res;
}

Result Receive(AppletId* pSenderId, u32* pCommand, u8* pParam, size_t paramSize, s32* pReadLen, nn::Handle *pHandle, nn::fnd::TimeSpan timeout)
{
    Result res;
    if (timeout == WAIT_INFINITE){
        res = TryReceive(pSenderId, pCommand, pParam, paramSize, pReadLen, pHandle, false);
        return res;
    }

    TimeoutChecker checker(timeout);
    for(;;)
    {
        res = TryReceive(pSenderId, pCommand, pParam, paramSize, pReadLen, pHandle, true);
        if (res.IsSuccess())
        {
            break;
        }
        else if (res == ResultNoData())
        {
            if (checker.Check())
            {
                    break;
            }
        }
        WaitBySleep(10);
    }

    return res;
}

Result Glance(AppletId* pSenderId, u32* pCommand, u8* pParam, size_t paramSize, s32* pReadLen, Handle* pHandle)
{
    Result res;

    LockAndConnect();
    {
        AppletId tmpSenderId;
        AppletId* pSenderId0 = (pSenderId)? pSenderId: &tmpSenderId;

        u32 tmpCommand;
        u32* pCommand0 = (pCommand)? pCommand: &tmpCommand;

        u8 tmpBuf[1];
        u8* pParam0 = pParam;
        if (pParam == NULL || paramSize == 0)
        {
            pParam0 = &tmpBuf[0];
            paramSize = 0;
        }
        s32 tmpReadLen;
        s32* pReadLen0 = (pReadLen)? pReadLen: &tmpReadLen;

        nn::Handle tmpHandle = nn::Handle();
        nn::Handle* pHandle0 = (pHandle)? pHandle: &tmpHandle;

        res = detail::APPLET::GlanceParameter(pSenderId0, GetId(), pCommand0, pParam0, paramSize, pReadLen0, pHandle0);
        if (tmpHandle.IsValid())
        {
            svc::CloseHandle(tmpHandle);
        }
    }
    DisconnectAndUnlock();
    return res;
}

/* Cancelling APT Parameters */

bool CancelParameter(bool isSenderCheck, nn::applet::CTR::AppletId senderId, bool isReceiverCheck, nn::applet::CTR::AppletId receiverId)
{
    bool isCanceled;
    LockAndConnect();
    Result res = APPLET::CancelParameter(isSenderCheck, senderId, isReceiverCheck, receiverId, &isCanceled);
    NN_ERR_THROW_FATAL(res);
    DisconnectAndUnlock();
    return isCanceled;
}

Result SendMessage(AppletId receiverId, const u8* pParam, size_t paramSize, nn::Handle handle, nn::fnd::TimeSpan timeout)
{
    return detail::Send( receiverId, COMMAND_MESSAGE, pParam, paramSize, handle, timeout );
}

/* APT Utilitys */

inline Result CallUtility(u32 utilityId)
{
    return CallUtility(utilityId,0,0,0,0,0);
}

inline Result CallUtility(u32 utilityId, u8* pInParam, size_t inParamSize)
{
    return CallUtility(utilityId, pInParam, inParamSize, 0,0,0);
}

void UnlockTransition(u32 action)
{
    Result res = CallUtility(7,reinterpret_cast<u8*>(&action), sizeof(action) );
    NN_UNUSED_VAR(res);
}

void LockTransition(u32 action,bool isForced)
{
    LockTransitionParam param = {action, isForced};
    Result res = CallUtility(5,reinterpret_cast<u8*>(&param), sizeof(LockTransitionParam));
    NN_UNUSED_VAR(res);
}

void SleepIfShellClosed() {
    Result result = CallUtility(4);
    NN_UNUSED_VAR(result);
}

bool IsRetryRequired(Result result)
{
    if (result == ResultBusy() || result == ResultTransitionBusy() || result == ResultNotEmpty())
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* Library Applet */

Result CancelLibraryApplet(bool isApplicationEnd)
{
    Result res;
    SetTransitionType(TRANSITION_CANCEL_APPLIB);

    PreloadLibraryApplet_Func::isApplicationEnd = isApplicationEnd;
    res = ExecFunctionTillSuccess(PreloadLibraryApplet_Func::PrepareCore);

    return res;
}

Result CancelLibraryAppletIfRegistered(bool isApplicationEnd, AppletWakeupState* pWakeupState)
{
    Result res = ResultSuccess();
    if(pWakeupState)
    {
        *pWakeupState = WAKEUP_SKIP;
    }
    if((!IsApplication() || IsRegistered(0x400)) && (!IsSystemApplet() || IsRegistered(0x200)))
    {
        res = CancelLibraryApplet(isApplicationEnd);
        if(res == ResultSuccess())
        {
            AppletWakeupState wakeup;
            wakeup = WaitForStarting();
            if(pWakeupState)
                *pWakeupState = wakeup;
            
        }
    }
    return res;
}

/* SystemApplet */

Result PrepareToStartSystemApplet(AppletId id)
{
    Result res;
    NN_TASSERTMSG_(!nngxGetIsRunning(), "Running command requests must be stopped before transition to menu.\n");
    CancelLibraryAppletIfRegistered(false);
    SetTransitionType(TRANSITION_START_SYS);
    bool sleep = DisableSleepForTransition();

    StartLibraryApplet_Func::id = id;
    res = ExecFunctionTillSuccess(StartLibraryApplet_Func::PrepareCore);

    RestoreSleepForTransition(sleep);
    if(IsApplication() && res == ResultAlreadyExist())
    {
        res = ResultSuccess();
    }
    return res;
}

/* StartSystemApplet */

Result StartSystemApplet(AppletId id,u8* pParam,size_t paramSize,Handle h){
    Result res;

    if (!IsApplication() && !IsSystemApplet())
    {
        return ResultNotAllowed();
    }

    if (IsSystemApplet())
    {
        AssignGpuRight(false);
    }
    else if (IsApplication())
    {
        SaveVramSysArea();

        AssignDspRight(false);
        AssignGpuRight(false);
        AssignCameraRight(false);
    }

    bool sleepEnabled = DisableSleepForTransition();

    StartSystemApplet_Func::id = id;
    StartSystemApplet_Func::pParam = pParam;
    StartSystemApplet_Func::paramSize = paramSize;
    StartSystemApplet_Func::pHandle = &h;
    res = ExecFunctionTillSuccess(StartSystemApplet_Func::StartCore);

    RestoreSleepForTransition(sleepEnabled);

    NN_ERR_THROW_FATAL_ALL(res);
    SetInactive();
    if (IsApplication())
        res = CaptureScreenForSystemApplet(id);

    if (IsSystemApplet())
        svc::ExitProcess();

    return res;
}

/* Close Applet */

Result PrepareToCloseApplication(bool isCancelPreload)
{
    CancelLibraryAppletIfRegistered(false);
    SetTransitionType(TRANSITION_CLOSE_APP);
    Result res;

    CloseApplication_Func::isToJumpHomeOrSystem = isCancelPreload;
    res = ExecFunctionTillSuccess(CloseApplication_Func::PrepareCore);

    NN_ERR_THROW_FATAL(res);
    return res;
}

Result CloseApplication(u8* pParam, size_t paramSize, Handle handle)
{
    if(GetTransitionType() != TRANSITION_CLOSE_APP)
    {
        PrepareToCloseApplication(false);
    }
    CloseAppletHook();
    AssignGpuRight(false);
    Result res;

    res = ExecFunctionTillSuccess(CloseApplication_Func::CloseCore);

    NN_ERR_THROW_FATAL(res);
    SetInactive();
    svc::ExitProcess();
    return res;
}

void AttachTransferMemoryHandle(os::TransferMemoryBlock* transferMemory, nn::Handle handle, size_t size, bit32 otherPermission)
{
    nn::os::HandleManager::AttachTransferMemoryBlockHandle(transferMemory, handle, size, otherPermission);
}

/* Starting a Library APT, WIP */

Result StartLibraryApplet(AppletId id, const u8* pParam,size_t paramSize, Handle handle)
{
    Result res;

    if((!IsApplication()) && (!IsSystemApplet()))
    {
        return ResultNotAllowed();
    }
    
    if(IsApplication())
    {
        SaveVramSysArea();
    }
    
    res = CaptureScreen(id);
    AssignGpuRight(false);
    bool sleep = DisableSleepForTransition();

    StartLibraryApplet_Func::id = id;
    StartLibraryApplet_Func::pParam = (u8*)pParam;
    StartLibraryApplet_Func::paramSize = paramSize;
    StartLibraryApplet_Func::pHandle = &handle;
    res = ExecFunctionTillSuccess(StartLibraryApplet_Func::StartCore);

    RestoreSleepForTransition(sleep);
    NN_ERR_THROW_FATAL(res);
    SetInactive();

    return res;
}

/* Jumping to HomeMenu */

Result PrepareToJumpToHomeMenu()
{
    SetTransitionType(TRANSITION_JUMP_HOME);
    return ExecFunctionTillSuccess(JumpToHomeMenu_Func::PrepareCore);
}

Result JumpToHomeMenu(u8* pParam, size_t paramSize, Handle handle)
{
    AppletId appletId; AppletId homemenuId; 
    Result res; 
    Handle hand_local;
    appletId = CTR::detail::GetHomeMenuAppletId();
    if(IsApplication())
    {
        while(!IsRegistered(appletId))
        {
            WaitBySleep(10);
        }
        SaveVramSysArea();
        res = CaptureScreenForSystemApplet(appletId);
    }
    AssignDspRight(false);
    AssignGpuRight(false);
    AssignCameraRight(false);
    LockAndConnect();
    res = APPLET::JumpToHomeMenu(pParam, paramSize, handle);
    NN_ERR_THROW_FATAL(res);
    DisconnectAndUnlock();
    SetInactive();
    return res;
}

void NotifyToWait()
{
    Result res;
    LockAndConnect();
    res = APPLET::NotifyToWait(GetId());
    NN_ERR_THROW_FATAL(res);
    DisconnectAndUnlock();
}

Result SendCaptureBufferInfo(u8* pParam, size_t paramSize)
{
    Result res;
    LockAndConnect();
    res = APPLET::SendCaptureBufferInfo(pParam,paramSize);
    DisconnectAndUnlock();
    return res;
}

Result CallUtility(u32 utilityId, u8* pInParam, size_t inParamSize, u8* pOutParam, size_t outParamSize, s32* pReadSize)
{
    Result res;
    u8 dummyInParam[1];
    u8 dummyOutParam[1];
    s32 dummyReadSize;
    size_t paramInSize;
    size_t paramOutSize;
    u8* pParamOut;
    LockAndConnect();
    {
        res = APPLET::AppletUtility(utilityId,(pInParam && inParamSize>0)? pInParam: dummyInParam, (pInParam && inParamSize>0)? inParamSize: 1,(pOutParam && outParamSize>0)? pOutParam: dummyOutParam, (pOutParam && outParamSize>0)? outParamSize: 1, &dummyReadSize);
    }
    NN_ERR_THROW_FATAL(res);

    if (!pOutParam || outParamSize==0) dummyReadSize = 0;
    if(pReadSize) *pReadSize = dummyReadSize;

    DisconnectAndUnlock();

    return res;
}

/* Application Wrapping */

Result Wrap(void* pWrappedBuffer, const void* pData, size_t dataSize, s32 idOffset, size_t idSize)
{
    Result result;
    detail::LockAndConnect();
    result = detail::APPLET::Wrap(reinterpret_cast<bit8*>(pWrappedBuffer), reinterpret_cast<const bit8*>(pData), dataSize + WRAP_SIZE, dataSize, idOffset, idSize);
    detail::DisconnectAndUnlock();
    return result;
}

Result Unwrap(void* pData, const void* pWrapped, size_t wrappedSize, s32 idOffset, size_t idSize)
{
    Result result;
    detail::LockAndConnect();
    result = detail::APPLET::Unwrap(reinterpret_cast<bit8*>(pData),reinterpret_cast<const bit8*>(pWrapped), wrappedSize - WRAP_SIZE, wrappedSize, idOffset, idSize);
    detail::DisconnectAndUnlock();
    return result;
}

} // detail
} // CTR
} // applet
} // nn

