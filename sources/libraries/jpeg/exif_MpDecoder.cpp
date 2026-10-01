// Filename: exif_MpDecoder.cpp
//
// Project: Horizon

#include "exif_MpDecoder.h"
#include <string.h>

namespace nn {
namespace jpeg {
namespace CTR {
namespace detail {

namespace
{

const u8 App1ExifTiffHeaderTemplateLittleEndian[] = {

    'E', 'x', 'i',  'f',  0x00, 0x00,

    'I', 'I', 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
const u8 App1ExifTiffHeaderTemplateBigEndian[] = {

    'E', 'x', 'i',  'f',  0x00, 0x00,

    'M', 'M', 0x00, 0x2A, 0x00, 0x00, 0x00, 0x08};

const u8 App2MpFormatIdCode[] = {

    'M', 'P', 'F', 0x00};
const u8 App2MpEndianInfoLittleEndian[] = {

    'I', 'I', 0x2A, 0x00};
const u8 App2MpEndianInfoBigEndian[] = {

    'M', 'M', 0x00, 0x2A};
}

namespace
{

const u8 ExifDataSizeShiftCountTbl[NUM_EXIF_TAG_TYPES] = {

    0, 0, 0, 1, 2, 3, 0, 0, 0, 2, 3};

const JpegMpDecoderExifTagITN app1Ifd0ITNTbl[] = {
    {JPEG_DECODER_APP1_TAG_INDEX_IFD0_ORIENTATION, EXIF_TAG_TYPE_SHORT, APP1_TAG_NUMBER_IFD0_ORIENTATION},
    {JPEG_DECODER_APP1_TAG_INDEX_IFD0_SOFTWARE, EXIF_TAG_TYPE_ASCII, APP1_TAG_NUMBER_IFD0_SOFTWARE},
    {JPEG_DECODER_APP1_TAG_INDEX_IFD0_DATE_TIME, EXIF_TAG_TYPE_ASCII, APP1_TAG_NUMBER_IFD0_DATE_TIME},
    {JPEG_DECODER_APP1_TAG_INDEX_IFD0_EXIF_IFD_POINTER, EXIF_TAG_TYPE_LONG, APP1_TAG_NUMBER_IFD0_EXIF_IFD_POINTER},
    {JPEG_DECODER_APP1_TAG_INDEX_IFD0_GPS_IFD_POINTER, EXIF_TAG_TYPE_LONG, APP1_TAG_NUMBER_IFD0_GPS_IFD_POINTER}};
const JpegMpDecoderExifTagITN app1ExifITNTbl[] = {
    {JPEG_DECODER_APP1_TAG_INDEX_EXIF_MAKERNOTE, EXIF_TAG_TYPE_UNDEFINED, APP1_TAG_NUMBER_EXIF_MAKERNOTE},
    {JPEG_DECODER_APP1_TAG_INDEX_EXIF_INTEROPERABILITY_IFD_POINTER, EXIF_TAG_TYPE_LONG,
     APP1_TAG_NUMBER_EXIF_INTEROPERABILITY_IFD_POINTER},
    {JPEG_DECODER_APP1_TAG_INDEX_EXIF_IMAGE_UNIQUE_ID, EXIF_TAG_TYPE_ASCII, APP1_TAG_NUMBER_EXIF_IMAGE_UNIQUE_ID}};
const JpegMpDecoderExifTagITN app1MakerNoteITNTbl[3 + NUM_MAKER_NOTES] = {
    {JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_TWL_PRIVATE, EXIF_TAG_TYPE_UNDEFINED, APP1_TAG_NUMBER_MAKERNOTE_TWL_PRIVATE},
    {JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_TWL_PHOTO, EXIF_TAG_TYPE_UNDEFINED, APP1_TAG_NUMBER_MAKERNOTE_TWL_PHOTO},
    {JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_TWL_USER, EXIF_TAG_TYPE_UNDEFINED, APP1_TAG_NUMBER_MAKERNOTE_TWL_USER},

    {JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_CTR, EXIF_TAG_TYPE_UNDEFINED, APP1_TAG_NUMBER_MAKERNOTE_CTR},
    {JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_CTR + MAKER_NOTE_INDEX_SYS_1, EXIF_TAG_TYPE_UNDEFINED,
     APP1_TAG_NUMBER_MAKERNOTE_CTR + MAKER_NOTE_INDEX_SYS_1},
    {JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_CTR + MAKER_NOTE_INDEX_SYS_2, EXIF_TAG_TYPE_UNDEFINED,
     APP1_TAG_NUMBER_MAKERNOTE_CTR + MAKER_NOTE_INDEX_SYS_2},
    {JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_CTR + MAKER_NOTE_INDEX_SYS_3, EXIF_TAG_TYPE_UNDEFINED,
     APP1_TAG_NUMBER_MAKERNOTE_CTR + MAKER_NOTE_INDEX_SYS_3}};
const JpegMpDecoderExifTagITN app1Ifd1ITNTbl[] = {{JPEG_DECODER_APP1_TAG_INDEX_THUMB_JPEG_INTERCHANGE_FORMAT,
                                                   EXIF_TAG_TYPE_LONG, APP1_TAG_NUMBER_THUMB_JPEG_INTERCHANGE_FORMAT},
                                                  {JPEG_DECODER_APP1_TAG_INDEX_THUMB_JPEG_INTERCHANGE_FORMAT_LENGTH,
                                                   EXIF_TAG_TYPE_LONG,
                                                   APP1_TAG_NUMBER_THUMB_JPEG_INTERCHANGE_FORMAT_LENGTH}};
const JpegMpDecoderExifTagITN app1GpsITNTbl[] = {
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_VERSION_ID, EXIF_TAG_TYPE_BYTE, APP1_TAG_NUMBER_GPS_VERSION_ID},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_LATITUDE_REF, EXIF_TAG_TYPE_ASCII, APP1_TAG_NUMBER_GPS_LATITUDE_REF},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_LATITUDE, EXIF_TAG_TYPE_RATIONAL, APP1_TAG_NUMBER_GPS_LATITUDE},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_LONGITUDE_REF, EXIF_TAG_TYPE_ASCII, APP1_TAG_NUMBER_GPS_LONGITUDE_REF},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_LONGITUDE, EXIF_TAG_TYPE_RATIONAL, APP1_TAG_NUMBER_GPS_LONGITUDE},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_ALTITUDE_REF, EXIF_TAG_TYPE_BYTE, APP1_TAG_NUMBER_GPS_ALTITUDE_REF},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_ALTITUDE, EXIF_TAG_TYPE_RATIONAL, APP1_TAG_NUMBER_GPS_ALTITUDE},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_TIME_STAMP, EXIF_TAG_TYPE_RATIONAL, APP1_TAG_NUMBER_GPS_TIME_STAMP},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_SATELLITES, EXIF_TAG_TYPE_ASCII, APP1_TAG_NUMBER_GPS_SATELLITES},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_STATUS, EXIF_TAG_TYPE_ASCII, APP1_TAG_NUMBER_GPS_STATUS},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_MEASURE_MODE, EXIF_TAG_TYPE_ASCII, APP1_TAG_NUMBER_GPS_MEASURE_MODE},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DOP, EXIF_TAG_TYPE_RATIONAL, APP1_TAG_NUMBER_GPS_DOP},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_SPEED_REF, EXIF_TAG_TYPE_ASCII, APP1_TAG_NUMBER_GPS_SPEED_REF},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_SPEED, EXIF_TAG_TYPE_RATIONAL, APP1_TAG_NUMBER_GPS_SPEED},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_TRACK_REF, EXIF_TAG_TYPE_ASCII, APP1_TAG_NUMBER_GPS_TRACK_REF},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_TRACK, EXIF_TAG_TYPE_RATIONAL, APP1_TAG_NUMBER_GPS_TRACK},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_IMG_DIRECTION_REF, EXIF_TAG_TYPE_ASCII, APP1_TAG_NUMBER_GPS_IMG_DIRECTION_REF},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_IMG_DIRECTION, EXIF_TAG_TYPE_RATIONAL, APP1_TAG_NUMBER_GPS_IMG_DIRECTION},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_MAP_DATUM, EXIF_TAG_TYPE_ASCII, APP1_TAG_NUMBER_GPS_MAP_DATUM},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DEST_LATITUDE_REF, EXIF_TAG_TYPE_ASCII, APP1_TAG_NUMBER_GPS_DEST_LATITUDE_REF},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DEST_LATITUDE, EXIF_TAG_TYPE_RATIONAL, APP1_TAG_NUMBER_GPS_DEST_LATITUDE},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DEST_LONGITUDE_REF, EXIF_TAG_TYPE_ASCII, APP1_TAG_NUMBER_GPS_DEST_LONGITUDE_REF},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DEST_LONGITUDE, EXIF_TAG_TYPE_RATIONAL, APP1_TAG_NUMBER_GPS_DEST_LONGITUDE},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DEST_BEARING_REF, EXIF_TAG_TYPE_ASCII, APP1_TAG_NUMBER_GPS_DEST_BEARING_REF},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DEST_BEARING, EXIF_TAG_TYPE_RATIONAL, APP1_TAG_NUMBER_GPS_DEST_BEARING},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DEST_DISTANCE_REF, EXIF_TAG_TYPE_ASCII, APP1_TAG_NUMBER_GPS_DEST_DISTANCE_REF},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DEST_DISTANCE, EXIF_TAG_TYPE_RATIONAL, APP1_TAG_NUMBER_GPS_DEST_DISTANCE},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_PROCESSING_METHOD, EXIF_TAG_TYPE_UNDEFINED, APP1_TAG_NUMBER_GPS_PROCESSING_METHOD},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_AREA_INFORMATION, EXIF_TAG_TYPE_UNDEFINED, APP1_TAG_NUMBER_GPS_AREA_INFORMATION},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DATE_STAMP, EXIF_TAG_TYPE_ASCII, APP1_TAG_NUMBER_GPS_DATE_STAMP},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DIFFERENTIAL, EXIF_TAG_TYPE_SHORT, APP1_TAG_NUMBER_GPS_DIFFERENTIAL}};

const JpegMpDecoderExifTagITN app2MpIndexIfdITNTbl[] = {
    {JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_MPF_VERSION, EXIF_TAG_TYPE_UNDEFINED, APP2_MP_TAG_NUMBER_MPF_VERSION},
    {JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_NUMBER_OF_IMAGES, EXIF_TAG_TYPE_LONG,
     APP2_MP_TAG_NUMBER_NUMBER_OF_IMAGES},
    {JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_MP_ENTRY, EXIF_TAG_TYPE_UNDEFINED, APP2_MP_TAG_NUMBER_MP_ENTRY},
    {JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_IMAGE_UID_LIST, EXIF_TAG_TYPE_UNDEFINED,
     APP2_MP_TAG_NUMBER_IMAGE_UID_LIST},
    {JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_TOTAL_FRAMES, EXIF_TAG_TYPE_LONG, APP2_MP_TAG_NUMBER_TOTAL_FRAMES}};

const JpegMpDecoderExifTagITN app2MpAttributeITNTbl[] = {
    {JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_MPF_VERSION, EXIF_TAG_TYPE_UNDEFINED, APP2_MP_TAG_NUMBER_MPF_VERSION},
    {JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_MP_INDIVIDUAL_NUM, EXIF_TAG_TYPE_LONG,
     APP2_MP_TAG_NUMBER_MP_INDIVIDUAL_NUM},
    {JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_PAN_ORIENTATION, EXIF_TAG_TYPE_LONG, APP2_MP_TAG_NUMBER_PAN_ORIENTATION},
    {JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_PAN_OVERLAP_H, EXIF_TAG_TYPE_RATIONAL, APP2_MP_TAG_NUMBER_PAN_OVERLAP_H},
    {JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_PAN_OVERLAP_V, EXIF_TAG_TYPE_RATIONAL, APP2_MP_TAG_NUMBER_PAN_OVERLAP_V},
    {JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_BASE_VIEWPOINT_NUM, EXIF_TAG_TYPE_LONG,
     APP2_MP_TAG_NUMBER_BASE_VIEWPOINT_NUM},
    {JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_CONVERGENCE_ANGLE, EXIF_TAG_TYPE_SRATIONAL,
     APP2_MP_TAG_NUMBER_CONVERGENCE_ANGLE},
    {JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_BASELINE_LENGTH, EXIF_TAG_TYPE_RATIONAL,
     APP2_MP_TAG_NUMBER_BASELINE_LENGTH},
    {JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_VERTICAL_DIVERGENCE, EXIF_TAG_TYPE_SRATIONAL,
     APP2_MP_TAG_NUMBER_VERTICAL_DIVERGENCE},
    {JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_AXIS_DISTANCE_X, EXIF_TAG_TYPE_SRATIONAL,
     APP2_MP_TAG_NUMBER_AXIS_DISTANCE_X},
    {JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_AXIS_DISTANCE_Y, EXIF_TAG_TYPE_SRATIONAL,
     APP2_MP_TAG_NUMBER_AXIS_DISTANCE_Y},
    {JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_AXIS_DISTANCE_Z, EXIF_TAG_TYPE_SRATIONAL,
     APP2_MP_TAG_NUMBER_AXIS_DISTANCE_Z},
    {JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_YAW_ANGLE, EXIF_TAG_TYPE_SRATIONAL, APP2_MP_TAG_NUMBER_YAW_ANGLE},
    {JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_PITCH_ANGLE, EXIF_TAG_TYPE_SRATIONAL, APP2_MP_TAG_NUMBER_PITCH_ANGLE},
    {JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_ROLL_ANGLE, EXIF_TAG_TYPE_SRATIONAL, APP2_MP_TAG_NUMBER_ROLL_ANGLE}};
}

namespace
{
struct App1GpsCheckInfo
{
    u8 index;
    u8 count;
    u8 flagOffset;
    u8 valueOffset;
};

const App1GpsCheckInfo app1GpsCheckInfoTbl[] = {

    {JPEG_DECODER_APP1_TAG_INDEX_GPS_VERSION_ID, GPS_VERSION_ID_SIZE, offsetof(GpsData, isVersionIdValid),
     offsetof(GpsData, versionId)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_LATITUDE_REF, GPS_REF_SIZE, 0, offsetof(GpsData, latitudeRef)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_LATITUDE, NUM_GPS_LATITUDE_RATIONALS, offsetof(GpsData, isLatitudeValid),
     offsetof(GpsData, latitude)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_LONGITUDE_REF, GPS_REF_SIZE, 0, offsetof(GpsData, longitudeRef)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_LONGITUDE, NUM_GPS_LONGITUDE_RATIONALS, offsetof(GpsData, isLongitudeValid),
     offsetof(GpsData, longitude)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_ALTITUDE_REF, 1, offsetof(GpsData, isAltitudeRefValid),
     offsetof(GpsData, altitudeRef)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_ALTITUDE, 1, offsetof(GpsData, isAltitudeValid), offsetof(GpsData, altitude)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_TIME_STAMP, NUM_GPS_TIME_STAMP_RATIONALS, offsetof(GpsData, isTimeStampValid),
     offsetof(GpsData, timeStamp)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_SATELLITES, 0, offsetof(GpsData, pSatellites), 0},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_STATUS, GPS_REF_SIZE, 0, offsetof(GpsData, status)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_MEASURE_MODE, GPS_REF_SIZE, 0, offsetof(GpsData, measureMode)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DOP, 1, offsetof(GpsData, isDopValid), offsetof(GpsData, dop)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_SPEED_REF, GPS_REF_SIZE, 0, offsetof(GpsData, speedRef)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_SPEED, 1, offsetof(GpsData, isSpeedValid), offsetof(GpsData, speed)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_TRACK_REF, GPS_REF_SIZE, 0, offsetof(GpsData, trackRef)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_TRACK, 1, offsetof(GpsData, isTrackValid), offsetof(GpsData, track)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_IMG_DIRECTION_REF, GPS_REF_SIZE, 0, offsetof(GpsData, imgDirectionRef)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_IMG_DIRECTION, 1, offsetof(GpsData, isImgDirectionValid),
     offsetof(GpsData, imgDirection)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_MAP_DATUM, 0, offsetof(GpsData, pMapDatum), 0},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DEST_LATITUDE_REF, GPS_REF_SIZE, 0, offsetof(GpsData, destLatitudeRef)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DEST_LATITUDE, NUM_GPS_LATITUDE_RATIONALS, offsetof(GpsData, isDestLatitudeValid),
     offsetof(GpsData, destLatitude)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DEST_LONGITUDE_REF, GPS_REF_SIZE, 0, offsetof(GpsData, destLongitudeRef)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DEST_LONGITUDE, NUM_GPS_LONGITUDE_RATIONALS,
     offsetof(GpsData, isDestLongitudeValid), offsetof(GpsData, destLongitude)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DEST_BEARING_REF, GPS_REF_SIZE, 0, offsetof(GpsData, destBearingRef)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DEST_BEARING, 1, offsetof(GpsData, isDestBearingValid),
     offsetof(GpsData, destBearing)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DEST_DISTANCE_REF, GPS_REF_SIZE, 0, offsetof(GpsData, destDistanceRef)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DEST_DISTANCE, 1, offsetof(GpsData, isDestDistanceValid),
     offsetof(GpsData, destDistance)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_PROCESSING_METHOD, 0, offsetof(GpsData, pProcessingMethod),
     offsetof(GpsData, processingMethodSize)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_AREA_INFORMATION, 0, offsetof(GpsData, pAreaInformation),
     offsetof(GpsData, areaInformationSize)},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DATE_STAMP, GPS_DATE_STAMP_SIZE, offsetof(GpsData, pDateStamp), 0},
    {JPEG_DECODER_APP1_TAG_INDEX_GPS_DIFFERENTIAL, 1, offsetof(GpsData, isDifferentialValid),
     offsetof(GpsData, differential)}};
}

namespace
{
inline u16 GetWord(const u8* pSrc, bool isLittleEndian)
{
    return isLittleEndian ? (pSrc[0] | (pSrc[1] << 8)) : ((pSrc[0] << 8) | pSrc[1]);
}

inline u32 GetLong(const u8* pSrc, bool isLittleEndian)
{
    return isLittleEndian ? (pSrc[0] | (pSrc[1] << 8) | (pSrc[2] << 16) | (pSrc[3] << 24))
                          : ((pSrc[0] << 24) | (pSrc[1] << 16) | (pSrc[2] << 8) | pSrc[3]);
}

inline s32 GetSlong(const u8* pSrc, bool isLittleEndian)
{
    return isLittleEndian ? (pSrc[0] | (pSrc[1] << 8) | (pSrc[2] << 16) | (pSrc[3] << 24))
                          : ((pSrc[0] << 24) | (pSrc[1] << 16) | (pSrc[2] << 8) | pSrc[3]);
}

inline void GetRational(Rational* pDst, const u8* pSrc, bool isLittleEndian)
{
    pDst->value[0] = GetLong(pSrc, isLittleEndian);
    pDst->value[1] = GetLong(pSrc + sizeof(u32), isLittleEndian);
}

inline void GetSrational(Srational* pDst, const u8* pSrc, bool isLittleEndian)
{
    pDst->value[0] = GetSlong(pSrc, isLittleEndian);
    pDst->value[1] = GetSlong(pSrc + sizeof(u32), isLittleEndian);
}

inline void JpegMpDecoderSetError(JpegMpDecoderContext* pCtx, s8 errorCode)
{
    if(!pCtx->u.s.errorCode)
    {

        pCtx->u.s.errorCode = errorCode;
    }
}

}

bool GetTag(JpegTagWorkObj* pTag, const u8* pSrc, const u8* pSrcOffsetBase, const u8* pSrcEnd, bool isLittleEndian)
{
    u32 type;
    u32 count;
    u32 valueOffset;

    memset(pTag, 0, sizeof(*pTag));

    pTag->tag = GetWord(pSrc, isLittleEndian);
    type = GetWord(pSrc + 2, isLittleEndian);
    count = GetLong(pSrc + 4, isLittleEndian);
    valueOffset = GetLong(pSrc + 8, isLittleEndian);

    if(type >= NUM_EXIF_TAG_TYPES)
    {
        goto error;
    }
    pTag->type = static_cast<u8>(type);

    if(count > 0xFFFF)
    {
        goto error;
    }
    pTag->dataSize = count << ExifDataSizeShiftCountTbl[type];

    if(pTag->dataSize > 4)
    {
        if(((pSrcEnd - pSrcOffsetBase) <= valueOffset) || ((pSrcEnd - pSrcOffsetBase) < pTag->dataSize) ||
           ((pSrcEnd - pSrcOffsetBase) < (valueOffset + pTag->dataSize)))
        {

            goto error;
        }
        pTag->pData = pSrcOffsetBase + valueOffset;
    }
    else
    {

        pTag->pData = pSrc + 8;
    }

    return true;

error:
    return false;
}

bool CopyTagWorkByITN(JpegTagWorkObj* pTag, const JpegTagWorkObj* pSrcTag, const JpegMpDecoderExifTagITN* pItn,
                      u32 count)
{
    size_t checkOffset;

    while(count)
    {
        if(pItn->tag == pSrcTag->tag)
        {
            if(pItn->type != pSrcTag->type || pTag[pItn->index].u.isFound)
            {

                goto error;
            }
            if(pSrcTag->dataSize)
            {
                switch(pSrcTag->type)
                {
                case EXIF_TAG_TYPE_ASCII:
                    if(pSrcTag->pData[pSrcTag->dataSize - 1])
                    {

                        goto ignore;
                    }
                    break;

                case EXIF_TAG_TYPE_RATIONAL:
                case EXIF_TAG_TYPE_SRATIONAL:

                    for(checkOffset = 0; checkOffset < pSrcTag->dataSize; checkOffset += 2 * sizeof(u32))
                    {
                        if(!(pSrcTag->pData[checkOffset + sizeof(u32) + 0] |
                             pSrcTag->pData[checkOffset + sizeof(u32) + 1] |
                             pSrcTag->pData[checkOffset + sizeof(u32) + 2] |
                             pSrcTag->pData[checkOffset + sizeof(u32) + 3]))
                        {
                            goto ignore;
                        }
                    }
                    break;

                default:
                    break;
                }

                memcpy(&pTag[pItn->index], pSrcTag, sizeof(*pTag));
                pTag[pItn->index].u.isFound = true;
            }
        ignore:
            break;
        }
        pItn++;
        count--;
    }

    return true;

error:
    return false;
}

void CopyTagValue(void* pDst, const JpegTagWorkObj* pTag, bool isLittleEndian)
{
    const u8* pSrc = pTag->pData;
    size_t dataSize = pTag->dataSize;

    if(pSrc && dataSize)
    {
        switch(pTag->type)
        {
        case EXIF_TAG_TYPE_BYTE:
        case EXIF_TAG_TYPE_ASCII:
        case EXIF_TAG_TYPE_UNDEFINED:
            memcpy(pDst, pSrc, dataSize);
            if(pTag->type == EXIF_TAG_TYPE_ASCII)
            {
                *(reinterpret_cast<char*>(pDst) + (dataSize - 1)) = '\0';
            }
            break;

        case EXIF_TAG_TYPE_SHORT:
            while(dataSize)
            {
                *reinterpret_cast<u16*>(pDst) = GetWord(pSrc, isLittleEndian);
                pDst = reinterpret_cast<u16*>(pDst) + 1;
                pSrc += sizeof(u16);
                dataSize -= sizeof(u16);
            }
            break;

        case EXIF_TAG_TYPE_LONG:
        case EXIF_TAG_TYPE_SLONG:
            while(dataSize)
            {
                *reinterpret_cast<u32*>(pDst) = GetLong(pSrc, isLittleEndian);
                pDst = reinterpret_cast<u32*>(pDst) + 1;
                pSrc += sizeof(u32);
                dataSize -= sizeof(u32);
            }
            break;

        case EXIF_TAG_TYPE_RATIONAL:
        case EXIF_TAG_TYPE_SRATIONAL:
            while(dataSize)
            {
                GetRational(reinterpret_cast<Rational*>(pDst), pSrc, isLittleEndian);
                pDst = reinterpret_cast<Rational*>(pDst) + 1;
                pSrc += 2 * sizeof(u32);
                dataSize -= 2 * sizeof(u32);
            }
            break;

        default:
            NN_PANIC_("jpeg library error");
            return;
        }
    }
}

bool DecodeJpegApp1(JpegMpDecoderContext* pCtx, size_t app1Size)
{
    JpegTagWorkObj tag;
    JpegTagWorkObj* pApp1 = pCtx->app1TagWork;
    const u8* pSrc = pCtx->pSrc + pCtx->srcOffset + 2;
    const u8* pSrcEnd = pCtx->pSrc + pCtx->srcOffset + app1Size;
    const size_t exifPadSize = 6;
    const u8* pSrcOffsetBase = pSrc + exifPadSize;
    bool isLittleEndian;
    u32 len;
    u32 i;

    if(app1MakerNoteITNTbl[(sizeof(app1MakerNoteITNTbl) / sizeof(*app1MakerNoteITNTbl)) - 1].index !=
       (JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_CTR + NUM_MAKER_NOTES - 1))
    {

        JpegMpDecoderSetError(pCtx, JPEG_DECODER_ERROR_INTERNAL);
        goto error;
    }

    if((app1Size >= pCtx->srcSize) || ((pCtx->srcOffset + app1Size) >= pCtx->srcSize) ||
       ((pCtx->srcOffset + app1Size) <= app1Size) || (pSrcEnd < pSrc))
    {

        goto app1Error;
    }

    if(pCtx->app1ExifFound)
    {
        goto skip;
    }

    if((app1Size <= sizeof(App1ExifTiffHeaderTemplateLittleEndian)) ||
       memcmp(pSrc, App1ExifTiffHeaderTemplateLittleEndian, exifPadSize))
    {
        goto skip;
    }

    pCtx->app1ExifFound = true;

    if((pSrc + sizeof(App1ExifTiffHeaderTemplateLittleEndian) + TIFF_COUNT_SIZE) >= pSrcEnd)
    {
        goto app1Error;
    }

    if(!memcmp(pSrc, App1ExifTiffHeaderTemplateLittleEndian, sizeof(App1ExifTiffHeaderTemplateLittleEndian)))
    {
        pCtx->isApp1LittleEndian = true;
    }
    else if(!memcmp(pSrc, App1ExifTiffHeaderTemplateBigEndian, sizeof(App1ExifTiffHeaderTemplateBigEndian)))
    {
        pCtx->isApp1LittleEndian = false;
    }
    else
    {

        goto app1Error;
    }
    isLittleEndian = pCtx->isApp1LittleEndian;
    pSrc += sizeof(App1ExifTiffHeaderTemplateLittleEndian);
    pCtx->pApp1ExifOffsetBase = pSrcOffsetBase;

    len = GetWord(pSrc, isLittleEndian);
    pSrc += TIFF_COUNT_SIZE;

    if(len > 0xFFFF)
    {
        goto app1Error;
    }

    if(((pSrc + len * TIFF_FIELD_ENTRY_SIZE + TIFF_OFFSET_SIZE) <= pSrcOffsetBase) ||
       ((pSrc + len * TIFF_FIELD_ENTRY_SIZE + TIFF_OFFSET_SIZE) > pSrcEnd))
    {
        goto app1Error;
    }

    while(len)
    {
        if((!GetTag(&tag, pSrc, pSrcOffsetBase, pSrcEnd, isLittleEndian)) ||
           (!CopyTagWorkByITN(pApp1, &tag, app1Ifd0ITNTbl, sizeof(app1Ifd0ITNTbl) / sizeof(*app1Ifd0ITNTbl))))
        {
            goto app1Error;
        }
        pSrc += TIFF_FIELD_ENTRY_SIZE;
        len--;
    }

    if(pCtx->decodeThumbnail)
    {
        i = GetLong(pSrc, isLittleEndian);
        if(i)
        {

            pSrc = pSrcOffsetBase + i;

            if((pSrc <= pSrcOffsetBase) || (pSrcEnd <= pSrc) || (pSrcEnd <= (pSrc + TIFF_COUNT_SIZE)))
            {
                goto app1Error;
            }

            len = GetWord(pSrc, isLittleEndian);
            pSrc += TIFF_COUNT_SIZE;

            if(len > 0xFFFF)
            {
                goto app1Error;
            }

            if(((pSrc + len * TIFF_FIELD_ENTRY_SIZE + TIFF_OFFSET_SIZE) <= pSrcOffsetBase) ||
               ((pSrc + len * TIFF_FIELD_ENTRY_SIZE + TIFF_OFFSET_SIZE) > pSrcEnd))
            {
                goto app1Error;
            }

            while(len)
            {
                if((!GetTag(&tag, pSrc, pSrcOffsetBase, pSrcEnd, isLittleEndian)) ||
                   (!CopyTagWorkByITN(pApp1, &tag, app1Ifd1ITNTbl, sizeof(app1Ifd1ITNTbl) / sizeof(*app1Ifd1ITNTbl))))
                {
                    goto app1Error;
                }
                pSrc += TIFF_FIELD_ENTRY_SIZE;
                len--;
            }
        }
        else
        {

            goto thumbnailError;
        }
    }

    if(pApp1[JPEG_DECODER_APP1_TAG_INDEX_IFD0_EXIF_IFD_POINTER].u.isFound)
    {

        pSrc = pSrcOffsetBase + GetLong(pApp1[JPEG_DECODER_APP1_TAG_INDEX_IFD0_EXIF_IFD_POINTER].pData, isLittleEndian);

        if((pSrc <= pSrcOffsetBase) || (pSrcEnd <= pSrc) || (pSrcEnd <= (pSrc + TIFF_COUNT_SIZE)))
        {
            goto app1Error;
        }
        len = GetWord(pSrc, isLittleEndian);
        pSrc += TIFF_COUNT_SIZE;

        if(len > 0xFFFF)
        {
            goto app1Error;
        }

        if(((pSrc + len * TIFF_FIELD_ENTRY_SIZE + TIFF_OFFSET_SIZE) <= pSrcOffsetBase) ||
           ((pSrc + len * TIFF_FIELD_ENTRY_SIZE + TIFF_OFFSET_SIZE) > pSrcEnd))
        {
            goto app1Error;
        }

        while(len)
        {
            if((!GetTag(&tag, pSrc, pSrcOffsetBase, pSrcEnd, isLittleEndian)) ||
               (!CopyTagWorkByITN(pApp1, &tag, app1ExifITNTbl, sizeof(app1ExifITNTbl) / sizeof(*app1ExifITNTbl))))
            {
                goto app1Error;
            }
            pSrc += TIFF_FIELD_ENTRY_SIZE;
            len--;
        }

        if(pApp1[JPEG_DECODER_APP1_TAG_INDEX_EXIF_INTEROPERABILITY_IFD_POINTER].u.isFound)
        {

            pSrc = pSrcOffsetBase +
                   GetLong(pApp1[JPEG_DECODER_APP1_TAG_INDEX_EXIF_INTEROPERABILITY_IFD_POINTER].pData, isLittleEndian);

            if((pSrc <= pSrcOffsetBase) || (pSrcEnd <= pSrc) || (pSrcEnd <= (pSrc + TIFF_COUNT_SIZE)))
            {
                goto app1Error;
            }
            len = GetWord(pSrc, isLittleEndian);
            pSrc += TIFF_COUNT_SIZE;

            if(len > 0xFFFF)
            {
                goto app1Error;
            }

            if(((pSrc + len * TIFF_FIELD_ENTRY_SIZE + TIFF_OFFSET_SIZE) <= pSrcOffsetBase) ||
               ((pSrc + len * TIFF_FIELD_ENTRY_SIZE + TIFF_OFFSET_SIZE) > pSrcEnd))
            {
                goto app1Error;
            }
        }

        if(pApp1[JPEG_DECODER_APP1_TAG_INDEX_EXIF_MAKERNOTE].u.isFound)
        {
            const u8* pSrcEndTwlMakerNote;
            bool isValid = false;

            pSrc = pApp1[JPEG_DECODER_APP1_TAG_INDEX_EXIF_MAKERNOTE].pData;
            pSrcEndTwlMakerNote = pSrc + pApp1[JPEG_DECODER_APP1_TAG_INDEX_EXIF_MAKERNOTE].dataSize;

            if((pSrc <= pSrcOffsetBase) || (pSrcEndTwlMakerNote <= pSrc) ||
               (pSrcEndTwlMakerNote <= (pSrc + TIFF_COUNT_SIZE)))
            {
                goto skipTwlMakerNote;
            }
            len = GetWord(pSrc, isLittleEndian);
            pSrc += TIFF_COUNT_SIZE;

            if(len > 0xFFFF)
            {

                goto skipTwlMakerNote;
            }

            if(((pSrc + len * TIFF_FIELD_ENTRY_SIZE + TIFF_OFFSET_SIZE) <= pSrcOffsetBase) ||
               ((pSrc + len * TIFF_FIELD_ENTRY_SIZE + TIFF_OFFSET_SIZE) > pSrcEndTwlMakerNote))
            {
                goto skipTwlMakerNote;
            }
            while(len)
            {
                if((!GetTag(&tag, pSrc, pSrcOffsetBase, pSrcEndTwlMakerNote, isLittleEndian)) ||
                   (!CopyTagWorkByITN(pApp1, &tag, app1MakerNoteITNTbl,
                                      sizeof(app1MakerNoteITNTbl) / sizeof(*app1MakerNoteITNTbl))))
                {
                    goto skipTwlMakerNote;
                }
                pSrc += TIFF_FIELD_ENTRY_SIZE;
                len--;
            }

            isValid = true;
        skipTwlMakerNote:
            if(!isValid)
            {

                memset(
                    &pApp1[JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_TWL_BEGIN], 0,
                    (JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_TWL_END - JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_TWL_BEGIN) *
                        sizeof(*pApp1));
            }
        }
    }

    if(pApp1[JPEG_DECODER_APP1_TAG_INDEX_IFD0_GPS_IFD_POINTER].u.isFound)
    {

        pSrc = pSrcOffsetBase + GetLong(pApp1[JPEG_DECODER_APP1_TAG_INDEX_IFD0_GPS_IFD_POINTER].pData, isLittleEndian);

        if((pSrc <= pSrcOffsetBase) || (pSrcEnd <= pSrc) || (pSrcEnd <= (pSrc + TIFF_COUNT_SIZE)))
        {
            goto app1Error;
        }
        len = GetWord(pSrc, isLittleEndian);
        pSrc += TIFF_COUNT_SIZE;

        if(len > 0xFFFF)
        {
            goto app1Error;
        }

        if(((pSrc + len * TIFF_FIELD_ENTRY_SIZE + TIFF_OFFSET_SIZE) <= pSrcOffsetBase) ||
           ((pSrc + len * TIFF_FIELD_ENTRY_SIZE + TIFF_OFFSET_SIZE) > pSrcEnd))
        {
            goto app1Error;
        }

        while(len)
        {
            if((!GetTag(&tag, pSrc, pSrcOffsetBase, pSrcEnd, isLittleEndian)) ||
               (!CopyTagWorkByITN(pApp1, &tag, app1GpsITNTbl, sizeof(app1GpsITNTbl) / sizeof(*app1GpsITNTbl))))
            {
                goto app1Error;
            }
            pSrc += TIFF_FIELD_ENTRY_SIZE;
            len--;
        }
    }

    if(pCtx->decodeThumbnail)
    {
        if(pApp1[JPEG_DECODER_APP1_TAG_INDEX_THUMB_JPEG_INTERCHANGE_FORMAT].u.isFound &&
           pApp1[JPEG_DECODER_APP1_TAG_INDEX_THUMB_JPEG_INTERCHANGE_FORMAT_LENGTH].u.isFound)
        {
            i = GetLong(pApp1[JPEG_DECODER_APP1_TAG_INDEX_THUMB_JPEG_INTERCHANGE_FORMAT].pData, isLittleEndian);

            pCtx->srcOffset = (pSrcOffsetBase - pCtx->pSrc) + i;
            if((!i) || (i > 0xFFFF) || (app1Size <= i) || (pCtx->srcOffset <= i))
            {

                goto thumbnailError;
            }

            i = GetLong(pApp1[JPEG_DECODER_APP1_TAG_INDEX_THUMB_JPEG_INTERCHANGE_FORMAT_LENGTH].pData, isLittleEndian);

            if((!i) || (i > 0xFFFF) || (app1Size <= i) || ((pCtx->srcOffset + i) <= i) ||
               ((pCtx->srcOffset + i) >= pCtx->srcSize))
            {
                goto thumbnailError;
            }
            pCtx->srcSize = pCtx->srcOffset + i;
            pCtx->pSrcLimit = pCtx->pSrc + (pCtx->srcSize - 1);
        }
        else
        {

            goto thumbnailError;
        }
    }
    else
    {
    skip:

        pCtx->srcOffset += app1Size;
    }

    return true;

error:
    return false;

app1Error:
    JpegMpDecoderSetError(pCtx, JPEG_DECODER_ERROR_EXIF);
    goto error;

thumbnailError:
    JpegMpDecoderSetError(pCtx, JPEG_DECODER_ERROR_THUMBNAIL);
    goto error;
}

bool DecodeJpegApp2Mp(JpegMpDecoderContext* pCtx, size_t app2Size)
{
    JpegTagWorkObj tag;
    JpegTagWorkObj* pApp2 = NULL;
    MpIndex* pIndex = &pCtx->app2MpForAppUnion.index;
    MpAttribute* pAttr = &pCtx->app2MpForAppUnion.attr;
    const u8* pSrc = pCtx->pSrc + pCtx->srcOffset + 2;
    const u8* pSrcEnd = pCtx->pSrc + pCtx->srcOffset + app2Size;
    const u8* pSrcOffsetBase;
    const u8* pSrcMp;
    bool isLittleEndian = false;
    u32 len;
    u32 i;
    u32 attributeOffset;

    if((pCtx->srcOffset + app2Size) > pCtx->srcSize)
    {

        goto app2MpError;
    }

    memset(&pCtx->app2MpTagWorkUnion, 0, sizeof(pCtx->app2MpTagWorkUnion));
    memset(&pCtx->app2MpForAppUnion, 0, sizeof(pCtx->app2MpForAppUnion));

    if((pSrc + sizeof(App2MpFormatIdCode)) >= pSrcEnd ||
       memcmp(pSrc, App2MpFormatIdCode, sizeof(sizeof(App2MpFormatIdCode))))
    {

        return true;
    }
    pSrc += sizeof(App2MpFormatIdCode);
    pSrcOffsetBase = pSrc;

    if(pCtx->app2MpIndexFound || pCtx->app2MpAttributeFound)
    {
        goto app2MpError;
    }

    if(!pCtx->pApp2Mp)
    {

        pCtx->pApp2Mp = pCtx->pSrc + pCtx->srcOffset - 2;
    }

    if((pSrc + sizeof(App2MpEndianInfoLittleEndian) + TIFF_OFFSET_SIZE) >= pSrcEnd)
    {
        goto app2MpError;
    }

    if(!memcmp(pSrc, App2MpEndianInfoLittleEndian, sizeof(App2MpEndianInfoLittleEndian)))
    {
        isLittleEndian = true;
    }
    else if(!memcmp(pSrc, App2MpEndianInfoBigEndian, sizeof(App2MpEndianInfoBigEndian)))
    {
        isLittleEndian = false;
    }
    else
    {

        goto app2MpError;
    }

    i = GetLong(pSrc + sizeof(App2MpEndianInfoLittleEndian), isLittleEndian);

    if((i < 8) || (i > 0xFFFF))
    {
        goto app2MpError;
    }
    pSrc = pSrcOffsetBase + i;

    pSrcMp = pSrc;

    if((pSrc + TIFF_COUNT_SIZE) >= pSrcEnd)
    {
        goto app2MpError;
    }

    len = GetWord(pSrc, isLittleEndian);
    pSrc += TIFF_COUNT_SIZE;

    if((pSrc + len * TIFF_FIELD_ENTRY_SIZE + TIFF_OFFSET_SIZE) > pSrcEnd)
    {
        goto app2MpError;
    }

    pApp2 = pCtx->app2MpTagWorkUnion.indexIfd;
    while(len)
    {
        if((!GetTag(&tag, pSrc, pSrcOffsetBase, pSrcEnd, isLittleEndian)) ||
           (!CopyTagWorkByITN(pApp2, &tag, app2MpIndexIfdITNTbl,
                              sizeof(app2MpIndexIfdITNTbl) / sizeof(*app2MpIndexIfdITNTbl))))
        {
            goto app2MpError;
        }
        pSrc += TIFF_FIELD_ENTRY_SIZE;
        len--;
    }

    if(pApp2[JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_NUMBER_OF_IMAGES].u.isFound ||
       pApp2[JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_MP_ENTRY].u.isFound ||
       pApp2[JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_IMAGE_UID_LIST].u.isFound ||
       pApp2[JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_TOTAL_FRAMES].u.isFound)
    {

        if(pApp2[JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_MPF_VERSION].u.isFound &&
           pApp2[JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_MPF_VERSION].dataSize == sizeof(pIndex->mpfVersion) &&
           pApp2[JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_NUMBER_OF_IMAGES].u.isFound &&
           pApp2[JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_MP_ENTRY].u.isFound)
        {

            pCtx->app2MpIndexFound = true;
        }
        else
        {

            goto app2MpError;
        }

        if(!pCtx->decodeApp2MpAttribute)
        {

            pSrc = pCtx->pSrc;
            pIndex->src = pSrc;
            pIndex->srcSize = pCtx->srcSize;
            pIndex->offsetToBase = pSrcOffsetBase - pSrc;
            pIndex->offsetToMpIndexIfd = pSrcMp - pSrc;
            pIndex->isLittleEndian = isLittleEndian;

            pIndex->isMpfVersionValid = true;
            memcpy(pIndex->mpfVersion, pApp2[JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_MPF_VERSION].pData,
                   sizeof(pIndex->mpfVersion));

            pIndex->isNumberOfImagesValid = true;
            pIndex->numberOfImages =
                GetLong(pApp2[JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_NUMBER_OF_IMAGES].pData, isLittleEndian);

            pIndex->offsetToMpEntry = pApp2[JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_MP_ENTRY].pData - pSrc;
            pIndex->mpEntrySize = pApp2[JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_MP_ENTRY].dataSize;

            if(pApp2[JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_IMAGE_UID_LIST].u.isFound)
            {
                pIndex->offsetToImageUidList =
                    pApp2[JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_IMAGE_UID_LIST].pData - pSrc;
                pIndex->imageUidListSize = pApp2[JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_IMAGE_UID_LIST].dataSize;
            }

            if(pApp2[JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_TOTAL_FRAMES].u.isFound)
            {
                pIndex->isTotalFramesValid = true;
                pIndex->totalFrames =
                    GetLong(pApp2[JPEG_DECODER_APP2_MP_INDEX_IFD_TAG_INDEX_TOTAL_FRAMES].pData, isLittleEndian);
            }

            return true;
        }

        attributeOffset = GetLong(pSrc + len * TIFF_FIELD_ENTRY_SIZE, isLittleEndian);
        if(!attributeOffset)
        {

            return true;
        }

        pSrc = pSrcOffsetBase + attributeOffset;
    }
    else
    {

        pSrc = pSrcMp;
    }

    memset(&pCtx->app2MpTagWorkUnion, 0, sizeof(pCtx->app2MpTagWorkUnion));
    memset(&pCtx->app2MpForAppUnion, 0, sizeof(pCtx->app2MpForAppUnion));

    if((pSrc + TIFF_COUNT_SIZE) >= pSrcEnd)
    {
        goto app2MpError;
    }

    len = GetWord(pSrc, isLittleEndian);
    pSrc += TIFF_COUNT_SIZE;

    if((pSrc + len * TIFF_FIELD_ENTRY_SIZE + TIFF_OFFSET_SIZE) > pSrcEnd)
    {
        goto app2MpError;
    }

    pApp2 = pCtx->app2MpTagWorkUnion.attribute;
    while(len)
    {
        if((!GetTag(&tag, pSrc, pSrcOffsetBase, pSrcEnd, isLittleEndian)) ||
           (!CopyTagWorkByITN(pApp2, &tag, app2MpAttributeITNTbl,
                              sizeof(app2MpAttributeITNTbl) / sizeof(*app2MpAttributeITNTbl))))
        {
            goto app2MpError;
        }
        pSrc += TIFF_FIELD_ENTRY_SIZE;
        len--;
    }

    pCtx->app2MpAttributeFound = true;

    if(pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_MPF_VERSION].u.isFound &&
       pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_MPF_VERSION].dataSize == sizeof(pAttr->mpfVersion))
    {
        pAttr->isMpfVersionValid = true;
        memcpy(pAttr->mpfVersion, pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_MPF_VERSION].pData,
               sizeof(pAttr->mpfVersion));
    }

    if(pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_MP_INDIVIDUAL_NUM].u.isFound)
    {
        pAttr->isMpIndividualNumValid = true;
        pAttr->mpIndividualNum =
            GetLong(pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_MP_INDIVIDUAL_NUM].pData, isLittleEndian);
    }

    if(pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_PAN_ORIENTATION].u.isFound)
    {
        pAttr->isPanOrientationValid = true;
        pAttr->panOrientation =
            GetLong(pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_PAN_ORIENTATION].pData, isLittleEndian);
    }

    if(pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_PAN_OVERLAP_H].u.isFound)
    {
        pAttr->isPanOverlapHValid = true;
        GetRational(&pAttr->panOverlapH, pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_PAN_OVERLAP_H].pData,
                    isLittleEndian);
    }

    if(pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_PAN_OVERLAP_V].u.isFound)
    {
        pAttr->isPanOverlapVValid = true;
        GetRational(&pAttr->panOverlapV, pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_PAN_OVERLAP_V].pData,
                    isLittleEndian);
    }

    if(pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_BASE_VIEWPOINT_NUM].u.isFound)
    {
        pAttr->isBaseViewpointNumValid = true;
        pAttr->baseViewpointNum =
            GetLong(pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_BASE_VIEWPOINT_NUM].pData, isLittleEndian);
    }

    if(pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_CONVERGENCE_ANGLE].u.isFound)
    {
        pAttr->isConvergenceAngleValid = true;
        GetSrational(&pAttr->convergenceAngle, pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_CONVERGENCE_ANGLE].pData,
                     isLittleEndian);
    }

    if(pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_BASELINE_LENGTH].u.isFound)
    {
        pAttr->isBaselineLengthValid = true;
        GetRational(&pAttr->baselineLength, pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_BASELINE_LENGTH].pData,
                    isLittleEndian);
    }

    if(pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_VERTICAL_DIVERGENCE].u.isFound)
    {
        pAttr->isVerticalDivergenceValid = true;
        GetSrational(&pAttr->verticalDivergence,
                     pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_VERTICAL_DIVERGENCE].pData, isLittleEndian);
    }

    if(pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_AXIS_DISTANCE_X].u.isFound)
    {
        pAttr->isAxisDistanceXValid = true;
        GetSrational(&pAttr->axisDistanceX, pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_AXIS_DISTANCE_X].pData,
                     isLittleEndian);
    }

    if(pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_AXIS_DISTANCE_Y].u.isFound)
    {
        pAttr->isAxisDistanceYValid = true;
        GetSrational(&pAttr->axisDistanceY, pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_AXIS_DISTANCE_Y].pData,
                     isLittleEndian);
    }

    if(pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_AXIS_DISTANCE_Z].u.isFound)
    {
        pAttr->isAxisDistanceZValid = true;
        GetSrational(&pAttr->axisDistanceZ, pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_AXIS_DISTANCE_Z].pData,
                     isLittleEndian);
    }

    if(pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_YAW_ANGLE].u.isFound)
    {
        pAttr->isYawAngleValid = true;
        GetSrational(&pAttr->yawAngle, pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_YAW_ANGLE].pData, isLittleEndian);
    }

    if(pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_PITCH_ANGLE].u.isFound)
    {
        pAttr->isPitchAngleValid = true;
        GetSrational(&pAttr->pitchAngle, pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_PITCH_ANGLE].pData,
                     isLittleEndian);
    }

    if(pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_ROLL_ANGLE].u.isFound)
    {
        pAttr->isRollAngleValid = true;
        GetSrational(&pAttr->rollAngle, pApp2[JPEG_DECODER_APP2_MP_ATTRIBUTE_TAG_INDEX_ROLL_ANGLE].pData,
                     isLittleEndian);
    }

    return true;

error:
    return false;

app2MpError:
    JpegMpDecoderSetError(pCtx, JPEG_DECODER_ERROR_MP);
    goto error;
}

size_t GetApp1DateTime(char* pBuffer, const JpegMpDecoderContext* pCtx)
{
    const JpegTagWorkObj* pApp1 = pCtx->app1TagWork;

    if(pCtx->app1ExifFound && pApp1[JPEG_DECODER_APP1_TAG_INDEX_IFD0_DATE_TIME].u.isFound &&
       pApp1[JPEG_DECODER_APP1_TAG_INDEX_IFD0_DATE_TIME].dataSize == DATE_TIME_SIZE)
    {
        memcpy(pBuffer, pApp1[JPEG_DECODER_APP1_TAG_INDEX_IFD0_DATE_TIME].pData, DATE_TIME_SIZE);
        pBuffer[DATE_TIME_SIZE - 1] = '\0';

        return DATE_TIME_SIZE;
    }

    return 0;
}

const char* GetApp1DateTimePointer(const JpegMpDecoderContext* pCtx)
{
    const JpegTagWorkObj* pApp1 = pCtx->app1TagWork;

    if(pCtx->app1ExifFound && pApp1[JPEG_DECODER_APP1_TAG_INDEX_IFD0_DATE_TIME].u.isFound &&
       pApp1[JPEG_DECODER_APP1_TAG_INDEX_IFD0_DATE_TIME].dataSize == DATE_TIME_SIZE)
    {
        return reinterpret_cast<const char*>(pApp1[JPEG_DECODER_APP1_TAG_INDEX_IFD0_DATE_TIME].pData);
    }

    return NULL;
}

size_t GetApp1ImageUid(char* pBuffer, const JpegMpDecoderContext* pCtx)
{
    const JpegTagWorkObj* pApp1 = pCtx->app1TagWork;

    if(pCtx->app1ExifFound && pApp1[JPEG_DECODER_APP1_TAG_INDEX_EXIF_IMAGE_UNIQUE_ID].u.isFound &&
       pApp1[JPEG_DECODER_APP1_TAG_INDEX_EXIF_IMAGE_UNIQUE_ID].dataSize == IMAGE_UID_SIZE)
    {
        memcpy(pBuffer, pApp1[JPEG_DECODER_APP1_TAG_INDEX_EXIF_IMAGE_UNIQUE_ID].pData, IMAGE_UID_SIZE);
        pBuffer[IMAGE_UID_SIZE - 1] = '\0';

        return IMAGE_UID_SIZE;
    }

    return 0;
}

bool GetApp1SoftwarePointer(App1PointerAndSize* pBuffer, const JpegMpDecoderContext* pCtx)
{
    const JpegTagWorkObj* pApp1 = pCtx->app1TagWork;
    const char* p;
    size_t size;

    if(pCtx->app1ExifFound && pApp1[JPEG_DECODER_APP1_TAG_INDEX_IFD0_SOFTWARE].u.isFound)
    {
        p = reinterpret_cast<const char*>(pApp1[JPEG_DECODER_APP1_TAG_INDEX_IFD0_SOFTWARE].pData);
        size = pApp1[JPEG_DECODER_APP1_TAG_INDEX_IFD0_SOFTWARE].dataSize;
        if((size > 1) && (!p[size - 1]))
        {

            pBuffer->pointer = reinterpret_cast<const u8*>(p);
            pBuffer->size = size - 1;
            return true;
        }
    }

    return false;
}

bool GetApp1MakerNotePointer(App1PointerAndSize* pBuffer, const JpegMpDecoderContext* pCtx, u32 index)
{
    const JpegTagWorkObj* pApp1 = pCtx->app1TagWork;

    if(index < NUM_MAKER_NOTES)
    {
        index += JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_CTR;
        if(pCtx->app1ExifFound && pApp1[index].u.isFound)
        {
            pBuffer->pointer = pApp1[index].pData;
            pBuffer->size = pApp1[index].dataSize;
            if(pBuffer->size)
            {
                return true;
            }
        }
    }

    return false;
}

bool GetApp1TwlPhotoMakerNotePointer(App1PointerAndSize* pBuffer, const JpegMpDecoderContext* pCtx)
{
    const JpegTagWorkObj* pApp1 = pCtx->app1TagWork;

    if(pCtx->app1ExifFound && pApp1[JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_TWL_PHOTO].u.isFound)
    {
        pBuffer->pointer = pApp1[JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_TWL_PHOTO].pData;
        pBuffer->size = pApp1[JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_TWL_PHOTO].dataSize;
        if(pBuffer->size == TWL_PHOTO_MAKER_NOTE_SIZE)
        {
            return true;
        }
    }

    return false;
}

bool GetApp1TwlUserMakerNotePointer(App1PointerAndSize* pBuffer, const JpegMpDecoderContext* pCtx)
{
    const JpegTagWorkObj* pApp1 = pCtx->app1TagWork;

    if(pCtx->app1ExifFound && pApp1[JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_TWL_USER].u.isFound)
    {
        pBuffer->pointer = pApp1[JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_TWL_USER].pData;
        pBuffer->size = pApp1[JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_TWL_USER].dataSize;
        if(pBuffer->size)
        {

            if(pBuffer->size == 2)
            {
                pBuffer->pointer += 2;
            }
            return true;
        }
    }

    return false;
}

bool GetApp1Orientation(u16* pBuffer, const JpegMpDecoderContext* pCtx)
{
    const JpegTagWorkObj* pApp1 = pCtx->app1TagWork;

    if(pCtx->app1ExifFound && pApp1[JPEG_DECODER_APP1_TAG_INDEX_IFD0_ORIENTATION].u.isFound)
    {
        *pBuffer = GetWord(pApp1[JPEG_DECODER_APP1_TAG_INDEX_IFD0_ORIENTATION].pData, pCtx->isApp1LittleEndian);
        return true;
    }

    return false;
}

bool GetApp1GpsData(GpsData* pBuffer, const JpegMpDecoderContext* pCtx)
{
    const JpegTagWorkObj* pApp1 = pCtx->app1TagWork;

    const App1GpsCheckInfo* pCheckInfo = app1GpsCheckInfoTbl;
    u32 checkCount = sizeof(app1GpsCheckInfoTbl) / sizeof(*app1GpsCheckInfoTbl);

    if(pCtx->app1ExifFound && pApp1[JPEG_DECODER_APP1_TAG_INDEX_IFD0_GPS_IFD_POINTER].u.isFound)
    {
        memset(pBuffer, 0, sizeof(*pBuffer));

        while(checkCount)
        {
            if(pApp1[pCheckInfo->index].u.isFound)
            {
                if(pCheckInfo->count)
                {
                    if((pCheckInfo->count << ExifDataSizeShiftCountTbl[pApp1[pCheckInfo->index].type]) !=
                       pApp1[pCheckInfo->index].dataSize)
                    {

                        goto ignore;
                    }

                    if(pCheckInfo->index == JPEG_DECODER_APP1_TAG_INDEX_GPS_VERSION_ID)
                    {

                        pBuffer->isVersionIdValid = true;
                    }
                    if(pCheckInfo->valueOffset)
                    {
                        if(pCheckInfo->flagOffset)
                        {
                            *reinterpret_cast<bool*>(reinterpret_cast<u8*>(pBuffer) + pCheckInfo->flagOffset) = true;
                        }
                        CopyTagValue(reinterpret_cast<u8*>(pBuffer) + pCheckInfo->valueOffset,
                                     &pApp1[pCheckInfo->index], pCtx->isApp1LittleEndian);
                    }
                    else
                    {
                        if(pCheckInfo->flagOffset)
                        {
                            *reinterpret_cast<const u8**>(reinterpret_cast<u8*>(pBuffer) + pCheckInfo->flagOffset) =
                                pApp1[pCheckInfo->index].pData;
                        }
                        else
                        {

                            goto internalError;
                        }
                    }
                }
                else
                {

                    if(pCheckInfo->flagOffset)
                    {
                        *reinterpret_cast<const u8**>(reinterpret_cast<u8*>(pBuffer) + pCheckInfo->flagOffset) =
                            pApp1[pCheckInfo->index].pData;
                    }
                    else
                    {

                        goto internalError;
                    }
                    if(pCheckInfo->valueOffset)
                    {
                        *reinterpret_cast<size_t*>(reinterpret_cast<u8*>(pBuffer) + pCheckInfo->valueOffset) =
                            pApp1[pCheckInfo->index].dataSize;
                    }
                }
            }
        ignore:
            pCheckInfo++;
            checkCount--;
        }

        return true;
    }

error:
    return false;

internalError:

    goto error;
}

#if 0

const u8* GetApp1TwlPrivateDataPointer(const JpegMpDecoderContext* pCtx)
{
    const JpegTagWorkObj* pApp1 = pCtx->app1TagWork;

    if (pCtx->app1ExifFound &&
        pApp1[JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_TWL_PRIVATE].u.isFound &&
        (pApp1[JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_TWL_PRIVATE].dataSize == TWL_PRIVATE_DATA_SIZE))
    {
        return pApp1[JPEG_DECODER_APP1_TAG_INDEX_MAKERNOTE_TWL_PRIVATE].pData;
    }

    return NULL;
}
#endif

}
}
}
}
