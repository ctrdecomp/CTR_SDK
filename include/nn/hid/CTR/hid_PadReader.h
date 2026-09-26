#pragma once

#include <nn/Handle.h>
#include <nn/Result.h>
#include <nn/types.h>
#include <nn/hid/CTR/hid_AnalogStickClamper.h>
#include <nn/hid/CTR/hid_Api.h>
#include <nn/hid/CTR/hid_Pad.h>
#include <nn/hid/CTR/hid_DeviceStatus.h>
#include <nn/hidlow/hidlow_Utils.h>
#include <nn/util/util_NonCopyable.h>

namespace nn{
namespace hid{
namespace CTR{

class PadReader : private nn::util::ADLFireWall::NonCopyable<PadReader>
{
public:
    typedef AnalogStickClamper::ClampMode StickClampMode;

    PadReader(Pad& pad=GetPad( ));
    ~PadReader() {};
    void Read(PadStatus* pBufs, s32* pReadLen, s32 bufLen);
    bool ReadLatest(PadStatus* pBuf);

    void SetStickClamp(short min, short max);

    void GetStickClamp(s16* pMin, s16* pMax) const;

    StickClampMode GetStickClampMode() const
    {
        return this->m_StickClamper.GetStickClampMode();
    }
    void SetStickClampMode(StickClampMode mode);
    
    f32 NormalizeStick(short x);
    void NormalizeStickWithScale(f32* normalized_x, f32* normalized_y, s16 x, s16 y);
    void SetNormalizeStickScaleSettings(f32 scale, s16 threshold);

    static const s8 MAX_READ_NUM = 7;
    
    static void HideKeyInfo(PadStatus* padStatus)
    {
        padStatus->hold = 0;
        padStatus->release = 0;
        padStatus->trigger = 0;
        padStatus->stick.x = 0;
        padStatus->stick.y = 0;
    }
protected:
    Pad& m_Pad;
    s32 m_IndexOfRead;
    bit32 m_LatestHold;
    AnalogStickClamper m_StickClamper;
    bool m_IsReadLatestFirst;
    s8 rev[3];
    s32 rev2;
    s64 m_TickOfRead;

public:
    static AnalogStickClamper::ClampMode  ClamperClampMode(const StickClampMode mode){ return (AnalogStickClamper::ClampMode)mode; }
};

}
}
}