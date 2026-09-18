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
#if NN_VERSION_MAJOR <= 2
    m_MinOfStickClampCircle(MIN_OF_STICK_CLAMP_MODE_CIRCLE),
    m_MinOfStickClampCross(MIN_OF_STICK_CLAMP_MODE_CROSS),
    m_MinOfStickClampMinimum(MIN_OF_STICK_CLAMP_MODE_CIRCLE),
    m_MaxOfStickClampCircle(LIMIT_OF_STICK_CLAMP_MAX),
    m_MaxOfStickClampCross(LIMIT_OF_STICK_CLAMP_MAX),
    m_MaxOfStickClampMinimum(LIMIT_OF_STICK_CLAMP_MAX),
    m_StickClampMode(STICK_CLAMP_MODE_CIRCLE),
#endif
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
    
#if NN_VERSION_MAJOR > 2
    this->m_StickClamper.ClampValueOfClamp();
#else
    this->ClampValueOfClamp();
#endif

    reinterpret_cast<nn::hidlow::CTR::PadLifoRing*>(m_Pad.GetResource())->ReadData(pBuf, 1, &readLen, &tick, &index);
    if(0 < readLen)
    {

#if NN_VERSION_MAJOR > 4
        this->m_StickClamper.ClampCore(&pBuf->stick.x,&pBuf->stick.y,pBuf->stick.x,pBuf->stick.y);
#else
        this->ClampCore(&pBuf->stick.x,&pBuf->stick.y,pBuf->stick.x,pBuf->stick.y);
#endif


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

#if NN_VERSION_MAJOR > 2
    this->m_StickClamper.ClampValueOfClamp();
#else
    this->ClampValueOfClamp();
#endif

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

#if NN_VERSION_MAJOR > 2
        this->m_StickClamper.ClampCore(&pBufs[i].stick.x, &pBufs[i].stick.y, pBufs[i].stick.x, pBufs[i].stick.y);
#else
        this->ClampCore(&pBufs[i].stick.x, &pBufs[i].stick.y, pBufs[i].stick.x, pBufs[i].stick.y);
#endif
    }
}

void PadReader::SetStickClamp(short min, short max)
{
#if NN_VERSION_MAJOR > 2
    return this->m_StickClamper.SetStickClamp(min, max);
#else
    NN_TASSERT_(0 <= min);
    NN_TASSERT_(min < max);
    
    if (LIMIT_OF_STICK_CLAMP_MAX < max)
    {
        max = LIMIT_OF_STICK_CLAMP_MAX;
    }

    if (m_StickClampMode == STICK_CLAMP_MODE_CIRCLE)
    {
        if (min < MIN_OF_STICK_CLAMP_MODE_CIRCLE)
        {
            min = MIN_OF_STICK_CLAMP_MODE_CIRCLE;
        }
        m_MinOfStickClampCircle = min;
        m_MaxOfStickClampCircle = max;
    
    }
    else if (m_StickClampMode == STICK_CLAMP_MODE_CROSS)
    {
        if (min < MIN_OF_STICK_CLAMP_MODE_CROSS)
        {
            min = MIN_OF_STICK_CLAMP_MODE_CROSS;
        }
        m_MinOfStickClampCross = min;
        m_MaxOfStickClampCross = max;
    }
    else
    {
        m_MaxOfStickClampMinimum = max;
    }
#endif
}

f32 PadReader::NormalizeStick(short x)
{
#if NN_VERSION_MAJOR > 2
    return this->m_StickClamper.NormalizeStick(pos);
#else
    f32 fx = (f32)x;
    s16 threshold;

    switch (m_StickClampMode)
    {
    case STICK_CLAMP_MODE_CIRCLE:
        threshold = m_MaxOfStickClampCircle - m_MinOfStickClampCircle;
        break;

    case STICK_CLAMP_MODE_CROSS:
        threshold = m_MaxOfStickClampCross - m_MinOfStickClampCross;
        break;

    case STICK_CLAMP_MODE_MINIMUM:
        threshold = LIMIT_OF_STICK_CLAMP_MAX - MIN_OF_STICK_CLAMP_MODE_CIRCLE;
        break;
    }

    if (0 == x)
        return 0.0f;
    else if(threshold <= x)
        return 1.0f;

    return fx / threshold;
#endif
}

void PadReader::NormalizeStickWithScale(f32* normalized_x, f32* normalized_y, s16 x, s16 y)
{
#ifdef NN_VERSION_MAJOR > 2
    return this->m_StickClamper.NormalizeStickWithScale(normalized_x, normalized_y, x, y);
#else
    f32 length;
    f32 diff_length = 0;
    f32 threshold;
    f32 ux, uy;
    f32 limit;
    f32 normal_x, normal_y;
    f32 speedAdjust;

    switch (m_StickClampMode)
    {
    case STICK_CLAMP_MODE_CIRCLE:
        threshold = m_Threshold - m_MinOfStickClampCircle;
        break;
    case STICK_CLAMP_MODE_CROSS:
        threshold = m_Threshold - m_MinOfStickClampCross;
        break;
    case STICK_CLAMP_MODE_MINIMUM:
        threshold = m_Threshold - m_MinOfStickClampMinimum;
        break;
    }

    length = nn::math::FSqrt( (f32)(x * x + y * y) ) ;

    if (length  == 0.0f)
    {
        normal_x = 0;
        normal_y = 0;
    }
    else
    {
        ux = (f32)x / length ;
        uy = (f32)y / length ;

        diff_length = length - m_LastLength;

        if (length >= threshold)
        {
            if (m_LastLength < threshold)
            {
                if (diff_length > m_LastDiff)
                {
                    m_StrokeVelocity = diff_length;
                }
                else
                {
                    m_StrokeVelocity = m_LastDiff;
                }
            }
            else
            {
                if (diff_length > m_StrokeVelocity)
                {
                    m_StrokeVelocity = diff_length;
                }
            }

            normal_x = ux;
            normal_y = uy;
        }
        else
        {
            if (m_LastLength >= threshold)
            {
                m_StrokeVelocity = diff_length ;
            }
            else
            {
                if (diff_length < m_StrokeVelocity)
                {
                    m_StrokeVelocity = diff_length ;
                }
            }
            normal_x = ux * length / threshold;
            normal_y = uy * length / threshold;
        }
    }

    if (m_Scale <= 1.0f)
    {
        limit = threshold;
    }
    else
    {
        limit = m_Scale * threshold ;
    }

    if (length >= threshold)
    {
        if (m_StrokeVelocity < 1.0f)
        {
            m_StrokeVelocity = 1.0f;
        }
    }
    else
    {
        if (m_StrokeVelocity > -1.0f)
        {
            m_StrokeVelocity = -1.0f;
        }
    }

    switch(m_StickClampMode)
    {
    case STICK_CLAMP_MODE_CIRCLE:
        speedAdjust = (f32)(m_MaxOfStickClampCircle - m_MinOfStickClampCircle)/ LIMIT_OF_STICK_CLAMP_MAX;
        break;
    case STICK_CLAMP_MODE_CROSS:
        speedAdjust = (f32)(m_MaxOfStickClampCross - m_MinOfStickClampCross)/ LIMIT_OF_STICK_CLAMP_MAX;
        break;
    case STICK_CLAMP_MODE_MINIMUM:
        speedAdjust = (f32)(m_MaxOfStickClampMinimum - MIN_OF_STICK_CLAMP_MODE_CIRCLE)/ LIMIT_OF_STICK_CLAMP_MAX;
        break;
    }

    m_Stroke += (m_StrokeVelocity * speedAdjust);

    if (m_Stroke < threshold)
    {
        m_Stroke = threshold;
    }
    else if (m_Stroke > limit)
    {
        m_Stroke = limit;
    }

    // set max stroking LMAO
    normal_x *= (m_Stroke / limit);
    normal_y *= (m_Stroke / limit);

    f32 len2 = normal_x * normal_x + normal_y * normal_y;
    if (len2 > 1.0f)
    {
        len2 = nn::math::FSqrt(len2);
        normal_x /= len2;
        normal_y /= len2;
    }

    m_LastLength = length;
    m_LastDiff = diff_length;

    // normalized = normal
    *normalized_x = normal_x;
    *normalized_y = normal_y;
#endif
}

void PadReader::SetNormalizeStickScaleSettings(f32 scale, s16 threshold)
{
#ifdef NN_VERSION_MAJOR > 2
    return this->m_StickClamper.SetNormalizeStickScaleSettings(scale,threshold);
#else
    if(LIMIT_OF_STICK_CLAMP_MAX < threshold) 
        threshold = LIMIT_OF_STICK_CLAMP_MAX;
    m_Scale = scale;
    m_Threshold = threshold;
#endif
}

}
}
}