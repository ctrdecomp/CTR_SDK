#pragma once

#include <nn/math/math_Vector2.h>
#include <nn/math/math_Vector3.h>
#include <nn/math/math_Vector4.h>

namespace nn { 
namespace math {
class Transform2
{
public:
    VEC2 scale;
    VEC2 rotate;
    VEC2 translate;
};

class Transform3
{
public:
    VEC3 scale;
    VEC3 rotate;
    VEC3 translate;
    Transform3()
    {
    }
    Transform3(const VEC3& s, const VEC3& r, const VEC3& t)
    {
        scale     = s;
        rotate    = r;
        translate = t;
    }
};

}
}
