// Filename: hid_AnalogStickClamper.cpp
//
// Project: Horizon

#include <nn/hid/CTR/hid_AnalogStickClamper.h>
#include <nn/hidlow/hidlow_Utils.h>
#include <nn/math.h>

namespace nn{
namespace hid{
namespace CTR{

AnalogStickClamper::AnalogStickClamper() : 
    m_MinOfStickClampCircle(MIN_OF_STICK_CLAMP_MODE_CIRCLE),
    m_MinOfStickClampCross(MIN_OF_STICK_CLAMP_MODE_CROSS),
    m_MinOfStickClampMinimum(MIN_OF_STICK_CLAMP_MODE_CIRCLE),
    m_MaxOfStickClampCircle(LIMIT_OF_STICK_CLAMP_MAX),
    m_MaxOfStickClampCross(LIMIT_OF_STICK_CLAMP_MAX),
    m_MaxOfStickClampMinimum(LIMIT_OF_STICK_CLAMP_MAX),
    m_StickClampMode(STICK_CLAMP_MODE_CIRCLE)
{
        
    m_Threshold = DEFAULT_THRESHOLD_OF_NORMALIZE_STICK;
    m_StrokeVelocity = 0.0f;
    m_LastLength = 0.0f;
    m_LastDiff = 0.0f;
    m_Scale = DEFAULT_SCALE_OF_NORMALIZE_STICK;
    m_Stroke = 141.0f;
}

void AnalogStickClamper::SetStickClampFree(s16 min, s16 max)
{
    if(m_StickClampMode == STICK_CLAMP_MODE_CIRCLE)
    {
        m_MinOfStickClampCircle = min;
        m_MaxOfStickClampCircle = max;
    }
    else if(m_StickClampMode == STICK_CLAMP_MODE_CROSS)
    {
        m_MinOfStickClampCross = min;
        m_MaxOfStickClampCross = max;
    }
    else
    {
        m_MaxOfStickClampMinimum = max;
        m_MinOfStickClampMinimum = min;
    }
}

void AnalogStickClamper::SetNormalizeStickScaleSettings(f32 scale, s16 threshold)
{
    if(LIMIT_OF_STICK_CLAMP_MAX < threshold) threshold = LIMIT_OF_STICK_CLAMP_MAX;
    m_Scale = scale;
    m_Threshold = threshold;
}

void AnalogStickClamper::ClampCore(short* pOutX, short* pOutY, s32 x, s32 y)
{
    switch (this->m_StickClampMode) 
    {
    case STICK_CLAMP_MODE_CIRCLE:
        hidlow::ClampStickCircle(pOutX, pOutY, x, y, this->m_MinOfStickClampCircle, this->m_MaxOfStickClampCircle);
        break;
    case STICK_CLAMP_MODE_CROSS:
        hidlow::ClampStickCross(pOutX, pOutY, x, y, this->m_MinOfStickClampCross, this->m_MaxOfStickClampCross);
        break;
    case STICK_CLAMP_MODE_MINIMUM:
        hidlow::ClampStickMinimum(pOutX, pOutY, x, y, this->m_MinOfStickClampMinimum, this->m_MaxOfStickClampMinimum);
        break;
    }
}

void AnalogStickClamper::ClampValueOfClamp() 
{
  if (m_MinOfStickClampCircle < MIN_OF_STICK_CLAMP_MODE_CIRCLE)
    m_MinOfStickClampCircle = MIN_OF_STICK_CLAMP_MODE_CIRCLE;
    
  if (m_MinOfStickClampCross < MIN_OF_STICK_CLAMP_MODE_CROSS)
    m_MinOfStickClampCross = MIN_OF_STICK_CLAMP_MODE_CROSS;
    
  if (m_MaxOfStickClampCircle > LIMIT_OF_STICK_CLAMP_MAX)
    m_MaxOfStickClampCircle = LIMIT_OF_STICK_CLAMP_MAX;
    
  if (m_MaxOfStickClampCross > LIMIT_OF_STICK_CLAMP_MAX)
    m_MaxOfStickClampCross = LIMIT_OF_STICK_CLAMP_MAX;
    
  if (m_MaxOfStickClampMinimum > LIMIT_OF_STICK_CLAMP_MAX)
    m_MaxOfStickClampMinimum = LIMIT_OF_STICK_CLAMP_MAX;
}

f32 AnalogStickClamper::NormalizeStick(s16 x)
{
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
}

void AnalogStickClamper::NormalizeStickWithScale(f32* normalized_x, f32* normalized_y, s16 x, s16 y)
{
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

    switch (m_StickClampMode)
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
}

void AnalogStickClamper::SetStickClamp(short min, short max) 
{
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
}

}
}
}