#ifndef NN_LIBRARIES_JPEG_CTR_EXIF_MP_DECODER_H_
#define NN_LIBRARIES_JPEG_CTR_EXIF_MP_DECODER_H_

#include "jpeg_MpDecoderInternal.h"

#ifdef __cplusplus

namespace nn {
namespace jpeg {
namespace CTR {
namespace detail {

bool DecodeJpegApp1(JpegMpDecoderContext* pCtx, size_t app1Size);
bool DecodeJpegApp2Mp(JpegMpDecoderContext* pCtx, size_t app2Size);

struct App1PointerAndSize
{
    const u8* pointer;
    size_t size;
};

size_t GetApp1DateTime(char* pBuffer, const JpegMpDecoderContext* pCtx);
const char* GetApp1DateTimePointer(const JpegMpDecoderContext* pCtx);
size_t GetApp1ImageUid(char* pBuffer, const JpegMpDecoderContext* pCtx);
bool GetApp1SoftwarePointer(App1PointerAndSize* pBuffer, const JpegMpDecoderContext* pCtx);
bool GetApp1MakerNotePointer(App1PointerAndSize* pBuffer, const JpegMpDecoderContext* pCtx, u32 index);
bool GetApp1TwlPhotoMakerNotePointer(App1PointerAndSize* pBuffer, const JpegMpDecoderContext* pCtx);
bool GetApp1TwlUserMakerNotePointer(App1PointerAndSize* pBuffer, const JpegMpDecoderContext* pCtx);
bool GetApp1Orientation(u16* pBuffer, const JpegMpDecoderContext* pCtx);
bool GetApp1GpsData(GpsData* pBuffer, const JpegMpDecoderContext* pCtx);

}
}
}
}

#endif

#endif
