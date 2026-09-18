// Filename: applet_Gfx.cpp
//
// Project: Horizon

#include <nn/applet/CTR/applet_Connect.h>
#include <nn/applet/CTR/applet_API.h>
#include <nn/applet/CTR/applet_Ipc.h>
#include <nn/srv/srv_Api.h>
#include <nn/err/CTR/err_Api.h>
#include <nn/gxlow/CTR/gxlow_SystemUse.h>
#include <nn/gx/CTR/gx_Lcd.h>

#include <string.h>

namespace nn{
namespace applet{
namespace CTR{
namespace detail{

size_t GetByteSizePerPixel(const DisplayBufferMode mode)
{
    int mul = 0;
    switch (mode) 
    {
        case FORMAT_R8G8B8A8:
            mul=4;
            break;
        case FORMAT_R8G8B8:
            mul=3;
            break;
        case FORMAT_R5G6B5:
        case FORMAT_R5G5B5A1:
        case FORMAT_R4G4B4A4:            
            mul=2;
            break;
        default: break;
    }

    return mul;
}

void GetDisplayInfo(AppletDisplayInfo* pInfo)
{
    if(!pInfo)
        return;
    gxlow::CTR::DisplayCaptureInfo infoTmp;

    nn::gxlow::CTR::ImportDisplayCaptureInfo(&infoTmp);
    pInfo->d[0].addr = infoTmp.surface[0].addr;
    pInfo->d[0].addrB = infoTmp.surface[0].addrB;
    pInfo->d[1].addr = infoTmp.surface[1].addr;
    pInfo->d[1].addrB = infoTmp.surface[1].addrB;
    pInfo->d[0].mode = static_cast<AppletDisplayBufferMode>(infoTmp.surface[0].mode);
    pInfo->d[1].mode = static_cast<AppletDisplayBufferMode>(infoTmp.surface[1].mode);
    pInfo->d[0].stride = infoTmp.surface[0].stride;
    pInfo->d[1].stride = infoTmp.surface[1].stride;
}

void CalcCaptureBufferInfo(CaptureBufferInfo *cInfo)
{
    int x = 256;
    size_t bufferSize = 0;
    size_t pixelSize;

    {
        pixelSize = GetByteSizePerPixel(cInfo->d[1].mode);
        cInfo->d[1].offset = cInfo->d[1].offsetB = 0;
        bufferSize += x * NN_GX_DISPLAY0_HEIGHT * ((pixelSize > 3) ? 3 : pixelSize);
    }

    {
        pixelSize = GetByteSizePerPixel(cInfo->d[0].mode);
        cInfo->d[0].offset = cInfo->d[0].offsetB = bufferSize;
        bufferSize += x * NN_GX_DISPLAY0_HEIGHT * ((pixelSize > 3) ? 3 : pixelSize);
    }

    if (cInfo->is3DCapture)
    {
        pixelSize = GetByteSizePerPixel(cInfo->d[0].mode);
        cInfo->d[0].offsetB = bufferSize;
        bufferSize += x * NN_GX_DISPLAY0_HEIGHT * ((pixelSize > 3) ? 3 : pixelSize);
    }

    bufferSize += x * (512 - NN_GX_DISPLAY0_HEIGHT) * ((pixelSize > 3) ? 3 : pixelSize);
    cInfo->size = bufferSize;
}

struct OffsetTable
{
    size_t table[5];
};

typedef void(* ConvertFunction)(u32* dst, const u32* src, const size_t offsetPerLine, const OffsetTable& ot);

void ConvertL24ToB24(u32* dst, const u32* src, const size_t offsetPerLine, const OffsetTable& ot)
{
    const u32* curSrc = src;
    u32* curDst = dst;

    for (int i = 4; i > 0; i--)
    {
        curSrc = src + ot.table[i];

        for (int j = 2; j > 0; j--)
        {
            const u32* next = curSrc + offsetPerLine;

            *curDst++ = *(curSrc);
            *curDst++ = (*(curSrc+1) & 0x0000ffff) | ((*(next) & 0x0000ffff) << 16);
            *curDst++ = ((*(next) >> 16) & 0x0000ffff) | ((*(next+1) & 0x0000ffff) << 16);

            *curDst++ = ((*(curSrc+1) >> 16) & 0x0000ffff) | ((*(curSrc+2) & 0x0000ffff) << 16);
            *curDst++ = ((*(curSrc+2) >> 16) & 0x0000ffff) | (*(next+1) & 0xffff0000);
            *curDst++ = *(next+2);

            curSrc += offsetPerLine << 1;
        }
    }
}

void ConvertL16ToB16(u32* dst, const u32* src, const size_t offsetPerLine, const OffsetTable& ot)
{
    const u32* curSrc = src;
    u32* curDst = dst;

    for (int i = 4; i > 0; i--)
    {
        curSrc = src + ot.table[i];

        for (int j = 2; j > 0; j--)
        {
            *curDst++ = *curSrc;
            *curDst++ = *(curSrc + offsetPerLine);
            *curDst++ = *(curSrc + 1);
            *curDst++ = *(curSrc + offsetPerLine + 1);

            curSrc += offsetPerLine << 1;
        }
    }
}

void CaptureDisplayBufferCore(uptr buffer, const AppletDisplayInfo* pInfo, bool isDisplay1, bool is3D)
{
    u32 width = (isDisplay1) ? nn::gx::CTR::DISPLAY1_WIDTH : nn::gx::CTR::DISPLAY0_WIDTH;
    u32 height = (isDisplay1) ? nn::gx::CTR::DISPLAY1_HEIGHT : nn::gx::CTR::DISPLAY0_HEIGHT;

    uptr addr  = (is3D) ? pInfo->d[isDisplay1].addrB : pInfo->d[isDisplay1].addr;
    u32 stride = pInfo->d[isDisplay1].stride;

    size_t pixelsPerLineOnBlock = 8;
    size_t linesPerBlock = 8;

    size_t byteSizePerLineOnBlock;
    if (pInfo->d[isDisplay1].mode == nn::applet::CTR::FORMAT_R8G8B8A8)
    {
        byteSizePerLineOnBlock = pixelsPerLineOnBlock * 3;
    }
    else
    {
        byteSizePerLineOnBlock = pixelsPerLineOnBlock * GetByteSizePerPixel(pInfo->d[isDisplay1].mode);
    }

    size_t byteSizePerBlock = byteSizePerLineOnBlock * linesPerBlock;
    size_t s_offset = stride >> 2;
    size_t d_offset = (2 * byteSizePerBlock) >> 2 ;

    OffsetTable ot;

    ConvertFunction convertFunc;
    switch (pInfo->d[isDisplay1].mode)
    {
    case nn::applet::CTR::FORMAT_R8G8B8A8:
    case nn::applet::CTR::FORMAT_R8G8B8:
    {
        convertFunc = ConvertL24ToB24;

        ot.table[0] = 0; // dummy
        ot.table[1] = (s_offset << 2) + 3;
        ot.table[2] = s_offset << 2;
        ot.table[3] = 3;
        ot.table[4] = 0;
    }
    break;
    case nn::applet::CTR::FORMAT_R5G6B5:
    case nn::applet::CTR::FORMAT_R5G5B5A1:
    case nn::applet::CTR::FORMAT_R4G4B4A4:
    {
        convertFunc = ConvertL16ToB16;

        ot.table[0] = 0;
        ot.table[1] = (s_offset << 2) + 2;
        ot.table[2] = s_offset << 2;
        ot.table[3] = 2;
        ot.table[4] = 0;
    }
    break;
    default:
        return;
    }

    if (buffer)
    {
        u32* s = reinterpret_cast<u32*>(addr);
        u32* d = reinterpret_cast<u32*>(buffer);

        for (int y = 0; y < height ;  y += 8)
        {
            u32* tmp_s = s;

            for (int x = 0; x < width; x += 8)
            {
                convertFunc(d, tmp_s, s_offset, ot);

                tmp_s += byteSizePerLineOnBlock >> 2;
                d += byteSizePerBlock >> 2;
            }

            s += s_offset * linesPerBlock;
            d += d_offset;
        }
    }
}

void CaptureDisplayBuffer(uptr buffer, const AppletDisplayInfo* pInfo, const CaptureBufferInfo* cInfo)
{
    if (pInfo == NULL || cInfo == NULL)
    {
        return;
    }

    if (!cInfo->is3DCapture)
    {
        CaptureDisplayBufferCore(buffer + cInfo->d[1].offset, pInfo, true, false);
        CaptureDisplayBufferCore(buffer + cInfo->d[0].offset, pInfo, false, false);
    }
    else
    {
        CaptureDisplayBufferCore(buffer + cInfo->d[1].offset,  pInfo, true, false);
        CaptureDisplayBufferCore(buffer + cInfo->d[0].offset,  pInfo, false, false);
        CaptureDisplayBufferCore(buffer + cInfo->d[0].offsetB, pInfo, false, true);
    }
    //NN_TLOG_("cap time: %lld us\n", (nn::os::Tick::GetSystemCurrent() - start).ToTimeSpan().GetMicroSeconds());
}

}
}
}
}