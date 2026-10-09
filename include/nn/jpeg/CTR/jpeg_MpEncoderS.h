#ifndef NN_JPEG_CTR_JPEG_MP_ENCODER_S_H_
#define NN_JPEG_CTR_JPEG_MP_ENCODER_S_H_

#include <nn/jpeg/CTR/jpeg_MpEncoder.h>

#ifdef __cplusplus

namespace nn {
namespace jpeg {
namespace CTR {

class JpegMpEncoderS : public JpegMpEncoder
{
public:
    void SetSysMakerNote(const u8* pBuffer, size_t size)
    {
        SetMakerNote(pBuffer, size, detail::MAKER_NOTE_INDEX_SYS_1);
    }

    void SetTwlPhotoMakerNote(const u8* pBuffer)
    {
        if(m_Initialized)
        {
            m_TemporarySetting.pTwlPhotoMakerNoteData = pBuffer;
        }
    }
};

}
}
}

#endif

#endif

