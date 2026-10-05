// Filename: gr_Scissor.cpp
//
// Project: Horizon

#include <nn/gr/CTR/gr_Scissor.h>

namespace nn {
namespace gr {
namespace CTR {

bit32* Scissor::MakeCommand(bit32* command) const
{
    s32 temp_width  = x + width  - 1;
    s32 temp_height = y + height - 1;
                
    *command++ = PICA_CMD_DATA_SCISSOR(isEnable);
    *command++ = PICA_CMD_HEADER_BURSTSEQ( PICA_REG_SCISSOR, 3);
    *command++ = PICA_CMD_DATA_SCISSOR_XY(x, y, bufferWidth, bufferHeight);
    *command++ = PICA_CMD_DATA_SCISSOR_SIZE(temp_width, temp_height);
                
    return command;
}

} // namespace CTR
} // namespace gr
} // namespace nn
