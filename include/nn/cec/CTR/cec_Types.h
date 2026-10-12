#pragma once

#include <nn/types.h>
#include <nn/fnd/fnd_DateTime.h>
#include <nn/cec/CTR/cec_MessageId.h>

#define MESSAGE_HMAC_KEYLEN (32)

namespace nn {
namespace cec {
namespace CTR {

    static const size_t MESSAGE_BODY_SIZE_MAX  = 96*1024; 

    typedef u32 TitleId;
    typedef u32 GroupId;
    typedef bit64 SenderId;
    typedef u8 SendMode;
    typedef u8 MessageTypeFlag;
    typedef MessageTypeFlag CecMessageTypeFlag;
    typedef u32 MessageExHeaderType;
    typedef MessageExHeaderType CecMessageExHeaderType;

    static const s32 CEC_SIZEOF_MESSAGEID = (8);
    static const u8 CEC_OUTBOX_MESSNUM_DEFAULT = 99;

    #define MESSAGE_MAGIC       (0x6060)
    #define MESSAGE_VERSION     (0x00000000)

    struct CecMessageHeader
    {
        u16 magic16;
        s16 __PADDING__;

        u32 messSize;
        u32 headerSize;
        u32 bodySize;

        u32 cecTitleId;
        u32 cecTitleId_reserve;
        u32 groupId;
        u32 sessionId;
        u8  messageId[8];
        u32 messVersion;
        u8  messageId_pair[8];
        u8  messageTypeFlag;
        u8  sendMode;
        bit8 flagUnread;
        bit8 flagNew;
        u64 senderId;
        u64 senderId2;
        nn::fnd::DateTimeParameters sendDate;
        nn::fnd::DateTimeParameters recvDate;
        nn::fnd::DateTimeParameters createDate;
        u8 sendCount;
        u8 propagationCount;
        bit16 tag;
    };

} // namespace CTR
} // namespace cec
} // namespace nn

