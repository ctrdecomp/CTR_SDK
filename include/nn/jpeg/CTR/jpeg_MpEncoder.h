#pragma once

#include <nn/util/util_NonCopyable.h>
#include <nn/jpeg/CTR/jpeg_MpTypes.h>
#include <string.h>

#ifdef __cplusplus

namespace nn {
namespace jpeg {
namespace CTR {

namespace detail {

struct JpegMpEncoderWorkObj;

}

class JpegMpEncoder : private nn::util::NonCopyable<JpegMpEncoder>
{
public:
    static size_t GetWorkBufferSize(u32 numImages = 0);

    JpegMpEncoder() : m_Initialized(false) {}

    bool Initialize(void* workBuffer, size_t workBufferSize, u32 numImages = 0);

    void Finalize()
    {
        m_Initialized = false;
    }

    ~JpegMpEncoder()
    {
        Finalize();
    }

    static void DoNotCallMe1() {}

    void SetThumbnailSize(u32 width, u32 height, PixelSampling dstPixelSampling = DEFAULT_THUMBNAIL_PIXEL_SAMPLING)
    {
        if(m_Initialized)
        {
            m_TemporarySetting.thumbnailWidth = width;
            m_TemporarySetting.thumbnailHeight = height;
            m_TemporarySetting.thumbnailSampling = dstPixelSampling;
        }
    }

    void SetInputBufferWidth(u32 width)
    {
        if(m_Initialized)
        {
            if(width <= MAX_ENCODER_INPUT_BUFFER_WIDTH)
            {
                m_TemporarySetting.inputBufferWidth = width;
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

        return JPEG_ENCODER_OPTION_NONE;
    }

    void SetDateTime(const char* pBuffer)
    {
        if(m_Initialized)
        {
            if(pBuffer)
            {
                memcpy(m_TemporarySetting.dateTimeBuffer, pBuffer, sizeof(m_TemporarySetting.dateTimeBuffer));
                m_TemporarySetting.dateTimeBuffer[sizeof(m_TemporarySetting.dateTimeBuffer) - 1] = '\0';
                m_TemporarySetting.isDateTimeSet = true;
            }
            else
            {
                m_TemporarySetting.isDateTimeSet = false;
            }
        }
    }

    static void GetDateTimeNow(char* pBuffer);

    void SetSoftware(const char* pBuffer)
    {
        if(m_Initialized)
        {
            m_TemporarySetting.pSoftware = pBuffer;
        }
    }

    void SetUserMakerNote(const u8* pBuffer, size_t size);

    void SetImageUid(const char* pBuffer)
    {
        if(m_Initialized)
        {
            if(pBuffer)
            {
                memcpy(m_TemporarySetting.imageUidBuffer, pBuffer, sizeof(m_TemporarySetting.imageUidBuffer));
                m_TemporarySetting.imageUidBuffer[sizeof(m_TemporarySetting.imageUidBuffer) - 1] = '\0';
                m_TemporarySetting.isImageUidSet = true;
            }
            else
            {
                m_TemporarySetting.isImageUidSet = false;
            }
        }
    }

    void SetOrientation(u16 orientation)
    {
        if(m_Initialized)
        {
            m_TemporarySetting.orientation = orientation;
            m_TemporarySetting.isOrientationSet = true;
        }
    }

    void ClearOrientation()
    {
        if(m_Initialized)
        {
            m_TemporarySetting.isOrientationSet = false;
        }
    }

    static void DoNotCallMe2() {}

    void SetMpTypeFlags(u32 flags = 0, u16 image1 = 0, u16 image2 = 0)
    {
        if(m_Initialized)
        {
            m_TemporarySetting.isDependentParent = (flags & MP_TYPE_FLAG_DEPENDENT_IMAGE_PARENT) ? true : false;
            m_TemporarySetting.isDependentChild = (flags & MP_TYPE_FLAG_DEPENDENT_IMAGE_CHILD) ? true : false;
            m_TemporarySetting.isRepresentativeSet = true;
            m_TemporarySetting.isRepresentative = (flags & MP_TYPE_FLAG_REPRESENTATIVE_IMAGE) ? true : false;
            m_TemporarySetting.dependentImage1EntryNum = image1;
            m_TemporarySetting.dependentImage2EntryNum = image2;
        }
    }

    static void DoNotCallMe3() {}

    void SetMpIndividualNum(u32 value)
    {
        if(m_Initialized)
        {
            m_TemporarySetting.mpAttribute.mpIndividualNum = value;
            m_TemporarySetting.mpAttribute.isMpIndividualNumValid = true;
        }
    }

    void ClearMpIndividualNum()
    {
        if(m_Initialized)
        {
            m_TemporarySetting.mpAttribute.isMpIndividualNumValid = false;
        }
    }

    void SetMpPanOrientation(u32 value)
    {
        if(m_Initialized)
        {
            m_TemporarySetting.mpAttribute.panOrientation = value;
            m_TemporarySetting.mpAttribute.isPanOrientationValid = true;
        }
    }

    void ClearMpPanOrientation()
    {
        if(m_Initialized)
        {
            m_TemporarySetting.mpAttribute.isPanOrientationValid = false;
        }
    }

    void SetMpPanOverlapH(const Rational* pValue)
    {
        if(m_Initialized)
        {
            if(pValue)
            {
                m_TemporarySetting.mpAttribute.panOverlapH = *pValue;
                m_TemporarySetting.mpAttribute.isPanOverlapHValid = true;
            }
            else
            {
                m_TemporarySetting.mpAttribute.isPanOverlapHValid = false;
            }
        }
    }

    void ClearMpPanOverlapH()
    {
        SetMpPanOverlapH(NULL);
    }

    void SetMpPanOverlapV(const Rational* pValue)
    {
        if(m_Initialized)
        {
            if(pValue)
            {
                m_TemporarySetting.mpAttribute.panOverlapV = *pValue;
                m_TemporarySetting.mpAttribute.isPanOverlapVValid = true;
            }
            else
            {
                m_TemporarySetting.mpAttribute.isPanOverlapVValid = false;
            }
        }
    }

    void ClearMpPanOverlapV()
    {
        SetMpPanOverlapV(NULL);
    }

    void SetMpBaseViewpointNum(u32 value)
    {
        if(m_Initialized)
        {
            m_TemporarySetting.mpAttribute.baseViewpointNum = value;
            m_TemporarySetting.mpAttribute.isBaseViewpointNumValid = true;
        }
    }

    void ClearMpBaseViewpointNum()
    {
        if(m_Initialized)
        {
            m_TemporarySetting.mpAttribute.isBaseViewpointNumValid = false;
        }
    }

    void SetMpConvergenceAngle(const Srational* pValue)
    {
        if(m_Initialized)
        {
            if(pValue)
            {
                m_TemporarySetting.mpAttribute.convergenceAngle = *pValue;
                m_TemporarySetting.mpAttribute.isConvergenceAngleValid = true;
            }
            else
            {
                m_TemporarySetting.mpAttribute.isConvergenceAngleValid = false;
            }
        }
    }

    void ClearMpConvergenceAngle()
    {
        SetMpConvergenceAngle(NULL);
    }

    void SetMpBaselineLength(const Rational* pValue)
    {
        if(m_Initialized)
        {
            if(pValue)
            {
                m_TemporarySetting.mpAttribute.baselineLength = *pValue;
                m_TemporarySetting.mpAttribute.isBaselineLengthValid = true;
            }
            else
            {
                m_TemporarySetting.mpAttribute.isBaselineLengthValid = false;
            }
        }
    }

    void ClearMpBaselineLength()
    {
        SetMpBaselineLength(NULL);
    }

    void SetMpVerticalDivergence(const Srational* pValue)
    {
        if(m_Initialized)
        {
            if(pValue)
            {
                m_TemporarySetting.mpAttribute.verticalDivergence = *pValue;
                m_TemporarySetting.mpAttribute.isVerticalDivergenceValid = true;
            }
            else
            {
                m_TemporarySetting.mpAttribute.isVerticalDivergenceValid = false;
            }
        }
    }

    void ClearMpVerticalDivergence()
    {
        SetMpVerticalDivergence(NULL);
    }

    void SetMpAxisDistanceX(const Srational* pValue)
    {
        if(m_Initialized)
        {
            if(pValue)
            {
                m_TemporarySetting.mpAttribute.axisDistanceX = *pValue;
                m_TemporarySetting.mpAttribute.isAxisDistanceXValid = true;
            }
            else
            {
                m_TemporarySetting.mpAttribute.isAxisDistanceXValid = false;
            }
        }
    }

    void ClearMpAxisDistanceX()
    {
        SetMpAxisDistanceX(NULL);
    }

    void SetMpAxisDistanceY(const Srational* pValue)
    {
        if(m_Initialized)
        {
            if(pValue)
            {
                m_TemporarySetting.mpAttribute.axisDistanceY = *pValue;
                m_TemporarySetting.mpAttribute.isAxisDistanceYValid = true;
            }
            else
            {
                m_TemporarySetting.mpAttribute.isAxisDistanceYValid = false;
            }
        }
    }

    void ClearMpAxisDistanceY()
    {
        SetMpAxisDistanceY(NULL);
    }

    void SetMpAxisDistanceZ(const Srational* pValue)
    {
        if(m_Initialized)
        {
            if(pValue)
            {
                m_TemporarySetting.mpAttribute.axisDistanceZ = *pValue;
                m_TemporarySetting.mpAttribute.isAxisDistanceZValid = true;
            }
            else
            {
                m_TemporarySetting.mpAttribute.isAxisDistanceZValid = false;
            }
        }
    }

    void ClearMpAxisDistanceZ()
    {
        SetMpAxisDistanceZ(NULL);
    }

    void SetMpYawAngle(const Srational* pValue)
    {
        if(m_Initialized)
        {
            if(pValue)
            {
                m_TemporarySetting.mpAttribute.yawAngle = *pValue;
                m_TemporarySetting.mpAttribute.isYawAngleValid = true;
            }
            else
            {
                m_TemporarySetting.mpAttribute.isYawAngleValid = false;
            }
        }
    }

    void ClearMpYawAngle()
    {
        SetMpYawAngle(NULL);
    }

    void SetMpPitchAngle(const Srational* pValue)
    {
        if(m_Initialized)
        {
            if(pValue)
            {
                m_TemporarySetting.mpAttribute.pitchAngle = *pValue;
                m_TemporarySetting.mpAttribute.isPitchAngleValid = true;
            }
            else
            {
                m_TemporarySetting.mpAttribute.isPitchAngleValid = false;
            }
        }
    }

    void ClearMpPitchAngle()
    {
        SetMpPitchAngle(NULL);
    }

    void SetMpRollAngle(const Srational* pValue)
    {
        if(m_Initialized)
        {
            if(pValue)
            {
                m_TemporarySetting.mpAttribute.rollAngle = *pValue;
                m_TemporarySetting.mpAttribute.isRollAngleValid = true;
            }
            else
            {
                m_TemporarySetting.mpAttribute.isRollAngleValid = false;
            }
        }
    }

    void ClearMpRollAngle()
    {
        SetMpRollAngle(NULL);
    }

    static void InitializeGpsData(GpsData* pGps)
    {
        memset(pGps, 0, sizeof(*pGps));

        pGps->versionId[0] = 2;
        pGps->versionId[1] = 2;
        pGps->isVersionIdValid = true;
    }

    static void SetGpsVersionId(GpsData* pGps, const u8* pVersionId)
    {
        NN_ASSERT_(pGps);

        if(pVersionId)
        {
            memcpy(pGps->versionId, pVersionId, sizeof(pGps->versionId));
            pGps->isVersionIdValid = true;
        }
        else
        {
            pGps->isVersionIdValid = false;
        }
    }

    static void ClearGpsVersionId(GpsData* pGps)
    {
        SetGpsVersionId(pGps, NULL);
    }

    static void SetGpsLatitude(GpsData* pGps, char ref, const Rational* pValue)
    {
        NN_ASSERT_(pGps);

        pGps->latitudeRef[0] = ref;
        pGps->latitudeRef[1] = '\0';

        if(pValue)
        {
            memcpy(pGps->latitude, pValue, sizeof(pGps->latitude));
            pGps->isLatitudeValid = true;
        }
        else
        {
            pGps->isLatitudeValid = false;
        }
    }

    static void ClearGpsLatitude(GpsData* pGps)
    {
        SetGpsLatitude(pGps, '\0', NULL);
    }

    static void SetGpsLongitude(GpsData* pGps, char ref, const Rational* pValue)
    {
        NN_ASSERT_(pGps);

        pGps->longitudeRef[0] = ref;
        pGps->longitudeRef[1] = '\0';

        if(pValue)
        {
            memcpy(pGps->longitude, pValue, sizeof(pGps->longitude));
            pGps->isLongitudeValid = true;
        }
        else
        {
            pGps->isLongitudeValid = false;
        }
    }

    static void ClearGpsLongitude(GpsData* pGps)
    {
        SetGpsLongitude(pGps, '\0', NULL);
    }

    static void SetGpsAltitude(GpsData* pGps, u8 ref, const Rational* pValue)
    {
        NN_ASSERT_(pGps);

        if(pValue)
        {
            pGps->altitudeRef = ref;
            memcpy(&pGps->altitude, pValue, sizeof(pGps->altitude));
            pGps->isAltitudeRefValid = pGps->isAltitudeValid = true;
        }
        else
        {
            pGps->isAltitudeRefValid = pGps->isAltitudeValid = false;
        }
    }

    static void ClearGpsAltitude(GpsData* pGps)
    {
        SetGpsAltitude(pGps, 0, NULL);
    }

    static void SetGpsTimeStamp(GpsData* pGps, const Rational* pValue)
    {
        NN_ASSERT_(pGps);

        if(pValue)
        {
            memcpy(pGps->timeStamp, pValue, sizeof(pGps->timeStamp));
            pGps->isTimeStampValid = true;
        }
        else
        {
            pGps->isTimeStampValid = false;
        }
    }

    static void ClearGpsTimeStamp(GpsData* pGps)
    {
        SetGpsTimeStamp(pGps, NULL);
    }

    static void SetGpsSatellites(GpsData* pGps, const char* pSatellites)
    {
        NN_ASSERT_(pGps);

        pGps->pSatellites = pSatellites;
    }

    static void ClearGpsSatellites(GpsData* pGps)
    {
        SetGpsSatellites(pGps, NULL);
    }

    static void SetGpsStatus(GpsData* pGps, char status)
    {
        NN_ASSERT_(pGps);

        pGps->status[0] = status;
        pGps->status[1] = '\0';
    }

    static void ClearGpsStatus(GpsData* pGps)
    {
        SetGpsStatus(pGps, '\0');
    }

    static void SetGpsMeasureMode(GpsData* pGps, char measureMode)
    {
        NN_ASSERT_(pGps);

        pGps->measureMode[0] = measureMode;
        pGps->measureMode[1] = '\0';
    }

    static void ClearGpsMeasureMode(GpsData* pGps)
    {
        SetGpsMeasureMode(pGps, '\0');
    }

    static void SetGpsDop(GpsData* pGps, const Rational* pValue)
    {
        NN_ASSERT_(pGps);

        if(pValue)
        {
            memcpy(&pGps->dop, pValue, sizeof(pGps->dop));
            pGps->isDopValid = true;
        }
        else
        {
            pGps->isDopValid = false;
        }
    }

    static void ClearGpsDop(GpsData* pGps)
    {
        SetGpsDop(pGps, NULL);
    }

    static void SetGpsSpeed(GpsData* pGps, char ref, const Rational* pValue)
    {
        NN_ASSERT_(pGps);

        pGps->speedRef[0] = ref;
        pGps->speedRef[1] = '\0';

        if(pValue)
        {
            memcpy(&pGps->speed, pValue, sizeof(pGps->speed));
            pGps->isSpeedValid = true;
        }
        else
        {
            pGps->isSpeedValid = false;
        }
    }

    static void ClearGpsSpeed(GpsData* pGps)
    {
        SetGpsSpeed(pGps, '\0', NULL);
    }

    static void SetGpsTrack(GpsData* pGps, char ref, const Rational* pValue)
    {
        NN_ASSERT_(pGps);

        pGps->trackRef[0] = ref;
        pGps->trackRef[1] = '\0';

        if(pValue)
        {
            memcpy(&pGps->track, pValue, sizeof(pGps->track));
            pGps->isTrackValid = true;
        }
        else
        {
            pGps->isTrackValid = false;
        }
    }

    static void ClearGpsTrack(GpsData* pGps)
    {
        SetGpsTrack(pGps, '\0', NULL);
    }

    static void SetGpsImgDirection(GpsData* pGps, char ref, const Rational* pValue)
    {
        NN_ASSERT_(pGps);

        pGps->imgDirectionRef[0] = ref;
        pGps->imgDirectionRef[1] = '\0';

        if(pValue)
        {
            memcpy(&pGps->imgDirection, pValue, sizeof(pGps->imgDirection));
            pGps->isImgDirectionValid = true;
        }
        else
        {
            pGps->isImgDirectionValid = false;
        }
    }

    static void ClearGpsImgDirection(GpsData* pGps)
    {
        SetGpsImgDirection(pGps, '\0', NULL);
    }

    static void SetGpsMapDatum(GpsData* pGps, const char* pMapDatum)
    {
        NN_ASSERT_(pGps);

        pGps->pMapDatum = pMapDatum;
    }

    static void ClearGpsMapDatum(GpsData* pGps)
    {
        SetGpsMapDatum(pGps, NULL);
    }

    static void SetGpsDestLatitude(GpsData* pGps, char ref, const Rational* pValue)
    {
        NN_ASSERT_(pGps);

        pGps->destLatitudeRef[0] = ref;
        pGps->destLatitudeRef[1] = '\0';

        if(pValue)
        {
            memcpy(pGps->destLatitude, pValue, sizeof(pGps->destLatitude));
            pGps->isDestLatitudeValid = true;
        }
        else
        {
            pGps->isDestLatitudeValid = false;
        }
    }

    static void ClearGpsDestLatitude(GpsData* pGps)
    {
        SetGpsDestLatitude(pGps, '\0', NULL);
    }

    static void SetGpsDestLongitude(GpsData* pGps, char ref, const Rational* pValue)
    {
        NN_ASSERT_(pGps);

        pGps->destLongitudeRef[0] = ref;
        pGps->destLongitudeRef[1] = '\0';

        if(pValue)
        {
            memcpy(pGps->destLongitude, pValue, sizeof(pGps->destLongitude));
            pGps->isDestLongitudeValid = true;
        }
        else
        {
            pGps->isDestLongitudeValid = false;
        }
    }

    static void ClearGpsDestLongitude(GpsData* pGps)
    {
        SetGpsDestLongitude(pGps, '\0', NULL);
    }

    static void SetGpsDestBearing(GpsData* pGps, char ref, const Rational* pValue)
    {
        NN_ASSERT_(pGps);

        pGps->destBearingRef[0] = ref;
        pGps->destBearingRef[1] = '\0';

        if(pValue)
        {
            memcpy(&pGps->destBearing, pValue, sizeof(pGps->destBearing));
            pGps->isDestBearingValid = true;
        }
        else
        {
            pGps->isDestBearingValid = false;
        }
    }

    static void ClearGpsDestBearing(GpsData* pGps)
    {
        SetGpsDestBearing(pGps, '\0', NULL);
    }

    static void SetGpsDestDistance(GpsData* pGps, char ref, const Rational* pValue)
    {
        NN_ASSERT_(pGps);

        pGps->destDistanceRef[0] = ref;
        pGps->destDistanceRef[1] = '\0';

        if(pValue)
        {
            memcpy(&pGps->destDistance, pValue, sizeof(pGps->destDistance));
            pGps->isDestDistanceValid = true;
        }
        else
        {
            pGps->isDestDistanceValid = false;
        }
    }

    static void ClearGpsDestDistance(GpsData* pGps)
    {
        SetGpsDestDistance(pGps, '\0', NULL);
    }

    static void SetGpsProcessingMethod(GpsData* pGps, const u8* pProcessingMethod, size_t processingMethodSize)
    {
        NN_ASSERT_(pGps);

        if(pProcessingMethod && processingMethodSize)
        {
            pGps->pProcessingMethod = pProcessingMethod;
            pGps->processingMethodSize = processingMethodSize;
        }
        else
        {
            pGps->pProcessingMethod = NULL;
            pGps->processingMethodSize = 0;
        }
    }

    static void ClearGpsProcessingMethod(GpsData* pGps)
    {
        SetGpsProcessingMethod(pGps, NULL, 0);
    }

    static void SetGpsAreaInformation(GpsData* pGps, const u8* pAreaInformation, size_t areaInformationSize)
    {
        NN_ASSERT_(pGps);

        if(pAreaInformation && areaInformationSize)
        {
            pGps->pAreaInformation = pAreaInformation;
            pGps->areaInformationSize = areaInformationSize;
        }
        else
        {
            pGps->pAreaInformation = NULL;
            pGps->areaInformationSize = 0;
        }
    }

    static void ClearGpsAreaInformation(GpsData* pGps)
    {
        SetGpsAreaInformation(pGps, NULL, 0);
    }

    static void SetGpsDateStamp(GpsData* pGps, const char* pDateStamp)
    {
        NN_ASSERT_(pGps);

        pGps->pDateStamp = pDateStamp;
    }

    static void ClearGpsDateStamp(GpsData* pGps)
    {
        SetGpsDateStamp(pGps, NULL);
    }

    static void SetGpsDifferential(GpsData* pGps, u16 differential)
    {
        NN_ASSERT_(pGps);

        pGps->differential = differential;
        pGps->isDifferentialValid = true;
    }

    static void ClearGpsDifferential(GpsData* pGps)
    {
        NN_ASSERT_(pGps);

        pGps->isDifferentialValid = false;
    }

    void SetGpsData(const GpsData* pGps)
    {
        if(m_Initialized)
        {
            m_TemporarySetting.pGpsData = pGps;
        }
    }

    void ClearGpsData()
    {
        SetGpsData(NULL);
    }

    static void DoNotCallMe4() {}

    size_t StartJpegEncoder(u8* dst, size_t limit, const void* src, u32 width, u32 height, u32 quality,
                            PixelSampling dstPixelSampling, PixelFormat srcPixelFormat, bool addThumbnail);

    size_t StartMpEncoderLR(u8* dst, size_t limit, const void* srcL, const void* srcR, u32 width, u32 height,
                            u32 quality, PixelSampling dstPixelSampling, PixelFormat srcPixelFormat, bool addThumbnailL,
                            bool addThumbnailR);

    size_t StartMpEncoderFirst(u8* dst, size_t limit, const void* src, u32 width, u32 height, u32 quality,
                               PixelSampling dstPixelSampling, PixelFormat srcPixelFormat, bool addThumbnail,
                               u32 numImages = 2, MpTypeCode typeCode = MP_TYPE_CODE_MULTI_VIEW_DISPARITY_IMAGE,
                               bool addImageUidList = false, bool addTotalFrames = false);

    size_t StartMpEncoderNext(const void* src, u32 width, u32 height, u32 quality, PixelSampling dstPixelSampling,
                              PixelFormat srcPixelFormat, bool addThumbnail,
                              MpTypeCode typeCode = MP_TYPE_CODE_MULTI_VIEW_DISPARITY_IMAGE,
                              bool omitPixelDimensions = false);

    bool GetMpRegionsToBuildJpegData(MpRegionsToBuildJpegData* pBuffer);

    s32 GetLastError() const;

protected:
    detail::JpegMpEncoderWorkObj* m_pWork;
    bool m_Initialized;
    bool m_Padding[3];

    detail::JpegMpEncoderTemporarySettingObj m_TemporarySetting;
    void ClearTemporarySetting();

    void SetMakerNote(const u8* pBuffer, size_t size, u32 index);
};

}
}
}

#endif
