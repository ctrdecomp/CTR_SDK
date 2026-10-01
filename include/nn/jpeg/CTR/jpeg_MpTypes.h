#ifndef NN_JPEG_JPEGMPTYPES_H_
#define NN_JPEG_JPEGMPTYPES_H_

#include <nn/types.h>
#include <nn/assert.h>

#ifdef __cplusplus

namespace nn
{
namespace jpeg
{
namespace CTR
{

const size_t DATE_TIME_SIZE = 20;
const size_t IMAGE_UID_SIZE = 33;

const size_t MPF_VERSION_SIZE = 4;
const size_t MP_ENTRY_SIZE = 16;

const size_t GPS_VERSION_ID_SIZE = 4;
const size_t GPS_REF_SIZE = 2;
const size_t GPS_DATE_STAMP_SIZE = 11;

const u32 NUM_GPS_LATITUDE_RATIONALS = 3;
const u32 NUM_GPS_LONGITUDE_RATIONALS = 3;
const u32 NUM_GPS_TIME_STAMP_RATIONALS = 3;

const u32 NUM_RATIONAL_WORDS = 2;
const u32 NUM_SRATIONAL_WORDS = 2;

const u32 MIN_DECODER_SHRINK_LEVEL = 1;
const u32 MAX_DECODER_SHRINK_LEVEL = 4;

const size_t TWL_PHOTO_MAKER_NOTE_SIZE = 8;

const u32 MAX_DECODER_OUTPUT_BUFFER_WIDTH = 65536;
const u32 MAX_ENCODER_INPUT_BUFFER_WIDTH = 65536;

enum PixelSampling
{
    PIXEL_SAMPLING_YUV444 = 1,
    PIXEL_SAMPLING_YUV420 = 2,
    PIXEL_SAMPLING_YUV422 = 3,

    NUM_PIXEL_SAMPLINGS
};

const PixelSampling DEFAULT_THUMBNAIL_PIXEL_SAMPLING = PIXEL_SAMPLING_YUV422;

const u32 DEFAULT_THUMBNAIL_WIDTH = 160;
const u32 DEFAULT_THUMBNAIL_HEIGHT = 120;

enum PixelFormat
{
    PIXEL_FORMAT_YUYV8 = 0,
    PIXEL_FORMAT_CTR_RGB565 = 1,
    PIXEL_FORMAT_CTR_RGB565_BLOCK8 = 2,
    PIXEL_FORMAT_RGB8 = 3,
    PIXEL_FORMAT_CTR_RGB8_BLOCK8 = 4,
    PIXEL_FORMAT_RGBA8 = 5,
    PIXEL_FORMAT_CTR_RGBA8_BLOCK8 = 6,
    PIXEL_FORMAT_BGR8 = 7,
    PIXEL_FORMAT_ABGR8 = 8,

    NUM_PIXEL_FORMATS
};

enum MpTypeFlag
{
    MP_TYPE_FLAG_DEPENDENT_IMAGE_PARENT = 0x80000000,
    MP_TYPE_FLAG_DEPENDENT_IMAGE_CHILD = 0x40000000,
    MP_TYPE_FLAG_REPRESENTATIVE_IMAGE = 0x20000000
};

enum MpTypeDataFormat
{
    MP_TYPE_DATA_FORMAT_JPEG = 0x00000000,

    MP_TYPE_DATA_FORMAT_MASK = 0x07000000
};

enum MpTypeCode
{
    MP_TYPE_CODE_BASELINE_MP_PRIMARY_IMAGE = 0x030000,

    MP_TYPE_CODE_LARGE_THUMBNAIL_IMAGE_CLASS_1 = 0x010001,
    MP_TYPE_CODE_LARGE_THUMBNAIL_IMAGE_CLASS_2 = 0x010002,

    MP_TYPE_CODE_MULTI_VIEW_PANORAMA_IMAGE = 0x020001,
    MP_TYPE_CODE_MULTI_VIEW_DISPARITY_IMAGE = 0x020002,
    MP_TYPE_CODE_MULTI_VIEW_MULTI_ANGLE_IMAGE = 0x020003,

    MP_TYPE_CODE_UNDEFINED = 0x000000,

    MP_TYPE_CODE_MASK = 0xffffff
};

struct MpIndex
{
    const u8* src;
    size_t srcSize;
    size_t mpEntrySize;
    size_t imageUidListSize;
    size_t offsetToMpEntry;
    size_t offsetToImageUidList;
    size_t offsetToMpIndexIfd;
    size_t offsetToBase;

    bool isLittleEndian;
    bool isMpfVersionValid;
    bool isNumberOfImagesValid;
    bool isTotalFramesValid;

    u8 mpfVersion[MPF_VERSION_SIZE];
    u32 numberOfImages;
    u32 totalFrames;
};

struct MpEntry
{
    u32 type;
    size_t imageDataSize;
    size_t offsetToImageData;
    u16 dependentImage1EntryNum;
    u16 dependentImage2EntryNum;
    const u8* pFirstImageData;
};

struct Rational
{
    u32 value[NUM_RATIONAL_WORDS];
};

struct Srational
{
    s32 value[NUM_SRATIONAL_WORDS];
};

struct MpAttribute
{
    bool isMpfVersionValid;
    bool isMpIndividualNumValid;
    bool isPanOrientationValid;
    bool isPanOverlapHValid;

    bool isPanOverlapVValid;
    bool isBaseViewpointNumValid;
    bool isConvergenceAngleValid;
    bool isBaselineLengthValid;

    bool isVerticalDivergenceValid;
    bool isAxisDistanceXValid;
    bool isAxisDistanceYValid;
    bool isAxisDistanceZValid;

    bool isYawAngleValid;
    bool isPitchAngleValid;
    bool isRollAngleValid;
    u8 reserved[1];

    u8 mpfVersion[MPF_VERSION_SIZE];
    u32 mpIndividualNum;
    u32 panOrientation;
    Rational panOverlapH;
    Rational panOverlapV;
    u32 baseViewpointNum;
    Srational convergenceAngle;
    Rational baselineLength;
    Srational verticalDivergence;
    Srational axisDistanceX;
    Srational axisDistanceY;
    Srational axisDistanceZ;
    Srational yawAngle;
    Srational pitchAngle;
    Srational rollAngle;
};

struct MpRegionsToBuildJpegData
{
    const u8* pSrc1;
    size_t size1;
    const u8* pSrc2;
    size_t size2;
};

struct GpsData
{
    bool isVersionIdValid;
    bool isLatitudeValid;
    bool isLongitudeValid;
    bool isAltitudeRefValid;

    bool isAltitudeValid;
    bool isTimeStampValid;
    bool isDopValid;
    bool isSpeedValid;

    bool isTrackValid;
    bool isImgDirectionValid;
    bool isDestLatitudeValid;
    bool isDestLongitudeValid;

    bool isDestBearingValid;
    bool isDestDistanceValid;
    bool isDifferentialValid;

    u8 altitudeRef;

    u8 versionId[GPS_VERSION_ID_SIZE];

    char latitudeRef[GPS_REF_SIZE];
    char longitudeRef[GPS_REF_SIZE];

    char status[GPS_REF_SIZE];
    char measureMode[GPS_REF_SIZE];

    char speedRef[GPS_REF_SIZE];
    char trackRef[GPS_REF_SIZE];

    char imgDirectionRef[GPS_REF_SIZE];
    char destLatitudeRef[GPS_REF_SIZE];

    char destLongitudeRef[GPS_REF_SIZE];
    char destBearingRef[GPS_REF_SIZE];

    char destDistanceRef[GPS_REF_SIZE];
    u16 differential;

    Rational latitude[NUM_GPS_LATITUDE_RATIONALS];
    Rational longitude[NUM_GPS_LONGITUDE_RATIONALS];
    Rational altitude;
    Rational timeStamp[NUM_GPS_TIME_STAMP_RATIONALS];
    const char* pSatellites;
    Rational dop;
    Rational speed;
    Rational track;
    Rational imgDirection;
    const char* pMapDatum;
    Rational destLatitude[NUM_GPS_LATITUDE_RATIONALS];
    Rational destLongitude[NUM_GPS_LONGITUDE_RATIONALS];
    Rational destBearing;
    Rational destDistance;
    const u8* pProcessingMethod;
    size_t processingMethodSize;
    const u8* pAreaInformation;
    size_t areaInformationSize;
    const char* pDateStamp;
};

enum EncoderErrorCode
{
    JPEG_ENCODER_ERROR_NONE = 0,
    JPEG_ENCODER_ERROR_NOT_INITIALIZED = -1,
    JPEG_ENCODER_ERROR_INVALID_ARGUMENT = -2,
    JPEG_ENCODER_ERROR_SRC_ALIGNMENT = -3,
    JPEG_ENCODER_ERROR_APP1 = -4,
    JPEG_ENCODER_ERROR_APP2_MP = -5,
    JPEG_ENCODER_ERROR_INVALID_MP_NUM_IMAGES = -6,
    JPEG_ENCODER_ERROR_INVALID_MP_TYPE_CODE = -7,
    JPEG_ENCODER_ERROR_INVALID_MP_SEQUENCE = -8,
    JPEG_ENCODER_ERROR_INSUFFICIENT_OUTPUT_BUFFER = -9,

    JPEG_ENCODER_ERROR_INTERNAL = -127
};

enum DecoderErrorCode
{
    JPEG_DECODER_ERROR_NONE = 0,
    JPEG_DECODER_ERROR_NOT_INITIALIZED = -1,
    JPEG_DECODER_ERROR_INVALID_ARGUMENT = -2,
    JPEG_DECODER_ERROR_DST_ALIGNMENT = -3,
    JPEG_DECODER_ERROR_DST_BUFFER_SIZE = -4,

    JPEG_DECODER_ERROR_STOPPED = -10,

    JPEG_DECODER_ERROR_WIDTH_HEIGHT = -20,
    JPEG_DECODER_ERROR_WIDTH_HEIGHT_DIFFERENT = -21,

    JPEG_DECODER_ERROR_EXIF = -30,
    JPEG_DECODER_ERROR_THUMBNAIL = -31,

    JPEG_DECODER_ERROR_MP = -32,
    JPEG_DECODER_ERROR_MP_NOT_FOUND = -33,

    JPEG_DECODER_ERROR_MARKER_COMBINATION = -50,

    JPEG_DECODER_ERROR_SOI = -60,
    JPEG_DECODER_ERROR_SOF = -61,
    JPEG_DECODER_ERROR_SOF_BLOCK_ID = -62,
    JPEG_DECODER_ERROR_DHT = -63,
    JPEG_DECODER_ERROR_SOS = -64,
    JPEG_DECODER_ERROR_DQT = -65,
    JPEG_DECODER_ERROR_DRI = -66,
    JPEG_DECODER_ERROR_SOS_NOT_FOUND = -67,
    JPEG_DECODER_ERROR_SOF_NOT_FOUND = -68,
    JPEG_DECODER_ERROR_DQT_NOT_FOUND = -69,
    JPEG_DECODER_ERROR_DHT_NOT_FOUND = -70,

    JPEG_DECODER_ERROR_UNDERRUN_0 = -90,
    JPEG_DECODER_ERROR_UNDERRUN_1 = -91,
    JPEG_DECODER_ERROR_UNDERRUN_2 = -92,
    JPEG_DECODER_ERROR_UNDERRUN_3 = -93,
    JPEG_DECODER_ERROR_UNDERRUN_4 = -94,
    JPEG_DECODER_ERROR_UNDERRUN_5 = -95,
    JPEG_DECODER_ERROR_UNDERRUN_6 = -96,

    JPEG_DECODER_ERROR_RANGE_0 = -110,
    JPEG_DECODER_ERROR_RANGE_1 = -111,
    JPEG_DECODER_ERROR_RANGE_2 = -112,
    JPEG_DECODER_ERROR_RANGE_3 = -113,
    JPEG_DECODER_ERROR_RANGE_4 = -114,
    JPEG_DECODER_ERROR_RANGE_5 = -115,

    JPEG_DECODER_ERROR_HLB_0 = -120,

    JPEG_DECODER_ERROR_INTERNAL = -127
};

enum EncoderOption
{
    JPEG_ENCODER_OPTION_NONE = 0,
    JPEG_ENCODER_OPTION_OMIT_APP1 = 0x80000000,
    JPEG_ENCODER_OPTION_OMIT_DHT = 0x40000000
};

enum DecoderOption
{
    JPEG_DECODER_OPTION_NONE = 0,
    JPEG_DECODER_OPTION_ENABLE_DEFAULT_DHT = 0x00000001,
    JPEG_DECODER_OPTION_MATCH_WIDTH_HEIGHT = 0x00000002
};

namespace detail
{
const size_t TWL_PRIVATE_DATA_SIZE = 28;
const size_t MAX_TWL_PHOTO_DATA_SIZE = (200 * 1024 - 1);

enum
{
    MAKER_NOTE_INDEX_USER = 0,
    MAKER_NOTE_INDEX_SYS_1,
    MAKER_NOTE_INDEX_SYS_2,
    MAKER_NOTE_INDEX_SYS_3,

    NUM_MAKER_NOTES
};

struct JpegMpEncoderTemporarySettingObj
{
    u16 dependentImage1EntryNum;
    u16 dependentImage2EntryNum;
    u16 orientation;
    bool isOrientationSet;
    bool padding[1];
    u32 thumbnailWidth;
    u32 thumbnailHeight;
    u32 inputBufferWidth;
    u32 option;
    struct
    {
        const u8* pData;
        size_t size;
    } makerNotes[NUM_MAKER_NOTES];
    const u8* pTwlPhotoMakerNoteData;
    const char* pSoftware;
    const GpsData* pGpsData;
    char dateTimeBuffer[DATE_TIME_SIZE];
    char imageUidBuffer[IMAGE_UID_SIZE];
    bool isImageUidSet;
    bool isDateTimeSet;
    PixelSampling thumbnailSampling;

    bool isDependentParent;
    bool isDependentChild;
    bool isRepresentativeSet;
    bool isRepresentative;
    MpAttribute mpAttribute;
};

struct JpegMpDecoderTemporarySettingObj
{
    u32 outputBufferWidth;
    u32 option;
};
}

}
}
}

#endif

#endif
