// Filename: hid_ExtraPad.cpp
//
// Project: Horizon

#include <nn/hid/CTR/hid_ExtraPad.h>
#include <nn/ir/CTR/ir_CepdApi.h>
#include <nn/os.h>

namespace nn{
namespace hid{
namespace CTR{
namespace {
    nn::os::CriticalSection startStopCriticalSection = nn::WithInitialize();
}


bool ExtraPad::IsSampling()
{
    return nn::ir::CTR::CepdGetStatus() == nn::ir::CTR::CEPD_STATUS_SAMPLING;
}

}
}
}
