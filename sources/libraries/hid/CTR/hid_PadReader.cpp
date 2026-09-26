// Filename: hid_PadReader.cpp
//
// Project: Horizon

#include <nn/hid/CTR/hid_PadReader.h>
#include <nn/hid/CTR/hid_ExtraPad.h>
#include <nn/hidlow/CTR/hidlow_PadLifoRing.h>
#include <nn/hidlow/hidlow_Utils.h>
#include <nn/applet/CTR/applet_Api.h>
#include <nn/applet/CTR/applet_Info.h>

#include <nn/dbg/dbg_Break.h>

namespace nn{
namespace hid{
namespace CTR{
namespace{
    bool s_IsEnableSelect;
}

PadReader::PadReader(Pad& pad): 
    m_Pad(pad),
    m_IndexOfRead(-1),
    m_IsReadLatestFirst(true),
    m_TickOfRead(-1)
{
}

bool PadReader::ReadLatest(PadStatus* pBuf)
{
    s64 tick = -1LL;
    s32 index = -1;
    s32 readLen;
    uint newHold;

    if(ExtraPad::IsSampling())
        return false;

    this->m_StickClamper.ClampValueOfClamp();

    reinterpret_cast<nn::hidlow::CTR::PadLifoRing*>(m_Pad.GetResource())->ReadData(pBuf, 1, &readLen, &tick, &index);
    if(0 < readLen)
    {
        this->m_StickClamper.ClampCore(&pBuf->stick.x,&pBuf->stick.y,pBuf->stick.x,pBuf->stick.y);


        if(m_IsReadLatestFirst != false)
        {
            m_LatestHold = pBuf->hold;
            m_IsReadLatestFirst = false;
        }

		pBuf->hold &= ~BUTTON_RESERVED;
        pBuf->trigger = (pBuf->hold ^ m_LatestHold) & ~m_LatestHold;
        pBuf->release = (pBuf->hold ^ m_LatestHold) & m_LatestHold;

        if((applet::CTR::IsInitialized()) && (!applet::CTR::detail::IsActive()))
        {
            this->HideKeyInfo(pBuf);
        }

        m_LatestHold = pBuf->hold;

        if(s_IsEnableSelect == false)
        {
            hidlow::GatherStartAndSelect(pBuf);
        }
        return true;
    }
    return false;
}

void PadReader::Read(PadStatus* pBufs, s32* pReadLen, s32 bufLen)
{
    NN_TASSERT_(NULL != pBufs);

    this->m_StickClamper.ClampValueOfClamp();

    reinterpret_cast<nn::hidlow::CTR::PadLifoRing*>(this->m_Pad.GetResource())->ReadData(pBufs, bufLen, pReadLen, &this->m_TickOfRead, &this->m_IndexOfRead);

    if(ExtraPad::IsSampling())
    {
        for(int i = 0; i < *pReadLen; i++)
        {
            this->HideKeyInfo(&pBufs[i]);
        }

        *pReadLen = 0;
        return;
    }

    for(int i = 0; i < *pReadLen; i++)
    {
		pBufs[i].hold    &= ~BUTTON_RESERVED;
		pBufs[i].trigger &= ~BUTTON_RESERVED;
		pBufs[i].release &= ~BUTTON_RESERVED;

        if((applet::CTR::IsInitialized()) && (!applet::CTR::detail::IsActive()))
        {
            this->HideKeyInfo(&pBufs[i]);
        }
        
        if(!s_IsEnableSelect)
        {
            hidlow::GatherStartAndSelect(&pBufs[i]);
        }

        this->m_StickClamper.ClampCore(&pBufs[i].stick.x, &pBufs[i].stick.y, pBufs[i].stick.x, pBufs[i].stick.y);
    }
}

void PadReader::SetStickClamp(short min, short max)
{
    return this->m_StickClamper.SetStickClamp(min, max);
}

f32 PadReader::NormalizeStick(short x)
{
    return this->m_StickClamper.NormalizeStick(x);
}

void PadReader::NormalizeStickWithScale(f32* normalized_x, f32* normalized_y, s16 x, s16 y)
{
    return this->m_StickClamper.NormalizeStickWithScale(normalized_x, normalized_y, x, y);
}

void PadReader::SetNormalizeStickScaleSettings(f32 scale, s16 threshold)
{
    return this->m_StickClamper.SetNormalizeStickScaleSettings(scale,threshold);
}

}
}
}