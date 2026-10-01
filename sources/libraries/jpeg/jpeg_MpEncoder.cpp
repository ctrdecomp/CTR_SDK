// Filename: jpeg_MpEncoder.cpp
//
// Project: Horizon

#include <nn/jpeg/CTR/jpeg_MpEncoder.h>
#include <nn/jpeg/CTR/jpeg_MpEncoderS.h>
#include <nn/fnd.h>
#include <nn/nstd.h>
#include "exif_MpEncoder.h"
#include <string.h>

namespace nn {
namespace jpeg {
namespace CTR {

namespace
{

const s16 s_Coef565R2cbMul = -710;
const s16 s_Coef565G2cbMul = -1373;
const s16 s_Coef565R2crMul_B2cbMul = 2106;
const s16 s_Coef565G2crMul = -1735;
const s16 s_Coef565R2yMul = 1259;
const s16 s_Coef565G2yMul = 2433;
const s16 s_Coef565B2yMul = 480;
const s16 s_Coef565B2crMul = -342;
const s32 s_Coef565Rgb2yAdd = 2048;
const s32 s_Coef565Rgb2cbcrAdd = 0x80000;

const s16 s_Coef8R2cbMul = -691;
const s16 s_Coef8G2cbMul = -1357;
const s16 s_Coef8R2crMul_B2cbMul = 2048;
const s16 s_Coef8G2crMul = -1715;
const s16 s_Coef8R2yMul = 1225;
const s16 s_Coef8G2yMul = 2404;
const s16 s_Coef8B2yMul = 467;
const s16 s_Coef8B2crMul = -333;
const s32 s_Coef8Rgb2yAdd = 2048;
const s32 s_Coef8Rgb2cbcrAdd = 0x80000;

}

#include "jpeg_MpEncoderAsmArmv6.inc"

void JpegMpEncoderCResampleYuyv8(JpegMpEncoderContext* pCtx, u32 col, u32 row);
void JpegMpEncoderCResampleCtrRgb565(JpegMpEncoderContext* pCtx, u32 col, u32 row);
void JpegMpEncoderCResampleCtrRgb565Block8(JpegMpEncoderContext* pCtx, u32 col, u32 row);
void JpegMpEncoderCResampleRgb8(JpegMpEncoderContext* pCtx, u32 col, u32 row);
void JpegMpEncoderCResampleCtrRgb8Block8(JpegMpEncoderContext* pCtx, u32 col, u32 row);
void JpegMpEncoderCResampleRgba8(JpegMpEncoderContext* pCtx, u32 col, u32 row);
void JpegMpEncoderCResampleCtrRgba8Block8(JpegMpEncoderContext* pCtx, u32 col, u32 row);
void JpegMpEncoderCResampleBgr8(JpegMpEncoderContext* pCtx, u32 col, u32 row);
void JpegMpEncoderCResampleAbgr8(JpegMpEncoderContext* pCtx, u32 col, u32 row);

namespace
{
inline u32 RoundUp4B(u32 i)
{
    return (i + 0x3) & ~0x3;
}

#ifdef JPEG_ENCODER_USE_DHT_CACHE
static const u8 s_DhtData[] NN_ATTRIBUTE_ALIGN(32) = {
    0xFF, 0xC4, 0x01, 0xA2, 0x00, 0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x01, 0x00, 0x03, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x07, 0x08, 0x09, 0x0A, 0x0B, 0x10, 0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03, 0x05, 0x05, 0x04, 0x04, 0x00,
    0x00, 0x01, 0x7D, 0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
    0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xA1, 0x08, 0x23, 0x42, 0xB1, 0xC1, 0x15, 0x52, 0xD1, 0xF0, 0x24, 0x33, 0x62,
    0x72, 0x82, 0x09, 0x0A, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A,
    0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x83, 0x84, 0x85,
    0x86, 0x87, 0x88, 0x89, 0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6,
    0xA7, 0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
    0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
    0xE8, 0xE9, 0xEA, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0x11, 0x00, 0x02, 0x01, 0x02, 0x04,
    0x04, 0x03, 0x04, 0x07, 0x05, 0x04, 0x04, 0x00, 0x01, 0x02, 0x77, 0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
    0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71, 0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xA1, 0xB1, 0xC1,
    0x09, 0x23, 0x33, 0x52, 0xF0, 0x15, 0x62, 0x72, 0xD1, 0x0A, 0x16, 0x24, 0x34, 0xE1, 0x25, 0xF1, 0x17, 0x18, 0x19,
    0x1A, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
    0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x73, 0x74,
    0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x92, 0x93, 0x94, 0x95,
    0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6,
    0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
    0xD8, 0xD9, 0xDA, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
    0xF9, 0xFA};

static const JpegMpEncoderHuffmanStructure s_DhtPresetDc0 = {

    {0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

    {0x0000, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x000e, 0x001e, 0x003e, 0x007e, 0x00fe, 0x01fe, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000}};

static const JpegMpEncoderHuffmanStructure s_DhtPresetDc1 = {

    {0x02, 0x02, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

    {0x0000, 0x0001, 0x0002, 0x0006, 0x000e, 0x001e, 0x003e, 0x007e, 0x00fe, 0x01fe, 0x03fe, 0x07fe, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000}};

static const JpegMpEncoderHuffmanStructure s_DhtPresetAc0 = {

    {0x04, 0x02, 0x02, 0x03, 0x04, 0x05, 0x07, 0x08, 0x0A, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x05,
     0x07, 0x09, 0x0B, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x08, 0x0A, 0x0C, 0x10,
     0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x09, 0x0C, 0x10, 0x10, 0x10, 0x10, 0x10,
     0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x0A, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0B, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x07, 0x0C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
     0x0C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x0F, 0x10, 0x10,
     0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
     0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x0A, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x0B, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10,
     0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
     0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

    {0x000a, 0x0000, 0x0001, 0x0004, 0x000b, 0x001a, 0x0078, 0x00f8, 0x03f6, 0xff82, 0xff83, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x000c, 0x001b, 0x0079, 0x01f6, 0x07f6, 0xff84, 0xff85, 0xff86, 0xff87, 0xff88, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x001c, 0x00f9, 0x03f7, 0x0ff4, 0xff89, 0xff8a, 0xff8b, 0xff8c, 0xff8d,
     0xff8e, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x003a, 0x01f7, 0x0ff5, 0xff8f, 0xff90, 0xff91, 0xff92,
     0xff93, 0xff94, 0xff95, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x003b, 0x03f8, 0xff96, 0xff97, 0xff98,
     0xff99, 0xff9a, 0xff9b, 0xff9c, 0xff9d, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x007a, 0x07f7, 0xff9e,
     0xff9f, 0xffa0, 0xffa1, 0xffa2, 0xffa3, 0xffa4, 0xffa5, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x007b,
     0x0ff6, 0xffa6, 0xffa7, 0xffa8, 0xffa9, 0xffaa, 0xffab, 0xffac, 0xffad, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x00fa, 0x0ff7, 0xffae, 0xffaf, 0xffb0, 0xffb1, 0xffb2, 0xffb3, 0xffb4, 0xffb5, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x01f8, 0x7fc0, 0xffb6, 0xffb7, 0xffb8, 0xffb9, 0xffba, 0xffbb, 0xffbc, 0xffbd, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x01f9, 0xffbe, 0xffbf, 0xffc0, 0xffc1, 0xffc2, 0xffc3, 0xffc4, 0xffc5,
     0xffc6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x01fa, 0xffc7, 0xffc8, 0xffc9, 0xffca, 0xffcb, 0xffcc,
     0xffcd, 0xffce, 0xffcf, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x03f9, 0xffd0, 0xffd1, 0xffd2, 0xffd3,
     0xffd4, 0xffd5, 0xffd6, 0xffd7, 0xffd8, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x03fa, 0xffd9, 0xffda,
     0xffdb, 0xffdc, 0xffdd, 0xffde, 0xffdf, 0xffe0, 0xffe1, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x07f8,
     0xffe2, 0xffe3, 0xffe4, 0xffe5, 0xffe6, 0xffe7, 0xffe8, 0xffe9, 0xffea, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0xffeb, 0xffec, 0xffed, 0xffee, 0xffef, 0xfff0, 0xfff1, 0xfff2, 0xfff3, 0xfff4, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x07f9, 0xfff5, 0xfff6, 0xfff7, 0xfff8, 0xfff9, 0xfffa, 0xfffb, 0xfffc, 0xfffd, 0xfffe, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000}};

static const JpegMpEncoderHuffmanStructure s_DhtPresetAc1 = {

    {0x02, 0x02, 0x03, 0x04, 0x05, 0x05, 0x06, 0x07, 0x09, 0x0a, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x06,
     0x08, 0x09, 0x0b, 0x0c, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x08, 0x0a, 0x0c, 0x0f,
     0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x08, 0x0a, 0x0c, 0x10, 0x10, 0x10, 0x10,
     0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x09, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x0a, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x07, 0x0b, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
     0x0b, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x10, 0x10,
     0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
     0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x09, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x0b, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x10, 0x10,
     0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x0f, 0x10, 0x10, 0x10, 0x10, 0x10,
     0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

    {0x0000, 0x0001, 0x0004, 0x000a, 0x0018, 0x0019, 0x0038, 0x0078, 0x01f4, 0x03f6, 0x0ff4, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x000b, 0x0039, 0x00f6, 0x01f5, 0x07f6, 0x0ff5, 0xff88, 0xff89, 0xff8a, 0xff8b, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x001a, 0x00f7, 0x03f7, 0x0ff6, 0x7fc2, 0xff8c, 0xff8d, 0xff8e, 0xff8f,
     0xff90, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x001b, 0x00f8, 0x03f8, 0x0ff7, 0xff91, 0xff92, 0xff93,
     0xff94, 0xff95, 0xff96, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x003a, 0x01f6, 0xff97, 0xff98, 0xff99,
     0xff9a, 0xff9b, 0xff9c, 0xff9d, 0xff9e, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x003b, 0x03f9, 0xff9f,
     0xffa0, 0xffa1, 0xffa2, 0xffa3, 0xffa4, 0xffa5, 0xffa6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0079,
     0x07f7, 0xffa7, 0xffa8, 0xffa9, 0xffaa, 0xffab, 0xffac, 0xffad, 0xffae, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x007a, 0x07f8, 0xffaf, 0xffb0, 0xffb1, 0xffb2, 0xffb3, 0xffb4, 0xffb5, 0xffb6, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x00f9, 0xffb7, 0xffb8, 0xffb9, 0xffba, 0xffbb, 0xffbc, 0xffbd, 0xffbe, 0xffbf, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x01f7, 0xffc0, 0xffc1, 0xffc2, 0xffc3, 0xffc4, 0xffc5, 0xffc6, 0xffc7,
     0xffc8, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x01f8, 0xffc9, 0xffca, 0xffcb, 0xffcc, 0xffcd, 0xffce,
     0xffcf, 0xffd0, 0xffd1, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x01f9, 0xffd2, 0xffd3, 0xffd4, 0xffd5,
     0xffd6, 0xffd7, 0xffd8, 0xffd9, 0xffda, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x01fa, 0xffdb, 0xffdc,
     0xffdd, 0xffde, 0xffdf, 0xffe0, 0xffe1, 0xffe2, 0xffe3, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x07f9,
     0xffe4, 0xffe5, 0xffe6, 0xffe7, 0xffe8, 0xffe9, 0xffea, 0xffeb, 0xffec, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x3fe0, 0xffed, 0xffee, 0xffef, 0xfff0, 0xfff1, 0xfff2, 0xfff3, 0xfff4, 0xfff5, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x03fa, 0x7fc3, 0xfff6, 0xfff7, 0xfff8, 0xfff9, 0xfffa, 0xfffb, 0xfffc, 0xfffd, 0xfffe, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000}};
#endif
}

namespace
{

const u8 jfif[23] NN_ATTRIBUTE_ALIGN(32) = {0xFF, 0xC0, 0x00, 0x11, 0x08, 0x01, 0xE0, 0x02, 0x80, 0x03, 0x01, 0x11,
                                            0x00, 0x02, 0x11, 0x01, 0x03, 0x11, 0x01, 0xFF, 0xDB, 0x00, 0x84};
const u8 jsos[14] NN_ATTRIBUTE_ALIGN(32) = {0xFF, 0xDA, 0x00, 0x0C, 0x03, 0x01, 0x00,
                                            0x02, 0x11, 0x03, 0x11, 0x00, 0x3F, 0x00};
const u32 scaleTable[64] = {

    512, 369, 392, 435, 512, 652,  946,  1856, 369,  266,  283,  314,  369,  470,  682,  1338,
    392, 283, 300, 333, 392, 499,  724,  1420, 435,  314,  333,  370,  435,  554,  805,  1578,
    512, 369, 392, 435, 512, 652,  946,  1856, 652,  470,  499,  554,  652,  829,  1204, 2362,
    946, 682, 724, 805, 946, 1204, 1748, 3429, 1856, 1338, 1420, 1578, 1856, 2362, 3429, 6726};
const u8 luminance_quantization_table[64] = {16, 11, 10, 16, 24,  40,  51,  61,  12, 12, 14, 19, 26,  58,  60,  55,
                                             14, 13, 16, 24, 40,  57,  69,  56,  14, 17, 22, 29, 51,  87,  80,  62,
                                             18, 22, 37, 56, 68,  109, 103, 77,  24, 35, 55, 64, 81,  104, 113, 92,
                                             49, 64, 78, 87, 103, 121, 120, 101, 72, 92, 95, 98, 112, 100, 103, 99};
const u8 chrominance_quantization_table[64] = {17, 18, 24, 47, 99, 99, 99, 99, 18, 21, 26, 66, 99, 99, 99, 99,
                                               24, 26, 56, 99, 99, 99, 99, 99, 47, 66, 99, 99, 99, 99, 99, 99,
                                               99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
                                               99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99};
const u8 JpegZigZagInputOrderCodes[64] = {0,  1,  8,  16, 9,  2,  3,  10, 17, 24, 32, 25, 18, 11, 4,  5,
                                          12, 19, 26, 33, 40, 48, 41, 34, 27, 20, 13, 6,  7,  14, 21, 28,
                                          35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51,
                                          58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63};

#ifdef JPEG_ENCODER_USE_DHT_CACHE
#else

const u8 dc0_tbl[28] = {0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};

const u8 dc1_tbl[28] = {0x00, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};

const u8 ac0_tbl[178] = {
    0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03, 0x05, 0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7D, 0x01, 0x02,
    0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07, 0x22, 0x71, 0x14, 0x32,
    0x81, 0x91, 0xa1, 0x08, 0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0, 0x24, 0x33, 0x62, 0x72, 0x82, 0x09,
    0x0a, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63,
    0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x83, 0x84, 0x85,
    0x86, 0x87, 0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
    0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
    0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2, 0xe3, 0xe4,
    0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xF9, 0xFA};

const u8 ac1_tbl[178] = {
    0x00, 0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04, 0x07, 0x05, 0x04, 0x04, 0x00, 0x01, 0x02, 0x77, 0x00, 0x01,
    0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71, 0x13, 0x22, 0x32, 0x81,
    0x08, 0x14, 0x42, 0x91, 0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0, 0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16,
    0x24, 0x34, 0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
    0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a,
    0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x82, 0x83,
    0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3,
    0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
    0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe2, 0xe3,
    0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xF9, 0xFA};
#endif

const JpegMpEncoderConvertToYuvFunc BlockConvFuncTbl[NUM_PIXEL_FORMATS][NUM_PIXEL_SAMPLINGS - 1] = {
    {JpegMpEncoderAsm_Yuyv8ToYuv444, JpegMpEncoderAsm_Yuyv8ToYuv420, JpegMpEncoderAsm_Yuyv8ToYuv422},
    {JpegMpEncoderAsm_CtrRgb565ToYuv444, JpegMpEncoderAsm_CtrRgb565ToYuv420, JpegMpEncoderAsm_CtrRgb565ToYuv422},
    {JpegMpEncoderAsm_CtrRgb565Block8ToYuv444, JpegMpEncoderAsm_CtrRgb565Block8ToYuv420,
     JpegMpEncoderAsm_CtrRgb565Block8ToYuv422},
    {JpegMpEncoderAsm_Rgb8ToYuv444, JpegMpEncoderAsm_Rgb8ToYuv420, JpegMpEncoderAsm_Rgb8ToYuv422},
    {JpegMpEncoderAsm_CtrRgb8Block8ToYuv444, JpegMpEncoderAsm_CtrRgb8Block8ToYuv420,
     JpegMpEncoderAsm_CtrRgb8Block8ToYuv422},
    {JpegMpEncoderAsm_Rgba8ToYuv444, JpegMpEncoderAsm_Rgba8ToYuv420, JpegMpEncoderAsm_Rgba8ToYuv422},
    {JpegMpEncoderAsm_CtrRgba8Block8ToYuv444, JpegMpEncoderAsm_CtrRgba8Block8ToYuv420,
     JpegMpEncoderAsm_CtrRgba8Block8ToYuv422},
    {JpegMpEncoderAsm_Bgr8ToYuv444, JpegMpEncoderAsm_Bgr8ToYuv420, JpegMpEncoderAsm_Bgr8ToYuv422},
    {JpegMpEncoderAsm_Abgr8ToYuv444, JpegMpEncoderAsm_Abgr8ToYuv420, JpegMpEncoderAsm_Abgr8ToYuv422}};

const JpegMpEncoderConvertToYuvFunc ThumbnailConvFuncTbl[NUM_PIXEL_FORMATS] = {
    JpegMpEncoderCResampleYuyv8,          JpegMpEncoderCResampleCtrRgb565,     JpegMpEncoderCResampleCtrRgb565Block8,
    JpegMpEncoderCResampleRgb8,           JpegMpEncoderCResampleCtrRgb8Block8, JpegMpEncoderCResampleRgba8,
    JpegMpEncoderCResampleCtrRgba8Block8, JpegMpEncoderCResampleBgr8,          JpegMpEncoderCResampleAbgr8};

}

bool CheckEncoderWidthHeight(u32 width, u32 height, PixelSampling dstPixelSampling)
{
    if(!width || width > 65535 || !height || height > 65535)
    {
        goto error;
    }

    switch(dstPixelSampling)
    {
    case PIXEL_SAMPLING_YUV444:
        if((width & 7) || (height & 7))
        {
            goto error;
        }
        break;

    case PIXEL_SAMPLING_YUV420:
        if((width & 0xF) || (height & 0xF))
        {
            goto error;
        }
        break;

    case PIXEL_SAMPLING_YUV422:
        if((width & 0xF) || (height & 7))
        {
            goto error;
        }
        break;

    default:

        goto error;
    }

    return true;

error:
    return false;
}

inline void SetJpegMpEncoderError(JpegMpEncoderContext* pCtx, s8 errorCode)
{
    if(!pCtx->errorCode)
    {

        pCtx->errorCode = errorCode;
    }
}

inline void UpdateJpegMpEncoderAsmWork(JpegMpEncoderContext* pCtx)
{

    pCtx->asmWork.inputBufferWidthM8x2 = static_cast<u32>((pCtx->inputBufferWidth - 8) * 2);
    pCtx->asmWork.inputBufferWidthM16x2 = static_cast<u32>((pCtx->inputBufferWidth - 16) * 2);
    pCtx->asmWork.inputBufferWidthM8x3 = static_cast<u32>((pCtx->inputBufferWidth - 8) * 3);
    pCtx->asmWork.inputBufferWidthM16x3 = static_cast<u32>((pCtx->inputBufferWidth - 16) * 3);
    pCtx->asmWork.inputBufferWidthM8x4 = static_cast<u32>((pCtx->inputBufferWidth - 8) * 4);
    pCtx->asmWork.inputBufferWidthM16x4 = static_cast<u32>((pCtx->inputBufferWidth - 16) * 4);
    pCtx->asmWork.inputBufferWidthX3 = pCtx->inputBufferWidth * 3;
}

void InitializeJpegMpEncoderQTable(JpegMpEncoderContext* pCtx)
{
    u32 quality = pCtx->quality;
    u32 x;
    u32 y;

    if(quality < 50)
    {
        quality = 5000 / quality;
    }
    else
    {
        quality = 200 - quality * 2;
    }

    for(y = 0; y < NUM_JPEG_ENCODER_Q_TBL_ELEMS; y++)
    {
        x = (luminance_quantization_table[y] * quality + 50) / 100;
        if(x <= 0)
        {
            x = 1;
        }
        if(x > 255)
        {
            x = 255;
        }
        pCtx->dqBuf[y] = static_cast<u8>(x);
        pCtx->qBuf[y] = scaleTable[y] / static_cast<u8>(x);
        x = (chrominance_quantization_table[y] * quality + 50) / 100;
        if(x <= 0)
        {
            x = 1;
        }
        if(x > 255)
        {
            x = 255;
        }
        pCtx->dqBuf[y + 64] = static_cast<u8>(x);
        pCtx->qBuf[y + 64] = scaleTable[y] / x;
    }
}

#ifdef JPEG_ENCODER_USE_DHT_CACHE
#else
void Yos_JpegMpEncoderHuffmanEncoderBuildTable(JpegMpEncoderHuffmanEncoderBuildTableWork* pBuildTableWork,
                                               JpegMpEncoderHuffmanStructure* hf, u32 idx)
{
    u32 ii, jj, kk, si;
    u16 code = 0;
    const u8* pHuffBits;
    const u8* pHuffValues;

    memset(hf, 0, sizeof(*hf));
    memset(pBuildTableWork, 0, sizeof(*pBuildTableWork));

    switch(idx)
    {
    case 0:
        pHuffBits = dc0_tbl;
        break;
    case 1:
        pHuffBits = dc1_tbl;
        break;
    case 2:
        pHuffBits = ac0_tbl;
        break;
    case 3:
        pHuffBits = ac1_tbl;
        break;

    default:

        NN_PANIC("jpeg library error");
        return;
    }

    pHuffValues = pHuffBits + JPEG_ENCODER_MAX_HUFFMAN_CODE_LENGTH;

    for(ii = 0, kk = 0; ii < JPEG_ENCODER_MAX_HUFFMAN_CODE_LENGTH; ii++)
    {
        for(jj = 0; jj < pHuffBits[ii]; jj++)
        {
            pBuildTableWork->s.huffSizes[kk] = ii + 1;
            kk++;
        }
        pBuildTableWork->s.huffSizes[kk] = 0;
    }

    for(kk = 0, si = pBuildTableWork->s.huffSizes[0]; pBuildTableWork->s.huffSizes[kk] != 0; si++, code <<= 1)
    {
        for(; pBuildTableWork->s.huffSizes[kk] == si; code++, kk++)
        {
            pBuildTableWork->s.huffCodes[kk] = code;
        }
    }

    for(kk = 0; kk < JPEG_ENCODER_MAX_NUMBER_OF_HUFFMAN_CODES; kk++)
    {
        if(pBuildTableWork->s.huffSizes[kk] != 0)
        {
            ii = pHuffValues[kk];
            hf->mEhufco[ii] = pBuildTableWork->s.huffCodes[kk];
            hf->mEhufsi[ii] = pBuildTableWork->s.huffSizes[kk];
        }
    }
}

size_t Yos_JpegMpEncoderHuffmanEncoderOutputSize(const JpegMpEncoderHuffmanStructure* hf)
{
    u32 i;
    u32 count = 0;

    for(i = 0; i < JPEG_ENCODER_MAX_HUFFMAN_CODE_LENGTH; i++)
    {
        count += hf->mHuffBits[i];
    }

    return (count + JPEG_ENCODER_MAX_HUFFMAN_CODE_LENGTH + 1);
}

u8* Yos_JpegMpEncoderHuffmanEncoderPrintTable(u8* pDst, const JpegMpEncoderHuffmanStructure* hf)
{
    u32 i;
    u32 count = 0;

    for(i = 0; i < JPEG_ENCODER_MAX_HUFFMAN_CODE_LENGTH; i++)
    {
        count += hf->mHuffBits[i];
        *pDst++ = hf->mHuffBits[i];
    }

    for(i = 0; i < count; i++)
    {
        *pDst++ = hf->mHuffValues[i];
    }

    return pDst;
}
#endif

#if 0

bool Yos_JpegMpEncoderComponentSequential(JpegMpEncoderContext* pCtx, JpegMpEncoderComponentStructure* component)
{
	u32 index,ssss,zerorun;
	s32 bits;
	s16 *du;
	u32 huffcode;
	u32 huffsize;
	s32 diff;
	JpegMpEncoderHuffmanStructure* dc = component->mDCTable;
	JpegMpEncoderHuffmanStructure* ac = component->mACTable;

	du = &component->mCoefficients[0];

	diff = du [0] - component->mLastDcValue;
	component->mLastDcValue = du[0];

	if (diff >= 0)bits = diff ;
	else{
		diff = -diff ;
		bits = ~diff ;
	}
	for(ssss = 0; diff != 0 ;){
		ssss++;
		diff >>= 1 ;
	}

	huffcode = dc->mEhufco[ssss];
	huffsize = dc->mEhufsi[ssss];
	if(!Yos_BitWrite(pCtx,huffcode,huffsize))
	{
		return false;
	}
	if(ssss)
	{
		if(!Yos_BitWrite(pCtx,static_cast<u32>(bits),ssss))
		    return false;
	}

	zerorun = 0 ;
	for(index = 1 ; index < 64 ; index++){
		if(du[JpegZigZagInputOrderCodes[index]] != 0){
			int value;
			u32 rrrrssss;

			while(zerorun >= 16){
				huffcode = ac->mEhufco[0xF0];
				huffsize = ac->mEhufsi[0xF0];
				if(!Yos_BitWrite(pCtx,huffcode,huffsize))
				    return false;
				zerorun -= 16 ;
			}

			value = du [JpegZigZagInputOrderCodes[index]] ;
			if (value >= 0)bits = value ;
			else{
				value = -value ;
				bits = ~value ;
			}
			ssss = 0 ;
			while(value != 0){
				value >>= 1 ;
				ssss++;
			}

			rrrrssss = (zerorun << 4) | ssss ;
			huffcode = ac->mEhufco[rrrrssss];
			huffsize = ac->mEhufsi[rrrrssss];
			if(!Yos_BitWrite(pCtx,huffcode,huffsize))
			    return false;
			if(ssss)
			{
				if(!Yos_BitWrite(pCtx,static_cast<u32>(bits),ssss))
				    return false;
			}
			zerorun = 0 ;
		}
		else zerorun++;
	}
	if (zerorun > 0){
		huffcode = ac->mEhufco[0];
		huffsize = ac->mEhufsi[0];
		if(!Yos_BitWrite(pCtx,huffcode,huffsize))
		    return false;
	}
	return true;
}
#endif

bool InitializeJpegMpEncoderContext(JpegMpEncoderContext* pCtx,
#ifdef JPEG_ENCODER_USE_DHT_CACHE

#else
                                    JpegMpEncoderHuffmanEncoderBuildTableWork* pBuildTableWork,
#endif
                                    u8* dst, size_t limit, const void* src, u32 width, u32 height, u32 quality,
                                    PixelSampling dstPixelSampling, PixelFormat srcPixelFormat, u32 inputBufferWidth,
                                    const detail::JpegMpEncoderTemporarySettingObj* pTempSetting)
{
    u32 i;

#ifdef JPEG_ENCODER_USE_DQT_CACHE

    u32 lastQuality = pCtx->quality;

    if(lastQuality && (lastQuality == quality))
    {

        memset(pCtx, 0, offsetof(JpegMpEncoderContext, dqBuf));
    }
    else
    {
        memset(pCtx, 0, sizeof(*pCtx));
    }
#else
    memset(pCtx, 0, sizeof(*pCtx));
#endif

    width &= 0xffff;
    height &= 0xffff;
    pCtx->width = static_cast<u16>(width);
    pCtx->height = static_cast<u16>(height);

    pCtx->inputBufferWidth = inputBufferWidth;

    pCtx->pSrc = src;
    pCtx->pDst = dst;
    pCtx->writeLimit = limit;

    if(!quality || quality > 100)
    {
        SetJpegMpEncoderError(pCtx, JPEG_ENCODER_ERROR_INVALID_ARGUMENT);
        goto error;
    }
    pCtx->quality = static_cast<u8>(quality);

    switch(dstPixelSampling)
    {
    case PIXEL_SAMPLING_YUV444:
        pCtx->samplingX = pCtx->samplingY = 1;
        break;
    case PIXEL_SAMPLING_YUV420:
        pCtx->samplingX = pCtx->samplingY = 2;
        break;
    case PIXEL_SAMPLING_YUV422:
        pCtx->samplingX = 2;
        pCtx->samplingY = 1;
        break;
    default:

        SetJpegMpEncoderError(pCtx, JPEG_ENCODER_ERROR_INTERNAL);
        goto error;
    }
    pCtx->loops = pCtx->samplingX * pCtx->samplingY;

    pCtx->mcu_cols = pCtx->width / (8 * pCtx->samplingX);
    pCtx->mcu_rows = pCtx->height / (8 * pCtx->samplingY);

    pCtx->dstPixelSampling = static_cast<u8>(dstPixelSampling);

    if(srcPixelFormat >= NUM_PIXEL_FORMATS)
    {
        SetJpegMpEncoderError(pCtx, JPEG_ENCODER_ERROR_INVALID_ARGUMENT);
        goto error;
    }
    pCtx->srcPixelFormat = static_cast<u8>(srcPixelFormat);

    pCtx->convFunc = BlockConvFuncTbl[srcPixelFormat][dstPixelSampling - 1];
    if(!pCtx->convFunc)
    {

        SetJpegMpEncoderError(pCtx, JPEG_ENCODER_ERROR_INTERNAL);
        goto error;
    }

#ifdef JPEG_ENCODER_USE_DHT_CACHE
#else
    if(pBuildTableWork)
    {
        Yos_JpegMpEncoderHuffmanEncoderBuildTable(pBuildTableWork, &pCtx->dcTbl[0], 0);
        Yos_JpegMpEncoderHuffmanEncoderBuildTable(pBuildTableWork, &pCtx->acTbl[0], 2);
        Yos_JpegMpEncoderHuffmanEncoderBuildTable(pBuildTableWork, &pCtx->dcTbl[1], 1);
        Yos_JpegMpEncoderHuffmanEncoderBuildTable(pBuildTableWork, &pCtx->acTbl[1], 3);
    }
#endif

    UpdateJpegMpEncoderAsmWork(pCtx);

    for(i = 0; i < NUM_JPEG_ENCODER_COMPONENTS; i++)
    {
#ifdef JPEG_ENCODER_USE_DHT_CACHE
        pCtx->component[i].mDcTable =
            const_cast<JpegMpEncoderHuffmanStructure*>(i ? (&s_DhtPresetDc1) : (&s_DhtPresetDc0));
        pCtx->component[i].mAcTable =
            const_cast<JpegMpEncoderHuffmanStructure*>(i ? (&s_DhtPresetAc1) : (&s_DhtPresetAc0));
#else
        pCtx->component[i].mDcTable = &pCtx->dcTbl[i ? 1 : 0];
        pCtx->component[i].mAcTable = &pCtx->acTbl[i ? 1 : 0];
#endif
    }

    if(true
#ifdef JPEG_ENCODER_USE_DQT_CACHE
       && (lastQuality != quality)
#endif
    )
    {
        InitializeJpegMpEncoderQTable(pCtx);
    }

    pCtx->omitDht = (pTempSetting->option & JPEG_ENCODER_OPTION_OMIT_DHT) ? true : false;

    return true;

error:
    return false;
}

inline void Yos_BitInit(JpegMpEncoderContext* pCtx)
{
    pCtx->bitCode = 0;
    pCtx->bitLen = 8;
}

namespace
{
const u32 Yos_BitWriteMasks[17] = {0,      1,      1 << 1,  1 << 2,  1 << 3,  1 << 4,  1 << 5,  1 << 6, 1 << 7,
                                   1 << 8, 1 << 9, 1 << 10, 1 << 11, 1 << 12, 1 << 13, 1 << 14, 1 << 15};
}

bool Yos_BitWrite(JpegMpEncoderContext* pCtx, u32 num, u32 size)
{
    u32 i;

    for(i = size; i > 0; i--)
    {
        if(num & Yos_BitWriteMasks[i])
        {
            pCtx->bitCode |= Yos_BitWriteMasks[pCtx->bitLen];
        }
        pCtx->bitLen--;
        if(pCtx->bitLen == 0)
        {
            pCtx->pDst[pCtx->dstOffset] = static_cast<u8>(pCtx->bitCode);
            if(pCtx->bitCode == 255)
            {
                if(pCtx->dstOffset + 2 >= pCtx->writeLimit)
                {
                    SetJpegMpEncoderError(pCtx, JPEG_ENCODER_ERROR_INSUFFICIENT_OUTPUT_BUFFER);
                    return false;
                }
                pCtx->pDst[pCtx->dstOffset + 1] = 0;
                pCtx->dstOffset += 2;
            }
            else
            {
                pCtx->dstOffset++;
                if(pCtx->dstOffset >= pCtx->writeLimit)
                {
                    SetJpegMpEncoderError(pCtx, JPEG_ENCODER_ERROR_INSUFFICIENT_OUTPUT_BUFFER);
                    return false;
                }
            }
            Yos_BitInit(pCtx);
        }
    }
    return true;
}

#if 0

void Yos_JpegMpEncoderForwardDct(JpegMpEncoderYCbCrType *data, const JpegMpEncoderQTableType *qtable, s16 *output)
{
	s32 tmp[64] ;
	s32 dst,loop,a0,a1,a2,a3,a4,a5,a6,a7,c0,c1,c2,c3,c4,c5,c6,c7;
    static const u8 idx2[] =
    {
        0*8, 2*8, 4*8, 6*8, 7*8, 5*8, 3*8, 1*8
    };
    static const u8 idx3[] =
    {
        0, 4, 2, 6, 1, 5, 3, 7
    };

	for(loop = 0 ; loop < 8 ; loop++){
		a0 = data[loop*8    ] + data[loop*8 + 2]            ;
		a1 = data[loop*8 + 1] + data[loop*8 + 3]            ;
		a2 = data[loop*8 + 7] + data[loop*8 + 5]            ;
		a3 = data[loop*8 + 6] + data[loop*8 + 4]            ;
		a4 = data[loop*8 + 6] - data[loop*8 + 4];
		a5 = data[loop*8 + 7] - data[loop*8 + 5];
		a6 = data[loop*8 + 1] - data[loop*8 + 3];
		a7 = data[loop*8    ] - data[loop*8 + 2];

		c0 = a0 + a3                ;
		c1 = a1 + a2                ;
		c2 = a1 - a2 + a0 - a3                          ;
		c3 = a0 - a3                ;
		c5 = a5 + a6 ;
		c6 = a6 + a7 ;
		c7 = a7 ;
		c4 = a4 + a5 + c6;

		c2 = (c2 * FC4);
		c5 = (c5 * FC4);
		a2 = (FSC24*c4);
		a6 = (FSC64*c4);
		a0 = (FSEC2*c6);
		a1 = (FSEC6*c6);
		c3 <<= 8;
		c7 <<= 8;

		tmp[idx2[loop]  ] = (c0 + c1 -256 -256 -256 -256)<<8 ;
		tmp[idx2[loop]+1] = (c0 - c1                          )<<8 ;
		tmp[idx2[loop]+2] = c2 + c3;
		tmp[idx2[loop]+3] = c3 - c2;
		tmp[idx2[loop]+4] = c7 + c5 + a2 + a0;
		tmp[idx2[loop]+5] = c7 - c5 + a6 - a1;
		tmp[idx2[loop]+6] = c7 - c5 - a6 + a1;
		tmp[idx2[loop]+7] = c7 + c5 - a2 - a0;

	}
	for(loop = dst = 0 ; loop < 8 ; loop++,dst+=8){
		a0 = tmp[idx3[loop]    ] + tmp[idx3[loop]+1*8];
		a1 = tmp[idx3[loop]+2*8] + tmp[idx3[loop]+3*8];
		a2 = tmp[idx3[loop]+4*8] + tmp[idx3[loop]+5*8];
		a3 = tmp[idx3[loop]+6*8] + tmp[idx3[loop]+7*8];
		a4 = tmp[idx3[loop]+6*8] - tmp[idx3[loop]+7*8];
		a5 = tmp[idx3[loop]+4*8] - tmp[idx3[loop]+5*8];
		a6 = tmp[idx3[loop]+2*8] - tmp[idx3[loop]+3*8];
		a7 = tmp[idx3[loop]    ] - tmp[idx3[loop]+1*8];

		c0 = a0 + a3;
		c1 = a1 + a2;
		c2 = a1 - a2 + a0 - a3;
		c3 = a0 - a3;
		c5 = a5 + a6;
		c6 = a6 + a7;
		c7 = a7;
		c4 = a4 + a5 + c6;

		c2 = (c2 * FC4);
		c5 = (c5 * FC4);
		c3 <<= 8;
		c7 <<= 8;
		a2 = (FSC24*c4);
		a6 = (FSC64*c4);
		a0 = (FSEC2*c6);
		a1 = (FSEC6*c6);

		output[dst    ] = static_cast<s16>(( ((c0 + c1 +128)>>8) * qtable[loop * 8] +2048)>>12);
		output[dst + 1] = static_cast<s16>(( ((c7 + c5 + a2 + a0+32768)>>16) * qtable[loop * 8+1] +2048)>>12);
		output[dst + 2] = static_cast<s16>(( ((c3 + c2 +32768)>>16) * qtable[loop * 8+2] +2048)>>12);
		output[dst + 3] = static_cast<s16>(( ((c7 - c5 - a6 + a1+32768)>>16) * qtable[loop * 8+3] +2048)>>12);
		output[dst + 4] = static_cast<s16>(( ((c0 - c1 +128)>>8) * qtable[loop * 8+4] +2048)>>12);
		output[dst + 5] = static_cast<s16>(( ((c7 - c5 + a6 - a1+32768)>>16) * qtable[loop * 8+5] +2048)>>12);
		output[dst + 6] = static_cast<s16>(( ((c3 - c2 +32768)>>16) * qtable[loop * 8+6] +2048)>>12);
		output[dst + 7] = static_cast<s16>(( ((c7 + c5 - a2 - a0+32768)>>16) * qtable[loop * 8+7] +2048)>>12);

	}
}
#endif

namespace
{
const u8 JpegMpEncoderResampleBlock8OffsetTbl[8 * 8] NN_ATTRIBUTE_ALIGN(32) = {
    0x00, 0x01, 0x04, 0x05, 0x10, 0x11, 0x14, 0x15, 0x02, 0x03, 0x06, 0x07, 0x12, 0x13, 0x16, 0x17,
    0x08, 0x09, 0x0c, 0x0d, 0x18, 0x19, 0x1c, 0x1d, 0x0a, 0x0b, 0x0e, 0x0f, 0x1a, 0x1b, 0x1e, 0x1f,
    0x20, 0x21, 0x24, 0x25, 0x30, 0x31, 0x34, 0x35, 0x22, 0x23, 0x26, 0x27, 0x32, 0x33, 0x36, 0x37,
    0x28, 0x29, 0x2c, 0x2d, 0x38, 0x39, 0x3c, 0x3d, 0x2a, 0x2b, 0x2e, 0x2f, 0x3a, 0x3b, 0x3e, 0x3f};
}

void JpegMpEncoderCResampleYuyv8(JpegMpEncoderContext* pCtx, u32 col, u32 row)
{
    const u32 prec = 4;
    u32 stepSrcW;
    u32 loopSrcW;
    u32 loopSrcH;
    u32 loopSrcWInitial;
    u32 loopDstW;
    u32 loopDstH;
    u32 loopX;
    u32 loopY;
    u32 ofs;
    u32 y;
    u32 u;
    u32 v = 0;
    u32 yuyvBak = 0;

    loopX = pCtx->samplingX * 8;
    loopY = pCtx->samplingY * 8;
    loopDstH = (row + 1) * loopY;

    stepSrcW = (pCtx->thumbSrcWidth << prec) / pCtx->width;
    loopSrcWInitial = (pCtx->thumbSrcWidth << prec) - (pCtx->width - ((col + 1) * loopX)) * stepSrcW;

    while(loopY)
    {
        loopY--;
        loopDstH--;
        loopSrcH =
            ((((pCtx->thumbSrcHeight * loopDstH) << prec) / pCtx->height) >> prec) * pCtx->thumbSrcInputBufferWidth;
        loopSrcW = loopSrcWInitial;
        loopDstW = loopX;
        while(loopDstW)
        {
            loopDstW--;
            loopSrcW -= stepSrcW;

            ofs = (loopSrcW >> prec);
            y = *(static_cast<const u32*>(pCtx->pThumbSrc) + ((loopSrcH + (ofs & ~1)) >> 1));
            u = y & 0x0000ff00;
            v = y & 0xff000000;
            if(ofs & 1)
            {
                y = (y >> 16) & 0xff;
            }
            else
            {
                y &= 0xff;
            }
            if(loopDstW & 1)
            {
                yuyvBak = y | u | v;
            }
            else
            {
                u = (yuyvBak >> 1) + (u >> 1) + (v >> 1);
                u &= 0xff00ff00;
                y = ((yuyvBak & 0x000000ff) << 16) | u | y;
                *(static_cast<u32*>(pCtx->pThumbDst) +
                  ((loopY * JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_WIDTH + loopDstW) >> 1)) = y;
            }
        }
    }

    pCtx->pSrc =
        reinterpret_cast<const u16*>(const_cast<const void*>(pCtx->pThumbDst)) -
        ((row * (pCtx->samplingY * 8) * JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_WIDTH) + (col * (pCtx->samplingX * 8)));
    pCtx->thumbSrcConvFunc(pCtx, col, row);
}

void JpegMpEncoderCResampleCtrRgb565(JpegMpEncoderContext* pCtx, u32 col, u32 row)
{
    const u32 prec = 4;
    u32 stepSrcW;
    u32 loopSrcW;
    u32 loopSrcH;
    u32 loopSrcWInitial;
    u32 loopDstW;
    u32 loopDstH;
    u32 loopX;
    u32 loopY;

    loopX = pCtx->samplingX * 8;
    loopY = pCtx->samplingY * 8;
    loopDstH = (row + 1) * loopY;

    stepSrcW = (pCtx->thumbSrcWidth << prec) / pCtx->width;
    loopSrcWInitial = (pCtx->thumbSrcWidth << prec) - (pCtx->width - ((col + 1) * loopX)) * stepSrcW;

    while(loopY)
    {
        loopY--;
        loopDstH--;
        loopSrcH =
            ((((pCtx->thumbSrcHeight * loopDstH) << prec) / pCtx->height) >> prec) * pCtx->thumbSrcInputBufferWidth;
        loopSrcW = loopSrcWInitial;
        loopDstW = loopX;
        while(loopDstW)
        {
            loopDstW--;
            loopSrcW -= stepSrcW;

            *(static_cast<u16*>(pCtx->pThumbDst) + loopY * JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_WIDTH + loopDstW) =
                *((static_cast<const u16*>(pCtx->pThumbSrc) + loopSrcH + (loopSrcW >> prec)));
        }
    }

    pCtx->pSrc =
        reinterpret_cast<const u16*>(const_cast<const void*>(pCtx->pThumbDst)) -
        ((row * (pCtx->samplingY * 8) * JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_WIDTH) + (col * (pCtx->samplingX * 8)));
    pCtx->thumbSrcConvFunc(pCtx, col, row);
}

void JpegMpEncoderCResampleCtrRgb565Block8(JpegMpEncoderContext* pCtx, u32 col, u32 row)
{
    const u32 prec = 4;
    u32 stepSrcW;
    u32 loopSrcW;
    u32 loopSrcH;
    u32 loopSrcWInitial;
    u32 loopDstW;
    u32 loopDstH;
    u32 u;
    u32 v = 0;
    u32 loopX;
    u32 loopY;

    loopX = pCtx->samplingX * 8;
    loopY = pCtx->samplingY * 8;
    loopDstH = (row + 1) * loopY;

    stepSrcW = (pCtx->thumbSrcWidth << prec) / pCtx->width;
    loopSrcWInitial = (pCtx->thumbSrcWidth << prec) - (pCtx->width - ((col + 1) * loopX)) * stepSrcW;

    while(loopY)
    {
        loopY--;
        loopDstH--;
        loopSrcH = (((pCtx->thumbSrcHeight * loopDstH) << prec) / pCtx->height) >> prec;

        v = (loopSrcH & 7) << 3;
        loopSrcH = (loopSrcH & ~7) * pCtx->thumbSrcInputBufferWidth;
        loopSrcW = loopSrcWInitial;
        loopDstW = loopX;
        while(loopDstW)
        {
            loopDstW--;
            loopSrcW -= stepSrcW;

            u = loopSrcW >> prec;
            *(static_cast<u16*>(pCtx->pThumbDst) + loopY * JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_WIDTH + loopDstW) =
                *((static_cast<const u16*>(pCtx->pThumbSrc) + loopSrcH + ((u & ~7) << 3) +
                   JpegMpEncoderResampleBlock8OffsetTbl[v | (u & 7)]));
        }
    }

    pCtx->pSrc =
        reinterpret_cast<const u16*>(const_cast<const void*>(pCtx->pThumbDst)) -
        ((row * (pCtx->samplingY * 8) * JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_WIDTH) + (col * (pCtx->samplingX * 8)));
    pCtx->thumbSrcConvFunc(pCtx, col, row);
}

void JpegMpEncoderCResampleRgb8(JpegMpEncoderContext* pCtx, u32 col, u32 row)
{
    const u32 prec = 4;
    u32 stepSrcW;
    u32 loopSrcW;
    u32 loopSrcH;
    u32 loopSrcWInitial;
    u32 loopDstW;
    u32 loopDstH;
    u32 loopX;
    u32 loopY;
    const u8* pRgb;

    loopX = pCtx->samplingX * 8;
    loopY = pCtx->samplingY * 8;
    loopDstH = (row + 1) * loopY;

    stepSrcW = (pCtx->thumbSrcWidth << prec) / pCtx->width;
    loopSrcWInitial = (pCtx->thumbSrcWidth << prec) - (pCtx->width - ((col + 1) * loopX)) * stepSrcW;

    while(loopY)
    {
        loopY--;
        loopDstH--;
        loopSrcH =
            ((((pCtx->thumbSrcHeight * loopDstH) << prec) / pCtx->height) >> prec) * pCtx->thumbSrcInputBufferWidth;
        loopSrcW = loopSrcWInitial;
        loopDstW = loopX;
        while(loopDstW)
        {
            loopDstW--;
            loopSrcW -= stepSrcW;

            pRgb = static_cast<const u8*>(pCtx->pThumbSrc) + (loopSrcH + (loopSrcW >> prec)) * 3;
            *(static_cast<u16*>(pCtx->pThumbDst) + loopY * JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_WIDTH + loopDstW) =
                ((pRgb[0] >> 3) << 11) | ((pRgb[1] >> 2) << 5) | (pRgb[2] >> 3);
        }
    }

    pCtx->pSrc =
        reinterpret_cast<const u16*>(const_cast<const void*>(pCtx->pThumbDst)) -
        ((row * (pCtx->samplingY * 8) * JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_WIDTH) + (col * (pCtx->samplingX * 8)));
    pCtx->thumbSrcConvFunc(pCtx, col, row);
}

void JpegMpEncoderCResampleBgr8(JpegMpEncoderContext* pCtx, u32 col, u32 row)
{
    const u32 prec = 4;
    u32 stepSrcW;
    u32 loopSrcW;
    u32 loopSrcH;
    u32 loopSrcWInitial;
    u32 loopDstW;
    u32 loopDstH;
    u32 loopX;
    u32 loopY;
    const u8* pRgb;

    loopX = pCtx->samplingX * 8;
    loopY = pCtx->samplingY * 8;
    loopDstH = (row + 1) * loopY;

    stepSrcW = (pCtx->thumbSrcWidth << prec) / pCtx->width;
    loopSrcWInitial = (pCtx->thumbSrcWidth << prec) - (pCtx->width - ((col + 1) * loopX)) * stepSrcW;

    while(loopY)
    {
        loopY--;
        loopDstH--;
        loopSrcH =
            ((((pCtx->thumbSrcHeight * loopDstH) << prec) / pCtx->height) >> prec) * pCtx->thumbSrcInputBufferWidth;
        loopSrcW = loopSrcWInitial;
        loopDstW = loopX;
        while(loopDstW)
        {
            loopDstW--;
            loopSrcW -= stepSrcW;

            pRgb = static_cast<const u8*>(pCtx->pThumbSrc) + (loopSrcH + (loopSrcW >> prec)) * 3;
            *(static_cast<u16*>(pCtx->pThumbDst) + loopY * JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_WIDTH + loopDstW) =
                ((pRgb[2] >> 3) << 11) | ((pRgb[1] >> 2) << 5) | (pRgb[0] >> 3);
        }
    }

    pCtx->pSrc =
        reinterpret_cast<const u16*>(const_cast<const void*>(pCtx->pThumbDst)) -
        ((row * (pCtx->samplingY * 8) * JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_WIDTH) + (col * (pCtx->samplingX * 8)));
    pCtx->thumbSrcConvFunc(pCtx, col, row);
}

void JpegMpEncoderCResampleCtrRgb8Block8(JpegMpEncoderContext* pCtx, u32 col, u32 row)
{
    const u32 prec = 4;
    u32 stepSrcW;
    u32 loopSrcW;
    u32 loopSrcH;
    u32 loopSrcWInitial;
    u32 loopDstW;
    u32 loopDstH;
    u32 u;
    u32 v = 0;
    u32 loopX;
    u32 loopY;
    const u8* pRgb;

    loopX = pCtx->samplingX * 8;
    loopY = pCtx->samplingY * 8;
    loopDstH = (row + 1) * loopY;

    stepSrcW = (pCtx->thumbSrcWidth << prec) / pCtx->width;
    loopSrcWInitial = (pCtx->thumbSrcWidth << prec) - (pCtx->width - ((col + 1) * loopX)) * stepSrcW;

    while(loopY)
    {
        loopY--;
        loopDstH--;
        loopSrcH = (((pCtx->thumbSrcHeight * loopDstH) << prec) / pCtx->height) >> prec;

        v = (loopSrcH & 7) << 3;
        loopSrcH = (loopSrcH & ~7) * pCtx->thumbSrcInputBufferWidth;
        loopSrcW = loopSrcWInitial;
        loopDstW = loopX;
        while(loopDstW)
        {
            loopDstW--;
            loopSrcW -= stepSrcW;

            u = loopSrcW >> prec;
            pRgb = static_cast<const u8*>(pCtx->pThumbSrc) +
                   (loopSrcH + ((u & ~7) << 3) + JpegMpEncoderResampleBlock8OffsetTbl[v | (u & 7)]) * 3;
            *(static_cast<u16*>(pCtx->pThumbDst) + loopY * JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_WIDTH + loopDstW) =
                ((pRgb[2] >> 3) << 11) | ((pRgb[1] >> 2) << 5) | (pRgb[0] >> 3);
        }
    }

    pCtx->pSrc =
        reinterpret_cast<const u16*>(const_cast<const void*>(pCtx->pThumbDst)) -
        ((row * (pCtx->samplingY * 8) * JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_WIDTH) + (col * (pCtx->samplingX * 8)));
    pCtx->thumbSrcConvFunc(pCtx, col, row);
}

void JpegMpEncoderCResampleRgba8(JpegMpEncoderContext* pCtx, u32 col, u32 row)
{
    const u32 prec = 4;
    u32 stepSrcW;
    u32 loopSrcW;
    u32 loopSrcH;
    u32 loopSrcWInitial;
    u32 loopDstW;
    u32 loopDstH;
    u32 loopX;
    u32 loopY;
    const u8* pRgb;

    loopX = pCtx->samplingX * 8;
    loopY = pCtx->samplingY * 8;
    loopDstH = (row + 1) * loopY;

    stepSrcW = (pCtx->thumbSrcWidth << prec) / pCtx->width;
    loopSrcWInitial = (pCtx->thumbSrcWidth << prec) - (pCtx->width - ((col + 1) * loopX)) * stepSrcW;

    while(loopY)
    {
        loopY--;
        loopDstH--;
        loopSrcH =
            ((((pCtx->thumbSrcHeight * loopDstH) << prec) / pCtx->height) >> prec) * pCtx->thumbSrcInputBufferWidth;
        loopSrcW = loopSrcWInitial;
        loopDstW = loopX;
        while(loopDstW)
        {
            loopDstW--;
            loopSrcW -= stepSrcW;

            pRgb = static_cast<const u8*>(pCtx->pThumbSrc) + (loopSrcH + (loopSrcW >> prec)) * 4;
            *(static_cast<u16*>(pCtx->pThumbDst) + loopY * JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_WIDTH + loopDstW) =
                ((pRgb[0] >> 3) << 11) | ((pRgb[1] >> 2) << 5) | (pRgb[2] >> 3);
        }
    }

    pCtx->pSrc =
        reinterpret_cast<const u16*>(const_cast<const void*>(pCtx->pThumbDst)) -
        ((row * (pCtx->samplingY * 8) * JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_WIDTH) + (col * (pCtx->samplingX * 8)));
    pCtx->thumbSrcConvFunc(pCtx, col, row);
}

void JpegMpEncoderCResampleAbgr8(JpegMpEncoderContext* pCtx, u32 col, u32 row)
{
    const u32 prec = 4;
    u32 stepSrcW;
    u32 loopSrcW;
    u32 loopSrcH;
    u32 loopSrcWInitial;
    u32 loopDstW;
    u32 loopDstH;
    u32 loopX;
    u32 loopY;
    const u8* pRgb;

    loopX = pCtx->samplingX * 8;
    loopY = pCtx->samplingY * 8;
    loopDstH = (row + 1) * loopY;

    stepSrcW = (pCtx->thumbSrcWidth << prec) / pCtx->width;
    loopSrcWInitial = (pCtx->thumbSrcWidth << prec) - (pCtx->width - ((col + 1) * loopX)) * stepSrcW;

    while(loopY)
    {
        loopY--;
        loopDstH--;
        loopSrcH =
            ((((pCtx->thumbSrcHeight * loopDstH) << prec) / pCtx->height) >> prec) * pCtx->thumbSrcInputBufferWidth;
        loopSrcW = loopSrcWInitial;
        loopDstW = loopX;
        while(loopDstW)
        {
            loopDstW--;
            loopSrcW -= stepSrcW;

            pRgb = static_cast<const u8*>(pCtx->pThumbSrc) + (loopSrcH + (loopSrcW >> prec)) * 4;
            *(static_cast<u16*>(pCtx->pThumbDst) + loopY * JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_WIDTH + loopDstW) =
                ((pRgb[3] >> 3) << 11) | ((pRgb[2] >> 2) << 5) | (pRgb[1] >> 3);
        }
    }

    pCtx->pSrc =
        reinterpret_cast<const u16*>(const_cast<const void*>(pCtx->pThumbDst)) -
        ((row * (pCtx->samplingY * 8) * JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_WIDTH) + (col * (pCtx->samplingX * 8)));
    pCtx->thumbSrcConvFunc(pCtx, col, row);
}

void JpegMpEncoderCResampleCtrRgba8Block8(JpegMpEncoderContext* pCtx, u32 col, u32 row)
{
    const u32 prec = 4;
    u32 stepSrcW;
    u32 loopSrcW;
    u32 loopSrcH;
    u32 loopSrcWInitial;
    u32 loopDstW;
    u32 loopDstH;
    u32 u;
    u32 v = 0;
    u32 loopX;
    u32 loopY;
    const u8* pRgb;

    loopX = pCtx->samplingX * 8;
    loopY = pCtx->samplingY * 8;
    loopDstH = (row + 1) * loopY;

    stepSrcW = (pCtx->thumbSrcWidth << prec) / pCtx->width;
    loopSrcWInitial = (pCtx->thumbSrcWidth << prec) - (pCtx->width - ((col + 1) * loopX)) * stepSrcW;

    while(loopY)
    {
        loopY--;
        loopDstH--;
        loopSrcH = (((pCtx->thumbSrcHeight * loopDstH) << prec) / pCtx->height) >> prec;

        v = (loopSrcH & 7) << 3;
        loopSrcH = (loopSrcH & ~7) * pCtx->thumbSrcInputBufferWidth;
        loopSrcW = loopSrcWInitial;
        loopDstW = loopX;
        while(loopDstW)
        {
            loopDstW--;
            loopSrcW -= stepSrcW;

            u = loopSrcW >> prec;
            pRgb = static_cast<const u8*>(pCtx->pThumbSrc) +
                   (loopSrcH + ((u & ~7) << 3) + JpegMpEncoderResampleBlock8OffsetTbl[v | (u & 7)]) * 4;
            *(static_cast<u16*>(pCtx->pThumbDst) + loopY * JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_WIDTH + loopDstW) =
                ((pRgb[3] >> 3) << 11) | ((pRgb[2] >> 2) << 5) | (pRgb[1] >> 3);
        }
    }

    pCtx->pSrc =
        reinterpret_cast<const u16*>(const_cast<const void*>(pCtx->pThumbDst)) -
        ((row * (pCtx->samplingY * 8) * JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_WIDTH) + (col * (pCtx->samplingX * 8)));
    pCtx->thumbSrcConvFunc(pCtx, col, row);
}

size_t Yos_JPEGEncodeFast(JpegMpEncoderContext* pCtx)
{
    u32 j, x, y;
    u8* pDst = pCtx->pDst + pCtx->dstOffset;
    size_t size;
    size_t dhtSize = 0;
    JpegMpEncoderYCbCrType* pYBuf = pCtx->convWork.yBuf;
    JpegMpEncoderYCbCrType* pCbBuf = pCtx->convWork.cbBuf;
    JpegMpEncoderYCbCrType* pCrBuf = pCtx->convWork.crBuf;
    const JpegMpEncoderQTableType* pQTbl = pCtx->qBuf;

    size = sizeof(jfif) +
           NUM_JPEG_ENCODER_Q_TBLS * (1 + NUM_JPEG_ENCODER_Q_TBL_ELEMS) * sizeof(JpegMpEncoderDqTableType) +
           sizeof(jsos);

    if(!pCtx->omitDht)
    {
#ifdef JPEG_ENCODER_USE_DHT_CACHE
        dhtSize = sizeof(s_DhtData);
#else
        dhtSize = 4 + Yos_JpegMpEncoderHuffmanEncoderOutputSize(&pCtx->dcTbl[0]) +
                  Yos_JpegMpEncoderHuffmanEncoderOutputSize(&pCtx->dcTbl[1]) +
                  Yos_JpegMpEncoderHuffmanEncoderOutputSize(&pCtx->acTbl[0]) +
                  Yos_JpegMpEncoderHuffmanEncoderOutputSize(&pCtx->acTbl[1]);
#endif
    }

    if((pDst + size + dhtSize) >= (pCtx->pDst + pCtx->writeLimit))
    {
        goto insufficientOutputBufferError;
    }

    memcpy(pDst, jfif, sizeof(jfif));

    pDst[5] = static_cast<u8>(pCtx->height >> 8);
    pDst[6] = static_cast<u8>(pCtx->height & 255);
    pDst[7] = static_cast<u8>(pCtx->width >> 8);
    pDst[8] = static_cast<u8>(pCtx->width & 255);
    pDst[11] = static_cast<u8>((pCtx->samplingX << 4) | pCtx->samplingY);
    pDst += sizeof(jfif);

    for(x = 0; x < NUM_JPEG_ENCODER_Q_TBLS; x++)
    {
        *pDst++ = static_cast<u8>(x);
        for(j = 0; j < NUM_JPEG_ENCODER_Q_TBL_ELEMS; j++)
        {
            *pDst++ = pCtx->dqBuf[x * 64 + JpegZigZagInputOrderCodes[j]];
        }
    }

    if(dhtSize)
    {
#ifdef JPEG_ENCODER_USE_DHT_CACHE
        memcpy(pDst, s_DhtData, sizeof(s_DhtData));
        pDst += sizeof(s_DhtData);
#else
        pDst[0] = 0xFF;
        pDst[1] = 0xC4;
        pDst[2] = static_cast<u8>((dhtSize - 2) >> 8);
        pDst[3] = static_cast<u8>((dhtSize - 2) & 255);
        pDst += 4;

        *pDst++ = 0x00;
        pDst = Yos_JpegMpEncoderHuffmanEncoderPrintTable(pDst, &pCtx->dcTbl[0]);
        *pDst++ = 0x01;
        pDst = Yos_JpegMpEncoderHuffmanEncoderPrintTable(pDst, &pCtx->dcTbl[1]);
        *pDst++ = 0x10;
        pDst = Yos_JpegMpEncoderHuffmanEncoderPrintTable(pDst, &pCtx->acTbl[0]);
        *pDst++ = 0x11;
        pDst = Yos_JpegMpEncoderHuffmanEncoderPrintTable(pDst, &pCtx->acTbl[1]);
#endif
    }

    memcpy(pDst, jsos, sizeof(jsos));
    pDst += sizeof(jsos);

    pCtx->dstOffset = pDst - pCtx->pDst;

    Yos_BitInit(pCtx);

    for(x = 0; x < NUM_JPEG_ENCODER_COMPONENTS; x++)
    {
        pCtx->component[x].mLastDcValue = 0;
    }

#if 0

#else

    JpegMpEncoderAsm_ConvertWorkToAsm(pCtx);
#endif

    for(y = 0; y < pCtx->mcu_rows; y++)
    {
        for(x = 0; x < pCtx->mcu_cols; x++)
        {
            pCtx->convFunc(pCtx, x, y);

            for(j = 0; j < pCtx->loops; j++)
            {
#if 0

                Yos_JpegMpEncoderForwardDct(&pYBuf[j*64], pQTbl, pCtx->component[0].mCoefficients);
#else
                JpegMpEncoderAsm_ForwardDct(&pYBuf[j * 64], pQTbl, pCtx->component[0].mCoefficients);
#endif
#if 0

                if(!Yos_JpegMpEncoderComponentSequential(pCtx, &pCtx->component[0]))
                {
                    goto insufficientOutputBufferError;
                }
#else
                if(!JpegMpEncoderAsm_ComponentSequential(pCtx, &pCtx->component[0]))
                {
                    goto insufficientOutputBufferError;
                }
#endif
            }

#if 0

            Yos_JpegMpEncoderForwardDct(pCbBuf, &pQTbl[64], pCtx->component[1].mCoefficients);
#else
            JpegMpEncoderAsm_ForwardDct(pCbBuf, &pQTbl[64], pCtx->component[1].mCoefficients);
#endif
#if 0

            if(!Yos_JpegMpEncoderComponentSequential(pCtx, &pCtx->component[1]))
            {
                goto insufficientOutputBufferError;
            }
#else
            if(!JpegMpEncoderAsm_ComponentSequential(pCtx, &pCtx->component[1]))
            {
                goto insufficientOutputBufferError;
            }
#endif

#if 0

            Yos_JpegMpEncoderForwardDct(pCrBuf, &pQTbl[64], pCtx->component[2].mCoefficients);
#else
            JpegMpEncoderAsm_ForwardDct(pCrBuf, &pQTbl[64], pCtx->component[2].mCoefficients);
#endif
#if 0

            if(!Yos_JpegMpEncoderComponentSequential(pCtx, &pCtx->component[2]))
            {
                goto insufficientOutputBufferError;
            }
#else
            if(!JpegMpEncoderAsm_ComponentSequential(pCtx, &pCtx->component[2]))
            {
                goto insufficientOutputBufferError;
            }
#endif
        }
    }

#if 0

#else

    JpegMpEncoderAsm_ConvertWorkToC(pCtx);
#endif

    if(pCtx->bitLen != 8)
    {
        if(!Yos_BitWrite(pCtx, 0, 7))
        {
            goto insufficientOutputBufferError;
        }
    }

    if(pCtx->dstOffset + 2 > pCtx->writeLimit)
    {
        goto insufficientOutputBufferError;
    }

    pCtx->pDst[pCtx->dstOffset] = 0xFF;
    pCtx->pDst[pCtx->dstOffset + 1] = 0xD9;
    pCtx->dstOffset += 2;

    return (pCtx->dstOffset);

insufficientOutputBufferError:
    SetJpegMpEncoderError(pCtx, JPEG_ENCODER_ERROR_INSUFFICIENT_OUTPUT_BUFFER);
    return 0;
}

size_t StartJpegEncoderCore(detail::JpegMpEncoderWorkObj* pWork, u8* dst, size_t limit, const void* src, u32 width,
                            u32 height, u32 quality, PixelSampling dstPixelSampling, PixelFormat srcPixelFormat,
                            bool addThumbnail, detail::JpegMpEncoderTemporarySettingObj* pTempSetting)
{
    u8* pDstLimit = dst + limit;
    u8* pTmp;
#ifdef JPEG_ENCODER_USE_DHT_CACHE
#else
    JpegMpEncoderHuffmanEncoderBuildTableWork* pBuildTableWork;
#endif
    PixelFormat thumbSrcPixelFormat;
    u32 inputBufferWidth = pTempSetting->inputBufferWidth;
    JpegMpEncoderContext* pThumbCtx;

    NN_ALIGN_ASSERT_(src, 4);
    if(reinterpret_cast<u32>(src) & 3)
    {
        SetJpegMpEncoderError(&pWork->ctx, JPEG_ENCODER_ERROR_SRC_ALIGNMENT);
        goto error;
    }

    if(pTempSetting->pTwlPhotoMakerNoteData)
    {

        if((limit > detail::MAX_TWL_PHOTO_DATA_SIZE) ||
           (!(((width == 640) && (height == 480)) || ((width == 160) && (height == 120)))) || (!addThumbnail))
        {
            SetJpegMpEncoderError(&pWork->ctx, JPEG_ENCODER_ERROR_INVALID_ARGUMENT);
            goto error;
        }
    }

    pWork->addThumbnail = addThumbnail;

    if(!CheckEncoderWidthHeight(width, height, dstPixelSampling))
    {
        SetJpegMpEncoderError(&pWork->ctx, JPEG_ENCODER_ERROR_INVALID_ARGUMENT);
        goto error;
    }

    switch(srcPixelFormat)
    {
    case PIXEL_FORMAT_YUYV8:
        inputBufferWidth &= ~1;
        break;

    case PIXEL_FORMAT_CTR_RGB565_BLOCK8:
    case PIXEL_FORMAT_CTR_RGB8_BLOCK8:
    case PIXEL_FORMAT_CTR_RGBA8_BLOCK8:
        inputBufferWidth &= ~7;
        break;

    default:
        break;
    }
    if(inputBufferWidth < width)
    {

        inputBufferWidth = width;
    }

    pTmp = reinterpret_cast<u8*>(RoundUp4B(reinterpret_cast<u32>(dst)));

#ifdef JPEG_ENCODER_USE_DHT_CACHE
#else

    if((pTmp + sizeof(*pBuildTableWork)) > pDstLimit)
    {
        goto insufficientOutputBufferError;
    }
    pBuildTableWork = reinterpret_cast<JpegMpEncoderHuffmanEncoderBuildTableWork*>(pTmp);
    pTmp += sizeof(*pBuildTableWork);
#endif

    PreEncodeJpegApp1(pWork, pTempSetting, addThumbnail);

    if(addThumbnail)
    {
        u8* pThumbSrc;
        size_t thumbSrcSize;
        size_t size;
        u32 thumbnailWidth = pTempSetting->thumbnailWidth;
        u32 thumbnailHeight = pTempSetting->thumbnailHeight;
        PixelSampling thumbnailSampling = pTempSetting->thumbnailSampling;

        if(!CheckEncoderWidthHeight(thumbnailWidth, thumbnailHeight, thumbnailSampling))
        {
            thumbnailWidth = DEFAULT_THUMBNAIL_WIDTH;
            thumbnailHeight = DEFAULT_THUMBNAIL_HEIGHT;
            thumbnailSampling = DEFAULT_THUMBNAIL_PIXEL_SAMPLING;
        }

        thumbSrcSize =
            JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_WIDTH * JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_HEIGHT * sizeof(u16);

        size = thumbSrcSize + sizeof(*pThumbCtx) + 2;
        if((pTmp + size) > pDstLimit)
        {

            goto insufficientOutputBufferError;
        }
        pThumbSrc = pTmp;
        pTmp += thumbSrcSize;

        pThumbCtx = reinterpret_cast<JpegMpEncoderContext*>(pTmp);
        pTmp += sizeof(*pThumbCtx);

        thumbSrcPixelFormat = srcPixelFormat;
        if((thumbSrcPixelFormat == PIXEL_FORMAT_CTR_RGB565_BLOCK8) || (thumbSrcPixelFormat == PIXEL_FORMAT_RGB8) ||
           (thumbSrcPixelFormat == PIXEL_FORMAT_CTR_RGB8_BLOCK8) || (thumbSrcPixelFormat == PIXEL_FORMAT_RGBA8) ||
           (thumbSrcPixelFormat == PIXEL_FORMAT_CTR_RGBA8_BLOCK8) || (thumbSrcPixelFormat == PIXEL_FORMAT_BGR8) ||
           (thumbSrcPixelFormat == PIXEL_FORMAT_ABGR8))
        {
            thumbSrcPixelFormat = PIXEL_FORMAT_CTR_RGB565;
        }

        pTmp[0] = 0xFF;
        pTmp[1] = 0xD8;

#ifdef JPEG_ENCODER_USE_DQT_CACHE

        pThumbCtx->quality = 0;
#endif
        if(!InitializeJpegMpEncoderContext(
               pThumbCtx,
#ifdef JPEG_ENCODER_USE_DHT_CACHE

#else
               pBuildTableWork,
#endif
               &pTmp[2], pDstLimit - &pTmp[2], pThumbSrc, thumbnailWidth, thumbnailHeight,
               (quality < JPEG_ENCODER_THUMBNAIL_QUALITY) ? quality : JPEG_ENCODER_THUMBNAIL_QUALITY, thumbnailSampling,
               thumbSrcPixelFormat, JPEG_ENCODER_THUMBNAIL_INPUT_BUFFER_WIDTH, pTempSetting))
        {
            goto thumbError;
        }

        pThumbCtx->thumbSrcConvFunc = pThumbCtx->convFunc;
        pThumbCtx->convFunc = ThumbnailConvFuncTbl[srcPixelFormat];
        if(!pThumbCtx->convFunc)
        {
            goto thumbError;
        }
        pThumbCtx->pThumbDst = pThumbSrc;
        pThumbCtx->pThumbSrc = src;
        pThumbCtx->thumbSrcWidth = width;
        pThumbCtx->thumbSrcHeight = height;
        pThumbCtx->thumbSrcInputBufferWidth = inputBufferWidth;

        size = Yos_JPEGEncodeFast(pThumbCtx);
        if(!size)
        {
            goto thumbError;
        }

        size += 2;

        if(size & 1)
        {
            if((pTmp + size + 1) > pDstLimit)
            {
                goto insufficientOutputBufferError;
            }
            pTmp[size++] = 0;
        }

        pWork->app1SizeTmp = CalcJpegMpEncoderApp1Size(pWork);
        pWork->app1JpegInterchangeFormatLengthData[0] = size;

        memmove(dst + APP1_LENGTH_OFFSET + pWork->app1SizeTmp, pTmp, size);

#ifdef JPEG_ENCODER_USE_DHT_CACHE
#else

        pTmp = reinterpret_cast<u8*>(RoundUp4B(reinterpret_cast<u32>(pTmp + size)));
        if((pTmp + sizeof(*pBuildTableWork)) > pDstLimit)
        {
            goto insufficientOutputBufferError;
        }
        pBuildTableWork = reinterpret_cast<JpegMpEncoderHuffmanEncoderBuildTableWork*>(pTmp);
#endif
    }

    if(!InitializeJpegMpEncoderContext(&pWork->ctx,
#ifdef JPEG_ENCODER_USE_DHT_CACHE

#else
                                       pBuildTableWork,
#endif
                                       dst, limit, src, width, height, quality, dstPixelSampling, srcPixelFormat,
                                       inputBufferWidth, pTempSetting))
    {
        goto error;
    }

    if(!EncodeJpegApp1(pWork))
    {
        goto error;
    }

    if(pWork->useMpFormat)
    {

        return pWork->ctx.dstOffset;
    }

    return Yos_JPEGEncodeFast(&pWork->ctx);

error:
    return 0;

insufficientOutputBufferError:
    SetJpegMpEncoderError(&pWork->ctx, JPEG_ENCODER_ERROR_INSUFFICIENT_OUTPUT_BUFFER);
    goto error;

thumbError:
    SetJpegMpEncoderError(&pWork->ctx, pThumbCtx->errorCode ? pThumbCtx->errorCode : JPEG_ENCODER_ERROR_INTERNAL);
    goto error;
}

size_t JpegMpEncoder::StartJpegEncoder(u8* dst, size_t limit, const void* src, u32 width, u32 height, u32 quality,
                                       PixelSampling dstPixelSampling, PixelFormat srcPixelFormat, bool addThumbnail)
{
    if(!m_Initialized)
    {
        return 0;
    }

    m_pWork->useMpFormat = false;

    size_t ret = StartJpegEncoderCore(m_pWork, dst, limit, src, width, height, quality, dstPixelSampling,
                                      srcPixelFormat, addThumbnail, &m_TemporarySetting);

    ClearTemporarySetting();

    return ret;
}

s32 JpegMpEncoder::GetLastError() const
{
    if(!m_Initialized)
    {

        return JPEG_ENCODER_ERROR_NOT_INITIALIZED;
    }

    return m_pWork->ctx.errorCode;
}

namespace
{
inline bool isNumImagesZeroOrTooBig(u32 numImages)
{

    return ((!numImages) || (numImages >= 0x1000));
}

size_t InitializeJpegMpEncoderWorkCommon(detail::JpegMpEncoderWorkObj* pWork)
{
    u8* pBuf = reinterpret_cast<u8*>(pWork);
    u32 numImages = pWork->maxNumImages;

    size_t clearSize = JpegMpEncoder::GetWorkBufferSize(numImages);

    memset(pBuf, 0, clearSize);
    pBuf += sizeof(*pWork);

    if(numImages)
    {

        pWork->maxNumImages = numImages;

        pWork->pApp2MpEntry = reinterpret_cast<JpegMpEncoderApp2TiffMpEntryObj*>(pBuf);
        pBuf += numImages * sizeof(JpegMpEncoderApp2TiffMpEntryObj);

        pWork->pApp2MpImageUidList = pBuf;
        pBuf += numImages * IMAGE_UID_SIZE;

        InitializeJpegMpEncoderApp2IndexTagWork(pWork);
        InitializeJpegMpEncoderApp2AttributeTagWork(pWork);
    }

    InitializeJpegMpEncoderApp1TagWork(pWork);

    return pBuf - reinterpret_cast<u8*>(pWork);
}

}

size_t JpegMpEncoder::GetWorkBufferSize(u32 numImages)
{
    size_t size = 0;

    size += sizeof(detail::JpegMpEncoderWorkObj);

    if(numImages)
    {
        if(isNumImagesZeroOrTooBig(numImages))
        {

            return 0;
        }

        size += numImages * sizeof(JpegMpEncoderApp2TiffMpEntryObj);

        size += numImages * IMAGE_UID_SIZE;
    }

    return size;
}

void JpegMpEncoder::ClearTemporarySetting()
{
    char dateTimeTemp[DATE_TIME_SIZE];

    memcpy(dateTimeTemp, m_TemporarySetting.dateTimeBuffer, sizeof(dateTimeTemp));
    memset(&m_TemporarySetting, 0, sizeof(m_TemporarySetting));
    memcpy(m_TemporarySetting.dateTimeBuffer, dateTimeTemp, sizeof(m_TemporarySetting.dateTimeBuffer));

    m_TemporarySetting.thumbnailWidth = DEFAULT_THUMBNAIL_WIDTH;
    m_TemporarySetting.thumbnailHeight = DEFAULT_THUMBNAIL_HEIGHT;
    m_TemporarySetting.thumbnailSampling = DEFAULT_THUMBNAIL_PIXEL_SAMPLING;

    InitializeJpegMpEncoderApp1TagWork(m_pWork);
}

bool JpegMpEncoder::Initialize(void* workBuffer, size_t workBufferSize, u32 numImages)
{
    u8* pBuf = static_cast<u8*>(workBuffer);
    size_t neededSize;

    m_Initialized = false;

    NN_ALIGN_ASSERT_(workBuffer, 4);
    if(reinterpret_cast<u32>(workBuffer) & 3)
    {

        goto error;
    }

    neededSize = GetWorkBufferSize(numImages);
    if(!neededSize)
    {

        goto error;
    }
    if(workBufferSize < neededSize)
    {

        goto error;
    }

    m_pWork = reinterpret_cast<detail::JpegMpEncoderWorkObj*>(pBuf);
    m_pWork->maxNumImages = numImages;

    pBuf += InitializeJpegMpEncoderWorkCommon(m_pWork);

    memset(&m_TemporarySetting, 0, sizeof(m_TemporarySetting));
    ClearTemporarySetting();

    if(neededSize != (pBuf - static_cast<u8*>(workBuffer)))
    {

        goto error;
    }

    m_Initialized = true;
    return true;

error:
    return false;
}

size_t JpegMpEncoder::StartMpEncoderLR(u8* dst, size_t limit, const void* srcL, const void* srcR, u32 width, u32 height,
                                       u32 quality, PixelSampling dstPixelSampling, PixelFormat srcPixelFormat,
                                       bool addThumbnailL, bool addThumbnailR)
{
    size_t ret;
    u32 thumbnailWidth;
    u32 thumbnailHeight;
    u32 inputBufferWidth;
    PixelSampling thumbnailSampling;
    const char* pSoftware;
    const GpsData* pGps;
    u16 orientation;
    bool isOrientationSet;
    u32 option;
    MpRegionsToBuildJpegData regions;

    if(!m_Initialized)
    {
        return 0;
    }

    memset(&m_TemporarySetting.mpAttribute, 0, sizeof(m_TemporarySetting.mpAttribute));
    SetMpTypeFlags(MP_TYPE_FLAG_REPRESENTATIVE_IMAGE);

    memset(&m_TemporarySetting.makerNotes, 0, sizeof(m_TemporarySetting.makerNotes));
    m_TemporarySetting.pTwlPhotoMakerNoteData = NULL;
    SetImageUid(NULL);

    thumbnailWidth = m_TemporarySetting.thumbnailWidth;
    thumbnailHeight = m_TemporarySetting.thumbnailHeight;
    inputBufferWidth = m_TemporarySetting.inputBufferWidth;
    thumbnailSampling = m_TemporarySetting.thumbnailSampling;
    pSoftware = m_TemporarySetting.pSoftware;
    pGps = m_TemporarySetting.pGpsData;
    isOrientationSet = m_TemporarySetting.isOrientationSet;
    orientation = m_TemporarySetting.orientation;
    option = m_TemporarySetting.option;

    ret = StartMpEncoderFirst(dst, limit, srcL, width, height, quality, dstPixelSampling, srcPixelFormat, addThumbnailL,
                              2, MP_TYPE_CODE_MULTI_VIEW_DISPARITY_IMAGE, false, false);
    if(ret)
    {

        m_TemporarySetting.thumbnailWidth = thumbnailWidth;
        m_TemporarySetting.thumbnailHeight = thumbnailHeight;
        m_TemporarySetting.inputBufferWidth = inputBufferWidth;
        m_TemporarySetting.thumbnailSampling = thumbnailSampling;
        m_TemporarySetting.pSoftware = pSoftware;
        m_TemporarySetting.pGpsData = pGps;
        m_TemporarySetting.isOrientationSet = isOrientationSet;
        m_TemporarySetting.orientation = orientation;
        m_TemporarySetting.option = option;

        regions = m_pWork->mpRegions;

        m_TemporarySetting.isDateTimeSet = true;
        ret = StartMpEncoderNext(srcR, width, height, quality, dstPixelSampling, srcPixelFormat, addThumbnailR);

        m_pWork->mpRegions = regions;
    }

    ClearTemporarySetting();

    return ret;
}

bool StartMpEncoderCore(detail::JpegMpEncoderWorkObj* pWork, u8* dst, size_t limit, const void* src, u32 width,
                        u32 height, u32 quality, PixelSampling dstPixelSampling, PixelFormat srcPixelFormat,
                        bool addThumbnail, detail::JpegMpEncoderTemporarySettingObj* pTempSetting)
{
    size_t ret;

    if(pTempSetting->pTwlPhotoMakerNoteData)
    {

        SetJpegMpEncoderError(&pWork->ctx, JPEG_ENCODER_ERROR_INTERNAL);
        return false;
    }

    pWork->useMpFormat = true;

    pWork->mpRegions.pSrc1 = dst;
    ret = StartJpegEncoderCore(pWork, dst, limit, src, width, height, quality, dstPixelSampling, srcPixelFormat,
                               addThumbnail, pTempSetting);

    if(ret)
    {
        pWork->mpRegions.size1 = ret;

        ret = EncodeJpegMpApp2(pWork, pTempSetting);
        if(ret)
        {
            pWork->mpRegions.pSrc2 = pWork->ctx.pDst + pWork->ctx.dstOffset;
            ret = Yos_JPEGEncodeFast(&pWork->ctx);
            if(ret)
            {
                if(PostEncodeJpegMpApp2(pWork, false))
                {
                    pWork->mpRegions.size2 = (pWork->ctx.pDst + pWork->ctx.dstOffset) - pWork->mpRegions.pSrc2;
                }
                else
                {

                    ret = 0;
                }
            }
        }
    }

    return ret ? true : false;
}

size_t JpegMpEncoder::StartMpEncoderFirst(u8* dst, size_t limit, const void* src, u32 width, u32 height, u32 quality,
                                          PixelSampling dstPixelSampling, PixelFormat srcPixelFormat, bool addThumbnail,
                                          u32 numImages, MpTypeCode typeCode, bool addImageUidList, bool addTotalFrames)
{
    if(!m_Initialized)
    {
        return 0;
    }

    if(isNumImagesZeroOrTooBig(numImages))
    {
        goto invalidMpNumImages;
    }

    if(numImages > m_pWork->maxNumImages)
    {

        goto invalidMpNumImages;
    }

    InitializeJpegMpEncoderWorkCommon(m_pWork);

    switch(typeCode)
    {
    case MP_TYPE_CODE_MULTI_VIEW_DISPARITY_IMAGE:
        if(numImages < 2)
        {
            goto invalidMpNumImages;
        }
        break;

    case MP_TYPE_CODE_BASELINE_MP_PRIMARY_IMAGE:

        if(numImages > 3)
        {
            goto invalidMpNumImages;
        }
        m_pWork->app2.isBaseline = true;

        addTotalFrames = false;
        break;

    case MP_TYPE_CODE_LARGE_THUMBNAIL_IMAGE_CLASS_1:
    case MP_TYPE_CODE_LARGE_THUMBNAIL_IMAGE_CLASS_2:

        goto invalidMpTypeCodeError;

    case MP_TYPE_CODE_MULTI_VIEW_PANORAMA_IMAGE:
    case MP_TYPE_CODE_MULTI_VIEW_MULTI_ANGLE_IMAGE:
    case MP_TYPE_CODE_UNDEFINED:

        break;

    default:

        goto invalidMpTypeCodeError;
    }

    m_pWork->app2.numberOfImages = numImages;
    m_pWork->app2.typeCode = typeCode;
    m_pWork->app2.firstTypeCode = typeCode;
    m_pWork->app2.addImageUidList = addImageUidList;
    m_pWork->app2.addTotalFrames = addTotalFrames;

    if(!StartMpEncoderCore(m_pWork, dst, limit, src, width, height, quality, dstPixelSampling, srcPixelFormat,
                           addThumbnail, &m_TemporarySetting))
    {
        goto error;
    }

    ClearTemporarySetting();

    return m_pWork->ctx.dstOffset;

error:
    ClearTemporarySetting();
    return 0;

invalidMpNumImages:
    SetJpegMpEncoderError(&m_pWork->ctx, JPEG_ENCODER_ERROR_INVALID_MP_NUM_IMAGES);
    goto error;

invalidMpTypeCodeError:
    SetJpegMpEncoderError(&m_pWork->ctx, JPEG_ENCODER_ERROR_INVALID_MP_TYPE_CODE);
    goto error;
}

size_t JpegMpEncoder::StartMpEncoderNext(const void* src, u32 width, u32 height, u32 quality,
                                         PixelSampling dstPixelSampling, PixelFormat srcPixelFormat, bool addThumbnail,
                                         MpTypeCode typeCode, bool omitPixelDimensions)
{
    JpegMpEncoderContext* pCtx = NULL;
    u8* pDstTop;
    size_t paddingSize;
    size_t limitBak;

    if(!m_Initialized)
    {
        return 0;
    }

    pCtx = &m_pWork->ctx;
    pDstTop = pCtx->pDst;

    if(pCtx->errorCode)
    {
        goto error;
    }

    if(!m_pWork->useMpFormat)
    {

        goto invalidMpSequenceError;
    }

    if(m_pWork->app2.curIndex >= m_pWork->app2.numberOfImages)
    {

        goto invalidMpSequenceError;
    }

    paddingSize = pCtx->dstOffset & 1;

    if((pCtx->dstOffset + paddingSize) >= pCtx->writeLimit)
    {
        goto insufficientOutputBufferError;
    }
    if(paddingSize)
    {
        pCtx->pDst[pCtx->dstOffset] = 0;
    }

    switch(typeCode)
    {
    case MP_TYPE_CODE_BASELINE_MP_PRIMARY_IMAGE:

        goto invalidMpTypeCodeError;

    case MP_TYPE_CODE_MULTI_VIEW_PANORAMA_IMAGE:
    case MP_TYPE_CODE_MULTI_VIEW_DISPARITY_IMAGE:
    case MP_TYPE_CODE_MULTI_VIEW_MULTI_ANGLE_IMAGE:
    case MP_TYPE_CODE_UNDEFINED:

        if(m_pWork->app2.isBaseline || (m_pWork->app2.firstTypeCode != typeCode))
        {
            goto invalidMpTypeCodeError;
        }
        break;

    case MP_TYPE_CODE_LARGE_THUMBNAIL_IMAGE_CLASS_1:
    case MP_TYPE_CODE_LARGE_THUMBNAIL_IMAGE_CLASS_2:

        break;

    default:

        goto invalidMpTypeCodeError;
    }

    m_pWork->app2.typeCode = typeCode;
    m_pWork->app2.omitPixelDimensions = omitPixelDimensions;

    limitBak = pCtx->writeLimit;
    if(!StartMpEncoderCore(m_pWork, pCtx->pDst + pCtx->dstOffset + paddingSize,
                           limitBak - (pCtx->dstOffset + paddingSize), src, width, height, quality, dstPixelSampling,
                           srcPixelFormat, addThumbnail, &m_TemporarySetting))
    {
        goto error;
    }

    pCtx->dstOffset += pCtx->pDst - pDstTop;
    pCtx->pDst = pDstTop;
    pCtx->writeLimit = limitBak;

    ClearTemporarySetting();

    return pCtx->dstOffset;

error:
    ClearTemporarySetting();
    return 0;

invalidMpTypeCodeError:
    SetJpegMpEncoderError(pCtx, JPEG_ENCODER_ERROR_INVALID_MP_TYPE_CODE);
    goto error;

invalidMpSequenceError:
    SetJpegMpEncoderError(pCtx, JPEG_ENCODER_ERROR_INVALID_MP_SEQUENCE);
    goto error;

insufficientOutputBufferError:
    SetJpegMpEncoderError(pCtx, JPEG_ENCODER_ERROR_INSUFFICIENT_OUTPUT_BUFFER);
    goto error;
}

bool JpegMpEncoder::GetMpRegionsToBuildJpegData(MpRegionsToBuildJpegData* pBuffer)
{
    NN_ASSERT_(pBuffer);

    if(!m_Initialized)
    {
        goto error;
    }

    if(m_pWork->ctx.errorCode)
    {
        goto error;
    }

    if(!m_pWork->useMpFormat)
    {

        goto error;
    }

    if((m_pWork->app2.typeCode == MP_TYPE_CODE_LARGE_THUMBNAIL_IMAGE_CLASS_1) ||
       (m_pWork->app2.typeCode == MP_TYPE_CODE_LARGE_THUMBNAIL_IMAGE_CLASS_2))
    {
        goto error;
    }

    *pBuffer = m_pWork->mpRegions;
    return true;

error:
    return false;
}

void JpegMpEncoder::SetMakerNote(const u8* pBuffer, size_t size, u32 index)
{
    if(m_Initialized)
    {
        if(index < detail::NUM_MAKER_NOTES)
        {
            if(pBuffer && size)
            {
                m_TemporarySetting.makerNotes[index].pData = pBuffer;
                m_TemporarySetting.makerNotes[index].size = size;
            }
            else
            {
                m_TemporarySetting.makerNotes[index].pData = NULL;
                m_TemporarySetting.makerNotes[index].size = 0;
            }
        }
    }
}

void JpegMpEncoder::SetUserMakerNote(const u8* pBuffer, size_t size)
{
    SetMakerNote(pBuffer, size, detail::MAKER_NOTE_INDEX_USER);
}

void JpegMpEncoder::GetDateTimeNow(char* pBuffer)
{
    nn::fnd::DateTime now = nn::fnd::DateTime::GetNow();
    nn::fnd::DateTimeParameters parameters;

    NN_ASSERT_(pBuffer);

    parameters = now.GetParameters();
    nn::nstd::TSNPrintf(pBuffer, DATE_TIME_SIZE, "%04d:%02d:%02d %02d:%02d:%02d", parameters.year, parameters.month,
                        parameters.day, parameters.hour, parameters.minute, parameters.second);
    pBuffer[DATE_TIME_SIZE - 1] = '\0';
}

#if 0

u8* JpegMpEncoderS::GetLastTwlPrivateDataPointer() const
{
    if (GetLastError() ||
        m_pWork->useMpFormat)
    {
        return NULL;
    }

    return m_pWork->pLastTwlPrivateDataBuffer;
}
#endif

}
}
}
