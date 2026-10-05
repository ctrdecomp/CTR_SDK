// Filename: gr_FrameBuffer.cpp
//
// Project: Horizon

#include <nn/gr/CTR/gr_FrameBuffer.h>
#include <nn/gx/CTR/gx_CommandAccess.h>

namespace nn{
namespace gr{
namespace CTR{

bit32* FrameBuffer::ColorBuffer::MakeRenderBlockModeCommand(bit32* command) const
{
    *command++ = blockSize;
    *command++ = PICA_CMD_HEADER_SINGLE( PICA_REG_RENDER_BLOCK_FORMAT );
                
    return command;
}

void FrameBuffer::MakeClearRequest(const u32 bufferBit, bool isAddSplitDrawCmdlist) const
{
    u32 colorByteSize = 0;
    u32 colorWidth = 0;
    u32 clearColor = 0;

    if (bufferBit & COLOR_BUFFER_BIT)
    {
        u8 red = FloatToUnsignedByte(colorBuffer.clearColor[0]);
        u8 green = FloatToUnsignedByte(colorBuffer.clearColor[1]);
        u8 blue = FloatToUnsignedByte(colorBuffer.clearColor[2]);

        if (colorBuffer.format == PICA_DATA_COLOR_RGBA8_OES)
        {
            u8 alpha = FloatToUnsignedByte(colorBuffer.clearColor[3]);

            colorByteSize = colorBuffer.width * colorBuffer.height * 4;
            colorWidth = 32;

            clearColor = (red << 24) |
                         (green << 16) |
                         (blue << 8) |
                         alpha;
        }
        else if (colorBuffer.format == PICA_DATA_COLOR_RGB5_A1)
        {
            u8 alpha = FloatToUnsignedByte(colorBuffer.clearColor[3]);

            colorByteSize = colorBuffer.width * colorBuffer.height * 2;
            colorWidth = 16;
            clearColor = (((red >> 3) & 0x1f) << 11) |
                         (((green >> 3) & 0x1f) << 6) |
                         (((blue >> 3) & 0x1f) << 1) |
                         ((alpha > 0x7f) ? 0x1 : 0);
        }
        else if (colorBuffer.format == PICA_DATA_COLOR_RGB565)
        {
            colorByteSize = colorBuffer.width * colorBuffer.height * 2;
            colorWidth = 16;
            clearColor = (((red >> 3) & 0x1f) << 11) |
                         (((green >> 2) & 0x3f) << 5) |
                         ((blue >> 3) & 0x1f);
        }
        else if (colorBuffer.format == PICA_DATA_COLOR_RGBA4)
        {
            u8 alpha = FloatToUnsignedByte(colorBuffer.clearColor[3]);

            colorByteSize = colorBuffer.width * colorBuffer.height * 2;
            colorWidth = 16;
            clearColor = (((red >> 4) & 0xf) << 12) |
                         (((green >> 4) & 0xf) << 8) |
                         (((blue >> 4) & 0xf) << 4) |
                         ((alpha >> 4) & 0xf);
        }
    }

    u32 depthByteSize = 0;
    u32 clearDepth = 0;
    u32 depthWidth = 0;

    if ((bufferBit & DEPTH_BUFFER_BIT) || (bufferBit & STENCIL_BUFFER_BIT))
    {
        clearDepth = depthStencilBuffer.clearDepth;
        clearDepth = (clearDepth > 1.0f) ? 1.0f :
                     ((clearDepth < 0.0f) ? 0.0f : clearDepth);

        if (depthStencilBuffer.format == PICA_DATA_DEPTH24_STENCIL8_EXT)
        {
            depthByteSize = depthStencilBuffer.width * depthStencilBuffer.height * 4;
            clearDepth = (depthStencilBuffer.clearStencil << 24) |
                         Float32ToUnsignedFix24(clearDepth);
            depthWidth = 32;
        }
        else if (depthStencilBuffer.format == PICA_DATA_DEPTH_COMPONENT16)
        {
            depthByteSize = depthStencilBuffer.width * depthStencilBuffer.height * 2;
            clearDepth = Float32ToUnsignedFix16(clearDepth);
            depthWidth = 16;
        }
        else if (depthStencilBuffer.format == PICA_DATA_DEPTH_COMPONENT24_OES)
        {
            depthByteSize = depthStencilBuffer.width * depthStencilBuffer.height * 3;
            clearDepth = Float32ToUnsignedFix24(clearDepth);
            depthWidth = 24;
        }
    }

    if ((bufferBit & COLOR_BUFFER_BIT) || (bufferBit & DEPTH_BUFFER_BIT))
    {
        if (isAddSplitDrawCmdlist)
        {
            nngxSplitDrawCmdlist();
        }

        nngxAddMemoryFillCommand(
            (bufferBit & COLOR_BUFFER_BIT)
                ? reinterpret_cast<GLvoid*>(colorBuffer.virtualAddr)
                : 0,
            colorByteSize,
            clearColor,
            colorWidth,
            ((bufferBit & DEPTH_BUFFER_BIT) ||
             (bufferBit & STENCIL_BUFFER_BIT) ||
             (bufferBit & DEPTH_STENCIL_BUFFER_BIT))
                ? reinterpret_cast<GLvoid*>(depthStencilBuffer.virtualAddr)
                : 0,
            depthByteSize,
            clearDepth,
            depthWidth);
    }
}

bit32* FrameBuffer::ColorBuffer::MakeCommand(bit32* command, bool isAddRenderBlockModeCommand) const
{
    *command++ =  PICA_CMD_DATA_RENDER_BUFFER_COLOR_MODE(((format == PICA_DATA_COLOR_RGBA8_OES) || (format == PICA_DATA_COLOR_GAS_DMP)) ? PICA_DATA_COLOR_PIXEL_SIZE32 : PICA_DATA_COLOR_PIXEL_SIZE16, format);
    *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_RENDER_BUFFER_COLOR_MODE);

    *command++ = PICA_CMD_DATA_RENDER_BUFFER_COLOR_ADDR(nngxGetPhysicalAddr(virtualAddr));
    *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_RENDER_BUFFER_COLOR_ADDR);

    if (isAddRenderBlockModeCommand){
        command = this->MakeRenderBlockModeCommand(command);
    }

    return command;
}

FrameBuffer::ColorBuffer::ColorBuffer(const FrameBuffer& frameBuffer_): 
    virtualAddr(0),
    format(PICA_DATA_COLOR_RGBA8_OES),
    blockSize(BLOCK_SIZE8),
    width(240), 
    height(400),  
    mFrameBuffer(frameBuffer_)
    {
    for (u32 index = 0; index < 4; index++){
        clearColor[index] = 0.0f;
    }
}

bit32* FrameBuffer::DepthStencilBuffer::MakeCommand(bit32* command) const
{
    *command++ = PICA_CMD_DATA_RENDER_BUFFER_DEPTH_MODE(format);
    *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_RENDER_BUFFER_DEPTH_MODE);

    *command++ = PICA_CMD_DATA_RENDER_BUFFER_DEPTH_ADDR(nngxGetPhysicalAddr(virtualAddr));
    *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_RENDER_BUFFER_DEPTH_ADDR);
                
    return command;
}

FrameBuffer::DepthStencilBuffer::DepthStencilBuffer(const FrameBuffer& frameBuffer_): 
    virtualAddr(0),
    format(PICA_DATA_DEPTH24_STENCIL8_EXT),
    width(240), 
    height(400),
    clearDepth(1.0f),
    clearStencil(0),
    mFrameBuffer(frameBuffer_)
{
}

bit32* FrameBuffer::FrameBuffer::MakeCommand(bit32* command, const u32 bufferBit, bool isClearCache) const
{   
    if (isClearCache)
    {
        command = MakeClearCacheCommand(command);
    }

    if (bufferBit & COLOR_BUFFER_BIT)
    {
        command = this->colorBuffer.MakeCommand(command);
    }

    if ((bufferBit & DEPTH_BUFFER_BIT) || (bufferBit & STENCIL_BUFFER_BIT))
    {
        command = this->depthStencilBuffer.MakeCommand(command);
    }

    *command++ = PICA_CMD_DATA_RENDER_BUFFER_RESOLUTION(width, height);
    *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_RENDER_BUFFER_RESOLUTION0);

    *command++ = PICA_CMD_DATA_RENDER_BUFFER_RESOLUTION(width, height);
    *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_RENDER_BUFFER_RESOLUTION1);

    command = this->MakeClearCacheCommand(command);

    return command;
}

bit32* FrameBuffer::MakeClearCacheCommand(bit32* command)
{
    *command++ = 0x1;
    *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_COLOR_DEPTH_BUFFER_CLEAR1);

    *command++ = 0x1;
    *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_COLOR_DEPTH_BUFFER_CLEAR0);

    return command;
}

FrameBuffer::FrameBuffer():
    colorBuffer(*this),
    depthStencilBuffer(*this),
    width(240), 
    height(400)
{
}

}
}
}