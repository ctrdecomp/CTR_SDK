#pragma once

#include <nn/types.h>

enum
{
    NW_FONT_CMD_CULL_FACE_DISABLE,
    NW_FONT_CMD_CULL_FACE_FRONT,
    NW_FONT_CMD_CULL_FACE_BACK,

    NW_FONT_CMD_CULL_FACE_MASK = 0x3
};

namespace nn {
namespace font {

struct ColorBufferInfo
{
    u16 width;
    u16 height;
    u8 depth;
};

}
}

