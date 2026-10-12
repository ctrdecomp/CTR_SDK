#include <nn/gr/CTR/gr_Viewport.h>
#include <nn/gr/CTR/gr_Utility.h>

namespace nn {
namespace gr {
namespace CTR {

bit32* Viewport::MakeCommand(bit32* command) const
{
    u32 width24 = Float32ToFloat24(width / 2.f);
    u32 width31 = Float32ToFloat31(2.f / width) << 1;
    u32 height24 = Float32ToFloat24(height / 2.f);
    u32 height31 = Float32ToFloat31(2.f / height) << 1;

    *command++ = width24;
    *command++ = PICA_CMD_HEADER_SINGLE_BE(PICA_REG_VIEWPORT_WIDTH1, 0x7);

    *command++ = width31;
    *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_VIEWPORT_WIDTH2);

    *command++ = height24;
    *command++ = PICA_CMD_HEADER_SINGLE_BE(PICA_REG_VIEWPORT_HEIGHT1, 0x7);

    *command++ = height31;
    *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_VIEWPORT_HEIGHT2);

    *command++ = PICA_CMD_DATA_VIEWPORT_XY(x, y);
    *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_VIEWPORT_XY);
            
    return command;
}

} // namespace CTR
} // namespace gr
} // namespace nn

