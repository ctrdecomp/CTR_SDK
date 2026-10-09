#pragma once

#include <nn/camera/CTR/camera_Paramaters.h>
#include <nn/Handle.h>
#include <nn/Result.h>

namespace nn{
namespace camera{
namespace CTR{
namespace detail{

class Camera
{
public:
    static Result Activate(CameraSelect pSelect);
    static Result GetActivatedCamera(CameraSelect* pSelect);
    static Result GetSleepCamera(CameraSelect* pSelect);
    static Result SetSleepCamera(CameraSelect camera);

    static nn::Handle s_Session;
};

}
}
}
}

