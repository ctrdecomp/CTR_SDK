#pragma once

#include <nn/util/util_NonCopyable.h>
#include <nn/jpeg/CTR/jpeg_MpTypes.h>

#ifdef __cplusplus

namespace nn {
namespace jpeg {
namespace CTR {

namespace detail {

struct JpegMpDecoderWorkObj;

}

class JpegMpDecoder : private nn::util::NonCopyable<JpegMpDecoder>
{
public:
    static size_t GetWorkBufferSize();

    JpegMpDecoder() : m_Initialized(false) {}

    bool Initialize(void* workBuffer, size_t workBufferSize);

    void Finalize()
    {
        m_Initialized = false;
    }

    ~JpegMpDecoder()
    {
        Finalize();
    }

    static size_t GetDstBufferSize(u32 maxWidth, u32 maxHeight, PixelFormat dstPixelFormat)
    {
        size_t size = 0;
        u64 size64;

        switch(dstPixelFormat)
        {
        case PIXEL_FORMAT_YUYV8:

            maxWidth = (maxWidth + 1) & ~1;
            size = 2;
            break;

        case PIXEL_FORMAT_CTR_RGB565:
            size = 2;
            break;

        case PIXEL_FORMAT_CTR_RGB565_BLOCK8:

            maxWidth = (maxWidth + 7) & ~7;
            maxHeight = (maxHeight + 7) & ~7;
            size = 2;
            break;

        case PIXEL_FORMAT_RGB8:
        case PIXEL_FORMAT_BGR8:
            size = 3;
            break;

        case PIXEL_FORMAT_CTR_RGB8_BLOCK8:
            maxWidth = (maxWidth + 7) & ~7;
            maxHeight = (maxHeight + 7) & ~7;
            size = 3;
            break;

        case PIXEL_FORMAT_RGBA8:
        case PIXEL_FORMAT_ABGR8:
            size = 4;
            break;

        case PIXEL_FORMAT_CTR_RGBA8_BLOCK8:
            maxWidth = (maxWidth + 7) & ~7;
            maxHeight = (maxHeight + 7) & ~7;
            size = 4;
            break;

        default:

            size = 0;
            break;
        }

        size64 = static_cast<u64>(size) * maxWidth * maxHeight;
        size = static_cast<size_t>(size64);

        if(size != size64)
        {
            size = 0;
        }

        return size;
    }

    void SetOutputBufferWidth(u32 width)
    {
        if(m_Initialized)
        {
            if(width <= MAX_DECODER_OUTPUT_BUFFER_WIDTH)
            {
                m_TemporarySetting.outputBufferWidth = width;
            }
        }
    }

    void SetOption(u32 option)
    {
        if(m_Initialized)
        {
            m_TemporarySetting.option = option;
        }
    }

    u32 GetOption()
    {
        if(m_Initialized)
        {
            return m_TemporarySetting.option;
        }

        return JPEG_DECODER_OPTION_NONE;
    }

    static void DoNotCallMe1() {}

    size_t StartJpegDecoder(void* dst, size_t dstSize, const u8* src, size_t srcSize, u32 maxWidth, u32 maxHeight,
                            PixelFormat dstPixelFormat, bool decodeThumbnail);

    size_t StartJpegDecoderShrink(void* dst, size_t dstSize, const u8* src, size_t srcSize, u32 maxWidth, u32 maxHeight,
                                  PixelFormat dstPixelFormat, bool decodeThumbnail, u32 shrinkLevel);

    size_t StartMpDecoderLR(void* dstL, void* dstR, size_t dstSize, const u8* src, size_t srcSize, u32 maxWidth,
                            u32 maxHeight, PixelFormat dstPixelFormat);

    void StopDecoder();

    bool ExtractExif(const u8* src, size_t srcSize, bool extractThumbnail);

    bool GetMpRegionsToBuildJpegData(MpRegionsToBuildJpegData* pBuffer, const u8* src, size_t srcSize);

    s32 GetLastError() const;

    static void DoNotCallMe2() {}

    u32 GetLastWidth() const;

    u32 GetLastHeight() const;

    u32 GetLastOutputBufferWidth() const;

    u32 GetLastOutputBufferHeight() const;

    size_t GetLastDateTime(char* pBuffer) const;

    const char* GetLastDateTimePointer() const;

    const char* GetLastSoftwarePointer() const;

    size_t GetLastSoftwareLength() const;

    const u8* GetLastUserMakerNotePointer() const;

    size_t GetLastUserMakerNoteSize() const;

    bool GetLastTwlPhotoMakerNote(u8* pBuffer) const;

    const u8* GetLastTwlUserMakerNotePointer() const;

    size_t GetLastTwlUserMakerNoteSize() const;

    size_t GetLastImageUid(char* pBuffer);

    bool GetLastOrientation(u16* pBuffer);

    bool GetMpIndex(MpIndex* pIndex, const u8* src, size_t srcSize);

    static bool GetMpNumberOfImages(u32* pNumber, const MpIndex* pIndex)
    {
        NN_ASSERT_(pNumber);
        NN_ASSERT_(pIndex);

        if(pIndex->isNumberOfImagesValid)
        {
            *pNumber = pIndex->numberOfImages;
            return true;
        }

        return false;
    }

    static size_t GetMpImageUidListSize(const MpIndex* pIndex)
    {
        NN_ASSERT_(pIndex);

        if(pIndex->imageUidListSize && pIndex->offsetToImageUidList)
        {
            return pIndex->imageUidListSize;
        }

        return 0;
    }

    static size_t GetMpImageUidListOffset(const MpIndex* pIndex)
    {
        NN_ASSERT_(pIndex);

        if(pIndex->imageUidListSize && pIndex->offsetToImageUidList)
        {
            return pIndex->offsetToImageUidList;
        }

        return 0;
    }

    static bool GetMpTotalFrames(u32* pFrames, const MpIndex* pIndex)
    {
        NN_ASSERT_(pFrames);
        NN_ASSERT_(pIndex);

        if(pIndex->isTotalFramesValid)
        {
            *pFrames = pIndex->totalFrames;
            return true;
        }

        return false;
    }

    static bool GetMpEntry(MpEntry* pEntry, const MpIndex* pIndex, u32 index);

    static u32 GetMpImageType(const MpEntry* pEntry)
    {
        NN_ASSERT_(pEntry);
        return pEntry->type;
    }

    static size_t GetMpImageSize(const MpEntry* pEntry)
    {
        NN_ASSERT_(pEntry);
        return pEntry->imageDataSize;
    }

    static size_t GetMpImageOffset(const MpEntry* pEntry)
    {
        NN_ASSERT_(pEntry);
        return pEntry->offsetToImageData;
    }

    static u16 GetMpDependentImage1EntryNum(const MpEntry* pEntry)
    {
        NN_ASSERT_(pEntry);
        return pEntry->dependentImage1EntryNum;
    }

    static u16 GetMpDependentImage2EntryNum(const MpEntry* pEntry)
    {
        NN_ASSERT_(pEntry);
        return pEntry->dependentImage2EntryNum;
    }

    bool GetMpAttribute(MpAttribute* pAttr, const u8* src, size_t srcSize);

    static bool GetMpIndividualNum(u32* pBuffer, const MpAttribute* pAttr)
    {
        NN_ASSERT_(pBuffer);
        NN_ASSERT_(pAttr);

        if(pAttr->isMpIndividualNumValid)
        {
            *pBuffer = pAttr->mpIndividualNum;
            return true;
        }

        return false;
    }

    static bool GetMpPanOrientation(u32* pBuffer, const MpAttribute* pAttr)
    {
        NN_ASSERT_(pBuffer);
        NN_ASSERT_(pAttr);

        if(pAttr->isPanOrientationValid)
        {
            *pBuffer = pAttr->panOrientation;
            return true;
        }

        return false;
    }

    static bool GetMpPanOverlapH(Rational* pBuffer, const MpAttribute* pAttr)
    {
        NN_ASSERT_(pBuffer);
        NN_ASSERT_(pAttr);

        if(pAttr->isPanOverlapHValid)
        {
            *pBuffer = pAttr->panOverlapH;
            return true;
        }

        return false;
    }

    static bool GetMpPanOverlapV(Rational* pBuffer, const MpAttribute* pAttr)
    {
        NN_ASSERT_(pBuffer);
        NN_ASSERT_(pAttr);

        if(pAttr->isPanOverlapVValid)
        {
            *pBuffer = pAttr->panOverlapV;
            return true;
        }

        return false;
    }

    static bool GetMpBaseViewpointNum(u32* pBuffer, const MpAttribute* pAttr)
    {
        NN_ASSERT_(pBuffer);
        NN_ASSERT_(pAttr);

        if(pAttr->isBaseViewpointNumValid)
        {
            *pBuffer = pAttr->baseViewpointNum;
            return true;
        }

        return false;
    }

    static bool GetMpConvergenceAngle(Srational* pBuffer, const MpAttribute* pAttr)
    {
        NN_ASSERT_(pBuffer);
        NN_ASSERT_(pAttr);

        if(pAttr->isConvergenceAngleValid)
        {
            *pBuffer = pAttr->convergenceAngle;
            return true;
        }

        return false;
    }

    static bool GetMpBaselineLength(Rational* pBuffer, const MpAttribute* pAttr)
    {
        NN_ASSERT_(pBuffer);
        NN_ASSERT_(pAttr);

        if(pAttr->isBaselineLengthValid)
        {
            *pBuffer = pAttr->baselineLength;
            return true;
        }

        return false;
    }

    static bool GetMpVerticalDivergence(Srational* pBuffer, const MpAttribute* pAttr)
    {
        NN_ASSERT_(pBuffer);
        NN_ASSERT_(pAttr);

        if(pAttr->isVerticalDivergenceValid)
        {
            *pBuffer = pAttr->verticalDivergence;
            return true;
        }

        return false;
    }

    static bool GetMpAxisDistanceX(Srational* pBuffer, const MpAttribute* pAttr)
    {
        NN_ASSERT_(pBuffer);
        NN_ASSERT_(pAttr);

        if(pAttr->isAxisDistanceXValid)
        {
            *pBuffer = pAttr->axisDistanceX;
            return true;
        }

        return false;
    }

    static bool GetMpAxisDistanceY(Srational* pBuffer, const MpAttribute* pAttr)
    {
        NN_ASSERT_(pBuffer);
        NN_ASSERT_(pAttr);

        if(pAttr->isAxisDistanceYValid)
        {
            *pBuffer = pAttr->axisDistanceY;
            return true;
        }

        return false;
    }

    static bool GetMpAxisDistanceZ(Srational* pBuffer, const MpAttribute* pAttr)
    {
        NN_ASSERT_(pBuffer);
        NN_ASSERT_(pAttr);

        if(pAttr->isAxisDistanceZValid)
        {
            *pBuffer = pAttr->axisDistanceZ;
            return true;
        }

        return false;
    }

    static bool GetMpYawAngle(Srational* pBuffer, const MpAttribute* pAttr)
    {
        NN_ASSERT_(pBuffer);
        NN_ASSERT_(pAttr);

        if(pAttr->isYawAngleValid)
        {
            *pBuffer = pAttr->yawAngle;
            return true;
        }

        return false;
    }

    static bool GetMpPitchAngle(Srational* pBuffer, const MpAttribute* pAttr)
    {
        NN_ASSERT_(pBuffer);
        NN_ASSERT_(pAttr);

        if(pAttr->isPitchAngleValid)
        {
            *pBuffer = pAttr->pitchAngle;
            return true;
        }

        return false;
    }

    static bool GetMpRollAngle(Srational* pBuffer, const MpAttribute* pAttr)
    {
        NN_ASSERT_(pBuffer);
        NN_ASSERT_(pAttr);

        if(pAttr->isRollAngleValid)
        {
            *pBuffer = pAttr->rollAngle;
            return true;
        }

        return false;
    }

    bool GetLastGpsData(GpsData* pBuffer);

    static const u8* GetGpsVersionId(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->isVersionIdValid ? pGps->versionId : NULL;
    }

    static char GetGpsLatitudeRef(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->latitudeRef[0];
    }

    static const Rational* GetGpsLatitude(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->isLatitudeValid ? pGps->latitude : NULL;
    }

    static char GetGpsLongitudeRef(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->longitudeRef[0];
    }

    static const Rational* GetGpsLongitude(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->isLongitudeValid ? pGps->longitude : NULL;
    }

    static bool GetGpsAltitudeRef(u8* pBuffer, const GpsData* pGps)
    {
        NN_ASSERT_(pBuffer);
        NN_ASSERT_(pGps);

        if(pGps->isAltitudeRefValid)
        {
            *pBuffer = pGps->altitudeRef;
        }

        return pGps->isAltitudeRefValid;
    }

    static const Rational* GetGpsAltitude(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->isAltitudeValid ? (&pGps->altitude) : NULL;
    }

    static const Rational* GetGpsTimeStamp(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->isTimeStampValid ? pGps->timeStamp : NULL;
    }

    static const char* GetGpsSatellites(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->pSatellites;
    }

    static char GetGpsStatus(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->status[0];
    }

    static char GetGpsMeasureMode(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->measureMode[0];
    }

    static const Rational* GetGpsDop(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->isDopValid ? (&pGps->dop) : NULL;
    }

    static char GetGpsSpeedRef(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->speedRef[0];
    }

    static const Rational* GetGpsSpeed(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->isSpeedValid ? (&pGps->speed) : NULL;
    }

    static char GetGpsTrackRef(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->trackRef[0];
    }

    static const Rational* GetGpsTrack(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->isTrackValid ? (&pGps->track) : NULL;
    }

    static char GetGpsImgDirectionRef(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->imgDirectionRef[0];
    }

    static const Rational* GetGpsImgDirection(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->isImgDirectionValid ? (&pGps->imgDirection) : NULL;
    }

    static const char* GetGpsMapDatum(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->pMapDatum;
    }

    static char GetGpsDestLatitudeRef(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->destLatitudeRef[0];
    }

    static const Rational* GetGpsDestLatitude(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->isDestLatitudeValid ? pGps->destLatitude : NULL;
    }

    static char GetGpsDestLongitudeRef(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->destLongitudeRef[0];
    }

    static const Rational* GetGpsDestLongitude(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->isDestLongitudeValid ? pGps->destLongitude : NULL;
    }

    static char GetGpsDestBearingRef(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->destBearingRef[0];
    }

    static const Rational* GetGpsDestBearing(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->isDestBearingValid ? (&pGps->destBearing) : NULL;
    }

    static char GetGpsDestDistanceRef(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->destDistanceRef[0];
    }

    static const Rational* GetGpsDestDistance(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->isDestDistanceValid ? (&pGps->destDistance) : NULL;
    }

    static const u8* GetGpsProcessingMethodPointer(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        if(pGps->pProcessingMethod && pGps->processingMethodSize)
        {
            return pGps->pProcessingMethod;
        }

        return NULL;
    }

    static size_t GetGpsProcessingMethodSize(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        if(pGps->pProcessingMethod && pGps->processingMethodSize)
        {
            return pGps->processingMethodSize;
        }

        return 0;
    }

    static const u8* GetGpsAreaInformationPointer(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        if(pGps->pAreaInformation && pGps->areaInformationSize)
        {
            return pGps->pAreaInformation;
        }

        return NULL;
    }

    static size_t GetGpsAreaInformationSize(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        if(pGps->pAreaInformation && pGps->areaInformationSize)
        {
            return pGps->areaInformationSize;
        }

        return 0;
    }

    static const char* GetGpsDateStamp(const GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        return pGps->pDateStamp;
    }

    static bool GetGpsDifferential(u16* pBuffer, const GpsData* pGps)
    {
        NN_ASSERT_(pBuffer);
        NN_ASSERT_(pGps);

        if(pGps->isDifferentialValid)
        {
            *pBuffer = pGps->differential;
        }

        return pGps->isDifferentialValid;
    }

protected:
    detail::JpegMpDecoderWorkObj* m_pWork;
    bool m_Initialized;
    bool m_Padding[3];

    detail::JpegMpDecoderTemporarySettingObj m_TemporarySetting;
    void ClearTemporarySetting();

    const u8* GetLastMakerNotePointer(u32 index) const;
    size_t GetLastMakerNoteSize(u32 index) const;
};

}
}
}

#endif

