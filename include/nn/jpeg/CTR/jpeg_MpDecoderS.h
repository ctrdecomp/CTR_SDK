#pragma once

#include <nn/jpeg/CTR/jpeg_MpDecoder.h>

#ifdef __cplusplus

namespace nn {
namespace jpeg {
namespace CTR {

class JpegMpDecoderS : public JpegMpDecoder
{
public:
    const u8* GetLastSysMakerNotePointer() const
    {
        return GetLastMakerNotePointer(detail::MAKER_NOTE_INDEX_SYS_1);
    }

    size_t GetLastSysMakerNoteSize() const
    {
        return GetLastMakerNoteSize(detail::MAKER_NOTE_INDEX_SYS_1);
    }
};

}
}
}

#endif