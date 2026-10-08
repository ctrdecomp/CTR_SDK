#pragma once

#include <nn/fnd/fnd_TimeSpan.h>
#include <nn/math/math_Misc.h>

#define NN_CTR_MPCORE_TICKS_PER_SECOND 134055928LL

namespace nn{
namespace svc{
    s64 GetSystemTick();
}

namespace os{

class Tick
{
public:
    s64 m_Tick;

    static const s64 TICKS_PER_SECOND  = NN_CTR_MPCORE_TICKS_PER_SECOND;

    Tick(s64 tick = 0): 
        m_Tick(tick)
    {
    }

    Tick(nn::fnd::TimeSpan span): 
        m_Tick(nnmathMultiplyRate32(span.GetNanoSeconds(), math::MakeRate32<TICKS_PER_SECOND, 1000 * 1000 * 1000>::VALUE))
    {
    }

    operator s64() const { return m_Tick; }
    operator nn::fnd::TimeSpan() const 
    { 
        return nn::fnd::TimeSpan::FromNanoSeconds(nnmathMultiplyRate(this->m_Tick,math::MakeRate<1000 * 1000 * 1000, TICKS_PER_SECOND>::VALUE)); 
    }
    Tick& operator-=(Tick rhs){ m_Tick -= rhs.m_Tick; return *this; }
    Tick operator-(Tick rhs) const{ Tick ret(*this); return ret -= rhs; }

    Tick& operator+=(Tick rhs){ m_Tick += rhs.m_Tick; return *this; }
    Tick operator+(Tick rhs) const{ Tick ret(*this); return ret += rhs; }

    Tick& operator+=(fnd::TimeSpan rhs)
    {
        const s64 tick = nnmathMultiplyRate32(
            rhs.GetNanoSeconds(), math::MakeRate32<TICKS_PER_SECOND, 1000 * 1000 * 1000>::VALUE);
        this->m_Tick += tick;
        return *this;
    }
    Tick operator+(fnd::TimeSpan rhs) const{ Tick ret(*this); return ret += rhs; }

    nn::fnd::TimeSpan ToTimeSpan() const{ return *this; }

    static Tick GetSystemCurrent()
    {
        return Tick(nn::svc::GetSystemTick());
    }
};

}
}
