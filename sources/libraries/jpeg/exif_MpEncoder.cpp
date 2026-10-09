// Filename: exif_MpEncoder.cpp
//
// Project: Horizon

#include <nn/jpeg/CTR/jpeg_MpEncoder.h>
#include "exif_MpEncoder.h"
#include <string.h>

namespace nn {
namespace jpeg {
namespace CTR {
namespace detail {

namespace
{

const char App1MakeString[] = "Nintendo";
const char App1DefaultModelString[] = "Nintendo 3DS";

const char App1TwlModelString[] = "NintendoDS";
const char App1TwlSoftwareString[] = "JANH";

const char App1InteroperabilityIndexString[] = "R98";
const char App1InteroperabilityRelatedImageFileFormat_2_2String[] = "JPEG Exif Ver 2.2";

const u8 App1TwlPrivateData[TWL_PRIVATE_DATA_SIZE] = {0};
const u8 App1ExifVersion_2_20Data[4] = {'0', '2', '2', '0'};
const u8 App1FlashpixVersionData[4] = {'0', '1', '0', '0'};
const u8 App1InteroperabilityVersionData[4] = {'0', '1', '0', '0'};

const u8 App1ComponentsConfigurationData[4] = {0x01, 0x02, 0x03, 0x00};

const u8 JpegFileHeaderTemplateLittleEndian[APP1_TIFF_OFFSET + APP1_TIFF_HEADER_SIZE] = {

    0xFF, 0xD8, 0xFF, 0xE1, 0x00, 0x00, 'E',  'x', 'i', 'f', 0x00, 0x00,

    'I',  'I',  0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
const u8 JpegFileHeaderTemplateBigEndian[APP1_TIFF_OFFSET + APP1_TIFF_HEADER_SIZE] = {

    0xFF, 0xD8, 0xFF, 0xE1, 0x00, 0x00, 'E',  'x', 'i', 'f', 0x00, 0x00,

    'M',  'M',  0x00, 0x2A, 0x00, 0x00, 0x00, 0x08};

const u16 App1ResolutionUnitData[1] = {2};

const u16 App1YCbCrPositioningData[1] = {2};

const u16 App1ColorSpaceData[1] = {1};

const u16 App1ThumbCompressionData[1] = {6};

const Rational App1XYResolutionData = {72, 1};
}

namespace
{

const u8 ExifDataSizeShiftCountTbl[NUM_EXIF_TAG_TYPES] = {

    0, 0, 0, 1, 2, 3, 0, 0, 0, 2, 3};

const u8 App1IfdSeparationTbl[NUM_APP1_IFDS] = {JPEG_ENCODER_APP1_TAG_INDEX_EXIF_LOOP_BEGIN,
                                                JPEG_ENCODER_APP1_TAG_INDEX_MAKERNOTE_LOOP_BEGIN,
                                                JPEG_ENCODER_APP1_TAG_INDEX_INTEROPERABILITY_LOOP_BEGIN,
                                                JPEG_ENCODER_APP1_TAG_INDEX_GPS_LOOP_BEGIN,
                                                JPEG_ENCODER_APP1_TAG_INDEX_THUMB_LOOP_BEGIN,
                                                JPEG_ENCODER_APP1_TAG_INDEX_THUMB_LOOP_END};
}

namespace
{

struct App1GpsCheckInfo
{
    u16 tag;
    u8 index;
    u8 type;
    u8 count;
    u8 flagOffset;
    u8 valueOffset;
    u8 padding;
};

const App1GpsCheckInfo app1GpsCheckInfoTbl[] = {

    {APP1_TAG_NUMBER_GPS_VERSION_ID, JPEG_ENCODER_APP1_TAG_INDEX_GPS_VERSION_ID, EXIF_TAG_TYPE_BYTE,
     GPS_VERSION_ID_SIZE, offsetof(GpsData, isVersionIdValid), offsetof(GpsData, versionId)},
    {APP1_TAG_NUMBER_GPS_LATITUDE_REF, JPEG_ENCODER_APP1_TAG_INDEX_GPS_LATITUDE_REF, EXIF_TAG_TYPE_ASCII, GPS_REF_SIZE,
     0, offsetof(GpsData, latitudeRef)},
    {APP1_TAG_NUMBER_GPS_LATITUDE, JPEG_ENCODER_APP1_TAG_INDEX_GPS_LATITUDE, EXIF_TAG_TYPE_RATIONAL,
     NUM_GPS_LATITUDE_RATIONALS, offsetof(GpsData, isLatitudeValid), offsetof(GpsData, latitude)},
    {APP1_TAG_NUMBER_GPS_LONGITUDE_REF, JPEG_ENCODER_APP1_TAG_INDEX_GPS_LONGITUDE_REF, EXIF_TAG_TYPE_ASCII,
     GPS_REF_SIZE, 0, offsetof(GpsData, longitudeRef)},
    {APP1_TAG_NUMBER_GPS_LONGITUDE, JPEG_ENCODER_APP1_TAG_INDEX_GPS_LONGITUDE, EXIF_TAG_TYPE_RATIONAL,
     NUM_GPS_LONGITUDE_RATIONALS, offsetof(GpsData, isLongitudeValid), offsetof(GpsData, longitude)},
    {APP1_TAG_NUMBER_GPS_ALTITUDE_REF, JPEG_ENCODER_APP1_TAG_INDEX_GPS_ALTITUDE_REF, EXIF_TAG_TYPE_BYTE, 1,
     offsetof(GpsData, isAltitudeRefValid), offsetof(GpsData, altitudeRef)},
    {APP1_TAG_NUMBER_GPS_ALTITUDE, JPEG_ENCODER_APP1_TAG_INDEX_GPS_ALTITUDE, EXIF_TAG_TYPE_RATIONAL, 1,
     offsetof(GpsData, isAltitudeValid), offsetof(GpsData, altitude)},
    {APP1_TAG_NUMBER_GPS_TIME_STAMP, JPEG_ENCODER_APP1_TAG_INDEX_GPS_TIME_STAMP, EXIF_TAG_TYPE_RATIONAL,
     NUM_GPS_TIME_STAMP_RATIONALS, offsetof(GpsData, isTimeStampValid), offsetof(GpsData, timeStamp)},
    {APP1_TAG_NUMBER_GPS_SATELLITES, JPEG_ENCODER_APP1_TAG_INDEX_GPS_SATELLITES, EXIF_TAG_TYPE_ASCII, 0,
     offsetof(GpsData, pSatellites), 0},
    {APP1_TAG_NUMBER_GPS_STATUS, JPEG_ENCODER_APP1_TAG_INDEX_GPS_STATUS, EXIF_TAG_TYPE_ASCII, GPS_REF_SIZE, 0,
     offsetof(GpsData, status)},
    {APP1_TAG_NUMBER_GPS_MEASURE_MODE, JPEG_ENCODER_APP1_TAG_INDEX_GPS_MEASURE_MODE, EXIF_TAG_TYPE_ASCII, GPS_REF_SIZE,
     0, offsetof(GpsData, measureMode)},
    {APP1_TAG_NUMBER_GPS_DOP, JPEG_ENCODER_APP1_TAG_INDEX_GPS_DOP, EXIF_TAG_TYPE_RATIONAL, 1,
     offsetof(GpsData, isDopValid), offsetof(GpsData, dop)},
    {APP1_TAG_NUMBER_GPS_SPEED_REF, JPEG_ENCODER_APP1_TAG_INDEX_GPS_SPEED_REF, EXIF_TAG_TYPE_ASCII, GPS_REF_SIZE, 0,
     offsetof(GpsData, speedRef)},
    {APP1_TAG_NUMBER_GPS_SPEED, JPEG_ENCODER_APP1_TAG_INDEX_GPS_SPEED, EXIF_TAG_TYPE_RATIONAL, 1,
     offsetof(GpsData, isSpeedValid), offsetof(GpsData, speed)},
    {APP1_TAG_NUMBER_GPS_TRACK_REF, JPEG_ENCODER_APP1_TAG_INDEX_GPS_TRACK_REF, EXIF_TAG_TYPE_ASCII, GPS_REF_SIZE, 0,
     offsetof(GpsData, trackRef)},
    {APP1_TAG_NUMBER_GPS_TRACK, JPEG_ENCODER_APP1_TAG_INDEX_GPS_TRACK, EXIF_TAG_TYPE_RATIONAL, 1,
     offsetof(GpsData, isTrackValid), offsetof(GpsData, track)},
    {APP1_TAG_NUMBER_GPS_IMG_DIRECTION_REF, JPEG_ENCODER_APP1_TAG_INDEX_GPS_IMG_DIRECTION_REF, EXIF_TAG_TYPE_ASCII,
     GPS_REF_SIZE, 0, offsetof(GpsData, imgDirectionRef)},
    {APP1_TAG_NUMBER_GPS_IMG_DIRECTION, JPEG_ENCODER_APP1_TAG_INDEX_GPS_IMG_DIRECTION, EXIF_TAG_TYPE_RATIONAL, 1,
     offsetof(GpsData, isImgDirectionValid), offsetof(GpsData, imgDirection)},
    {APP1_TAG_NUMBER_GPS_MAP_DATUM, JPEG_ENCODER_APP1_TAG_INDEX_GPS_MAP_DATUM, EXIF_TAG_TYPE_ASCII, 0,
     offsetof(GpsData, pMapDatum), 0},
    {APP1_TAG_NUMBER_GPS_DEST_LATITUDE_REF, JPEG_ENCODER_APP1_TAG_INDEX_GPS_DEST_LATITUDE_REF, EXIF_TAG_TYPE_ASCII,
     GPS_REF_SIZE, 0, offsetof(GpsData, destLatitudeRef)},
    {APP1_TAG_NUMBER_GPS_DEST_LATITUDE, JPEG_ENCODER_APP1_TAG_INDEX_GPS_DEST_LATITUDE, EXIF_TAG_TYPE_RATIONAL,
     NUM_GPS_LATITUDE_RATIONALS, offsetof(GpsData, isDestLatitudeValid), offsetof(GpsData, destLatitude)},
    {APP1_TAG_NUMBER_GPS_DEST_LONGITUDE_REF, JPEG_ENCODER_APP1_TAG_INDEX_GPS_DEST_LONGITUDE_REF, EXIF_TAG_TYPE_ASCII,
     GPS_REF_SIZE, 0, offsetof(GpsData, destLongitudeRef)},
    {APP1_TAG_NUMBER_GPS_DEST_LONGITUDE, JPEG_ENCODER_APP1_TAG_INDEX_GPS_DEST_LONGITUDE, EXIF_TAG_TYPE_RATIONAL,
     NUM_GPS_LONGITUDE_RATIONALS, offsetof(GpsData, isDestLongitudeValid), offsetof(GpsData, destLongitude)},
    {APP1_TAG_NUMBER_GPS_DEST_BEARING_REF, JPEG_ENCODER_APP1_TAG_INDEX_GPS_DEST_BEARING_REF, EXIF_TAG_TYPE_ASCII,
     GPS_REF_SIZE, 0, offsetof(GpsData, destBearingRef)},
    {APP1_TAG_NUMBER_GPS_DEST_BEARING, JPEG_ENCODER_APP1_TAG_INDEX_GPS_DEST_BEARING, EXIF_TAG_TYPE_RATIONAL, 1,
     offsetof(GpsData, isDestBearingValid), offsetof(GpsData, destBearing)},
    {APP1_TAG_NUMBER_GPS_DEST_DISTANCE_REF, JPEG_ENCODER_APP1_TAG_INDEX_GPS_DEST_DISTANCE_REF, EXIF_TAG_TYPE_ASCII,
     GPS_REF_SIZE, 0, offsetof(GpsData, destDistanceRef)},
    {APP1_TAG_NUMBER_GPS_DEST_DISTANCE, JPEG_ENCODER_APP1_TAG_INDEX_GPS_DEST_DISTANCE, EXIF_TAG_TYPE_RATIONAL, 1,
     offsetof(GpsData, isDestDistanceValid), offsetof(GpsData, destDistance)},
    {APP1_TAG_NUMBER_GPS_PROCESSING_METHOD, JPEG_ENCODER_APP1_TAG_INDEX_GPS_PROCESSING_METHOD, EXIF_TAG_TYPE_UNDEFINED,
     0, offsetof(GpsData, pProcessingMethod), offsetof(GpsData, processingMethodSize)},
    {APP1_TAG_NUMBER_GPS_AREA_INFORMATION, JPEG_ENCODER_APP1_TAG_INDEX_GPS_AREA_INFORMATION, EXIF_TAG_TYPE_UNDEFINED, 0,
     offsetof(GpsData, pAreaInformation), offsetof(GpsData, areaInformationSize)},
    {APP1_TAG_NUMBER_GPS_DATE_STAMP, JPEG_ENCODER_APP1_TAG_INDEX_GPS_DATE_STAMP, EXIF_TAG_TYPE_ASCII,
     GPS_DATE_STAMP_SIZE, offsetof(GpsData, pDateStamp), 0},
    {APP1_TAG_NUMBER_GPS_DIFFERENTIAL, JPEG_ENCODER_APP1_TAG_INDEX_GPS_DIFFERENTIAL, EXIF_TAG_TYPE_SHORT, 1,
     offsetof(GpsData, isDifferentialValid), offsetof(GpsData, differential)}};
}

namespace
{
inline void SetJpegMpEncoderError(JpegMpEncoderContext* pCtx, s8 errorCode)
{
    if(!pCtx->errorCode)
    {

        pCtx->errorCode = errorCode;
    }
}
}

void InitializeJpegMpEncoderApp1TagWork(detail::JpegMpEncoderWorkObj* pWork)
{
    JpegTagWorkObj* pApp1 = pWork->app1TagWork;

    memset(pWork->app1TagWork, 0, sizeof(pWork->app1TagWork));

    InitializeTagDataAscii(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_IFD0_MAKE], APP1_TAG_NUMBER_IFD0_MAKE, App1MakeString,
                           sizeof(App1MakeString));
    InitializeTagDataAscii(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_IFD0_MODEL], APP1_TAG_NUMBER_IFD0_MODEL,
                           App1DefaultModelString, sizeof(App1DefaultModelString));
    InitializeTagDataRational(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_IFD0_X_RESOLUTION], APP1_TAG_NUMBER_IFD0_X_RESOLUTION,
                              &App1XYResolutionData);
    InitializeTagDataRational(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_IFD0_Y_RESOLUTION], APP1_TAG_NUMBER_IFD0_Y_RESOLUTION,
                              &App1XYResolutionData);
    InitializeTagDataShort(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_IFD0_RESOLUTION_UNIT],
                           APP1_TAG_NUMBER_IFD0_RESOLUTION_UNIT, App1ResolutionUnitData);

    InitializeTagDataShort(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_IFD0_YCBCR_POSITIONING],
                           APP1_TAG_NUMBER_IFD0_YCBCR_POSITIONING, App1YCbCrPositioningData);
    InitializeTagDataLong(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_IFD0_EXIF_IFD_POINTER],
                          APP1_TAG_NUMBER_IFD0_EXIF_IFD_POINTER, pWork->app1ExifIfdPointerData);

    InitializeTagDataUndefined(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_EXIF_VERSION], APP1_TAG_NUMBER_EXIF_VERSION,
                               App1ExifVersion_2_20Data, sizeof(App1ExifVersion_2_20Data));
    InitializeTagDataUndefined(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_EXIF_COMPONENTS_CONFIGURATION],
                               APP1_TAG_NUMBER_EXIF_COMPONENTS_CONFIGURATION, App1ComponentsConfigurationData,
                               sizeof(App1ComponentsConfigurationData));

    InitializeTagDataUndefined(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_EXIF_FLASHPIX_VERSION],
                               APP1_TAG_NUMBER_EXIF_FLASHPIX_VERSION, App1FlashpixVersionData,
                               sizeof(App1FlashpixVersionData));
    InitializeTagDataShort(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_EXIF_COLOR_SPACE], APP1_TAG_NUMBER_EXIF_COLOR_SPACE,
                           App1ColorSpaceData);

    InitializeTagDataLong(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_EXIF_PIXEL_X_DIMENSION],
                          APP1_TAG_NUMBER_EXIF_PIXEL_X_DIMENSION, pWork->app1PixelXDimensionData);
    InitializeTagDataLong(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_EXIF_PIXEL_Y_DIMENSION],
                          APP1_TAG_NUMBER_EXIF_PIXEL_Y_DIMENSION, pWork->app1PixelYDimensionData);
    InitializeTagDataLong(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_EXIF_INTEROPERABILITY_IFD_POINTER],
                          APP1_TAG_NUMBER_EXIF_INTEROPERABILITY_IFD_POINTER, pWork->app1InteroperabilityIfdPointerData);

    InitializeTagDataAscii(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_INTEROPERABILITY_INDEX],
                           APP1_TAG_NUMBER_INTEROPERABILITY_INDEX, App1InteroperabilityIndexString,
                           sizeof(App1InteroperabilityIndexString));
    InitializeTagDataUndefined(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_INTEROPERABILITY_VERSION],
                               APP1_TAG_NUMBER_INTEROPERABILITY_VERSION, App1InteroperabilityVersionData,
                               sizeof(App1InteroperabilityVersionData));
    InitializeTagDataAscii(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_INTEROPERABILITY_RELATED_IMAGE_FILE_FORMAT],
                           APP1_TAG_NUMBER_INTEROPERABILITY_RELATED_IMAGE_FILE_FORMAT,
                           App1InteroperabilityRelatedImageFileFormat_2_2String,
                           sizeof(App1InteroperabilityRelatedImageFileFormat_2_2String));

    InitializeTagDataShort(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_THUMB_COMPRESSION], APP1_TAG_NUMBER_THUMB_COMPRESSION,
                           App1ThumbCompressionData);
    InitializeTagDataRational(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_THUMB_X_RESOLUTION],
                              APP1_TAG_NUMBER_THUMB_X_RESOLUTION, &App1XYResolutionData);
    InitializeTagDataRational(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_THUMB_Y_RESOLUTION],
                              APP1_TAG_NUMBER_THUMB_Y_RESOLUTION, &App1XYResolutionData);
    InitializeTagDataShort(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_THUMB_RESOLUTION_UNIT],
                           APP1_TAG_NUMBER_THUMB_RESOLUTION_UNIT, App1ResolutionUnitData);
    InitializeTagDataLong(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_THUMB_JPEG_INTERCHANGE_FORMAT],
                          APP1_TAG_NUMBER_THUMB_JPEG_INTERCHANGE_FORMAT, pWork->app1JpegInterchangeFormatData);
    InitializeTagDataLong(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_THUMB_JPEG_INTERCHANGE_FORMAT_LENGTH],
                          APP1_TAG_NUMBER_THUMB_JPEG_INTERCHANGE_FORMAT_LENGTH,
                          pWork->app1JpegInterchangeFormatLengthData);
}

size_t CalcSegmentSizeCommon(JpegTagWorkObj* pTag, JpegMpEncoderIfdWorkObj* pIfd, const u8* pIfdSeparationTbl,
                             u32 numIfd, u32 tagIndexBegin, u32 tagIndexEnd)
{
    size_t size;
    size_t totalSize = 0;
    size_t lastSize = 0;
    u32 entryCount = 0;
    u32 ifdIndex = 0;
    u32 tagIndex = tagIndexBegin;
    u32 tagIndexSeparation = pIfdSeparationTbl[0];

    memset(pIfd, 0, numIfd * sizeof(*pIfd));

    pTag += tagIndex;

    while(true)
    {
        if(tagIndex == tagIndexSeparation)
        {
            pIfd[ifdIndex].count = static_cast<u16>(entryCount);
            if(entryCount)
            {

                totalSize += TIFF_COUNT_AND_OFFSET_SIZE;
                entryCount = 0;
            }
            pIfd[ifdIndex].offset = lastSize;
            pIfd[ifdIndex++].size = totalSize - lastSize;
            lastSize = totalSize;
            tagIndexSeparation = pIfdSeparationTbl[ifdIndex];
        }

        if(tagIndex == tagIndexEnd)
        {
            break;
        }

        size = pTag->dataSize;
        if(size)
        {

            totalSize += TIFF_FIELD_ENTRY_SIZE;

            if(!pTag->u.isMakerNoteSpecial && (size > 4))
            {

                if(size & 1)
                {
                    size++;
                }
                totalSize += size;
            }
            entryCount++;
        }
        tagIndex++;
        pTag++;
    }

    return totalSize;
}

size_t EncodeSegmentCommon(u8* pDst, const u8* pDstTiffOffsetBase, bool isLittleEndian, const JpegTagWorkObj* pTag,
                           const JpegMpEncoderIfdWorkObj* pIfd, const u8* pIfdSeparationTbl, u32 tagIndexBegin,
                           u32 tagIndexEnd, detail::JpegMpEncoderWorkObj* pWork)
{
    u8* pDstTop = pDst;
    u8* pDstValue;
    u8* pDstValueEnd;
    u32 count;
    size_t size;
    u32 type;
    const u8* pData;
    u32 ifdIndex = 0;
    u32 tagIndex = tagIndexBegin;
    u32 tagIndexSeparation = pIfdSeparationTbl[0];

    pTag += tagIndex;
    pWork->pLastTwlPrivateDataBuffer = NULL;

    while(tagIndex < tagIndexEnd)
    {
        count = pIfd[ifdIndex].count;

        if(count)
        {

            pDstValue = pDst + count * TIFF_FIELD_ENTRY_SIZE + TIFF_COUNT_AND_OFFSET_SIZE;

            EncodeTiffDataShort(pDst, count, isLittleEndian);
            pDst += 2;
        }
        else
        {
            while(tagIndex != tagIndexSeparation)
            {
                tagIndex++;
                pTag++;
            }
            goto skipIfd;
        }

        while(tagIndex != tagIndexSeparation)
        {
            size = pTag->dataSize;

            if(size)
            {
                type = pTag->type;
                pData = pTag->pData;

                EncodeTiffDataShort(pDst, pTag->tag, isLittleEndian);
                EncodeTiffDataShort(pDst + 2, type, isLittleEndian);
                EncodeTiffDataLong(pDst + 4, size >> ExifDataSizeShiftCountTbl[type], isLittleEndian);

                if(size <= 4)
                {
                    if(JPEG_ENCODER_EXIF_ENDIAN_IS isLittleEndian)
                    {
                    sameEndianSmallData:

                        pDst[9] = pDst[10] = pDst[11] = 0;
                        do
                        {
                            size--;
                            pDst[8 + size] = pData[size];
                        } while(size);
                    }
                    else
                    {

                        switch(type)
                        {
                        case EXIF_TAG_TYPE_SHORT:
                            pDst[8] = pData[1];
                            pDst[9] = pData[0];
                            if(size == 4)
                            {
                                pDst[10] = pData[3];
                                pDst[11] = pData[2];
                            }
                            else
                            {
                                pDst[10] = pDst[11] = 0;
                            }
                            break;

                        case EXIF_TAG_TYPE_LONG:
                        case EXIF_TAG_TYPE_SLONG:
                            EncodeTiffDataLong(pDst + 8, *reinterpret_cast<const u32*>(pData),
                                               JPEG_ENCODER_EXIF_ENDIAN_IS false);
                            break;

                        default:
                            goto sameEndianSmallData;
                        }
                    }
                }
                else
                {

                    EncodeTiffDataLong(pDst + 8, pDstValue - pDstTiffOffsetBase, isLittleEndian);
                    pDstValueEnd = pDstValue + size;

                    if(pTag->u.isMakerNoteSpecial)
                    {

                        goto skipCopyData;
                    }

                    if(JPEG_ENCODER_EXIF_ENDIAN_IS isLittleEndian)
                    {

                    sameEndianLargeData:

                        if((!pWork->useMpFormat) && (tagIndex == JPEG_ENCODER_APP1_TAG_INDEX_MAKERNOTE_TWL_PRIVATE))
                        {
                            pWork->pLastTwlPrivateDataBuffer = pDstValue;
                        }

                        do
                        {
                            *pDstValue++ = *pData++;
                        } while(pDstValue < pDstValueEnd);

                        if(size & 1)
                        {
                            *pDstValue++ = 0;
                        }
                    }
                    else
                    {

                        switch(type)
                        {
                        case EXIF_TAG_TYPE_SHORT:
                            do
                            {
                                EncodeTiffDataShort(pDstValue, *reinterpret_cast<const u16*>(pData),
                                                    JPEG_ENCODER_EXIF_ENDIAN_IS false);
                                pDstValue += 2;
                                pData += 2;
                            } while(pDstValue < pDstValueEnd);
                            break;

                        case EXIF_TAG_TYPE_LONG:
                        case EXIF_TAG_TYPE_SLONG:
                        case EXIF_TAG_TYPE_RATIONAL:
                        case EXIF_TAG_TYPE_SRATIONAL:
                            do
                            {
                                EncodeTiffDataLong(pDstValue, *reinterpret_cast<const u32*>(pData),
                                                   JPEG_ENCODER_EXIF_ENDIAN_IS false);
                                pDstValue += 4;
                                pData += 4;
                            } while(pDstValue < pDstValueEnd);
                            break;

                        default:
                            goto sameEndianLargeData;
                        }
                    }
                }
            skipCopyData:
                pDst += TIFF_FIELD_ENTRY_SIZE;
            }
            tagIndex++;
            pTag++;
        }

        EncodeTiffDataLong(pDst, pIfd[ifdIndex].offsetToNextIfd, isLittleEndian);

        pDst = pDstValue;

    skipIfd:
        ifdIndex++;
        tagIndexSeparation = pIfdSeparationTbl[ifdIndex];
    }

    return pDst - pDstTop;
}

size_t CalcJpegMpEncoderApp1Size(detail::JpegMpEncoderWorkObj* pWork)
{

    return sizeof(JpegFileHeaderTemplateLittleEndian) - APP1_LENGTH_OFFSET +
           CalcSegmentSizeCommon(pWork->app1TagWork, pWork->app1IfdWork, App1IfdSeparationTbl, NUM_APP1_IFDS,
                                 JPEG_ENCODER_APP1_TAG_INDEX_IFD0_LOOP_BEGIN,
                                 pWork->addThumbnail ? JPEG_ENCODER_APP1_TAG_INDEX_THUMB_LOOP_END
                                                     : JPEG_ENCODER_APP1_TAG_INDEX_THUMB_LOOP_BEGIN);
}

size_t EncodeJpegApp1Common(u8* pDst, detail::JpegMpEncoderWorkObj* pWork)
{
    u8* pDstTop = pDst;
    size_t size;

    size = sizeof(JpegFileHeaderTemplateLittleEndian);
    memcpy(pDst, pWork->isTiffLittleEndian ? JpegFileHeaderTemplateLittleEndian : JpegFileHeaderTemplateBigEndian,
           size);
    pDst += size;

    size = EncodeSegmentCommon(pDst, pDstTop + APP1_TIFF_OFFSET, pWork->isTiffLittleEndian, pWork->app1TagWork,
                               pWork->app1IfdWork, App1IfdSeparationTbl, JPEG_ENCODER_APP1_TAG_INDEX_IFD0_LOOP_BEGIN,
                               pWork->addThumbnail ? JPEG_ENCODER_APP1_TAG_INDEX_THUMB_LOOP_END
                                                   : JPEG_ENCODER_APP1_TAG_INDEX_THUMB_LOOP_BEGIN,
                               pWork);

    pDst += size;

    if(pWork->addThumbnail)
    {

        pDst += pWork->app1JpegInterchangeFormatLengthData[0];
    }

    return pDst - pDstTop;
}

bool PreEncodeJpegApp1GpsData(JpegTagWorkObj* pApp1, const GpsData* pGps)
{

    const App1GpsCheckInfo* pCheckInfo = app1GpsCheckInfoTbl;
    u32 checkCount = sizeof(app1GpsCheckInfoTbl) / sizeof(*app1GpsCheckInfoTbl);
    const char* pString;
    size_t stringLength;
    const u8* pData;
    size_t dataSize;
    bool result = false;

    while(checkCount)
    {
        switch(pCheckInfo->type)
        {
        case EXIF_TAG_TYPE_BYTE:
        case EXIF_TAG_TYPE_SHORT:
        case EXIF_TAG_TYPE_RATIONAL:

            if(*reinterpret_cast<const bool*>(reinterpret_cast<const u8*>(pGps) + pCheckInfo->flagOffset))
            {
                switch(pCheckInfo->type)
                {
                case EXIF_TAG_TYPE_BYTE:
                    InitializeTagDataByteArray(&pApp1[pCheckInfo->index], pCheckInfo->tag,
                                               reinterpret_cast<const u8*>(pGps) + pCheckInfo->valueOffset,
                                               pCheckInfo->count);
                    break;

                case EXIF_TAG_TYPE_SHORT:
                    InitializeTagDataShortArray(
                        &pApp1[pCheckInfo->index], pCheckInfo->tag,
                        reinterpret_cast<const u16*>(reinterpret_cast<const u8*>(pGps) + pCheckInfo->valueOffset),
                        pCheckInfo->count);
                    break;

                case EXIF_TAG_TYPE_RATIONAL:
                    InitializeTagDataRationalArray(
                        &pApp1[pCheckInfo->index], pCheckInfo->tag,
                        reinterpret_cast<const Rational*>(reinterpret_cast<const u8*>(pGps) + pCheckInfo->valueOffset),
                        pCheckInfo->count);
                    break;

                default:

                    NN_ASSERT_(false);
                    goto error;
                }
                result = true;
            }
            break;

        case EXIF_TAG_TYPE_ASCII:

            if(pCheckInfo->valueOffset)
            {
                pString = reinterpret_cast<const char*>(pGps) + pCheckInfo->valueOffset;
                if(!pString[0])
                {

                    break;
                }
            }
            else
            {
                pString =
                    *reinterpret_cast<const char* const*>(reinterpret_cast<const u8*>(pGps) + pCheckInfo->flagOffset);
                if(!pString)
                {

                    break;
                }
            }

            stringLength = strlen(pString);

            if(stringLength && (stringLength < 0x10000))
            {
                if((!pCheckInfo->count) || (pCheckInfo->count == (stringLength + 1)))
                {
                    InitializeTagDataAscii(&pApp1[pCheckInfo->index], pCheckInfo->tag, pString, stringLength + 1);
                    result = true;
                }
            }
            break;

        case EXIF_TAG_TYPE_UNDEFINED:

            pData = *reinterpret_cast<const u8* const*>(reinterpret_cast<const u8*>(pGps) + pCheckInfo->flagOffset);
            dataSize = *reinterpret_cast<const size_t*>(reinterpret_cast<const u8*>(pGps) + pCheckInfo->valueOffset);
            if(pData && dataSize)
            {
                InitializeTagDataUndefined(&pApp1[pCheckInfo->index], pCheckInfo->tag, pData, dataSize);
                result = true;
            }
            break;

        case EXIF_TAG_TYPE_LONG:
        case EXIF_TAG_TYPE_SLONG:
        case EXIF_TAG_TYPE_SRATIONAL:
        default:

            NN_ASSERT_(false);
            goto error;
        }
        pCheckInfo++;
        checkCount--;
    }

    return result;

error:

    return false;
}

void PreEncodeJpegApp1(detail::JpegMpEncoderWorkObj* pWork, detail::JpegMpEncoderTemporarySettingObj* pTempSetting,
                       bool addThumbnail)
{
    JpegTagWorkObj* pApp1 = pWork->app1TagWork;
    u32 index;
    bool isMakerNoteUsed = false;
    size_t stringLength;
    bool isApp1Used = false;

    if((!addThumbnail) && (pTempSetting->option & JPEG_ENCODER_OPTION_OMIT_APP1))
    {
        memset(pWork->app1TagWork, 0, sizeof(pWork->app1TagWork));
        return;
    }

    if(!pTempSetting->isDateTimeSet)
    {

        JpegMpEncoder::GetDateTimeNow(pTempSetting->dateTimeBuffer);

        pTempSetting->isDateTimeSet = true;
    }

    InitializeTagDataAscii(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_IFD0_DATE_TIME], APP1_TAG_NUMBER_IFD0_DATE_TIME,
                           pTempSetting->dateTimeBuffer, sizeof(pTempSetting->dateTimeBuffer));
    InitializeTagDataAscii(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_EXIF_DATE_TIME_ORIGINAL],
                           APP1_TAG_NUMBER_EXIF_DATE_TIME_ORIGINAL, pTempSetting->dateTimeBuffer,
                           sizeof(pTempSetting->dateTimeBuffer));
    InitializeTagDataAscii(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_EXIF_DATE_TIME_DIGITIZED],
                           APP1_TAG_NUMBER_EXIF_DATE_TIME_DIGITIZED, pTempSetting->dateTimeBuffer,
                           sizeof(pTempSetting->dateTimeBuffer));

    if(pTempSetting->pTwlPhotoMakerNoteData)
    {

        isMakerNoteUsed = true;
        InitializeTagDataAscii(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_IFD0_MODEL], APP1_TAG_NUMBER_IFD0_MODEL,
                               App1TwlModelString, sizeof(App1TwlModelString));
        InitializeTagDataAscii(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_IFD0_SOFTWARE], APP1_TAG_NUMBER_IFD0_SOFTWARE,
                               App1TwlSoftwareString, sizeof(App1TwlSoftwareString));

        InitializeTagDataUndefined(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_MAKERNOTE_TWL_PRIVATE],
                                   APP1_TAG_NUMBER_MAKERNOTE_TWL_PRIVATE, App1TwlPrivateData,
                                   sizeof(App1TwlPrivateData));
        InitializeTagDataUndefined(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_MAKERNOTE_TWL_PHOTO],
                                   APP1_TAG_NUMBER_MAKERNOTE_TWL_PHOTO, pTempSetting->pTwlPhotoMakerNoteData,
                                   TWL_PHOTO_MAKER_NOTE_SIZE);
    }
    else
    {
        if(pTempSetting->pSoftware)
        {
            stringLength = strlen(pTempSetting->pSoftware);

            if(stringLength && (stringLength < 0x10000))
            {

                InitializeTagDataAscii(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_IFD0_SOFTWARE], APP1_TAG_NUMBER_IFD0_SOFTWARE,
                                       pTempSetting->pSoftware, stringLength + 1);
            }
        }
    }

    for(index = MAKER_NOTE_INDEX_USER; index < NUM_MAKER_NOTES; index++)
    {
        if(pTempSetting->makerNotes[index].size)
        {
            isMakerNoteUsed = true;
            InitializeTagDataUndefined(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_MAKERNOTE_CTR + index],
                                       APP1_TAG_NUMBER_MAKERNOTE_CTR + index, pTempSetting->makerNotes[index].pData,
                                       pTempSetting->makerNotes[index].size);
        }
    }

    if(isMakerNoteUsed)
    {

        InitializeTagDataUndefined(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_EXIF_MAKERNOTE], APP1_TAG_NUMBER_EXIF_MAKERNOTE,
                                   NULL, 1);

        pApp1[JPEG_ENCODER_APP1_TAG_INDEX_EXIF_MAKERNOTE].u.isMakerNoteSpecial = true;
    }

    if(pTempSetting->isImageUidSet)
    {
        InitializeTagDataAscii(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_EXIF_IMAGE_UNIQUE_ID],
                               APP1_TAG_NUMBER_EXIF_IMAGE_UNIQUE_ID, pTempSetting->imageUidBuffer,
                               sizeof(pTempSetting->imageUidBuffer));
    }

    if(pTempSetting->isOrientationSet)
    {
        InitializeTagDataShort(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_IFD0_ORIENTATION], APP1_TAG_NUMBER_IFD0_ORIENTATION,
                               &pTempSetting->orientation);
    }

    if(pTempSetting->pGpsData)
    {
        if(PreEncodeJpegApp1GpsData(pApp1, pTempSetting->pGpsData))
        {
            InitializeTagDataLong(&pApp1[JPEG_ENCODER_APP1_TAG_INDEX_IFD0_GPS_IFD_POINTER],
                                  APP1_TAG_NUMBER_IFD0_GPS_IFD_POINTER, pWork->app1GpsIfdPointerData);
        }
    }

    if(pWork->useMpFormat && ((pWork->app2.typeCode == MP_TYPE_CODE_LARGE_THUMBNAIL_IMAGE_CLASS_1) ||
                              (pWork->app2.typeCode == MP_TYPE_CODE_LARGE_THUMBNAIL_IMAGE_CLASS_2)))
    {

        for(index = 0; index < NUM_JPEG_ENCODER_APP1_TAGS; index++)
        {

            if(((JPEG_ENCODER_APP1_TAG_INDEX_MAKERNOTE_LOOP_BEGIN <= index) &&
                (index < JPEG_ENCODER_APP1_TAG_INDEX_MAKERNOTE_LOOP_END)) ||
               (index == JPEG_ENCODER_APP1_TAG_INDEX_EXIF_MAKERNOTE) ||
               (index == JPEG_ENCODER_APP1_TAG_INDEX_EXIF_IMAGE_UNIQUE_ID))
            {
                if(pApp1[index].dataSize)
                {
                    isApp1Used = true;
                }
                continue;
            }
            else if(index == JPEG_ENCODER_APP1_TAG_INDEX_IFD0_EXIF_IFD_POINTER)
            {

                continue;
            }
            else if((index == JPEG_ENCODER_APP1_TAG_INDEX_THUMB_JPEG_INTERCHANGE_FORMAT) ||
                    (index == JPEG_ENCODER_APP1_TAG_INDEX_THUMB_JPEG_INTERCHANGE_FORMAT_LENGTH))
            {

                if(pWork->addThumbnail)
                {
                    isApp1Used = true;
                }
                continue;
            }
            else if((index == JPEG_ENCODER_APP1_TAG_INDEX_EXIF_PIXEL_X_DIMENSION) ||
                    (index == JPEG_ENCODER_APP1_TAG_INDEX_EXIF_PIXEL_Y_DIMENSION))
            {

                if(!pWork->app2.omitPixelDimensions)
                {
                    isApp1Used = true;
                    continue;
                }
            }

            pApp1[index].dataSize = 0;
        }

        if(!isApp1Used)
        {
            pApp1[JPEG_ENCODER_APP1_TAG_INDEX_IFD0_EXIF_IFD_POINTER].dataSize = 0;
        }
    }
}

bool EncodeJpegApp1(detail::JpegMpEncoderWorkObj* pWork)
{
    JpegMpEncoderContext* pCtx = &pWork->ctx;
    size_t app1Size;
    size_t totalSize;
    JpegTagWorkObj* pApp1 = pWork->app1TagWork;
    u8* pDst = pCtx->pDst + pCtx->dstOffset;

    if(pWork->addThumbnail)
    {

        app1Size = pWork->app1SizeTmp + pWork->app1JpegInterchangeFormatLengthData[0];

        pWork->app1JpegInterchangeFormatData[0] =
            pWork->app1SizeTmp - (sizeof(JpegFileHeaderTemplateLittleEndian) - APP1_TIFF_OFFSET);

        pWork->app1IfdWork[APP1_IFD_INDEX_IFD0].offsetToNextIfd =
            pWork->app1IfdWork[APP1_IFD_INDEX_THUMB].offset + APP1_TIFF_HEADER_SIZE;
    }
    else
    {
        app1Size = CalcJpegMpEncoderApp1Size(pWork);
        if(app1Size == (sizeof(JpegFileHeaderTemplateLittleEndian) - APP1_LENGTH_OFFSET))
        {

            totalSize = 2;
            if((pCtx->dstOffset + totalSize) >= pCtx->writeLimit)
            {
                goto insufficientOutputBufferError;
            }

            pDst[0] = 0xFF;
            pDst[1] = 0xD8;
            goto app1Done;
        }
    }

    pApp1[JPEG_ENCODER_APP1_TAG_INDEX_EXIF_MAKERNOTE].dataSize = pWork->app1IfdWork[APP1_IFD_INDEX_MAKERNOTE].size;

    pWork->app1ExifIfdPointerData[0] = pWork->app1IfdWork[APP1_IFD_INDEX_EXIF].offset + APP1_TIFF_HEADER_SIZE;
    pWork->app1InteroperabilityIfdPointerData[0] =
        pWork->app1IfdWork[APP1_IFD_INDEX_INTEROPERABILITY].offset + APP1_TIFF_HEADER_SIZE;
    pWork->app1GpsIfdPointerData[0] = pWork->app1IfdWork[APP1_IFD_INDEX_GPS].offset + APP1_TIFF_HEADER_SIZE;

    pWork->app1PixelXDimensionData[0] = pCtx->width;
    pWork->app1PixelYDimensionData[0] = pCtx->height;

    if(app1Size > 0xFFFF)
    {

        SetJpegMpEncoderError(pCtx, JPEG_ENCODER_ERROR_APP1);
        goto error;
    }

    totalSize = APP1_LENGTH_OFFSET + app1Size;

    if((pCtx->dstOffset + totalSize) >= pCtx->writeLimit)
    {
        goto insufficientOutputBufferError;
    }

    if(totalSize != EncodeJpegApp1Common(pDst, pWork))
    {

        SetJpegMpEncoderError(pCtx, JPEG_ENCODER_ERROR_INTERNAL);
        goto error;
    }

    EncodeTiffDataShort(pDst + APP1_LENGTH_OFFSET, app1Size, false);

app1Done:
    pDst += totalSize;

    pCtx->dstOffset = pDst - pCtx->pDst;
    return true;

error:
    return false;

insufficientOutputBufferError:
    SetJpegMpEncoderError(pCtx, JPEG_ENCODER_ERROR_INSUFFICIENT_OUTPUT_BUFFER);
    goto error;
}

namespace
{

const u8 JpegMpEncoderApp2TiffHeaderVer0100LittleEndian[APP2_MP_TIFF_HEADER_SIZE] = {

    0x4D, 0x50, 0x46, 0x00, 0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
const u8 JpegMpEncoderApp2TiffHeaderVer0100BigEndian[APP2_MP_TIFF_HEADER_SIZE] = {

    0x4D, 0x50, 0x46, 0x00, 0x4D, 0x4D, 0x00, 0x2A, 0x00, 0x00, 0x00, 0x08};

const u8 App2MpfVersion_0100_Data[4] = {'0', '1', '0', '0'};
}

namespace
{
const u8 App2IndexIfdSeparationTbl[NUM_APP2_MP_INDEX_IFDS] = {JPEG_MP_ENCODER_APP2_TAG_INDEX_ATTRIBUTE_LOOP_BEGIN,
                                                              JPEG_MP_ENCODER_APP2_TAG_INDEX_ATTRIBUTE_LOOP_END};

const u8 App2AttributeIfdSeparationTbl[NUM_APP2_MP_ATTRIBUTE_IFDS] = {
    JPEG_MP_ENCODER_APP2_TAG_INDEX_ATTRIBUTE_LOOP_END};
}

void InitializeJpegMpEncoderApp2IndexTagWork(detail::JpegMpEncoderWorkObj* pWork)
{
    JpegTagWorkObj* pApp2 = pWork->app2.tagWork;

    pWork->app2.baseViewpointNumData[0] = 1;

    InitializeTagDataUndefined(&pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_MPF_VERSION], APP2_MP_TAG_NUMBER_MPF_VERSION,
                               App2MpfVersion_0100_Data, sizeof(App2MpfVersion_0100_Data));
    InitializeTagDataLong(&pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_NUMBER_OF_IMAGES], APP2_MP_TAG_NUMBER_NUMBER_OF_IMAGES,
                          pWork->app2.numberOfImagesData);
}

void InitializeJpegMpEncoderApp2AttributeTagWork(detail::JpegMpEncoderWorkObj* pWork)
{

    memset(&pWork->app2.tagWork[JPEG_MP_ENCODER_APP2_TAG_INDEX_ATTRIBUTE_LOOP_BEGIN], 0,
           reinterpret_cast<size_t>(&pWork->app2.tagWork[JPEG_MP_ENCODER_APP2_TAG_INDEX_ATTRIBUTE_LOOP_END]) -
               reinterpret_cast<size_t>(&pWork->app2.tagWork[JPEG_MP_ENCODER_APP2_TAG_INDEX_ATTRIBUTE_LOOP_BEGIN]));

    pWork->app2.individualNumData[0] = pWork->app2.curIndex + 1;
}

size_t CalcJpegMpEncoderApp2Size(detail::JpegMpEncoderWorkObj* pWork)
{
    size_t size;

    if(!pWork->app2.curIndex)
    {
        size = CalcSegmentSizeCommon(pWork->app2.tagWork, pWork->app2.indexIfdWork, App2IndexIfdSeparationTbl,
                                     NUM_APP2_MP_INDEX_IFDS, JPEG_MP_ENCODER_APP2_TAG_INDEX_INDEX_LOOP_BEGIN,
                                     JPEG_MP_ENCODER_APP2_TAG_INDEX_ATTRIBUTE_LOOP_END);

        if(!pWork->app2.isBaseline)
        {
            pWork->app2.indexIfdWork[APP2_MP_INDEX_IFD_INDEX].offsetToNextIfd =
                pWork->app2.indexIfdWork[APP2_MP_INDEX_IFD_INDEX_ATTRIBUTE].offset + APP2_MP_TIFF_OFFSET;
        }
    }
    else
    {
        size = CalcSegmentSizeCommon(pWork->app2.tagWork, pWork->app2.attributeIfdWork, App2AttributeIfdSeparationTbl,
                                     NUM_APP2_MP_ATTRIBUTE_IFDS, JPEG_MP_ENCODER_APP2_TAG_INDEX_ATTRIBUTE_LOOP_BEGIN,
                                     JPEG_MP_ENCODER_APP2_TAG_INDEX_ATTRIBUTE_LOOP_END);
    }

    size += APP2_LENGTH_SIZE + sizeof(JpegMpEncoderApp2TiffHeaderVer0100LittleEndian);
    if(size > 0xFFFF)
    {

        SetJpegMpEncoderError(&pWork->ctx, JPEG_ENCODER_ERROR_APP2_MP);
        size = 0;
    }

    return size;
}

size_t EncodeJpegMpApp2Common(u8* pDst, detail::JpegMpEncoderWorkObj* pWork)
{
    u8* pDstTop = pDst;
    size_t size;

    pDst += APP2_LENGTH_OFFSET + APP2_LENGTH_SIZE;

    size = sizeof(JpegMpEncoderApp2TiffHeaderVer0100LittleEndian);
    memcpy(pDst,
           pWork->isApp2TiffLittleEndian ? JpegMpEncoderApp2TiffHeaderVer0100LittleEndian
                                         : JpegMpEncoderApp2TiffHeaderVer0100BigEndian,
           size);
    pDst += size;

    if(!pWork->app2.curIndex)
    {
        size = EncodeSegmentCommon(pDst, pDstTop + APP2_MP_TIFF_OFFSET, pWork->isApp2TiffLittleEndian,
                                   pWork->app2.tagWork, pWork->app2.indexIfdWork, App2IndexIfdSeparationTbl,
                                   JPEG_MP_ENCODER_APP2_TAG_INDEX_INDEX_LOOP_BEGIN,
                                   JPEG_MP_ENCODER_APP2_TAG_INDEX_ATTRIBUTE_LOOP_END, pWork);
    }
    else
    {
        size = EncodeSegmentCommon(pDst, pDstTop + APP2_MP_TIFF_OFFSET, pWork->isApp2TiffLittleEndian,
                                   pWork->app2.tagWork, pWork->app2.attributeIfdWork, App2AttributeIfdSeparationTbl,
                                   JPEG_MP_ENCODER_APP2_TAG_INDEX_ATTRIBUTE_LOOP_BEGIN,
                                   JPEG_MP_ENCODER_APP2_TAG_INDEX_ATTRIBUTE_LOOP_END, pWork);
    }
    pDst += size;

    return pDst - pDstTop;
}

bool EncodeJpegMpApp2(detail::JpegMpEncoderWorkObj* pWork, detail::JpegMpEncoderTemporarySettingObj* pTempSetting)
{
    JpegMpEncoderContext* pCtx = &pWork->ctx;
    JpegTagWorkObj* pApp2 = pWork->app2.tagWork;
    u8* pDst = pCtx->pDst + pCtx->dstOffset;
    size_t app2Size;
    size_t size;
    u32 index;

    if(!pWork->app2.curIndex)
    {

        pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_MPF_VERSION_SPECIAL].dataSize = 0;

        if(pTempSetting->mpAttribute.isBaseViewpointNumValid)
        {
            pWork->app2.baseViewpointNumData[0] = pTempSetting->mpAttribute.baseViewpointNum;
        }

        InitializeTagDataUndefined(&pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_MP_ENTRY], APP2_MP_TAG_NUMBER_MP_ENTRY,
                                   pWork->pApp2MpEntry,
                                   pWork->app2.numberOfImages * sizeof(JpegMpEncoderApp2TiffMpEntryObj));

        if(pWork->app2.addImageUidList)
        {
            InitializeTagDataUndefined(&pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_IMAGE_UID_LIST],
                                       APP2_MP_TAG_NUMBER_IMAGE_UID_LIST, pWork->pApp2MpImageUidList,
                                       pWork->app2.numberOfImages * IMAGE_UID_SIZE);
        }

        if(pWork->app2.addTotalFrames)
        {
            InitializeTagDataLong(&pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_TOTAL_FRAMES], APP2_MP_TAG_NUMBER_TOTAL_FRAMES,
                                  pWork->app2.totalFramesData);
        }
    }
    else
    {

        InitializeTagDataUndefined(&pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_MPF_VERSION_SPECIAL],
                                   APP2_MP_TAG_NUMBER_MPF_VERSION_SPECIAL, App2MpfVersion_0100_Data,
                                   sizeof(App2MpfVersion_0100_Data));
    }

    if(pTempSetting->mpAttribute.isMpIndividualNumValid)
    {
        pWork->app2.individualNumData[0] = pTempSetting->mpAttribute.mpIndividualNum;
    }
    InitializeTagDataLong(&pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_MP_INDIVIDUAL_NUM],
                          APP2_MP_TAG_NUMBER_MP_INDIVIDUAL_NUM, pWork->app2.individualNumData);

    if((pWork->app2.typeCode != MP_TYPE_CODE_LARGE_THUMBNAIL_IMAGE_CLASS_1) &&
       (pWork->app2.typeCode != MP_TYPE_CODE_LARGE_THUMBNAIL_IMAGE_CLASS_2) && pWork->app2.individualNumData[0])
    {
        pWork->app2.totalFramesData[0]++;
    }

    if(pTempSetting->mpAttribute.isPanOrientationValid)
    {
        InitializeTagDataLong(&pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_PAN_ORIENTATION],
                              APP2_MP_TAG_NUMBER_PAN_ORIENTATION, &pTempSetting->mpAttribute.panOrientation);
    }

    if(pTempSetting->mpAttribute.isPanOverlapHValid)
    {
        InitializeTagDataRational(&pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_PAN_OVERLAP_H],
                                  APP2_MP_TAG_NUMBER_PAN_OVERLAP_H, &pTempSetting->mpAttribute.panOverlapH);
    }

    if(pTempSetting->mpAttribute.isPanOverlapVValid)
    {
        InitializeTagDataRational(&pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_PAN_OVERLAP_V],
                                  APP2_MP_TAG_NUMBER_PAN_OVERLAP_V, &pTempSetting->mpAttribute.panOverlapV);
    }

    InitializeTagDataLong(&pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_BASE_VIEWPOINT_NUM],
                          APP2_MP_TAG_NUMBER_BASE_VIEWPOINT_NUM, pWork->app2.baseViewpointNumData);

    if(!pTempSetting->mpAttribute.isConvergenceAngleValid)
    {

        pTempSetting->mpAttribute.convergenceAngle.value[0] = pTempSetting->mpAttribute.convergenceAngle.value[1] =
            TIFF_SRATIONAL_UNKNOWN_DATA_VALUE;
    }
    InitializeTagDataSrational(&pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_CONVERGENCE_ANGLE],
                               APP2_MP_TAG_NUMBER_CONVERGENCE_ANGLE, &pTempSetting->mpAttribute.convergenceAngle);

    if(!pTempSetting->mpAttribute.isBaselineLengthValid)
    {

        pTempSetting->mpAttribute.baselineLength.value[0] = pTempSetting->mpAttribute.baselineLength.value[1] =
            TIFF_RATIONAL_UNKNOWN_DATA_VALUE;
    }
    InitializeTagDataRational(&pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_BASELINE_LENGTH],
                              APP2_MP_TAG_NUMBER_BASELINE_LENGTH, &pTempSetting->mpAttribute.baselineLength);

    if(pTempSetting->mpAttribute.isVerticalDivergenceValid)
    {
        InitializeTagDataSrational(&pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_VERTICAL_DIVERGENCE],
                                   APP2_MP_TAG_NUMBER_VERTICAL_DIVERGENCE,
                                   &pTempSetting->mpAttribute.verticalDivergence);
    }

    if(pTempSetting->mpAttribute.isAxisDistanceXValid)
    {
        InitializeTagDataSrational(&pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_AXIS_DISTANCE_X],
                                   APP2_MP_TAG_NUMBER_AXIS_DISTANCE_X, &pTempSetting->mpAttribute.axisDistanceX);
    }

    if(pTempSetting->mpAttribute.isAxisDistanceYValid)
    {
        InitializeTagDataSrational(&pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_AXIS_DISTANCE_Y],
                                   APP2_MP_TAG_NUMBER_AXIS_DISTANCE_Y, &pTempSetting->mpAttribute.axisDistanceY);
    }

    if(pTempSetting->mpAttribute.isAxisDistanceZValid)
    {
        InitializeTagDataSrational(&pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_AXIS_DISTANCE_Z],
                                   APP2_MP_TAG_NUMBER_AXIS_DISTANCE_Z, &pTempSetting->mpAttribute.axisDistanceZ);
    }

    if(pTempSetting->mpAttribute.isYawAngleValid)
    {
        InitializeTagDataSrational(&pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_YAW_ANGLE], APP2_MP_TAG_NUMBER_YAW_ANGLE,
                                   &pTempSetting->mpAttribute.yawAngle);
    }

    if(pTempSetting->mpAttribute.isPitchAngleValid)
    {
        InitializeTagDataSrational(&pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_PITCH_ANGLE], APP2_MP_TAG_NUMBER_PITCH_ANGLE,
                                   &pTempSetting->mpAttribute.pitchAngle);
    }

    if(pTempSetting->mpAttribute.isRollAngleValid)
    {
        InitializeTagDataSrational(&pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_ROLL_ANGLE], APP2_MP_TAG_NUMBER_ROLL_ANGLE,
                                   &pTempSetting->mpAttribute.rollAngle);
    }

    if(pWork->app2.isRepresentativeSet)
    {

        pWork->app2.isRepresentative = false;
    }
    else
    {

        if(!pTempSetting->isRepresentativeSet)
        {
            pTempSetting->isRepresentativeSet = true;
            pTempSetting->isRepresentative = true;
        }
        pWork->app2.isRepresentative = pTempSetting->isRepresentative;
        if(pWork->app2.isRepresentative)
        {

            pWork->app2.isRepresentativeSet = true;
        }
    }

    pWork->app2.isDependentParent = pTempSetting->isDependentParent;
    pWork->app2.isDependentChild = pTempSetting->isDependentChild;
    pWork->app2.dependentImage1EntryNum = pTempSetting->dependentImage1EntryNum;
    pWork->app2.dependentImage2EntryNum = pTempSetting->dependentImage2EntryNum;

    switch(pWork->app2.typeCode)
    {
    case MP_TYPE_CODE_MULTI_VIEW_PANORAMA_IMAGE:
        pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_BASE_VIEWPOINT_NUM].dataSize =
            pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_CONVERGENCE_ANGLE].dataSize =
                pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_BASELINE_LENGTH].dataSize =
                    pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_VERTICAL_DIVERGENCE].dataSize =
                        pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_AXIS_DISTANCE_X].dataSize =
                            pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_AXIS_DISTANCE_Y].dataSize =
                                pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_AXIS_DISTANCE_Z].dataSize =
                                    pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_YAW_ANGLE].dataSize =
                                        pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_PITCH_ANGLE].dataSize =
                                            pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_ROLL_ANGLE].dataSize = 0;
        break;

    case MP_TYPE_CODE_MULTI_VIEW_DISPARITY_IMAGE:
        pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_PAN_ORIENTATION].dataSize =
            pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_PAN_OVERLAP_H].dataSize =
                pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_PAN_OVERLAP_V].dataSize =
                    pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_AXIS_DISTANCE_X].dataSize =
                        pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_AXIS_DISTANCE_Y].dataSize =
                            pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_AXIS_DISTANCE_Z].dataSize =
                                pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_YAW_ANGLE].dataSize =
                                    pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_PITCH_ANGLE].dataSize =
                                        pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_ROLL_ANGLE].dataSize = 0;
        break;

    case MP_TYPE_CODE_MULTI_VIEW_MULTI_ANGLE_IMAGE:
        pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_PAN_ORIENTATION].dataSize =
            pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_PAN_OVERLAP_H].dataSize =
                pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_PAN_OVERLAP_V].dataSize =
                    pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_CONVERGENCE_ANGLE].dataSize =
                        pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_BASELINE_LENGTH].dataSize =
                            pApp2[JPEG_MP_ENCODER_APP2_TAG_INDEX_VERTICAL_DIVERGENCE].dataSize = 0;
        break;

    case MP_TYPE_CODE_BASELINE_MP_PRIMARY_IMAGE:

        for(index = JPEG_MP_ENCODER_APP2_TAG_INDEX_ATTRIBUTE_LOOP_BEGIN;
            index < JPEG_MP_ENCODER_APP2_TAG_INDEX_ATTRIBUTE_LOOP_END; index++)
        {
            pApp2[index].dataSize = 0;
        }
        break;

    case MP_TYPE_CODE_LARGE_THUMBNAIL_IMAGE_CLASS_1:
    case MP_TYPE_CODE_LARGE_THUMBNAIL_IMAGE_CLASS_2:

        return true;

    case MP_TYPE_CODE_UNDEFINED:

        for(index = JPEG_MP_ENCODER_APP2_TAG_INDEX_UNDEFINED_LOOP_BEGIN;
            index < JPEG_MP_ENCODER_APP2_TAG_INDEX_UNDEFINED_LOOP_END; index++)
        {
            pApp2[index].dataSize = 0;
        }
        break;

    default:

        SetJpegMpEncoderError(pCtx, JPEG_ENCODER_ERROR_INVALID_MP_TYPE_CODE);
        goto error;
    }

    app2Size = CalcJpegMpEncoderApp2Size(pWork);
    if(!app2Size)
    {
        SetJpegMpEncoderError(pCtx, JPEG_ENCODER_ERROR_APP2_MP);
        goto error;
    }

    if((pCtx->dstOffset + APP2_LENGTH_OFFSET + app2Size) >= pCtx->writeLimit)
    {
        SetJpegMpEncoderError(pCtx, JPEG_ENCODER_ERROR_INSUFFICIENT_OUTPUT_BUFFER);
        goto error;
    }
    pCtx->dstOffset += APP2_LENGTH_OFFSET + app2Size;

    pDst[0] = 0xFF;
    pDst[1] = 0xE2;

    EncodeTiffDataShort(pDst + APP2_LENGTH_OFFSET, app2Size, false);

    if(!pWork->app2.curIndex)
    {

        pWork->app2.pFirstDst = pDst;
        pWork->app2.firstSize = app2Size;
    }

    size = EncodeJpegMpApp2Common(pDst, pWork);
    if((APP2_LENGTH_OFFSET + app2Size) != size)
    {

        SetJpegMpEncoderError(pCtx, JPEG_ENCODER_ERROR_INTERNAL);
        goto error;
    }

    return true;

error:
    return false;
}

bool PostEncodeJpegMpApp2(detail::JpegMpEncoderWorkObj* pWork, bool fromApi)
{
    size_t size;
    u32 numImages;
    JpegMpEncoderApp2TiffMpEntryObj* pApp2MpEntry;
    bool isLittleEndian = pWork->isApp2TiffLittleEndian;
    const u8* pImageTiffOffsetBase;
    u8* pApp2ImageUid;
    u32 index = pWork->app2.curIndex;
    MpTypeCode typeCodeBak;

    if(index >= pWork->app2.numberOfImages)
    {

        goto skip;
    }

    if(fromApi)
    {

        goto internalError;
    }
    else
    {

        pApp2MpEntry = &pWork->pApp2MpEntry[index];
        pImageTiffOffsetBase = pWork->app2.pFirstDst + APP2_MP_TIFF_OFFSET;

        EncodeTiffDataLong(pApp2MpEntry->s.typeInfoLongData,
                           pWork->app2.typeCode | MP_TYPE_DATA_FORMAT_JPEG |
                               (pWork->app2.isDependentParent ? MP_TYPE_FLAG_DEPENDENT_IMAGE_PARENT : 0) |
                               (pWork->app2.isDependentChild ? MP_TYPE_FLAG_DEPENDENT_IMAGE_CHILD : 0) |
                               (pWork->app2.isRepresentative ? MP_TYPE_FLAG_REPRESENTATIVE_IMAGE : 0),
                           isLittleEndian);

        EncodeTiffDataLong(pApp2MpEntry->s.imageSizeLongData, pWork->ctx.dstOffset, isLittleEndian);
        EncodeTiffDataLong(pApp2MpEntry->s.imageOffsetLongData, index ? (pWork->ctx.pDst - pImageTiffOffsetBase) : 0,
                           isLittleEndian);

        EncodeTiffDataShort(pApp2MpEntry->s.dependentImage1EntryNumShortData, pWork->app2.dependentImage1EntryNum,
                            isLittleEndian);
        EncodeTiffDataShort(pApp2MpEntry->s.dependentImage2EntryNumShortData, pWork->app2.dependentImage2EntryNum,
                            isLittleEndian);

        if(pWork->app2.addImageUidList)
        {

            if(index)
            {
                pApp2ImageUid = &pWork->pApp2MpImageUidList[index * IMAGE_UID_SIZE];
                if(pWork->app1TagWork[JPEG_ENCODER_APP1_TAG_INDEX_EXIF_IMAGE_UNIQUE_ID].dataSize == IMAGE_UID_SIZE)
                {
                    memcpy(pApp2ImageUid, pWork->app1TagWork[JPEG_ENCODER_APP1_TAG_INDEX_EXIF_IMAGE_UNIQUE_ID].pData,
                           IMAGE_UID_SIZE);
                }
                else
                {

                    memset(pApp2ImageUid, 0, IMAGE_UID_SIZE);
                }
            }
        }

        pWork->app2.curIndex++;
        InitializeJpegMpEncoderApp2AttributeTagWork(pWork);
        if(pWork->app2.curIndex < pWork->app2.numberOfImages)
        {

            goto skip;
        }
    }

    numImages = pWork->app2.numberOfImages;
    pWork->app2.numberOfImagesData[0] = numImages;

    size = EncodeSegmentCommon(pWork->app2.pFirstDst + APP2_LENGTH_OFFSET + APP2_LENGTH_SIZE +
                                   sizeof(JpegMpEncoderApp2TiffHeaderVer0100LittleEndian),
                               pWork->app2.pFirstDst + APP2_MP_TIFF_OFFSET, pWork->isApp2TiffLittleEndian,
                               pWork->app2.tagWork, pWork->app2.indexIfdWork, App2IndexIfdSeparationTbl,
                               JPEG_MP_ENCODER_APP2_TAG_INDEX_INDEX_LOOP_BEGIN,
                               JPEG_MP_ENCODER_APP2_TAG_INDEX_INDEX_LOOP_END, pWork);

    if(size > pWork->app2.firstSize)
    {

        goto internalError;
    }

    typeCodeBak = pWork->app2.typeCode;
    memset(&pWork->app2, 0, sizeof(pWork->app2));
    pWork->app2.typeCode = typeCodeBak;

    InitializeJpegMpEncoderApp2IndexTagWork(pWork);

skip:
    return true;

internalError:
    SetJpegMpEncoderError(&pWork->ctx, JPEG_ENCODER_ERROR_INTERNAL);
    return false;
}

}
}
}
}

