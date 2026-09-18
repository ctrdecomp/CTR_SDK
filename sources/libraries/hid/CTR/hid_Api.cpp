// Filename: hid_Api.cpp
//
// Project: Horizon

#include <nn/hid/CTR/hid_Api.h>
#include <nn/hid/CTR/hid_IpcClient.h>
#include <nn/hidlow/CTR/hidlow_LifoRingCollector.h>
#include <nn/srv/srv_API.h>
#include <nn/Assert.h>
#include <nn/os/os_HandleManager.h>

namespace nn{
namespace hid{
namespace CTR{

struct HidDevices
{
    Pad pad;
    TouchPanel touchPanel;
    Accelerometer accelerometer;
    Gyroscope gyroscope;
    DebugPad debugPad;
    os::SharedMemoryBlock m_SharedMemory;

    void Finalize();
    Result Initialize(const char* portName);
};

Result MakeResultAlreadyInitialized()
{
    return nn::MakeUsageResult(Result::SUMMARY_INVALID_STATE, Result::MODULE_NN_HID, Result::DESCRIPTION_ALREADY_INITIALIZED);
}

HidDevices s_Devices;
bool isInitialized;

Result HidDevices::Initialize(const char* portName)
{
    Handle hSharedMemory;
    Result res;
    Handle padEventHandle;
    Handle touchEventHandle;
    Handle accelerometerEventHandle;
    Handle gyroscopeEventHandle;
    Handle debugPadEventHandle;
    if(isInitialized)
    {
        return MakeResultAlreadyInitialized();
    }

    res = srv::Initialize();
    if(res.GetDescription() != Result::DESCRIPTION_ALREADY_INITIALIZED)
        NN_UTIL_PANIC_IF_FAILED(res);
    res = srv::GetServiceHandle(&detail::Ipc::s_Session, portName);
    NN_UTIL_PANIC_IF_FAILED(res);

    res = detail::Ipc::GetIPCHandles(&hSharedMemory, &padEventHandle, &touchEventHandle, &accelerometerEventHandle, &gyroscopeEventHandle, &debugPadEventHandle);
    NN_UTIL_PANIC_IF_FAILED(res);

    nn::os::HandleManager::AttachSharedMemoryHandle(&this->m_SharedMemory, hSharedMemory, 0x2B0, true);
    uptr instanceAddress = this->m_SharedMemory.GetAddress();
    NN_TASSERT_(instanceAddress);

    hidlow::CTR::LifoRingCollector* ring;
    this->pad.SetResource(reinterpret_cast<uptr>(ring->GetPadLifoRingAddress()));
    this->touchPanel.SetResource(reinterpret_cast<uptr>(ring->GetTouchPanelLifoRingAddress()));
    this->accelerometer.SetResource(reinterpret_cast<uptr>(ring->GetAccelerometerLifoRingAddress()));
    this->gyroscope.SetResource(reinterpret_cast<uptr>(ring->GetGyroscopeLowLifoRingAddress()));
    this->debugPad.SetResource(reinterpret_cast<uptr>(ring->GetDebugPadLifoRingAddress()));

    nn::os::HandleManager::AttachHandle(&this->pad, padEventHandle);
    nn::os::HandleManager::AttachHandle(&this->touchPanel, touchEventHandle);
    nn::os::HandleManager::AttachHandle(&this->accelerometer, accelerometerEventHandle);
    nn::os::HandleManager::AttachHandle(&this->gyroscope, gyroscopeEventHandle);
    nn::os::HandleManager::AttachHandle(&this->debugPad, debugPadEventHandle);

    isInitialized = true;
    return ResultSuccess();
}

void HidDevices::Finalize()
{
    Result res;
    if(isInitialized)
    {
        res = svc::CloseHandle(nn::os::HandleManager::DetachHandle(&this->pad));
        NN_UTIL_PANIC_IF_FAILED(res);

        res = svc::CloseHandle(nn::os::HandleManager::DetachHandle(&this->touchPanel));
        NN_UTIL_PANIC_IF_FAILED(res);

        res = svc::CloseHandle(nn::os::HandleManager::DetachHandle(&this->accelerometer));
        NN_UTIL_PANIC_IF_FAILED(res);

        res = svc::CloseHandle(nn::os::HandleManager::DetachHandle(&this->gyroscope));
        NN_UTIL_PANIC_IF_FAILED(res);

        res = svc::CloseHandle(nn::os::HandleManager::DetachHandle(&this->debugPad));
        NN_UTIL_PANIC_IF_FAILED(res);

        res = svc::CloseHandle(nn::os::HandleManager::DetachHandle(&this->m_SharedMemory));
        NN_UTIL_PANIC_IF_FAILED(res);

        this->m_SharedMemory.Finalize();

        res = svc::CloseHandle(detail::Ipc::s_Session);
        NN_UTIL_PANIC_IF_FAILED(res);
        
        isInitialized = false;
    }
}

void Finalize()
{
    s_Devices.Finalize();
}

Result Initialize()
{
    return s_Devices.Initialize(PORT_NAME_USER);
}

Pad& GetPad()
{
    NN_TASSERT_(isInitialized);

    return s_Devices.pad;
}

DebugPad& GetDebugPad()
{
    NN_TASSERT_(isInitialized);

    return s_Devices.debugPad;
}

TouchPanel& GetTouchPanel()
{
    NN_TASSERT_(isInitialized);

    return s_Devices.touchPanel;
}

Accelerometer& GetAccelerometer()
{
    NN_TASSERT_(isInitialized);
    
    return s_Devices.accelerometer;
}

Gyroscope& GetGyroscope()
{
    NN_TASSERT_(isInitialized);

    return s_Devices.gyroscope;
}

}
}
}