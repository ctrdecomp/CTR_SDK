#ifndef NN_LIBRARIES_JPEG_CTR_EXIF_MP_ENCODER_H_
#define NN_LIBRARIES_JPEG_CTR_EXIF_MP_ENCODER_H_

#include "jpeg_MpEncoderInternal.h"

#ifdef __cplusplus

namespace nn {
namespace jpeg {
namespace CTR {
namespace detail{

inline void EncodeTiffDataShort(u8* pDst, u16 data, bool isLittleEndian)
{
    if(isLittleEndian)
    {
        pDst[0] = static_cast<u8>(data);
        pDst[1] = static_cast<u8>(data >> 8);
    }
    else
    {
        pDst[0] = static_cast<u8>(data >> 8);
        pDst[1] = static_cast<u8>(data);
    }
}

inline void EncodeTiffDataLong(u8* pDst, u32 data, bool isLittleEndian)
{
    if(isLittleEndian)
    {
        pDst[0] = static_cast<u8>(data);
        pDst[1] = static_cast<u8>(data >> 8);
        pDst[2] = static_cast<u8>(data >> 16);
        pDst[3] = static_cast<u8>(data >> 24);
    }
    else
    {
        pDst[0] = static_cast<u8>(data >> 24);
        pDst[1] = static_cast<u8>(data >> 16);
        pDst[2] = static_cast<u8>(data >> 8);
        pDst[3] = static_cast<u8>(data);
    }
}

template <typename T> inline void InitializeTagCommon(JpegTagWorkObj* pTag, T tag, ExifTagType type)
{
    pTag->tag = static_cast<u16>(tag);
    pTag->type = static_cast<u8>(type);
}

template <typename T> inline void InitializeTagSizeCommon(JpegTagWorkObj* pTag, T tag, ExifTagType type, size_t size)
{
    InitializeTagCommon(pTag, tag, type);
    pTag->dataSize = size;
}

template <typename T>
inline void InitializeTagSizeDataCommon(JpegTagWorkObj* pTag, T tag, ExifTagType type, size_t size, const void* pData)
{
    InitializeTagSizeCommon(pTag, tag, type, size);
    pTag->pData = static_cast<const u8*>(pData);
}

template <typename T> inline void InitializeTagDataByte(JpegTagWorkObj* pTag, T tag, const u8* pData)
{
    InitializeTagSizeDataCommon(pTag, tag, EXIF_TAG_TYPE_BYTE, 1 * 1, pData);
}

template <typename T> inline void InitializeTagDataByteArray(JpegTagWorkObj* pTag, T tag, const u8* pData, u32 count)
{
    InitializeTagSizeDataCommon(pTag, tag, EXIF_TAG_TYPE_BYTE, count * 1, pData);
}

template <typename T> inline void InitializeTagDataAscii(JpegTagWorkObj* pTag, T tag, const char* pData, u32 count)
{
    InitializeTagSizeDataCommon(pTag, tag, EXIF_TAG_TYPE_ASCII, count, pData);
}

template <typename T> inline void InitializeTagDataShort(JpegTagWorkObj* pTag, T tag, const u16* pData)
{
    InitializeTagSizeDataCommon(pTag, tag, EXIF_TAG_TYPE_SHORT, 1 * 2, pData);
}

template <typename T> inline void InitializeTagDataShortArray(JpegTagWorkObj* pTag, T tag, const u16* pData, u32 count)
{
    InitializeTagSizeDataCommon(pTag, tag, EXIF_TAG_TYPE_SHORT, count * 2, pData);
}

template <typename T> inline void InitializeTagDataLong(JpegTagWorkObj* pTag, T tag, const u32* pData)
{
    InitializeTagSizeDataCommon(pTag, tag, EXIF_TAG_TYPE_LONG, 1 * 4, pData);
}

template <typename T> inline void InitializeTagDataRational(JpegTagWorkObj* pTag, T tag, const Rational* pData)
{
    InitializeTagSizeDataCommon(pTag, tag, EXIF_TAG_TYPE_RATIONAL, 1 * 8, pData);
}

template <typename T>
inline void InitializeTagDataRationalArray(JpegTagWorkObj* pTag, T tag, const Rational* pData, u32 count)
{
    InitializeTagSizeDataCommon(pTag, tag, EXIF_TAG_TYPE_RATIONAL, count * 8, pData);
}

template <typename T> inline void InitializeTagDataUndefined(JpegTagWorkObj* pTag, T tag, const void* pData, u32 count)
{
    InitializeTagSizeDataCommon(pTag, tag, EXIF_TAG_TYPE_UNDEFINED, count, pData);
}

template <typename T> inline void InitializeTagDataSlong(JpegTagWorkObj* pTag, T tag, const s32* pData)
{
    InitializeTagSizeDataCommon(pTag, tag, EXIF_TAG_TYPE_SLONG, 1 * 4, pData);
}

template <typename T> inline void InitializeTagDataSrational(JpegTagWorkObj* pTag, T tag, const Srational* pData)
{
    InitializeTagSizeDataCommon(pTag, tag, EXIF_TAG_TYPE_SRATIONAL, 1 * 8, pData);
}

void InitializeJpegMpEncoderApp1TagWork(detail::JpegMpEncoderWorkObj* pWork);

size_t CalcJpegMpEncoderApp1Size(detail::JpegMpEncoderWorkObj* pWork);

void PreEncodeJpegApp1(detail::JpegMpEncoderWorkObj* pWork, detail::JpegMpEncoderTemporarySettingObj* pTempSetting,
                       bool addThumbnail);
bool EncodeJpegApp1(detail::JpegMpEncoderWorkObj* pWork);

bool EncodeJpegMpApp2(detail::JpegMpEncoderWorkObj* pWork, detail::JpegMpEncoderTemporarySettingObj* pTempSetting);

void InitializeJpegMpEncoderApp2IndexTagWork(detail::JpegMpEncoderWorkObj* pWork);
void InitializeJpegMpEncoderApp2AttributeTagWork(detail::JpegMpEncoderWorkObj* pWork);

bool PostEncodeJpegMpApp2(detail::JpegMpEncoderWorkObj* pWork, bool fromApi);

}
}
}
}

#endif

#endif
