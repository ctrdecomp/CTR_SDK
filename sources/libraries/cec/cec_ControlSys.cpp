// Filename: cec_ControlSys.cpp
//
// Project: Horizon

#include <nn/cec/CTR/cec_ControlSys.h>

namespace
{
    bool s_InitializedSys = false;
}

namespace nn {
namespace cec {
namespace CTR {

bool CecControlSys::IsInitializedSys()
{
    return s_InitializedSys;
}

}
}
}
