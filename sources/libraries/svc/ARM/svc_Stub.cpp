// Filename: svc_Stub.cpp
//
// Project: Horizon

#include <nn/svc/svc_Stub.h>

namespace nn {
namespace svc {

// ControlMemory

asm nn::Result __attribute__((section("i._ZN2nn3svc13ControlMemoryEPjjjjjj"))) ControlMemory(uptr* pOut, uptr addr0, uptr addr1, size_t size, u32 op, u32 perms)
{
    push {r0, r4};
    ldr r0, [sp, #8];
    ldr r4, [sp, #0xc];
    svc 0x1;
    ldr r2, [sp];
    str r1, [r2];
    add sp, sp, #4;
    pop {r4};
    bx lr;
}

// QueryMemory

asm nn::Result __attribute__((section("i._ZN2nn3svc11QueryMemoryEPN2nn2os10MemoryInfoEPNS1_8PageInfoEj"))) QueryMemory(nn::os::MemoryInfo* info, nn::os::PageInfo* pageInfo, uint)
{
    push {r0, r6};
    svc 0x2;
    ldr r6, [sp, #0x0];
    str r1, [r6, #0x0];
    str r2, [r6, #0x4];
    str r3, [r6, #0x8];
    str r4, [r6, #0xc];
    ldr r6, [sp, #0x4];
    str r5, [r6, #0x0];
    add sp, sp, #8;
    pop {r4, r5, r6};
}

// ExitProcess

asm nn::Result __attribute__((section("i._ZN2nn3svc11ExitProcessEv"))) ExitProcess()
{
    svc 0x3;
    bx lr;
}

// CreateThread

asm nn::Result __attribute__((section("i._ZN2nn3svc12CreateThreadEPNS_6HandleEPFvjjjjii"))) CreateThread(nn::Handle*, void (*)(uint), uint, uint, int, int)
{
    push {r0, r4};
    ldr r0, [sp, #8];
    ldr r4, [sp, #0xc];
    svc 0x8;
    ldr r2, [sp];
    str r1, [r2];
    add sp, sp, #4;
    pop {r4};
    bx lr;
}

// ExitThread

asm nn::Result __attribute__((section("i._ZN2nn3svc10ExitThreadEv"))) ExitThread()
{
    svc 0x9;
    bx lr;
}

// SleepThread

asm nn::Result __attribute__((section("i._ZN2nn3svc11SleepThreadEx"))) SleepThread(long long)
{
    svc 0xa;
    bx lr;
}

// GetThreadPriority

asm nn::Result __attribute__((section("i._ZN2nn3svc17GetThreadPriorityEPiNS_6HandleE"))) GetThreadPriority(s32* pOut, nn::Handle thread)
{
    push {r0};
    svc 0xb;
    ldr r2, [sp];
    str r1, [r2];
    add sp, sp, #4;
    bx lr;
}

// SetThreadPriority

asm nn::Result __attribute__((section("i._ZN2nn3svc17SetThreadPriorityENS_6HandleEi"))) SetThreadPriority(nn::Handle thread, s32 prio)
{
    svc 0xC;
    bx lr;
}

// CreateMutex

asm nn::Result __attribute__((section("i._ZN2nn3svc11CreateMutexEPNS_6HandleEb"))) CreateMutex(nn::Handle*, bool)
{
    push {r0};
    svc 0x13;
    ldr r2, [sp, #4];
    str r1, [r2];
    add sp, sp, #4;
    bx lr;
}

// ReleaseMutex

asm nn::Result __attribute__((section("i._ZN2nn3svc12ReleaseMutexENS_6HandleE"))) ReleaseMutex(nn::Handle)
{
    svc 0x14;
    bx lr;
}

// CreateEvent

asm nn::Result __attribute__((section("i._ZN2nn3svc11CreateEventEPNS_6HandleENS_2os9ResetTypeE"))) CreateEvent(nn::Handle*, nn::os::ResetType)
{
    push {r0};
    svc 0x17;
    ldr r2, [sp, #4];
    str r1, [r2];
    add sp, sp, #4;
    bx lr;
}

// SignalEvent

asm nn::Result __attribute__((section("i._ZN2nn3svc11SignalEventENS_6HandleE"))) SignalEvent(nn::Handle)
{
    svc 0x18;
    bx lr;
}

// ClearEvent

asm nn::Result __attribute__((section("i._ZN2nn3svc10ClearEventENS_6HandleE"))) ClearEvent(nn::Handle)
{
    svc 0x19;
    bx lr;
}

// CreateMemoryBlock

asm nn::Result __attribute__((section("i._ZN2nn3svc17CreateMemoryBlockEPNS_6HandleEjjjj"))) CreateMemoryBlock(nn::Handle* pOut, uptr pMemory, size_t size, bit32 myPermission, bit32 otherPermission)
{
    push {r0};
    ldr r0, [sp, #4];
    svc 0x1e;
    ldr r2, [sp, #0];
    str r1, [r2];
    add sp, sp, #4;
    bx lr;
}

// MapMemoryBlock

asm nn::Result __attribute__((section("i._ZN2nn3svc14MapMemoryBlockENS_6HandleEjjj"))) MapMemoryBlock(nn::Handle, uptr, bit32, uint)
{
    svc 0x1f;
    bx lr;
}

// UnmapMemoryBlock

asm nn::Result __attribute__((section("i._ZN2nn3svc16UnmapMemoryBlockENS_6HandleEj"))) UnmapMemoryBlock(nn::Handle, uptr)
{
    svc 0x20;
    bx lr;
}

// CreateAddressArbiter

asm nn::Result __attribute__((section("i._ZN2nn3svc19CreateAddressArbiterEPNS_6HandleE"))) CreateAddressArbiter(nn::Handle*)
{
    str r0, [sp, #4]!;
    svc 0x21;
    ldr r2, [sp, #0];
    str r1, [r2, #0];
    add sp, sp, #4;
    bx lr;
}

// ArbitrateAddress

asm nn::Result __attribute__((section("i._ZN2nn3svc16ArbitrateAddressENS_6HandleEjNS_2os15ArbitrationTypeEix"))) ArbitrateAddress(nn::Handle, uint, nn::os::ArbitrationType, s32, s64)
{
    push {r4, r5};
    ldr r4, [sp, #0];
    ldr r5, [sp, #4];
    svc 0x22;
    pop {r4, r5};
    bx lr;
}

// CloseHandle

asm nn::Result __attribute__((section("i._ZN2nn3svc11CloseHandleENS_6HandleE"))) CloseHandle(nn::Handle)
{
    svc 0x23;
    bx lr;
}

// WaitSynchronizationN

asm nn::Result __attribute__((section("i._ZN2nn3svc20WaitSynchronizationNEPiPKNS_6HandleEibx"))) WaitSynchronizationN(int*, const nn::Handle*, int, bool, long long)
{
    push {r0, r4};
    ldr r0, [sp, #0];
    ldr r4, [sp, #4];
    svc 0x25;
    ldr r2, [sp, #0];
    str r1, [r2, #0];
    add sp, sp, #4;
    ldr r4, [sp], #4;
    bx lr;
}

// DuplicateHandle

asm nn::Result __attribute__((section("i._ZN2nn3svc15DuplicateHandleEPNS_6HandleES1_"))) DuplicateHandle(nn::Handle*, Handle)
{
    str r0, [sp, #4];
    svc 0x27;
    ldr r2, [sp, #0];
    str r1, [r2, #0];
    add sp, sp, #4;
    bx lr;
}

// GetSystemTick

asm s64 __attribute__((section("i._ZN2nn3svc13GetSystemTickEv"))) GetSystemTick()
{
    svc 0x28;
    bx lr;
}

// ConnectToPort

asm nn::Result __attribute__((section("i._ZN2nn3svc13ConnectToPortEPNS_6HandleEPKc"))) ConnectToPort(nn::Handle*, const char*)
{
    str r0, [sp, #4];
    svc 0x2d;
    ldr r2, [sp, #0];
    str r1, [r2, #0];
    add sp, sp, #4;
    bx lr;
}

// SendSyncRequest

asm nn::Result __attribute__((section("i._ZN2nn3svc15SendSyncRequestENS_6HandleE"))) SendSyncRequest(nn::Handle)
{
    svc 0x32;
    bx lr;
}

// GetProcessId

asm nn::Result __attribute__((section("i._ZN2nn3svc12GetProcessIdEPjNS_6HandleE"))) GetProcessId(uint*, nn::Handle)
{
    str r0, [sp, #4];
    svc 0x35;
    ldr r2, [sp, #0];
    str r1, [r2, #0];
    add sp, sp, #4;
    bx lr;
}

// GetThreadId

asm nn::Result __attribute__((section("i._ZN2nn3svc11GetThreadIdEPjNS_6HandleE"))) GetThreadId(uint*, nn::Handle)
{
    str r0, [sp, #4];
    svc 0x37;
    ldr r2, [sp, #0];
    str r1, [r2, #0];
    add sp, sp, #4;
    bx lr;
}

// GetResourceLimit

asm nn::Result __attribute__((section("i._ZN2nn3svc16GetResourceLimitEPNS_6HandleES1_"))) GetResourceLimit(nn::Handle*, Handle)
{
    str r0, [sp, #4];
    svc 0x38;
    ldr r2, [sp, #0];
    str r1, [r2, #0];
    add sp, sp, #4;
    bx lr;
}

// GetResourceLimitCurrentValues

asm nn::Result __attribute__((section("i._ZN2nn3svc28GetResourceLimitCurrentValuesEPxNS_6HandleEPKNS_2os16LimitableResourceEi"))) GetResourceLimitCurrentValues(s64 values[], nn::Handle resourceLimit, const nn::os::LimitableResource names[], s32 umNames)
{
    svc 0x3a;
    bx lr;
}

// Break

asm nn::Result __attribute__((section("i._ZN2nn3svc5BreakENS_3dbg11BreakReasonEPKvi"))) Break(nn::dbg::BreakReason, const void*, int)
{
    svc 0x3c;
    bx lr;
}

// OutputDebugString

asm nn::Result __attribute__((section("i._ZN2nn3svc16OutputDebugStringEPKci"))) OutputDebugString(const char* text, s32 length)
{
    svc 0x3d;
    bx lr;
}

} // namespace svc
} // namespace nn

