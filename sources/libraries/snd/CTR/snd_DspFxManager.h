#pragma once

#include <nn/types.h>
#include <nn/snd/CTR/MPCore/snd_DspFxDelay.h>
#include <nn/snd/CTR/MPCore/snd_DspFxReverb.h>

namespace nn{
namespace snd{
namespace CTR{

class DspFxManagerImpl;

class DspFxManagerImpl
{
protected:
    DspFxDelayParams m_DspFxDelayParams[AUX_BUS_NUM];
    DspFxReverbParams m_DspFxReverbParams[AUX_BUS_NUM];

public:
    void Initialize();
    void Finalize();
    void ForceUpdateParams();
    static DspFxManagerImpl& GetInstance();
    bool SetDspDelayEffect(AuxBusId id, DspFxDelayParams* param);
    bool SetDspReverbEffect(AuxBusId id, DspFxReverbParams* param);
};

class DspFxManager
{
protected:
    bool m_IsAttached[AUX_BUS_NUM][AUX_BUS_NUM];
    bool m_IsEnabled[AUX_BUS_NUM][AUX_BUS_NUM];
    s8 m_ChannelNum[AUX_BUS_NUM][AUX_BUS_NUM];

public:
    enum DspEffectType
    {
        DSP_EFFECT_TYPE_DELAY = 0,
        DSP_EFFECT_TYPE_REVERB = 1,
        DSP_EFFECT_TYPE_NUM = 2
    };
public:
    void Initialize();
    void Finalize();
    static DspFxManager& GetInstance();
    bool Detach(DspEffectType type,AuxBusId id);
    bool Attach(DspEffectType,AuxBusId);
    s32 GetDspCycles();
    bool SetDspDelayEffect(AuxBusId id, DspFxDelayParams* param);
    bool SetDspReverbEffect(AuxBusId id, DspFxReverbParams* param);
    s32 GetChannelNum(DspEffectType type, AuxBusId id);

    DspFxManagerImpl* GetImpl(){ return &DspFxManagerImpl::GetInstance(); }
};

}
}
}