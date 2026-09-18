// Filename: snd_DspFxManagerImpl.cpp
//
// Project: Horizon

#include "snd_DspFxManager.h"
#include <nn/snd/CTR/MPCore/snd_OperateMaster.h>

namespace nn {
namespace snd {
namespace CTR {
namespace{
    enum{
        CTRL_ENABLE     =   1,
        CTRL_ADDRESSES  =   2,
        CTRL_COEFS      =   4
    };
}

void DspFxManagerImpl::Initialize() {
    for(int i = 0; i < AUX_BUS_NUM; i++){
        {
            DspFxDelayParams params;
            params.enable = false;
            params.ctrl = 1;
            this->SetDspDelayEffect((AuxBusId)i,&params);
        }

        {
            DspFxReverbParams params;
            params.enable = 0;
            params.ctrl = 1;
            this->SetDspReverbEffect((AuxBusId)i,&params);
        }
    }
}

void DspFxManagerImpl::Finalize()
{ 
}

void DspFxManagerImpl::ForceUpdateParams() 
{
    for(int i = 0; i < AUX_BUS_NUM; i++)
    {
        AuxBusId busId = (AuxBusId)i;
        mDspFxDelayParams[busId].ctrl= 0xffff;
        Dspsnd::GetInstance().SetDspDelayEffect(busId, &this->m_DspFxDelayParams[busId]);
        mDspFxReverbParams[busId].ctrl = 0xffff;
        Dspsnd::GetInstance().SetDspReverbEffect(busId, &this->m_DspFxReverbParams[busId]);
    }
}

DspFxManagerImpl& DspFxManagerImpl::GetInstance() 
{
    static DspFxManagerImpl instance;
    return instance;
}

bool DspFxManagerImpl::SetDspDelayEffect(AuxBusId id, DspFxDelayParams* param) 
{
    if((param->ctrl & CTRL_ENABLE) != 0)
    {
        mDspFxDelayParams[id].enable = param->enable;
    }
    if((param->ctrl & CTRL_COEFS) != 0)
    {
        m_DspFxDelayParams[id].channels = param->channels;
        m_DspFxDelayParams[id].delayFrames = param->delayFrames;
        m_DspFxDelayParams[id].delayFeedbackGain = param->delayFeedbackGain;

        for(int i = 0; i < AUX_BUS_NUM; i += 1)
        {
            m_DspFxDelayParams[id].aLpfCoefs[i] = param->aLpfCoefs[i];
        }

    }
    if((param->ctrl & CTRL_ADDRESSES) != 0)
    {
        m_DspFxDelayParams[id].delayBufferAddress = param->delayBufferAddress;
    }
    return Dspsnd::GetInstance().SetDspDelayEffect(id,param);
}

bool DspFxManagerImpl::SetDspReverbEffect(AuxBusId id, DspFxReverbParams* param) 
{
    if((param->ctrl & CTRL_ENABLE) != 0)
    {
        this->m_DspFxReverbParams[id].enable = param->enable;
    }
    if((param->ctrl & CTRL_COEFS) != 0)
    {
        m_DspFxReverbParams[id].channels = param->channels;
        m_DspFxReverbParams[id].earlyDelayFrames = param->earlyDelayFrames;
        m_DspFxReverbParams[id].preDelayFrames = param->preDelayFrames;

        for(int i = 0; i < AUX_BUS_NUM; i++)
        {
            m_DspFxReverbParams[id].combFrames[i] = param->combFrames[i];
        }

        m_DspFxReverbParams[id].allPassFrames = param->allPassFrames;
        m_DspFxReverbParams[id].earlyGain = param->earlyGain;
        m_DspFxReverbParams[id].fusedGain = param->fusedGain;
        m_DspFxReverbParams[id].allPassCoef = param->allPassCoef;

        for(int coefi = 0; coefi < AUX_BUS_NUM; coefi++)
        {
            m_DspFxReverbParams[id].aCombCoefs[coefi] = param->aCombCoefs[coefi];
        }

        for(int lpfco = 0; lpfco < AUX_BUS_NUM; lpfco++)
        {
            m_DspFxReverbParams[id].aLpfCoefs[lpfco] = param->aLpfCoefs[lpfco];
        }
    }
    if((param->ctrl & CTRL_ADDRESSES) != 0)
    {
        m_DspFxReverbParams[id].earlyDelayBufferAddress = param->earlyDelayBufferAddress;
        m_DspFxReverbParams[id].preDelayBufferAddress = param->preDelayBufferAddress;
        m_DspFxReverbParams[id].combBufferAddress[0] = param->combBufferAddress[0];
        m_DspFxReverbParams[id].combBufferAddress[1] = param->combBufferAddress[1];
        m_DspFxReverbParams[id].allPassBufferAddress = param->allPassBufferAddress;
    }
    Dspsnd::GetInstance().SetDspReverbEffect(id,param);
}

}
}
}