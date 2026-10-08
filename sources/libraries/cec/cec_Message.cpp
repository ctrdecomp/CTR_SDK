// Filename: cec_Message.cpp
//
// Project: Horizon

#include <cstring>
#include <cstdlib>

#include <nn/math.h>
#include <nn/os.h>
#include <nn/dbg.h>
#include <nn/cec.h>
#include <nn/os/os_Memory.h>
#include <nn/fnd.h>

#include <nn/cec/CTR/cec_Api.h>
#include <nn/cec/CTR/cec_CecAPI.h>
#include <nn/cec/CTR/cec_ControlSys.h>
#include <nn/cec/cec_Result.h>

#define DBG_PRINTF_ERR(format, args...) NN_LOG_ERROR_(format, ##args)

#define NWM_BE2LE16(x)       ((static_cast<u16>(          \
                              (((x) & 0xFF00UL) >> 8UL) | \
                              (((x) & 0x00FFUL) << 8UL))) )
#define NWM_BE2LE32(x)       ((static_cast<u32>(               \
                              (((x) & 0xFF000000UL) >> 24UL) | \
                              (((x) & 0x00FF0000UL) >> 8UL) |  \
                              (((x) & 0x0000FF00UL) << 8UL) |  \
                              (((x) & 0x000000FFUL) << 24UL))) )
#define NWM_LE2BE16(x)       NWM_BE2LE16(x)
#define NWM_LE2BE32(x)       NWM_BE2LE32(x)

namespace nn {
namespace cec {
namespace CTR {

namespace {
    inline u32 GetExHeaderCoreSize()
    {
        CecMessageExHeader dummy;
        return sizeof(dummy.exHeaderType) + sizeof(dummy.exHeaderLen);
    }
    inline u32 GetExHeaderBodySize(const CecMessageExHeader& rMessageExHeader)
    {
        return nn::math::RoundUp(rMessageExHeader.exHeaderLen, 4);
    }
    inline u32 GetExHeaderSize(const CecMessageExHeader& rMessageExHeader)
    {
        return GetExHeaderCoreSize() + GetExHeaderBodySize(rMessageExHeader);
    }
}

Message::Message()
{
    numOfExHeader = 0;
    m_pMessBody = NULL;
    m_pHash = NULL;
    m_hashSize = 0;
    std::memset(m_hmacKey, 0, MESSAGE_HMAC_KEYLEN);
    Init_Message();
}

Message::Message(const void* messData, size_t messSize)
{
    numOfExHeader = 0;
    m_pMessBody = NULL;
    m_pHash = NULL;
    m_hashSize = 0;
    std::memset(m_hmacKey, 0, MESSAGE_HMAC_KEYLEN);
    Init_Message();

    InputMessage(messData, messSize);
}

Result Message::Init_Message()
{
    {
        const s8 nExHeader = numOfExHeader;
        for(int i = 0; i < nExHeader; ++i)
        {
            m_cec_mhex[i].exHeaderLen = 0;
            m_cec_mhex[i].exHeaderType = 0;
        }
        numOfExHeader = 0;
    }
    std::memset(&m_cec_mh, 0, sizeof(CecMessageHeader));
    m_messBodyLen = 0;
    m_messBody = NULL;
    numOfExHeader = 0;
    m_cec_mh.magic16 = MESSAGE_MAGIC;
    m_flag_input = 0;

    return ResultSuccess();
}

Result Message::NewMessage(
    u32 cecTitleId, u32 groupId, MessageTypeFlag messageTypeFlag,
    SendMode sendMode, u8 sendCount, u8 propagationCount,
    const void* icon, size_t iconSize,
    const wchar_t* infoTextData, size_t infoTextSize)
{
    MessageId messageId;
    GetMessageId(&messageId);
    if (m_flag_input || !messageId.IsEmpty())
    {
        Init_Message();
    }

    NN_UTIL_RETURN_IF_FAILED(SetCecTitleId(cecTitleId));
    NN_UTIL_RETURN_IF_FAILED(SetGroupID(groupId));
    NN_UTIL_RETURN_IF_FAILED(SetMessageTypeFlag(messageTypeFlag));
    NN_UTIL_RETURN_IF_FAILED(SetSendMode(sendMode));
    NN_UTIL_RETURN_IF_FAILED(SetSendCount(sendCount));
    NN_UTIL_RETURN_IF_FAILED(SetPropagationCount(propagationCount));

    if (propagationCount > 1 && sendCount > 1)
    {
        return ResultInvalidCombination();
    }

    NN_UTIL_RETURN_IF_FAILED(SetExHeader(2, iconSize, icon));
    NN_UTIL_RETURN_IF_FAILED(SetExHeader(4, infoTextSize, infoTextData));

    return ResultSuccess();
}

Result Message::NewMessage(
    u32 cecTitleId, u32 groupId, 
    MessageTypeFlag messageTypeFlag,
    SendMode sendMode, u8 sendCount,
    u8 propagationCount)
{
    NN_UTIL_RETURN_IF_FAILED(SetCecTitleId(cecTitleId));
    NN_UTIL_RETURN_IF_FAILED(SetGroupID(groupId));
    NN_UTIL_RETURN_IF_FAILED(SetMessageTypeFlag(messageTypeFlag));
    NN_UTIL_RETURN_IF_FAILED(SetSendMode(sendMode));
    NN_UTIL_RETURN_IF_FAILED(SetSendCount(sendCount));
    NN_UTIL_RETURN_IF_FAILED(SetPropagationCount(propagationCount));

    if (propagationCount > 1 && sendCount > 1)
    {
        return ResultInvalidCombination();
    }

    return ResultSuccess();
}

Result Message::SetCecTitleId(u32 cecTitleId)
{
    if (cecTitleId == 0)
    {
        return ResultInvalidArgument();
    }
    m_cec_mh.cecTitleId = cecTitleId;
    return ResultSuccess();
}

Result Message::SetGroupID(u32 groupId)
{
    m_cec_mh.groupId = groupId;
    return ResultSuccess();
}

Result Message::SetSessionID(u32 sessionId)
{
    m_cec_mh.sessionId = sessionId;
    return ResultSuccess();
}

Result Message::SetMessSize(u32 messSize)
{
    m_cec_mh.messSize = messSize;
    return ResultSuccess();
}

Result Message::SetHeaderSize(u32 headerSize)
{
    m_cec_mh.headerSize = headerSize;
    return ResultSuccess();
}

Result Message::SetBodySize(u32 bodySize)
{
    m_cec_mh.bodySize = bodySize;
    return ResultSuccess();
}

Result Message::SetMessageId(const MessageId& messageId)
{
    messageId.GetBinary(m_cec_mh.messageId);
    return ResultSuccess();
}

MessageId Message::GetMessageId(MessageId* messageId) const
{
    if (messageId != NULL)
    {
        *messageId = MessageId(m_cec_mh.messageId);
    }

    return MessageId(m_cec_mh.messageId);
}

Result Message::SetMessageVersion(u32 messVersion)
{
    m_cec_mh.messVersion = messVersion;
    return ResultSuccess();
}

Result Message::SetMessageId_Pair(const MessageId& messageId)
{
    messageId.GetBinary(m_cec_mh.messageId_pair);
    return ResultSuccess();
}

MessageId Message::GetMessageId_Pair(MessageId* messageId) const
{
    if (messageId != NULL)
    {
        *messageId = MessageId(m_cec_mh.messageId_pair);
    }
    return MessageId(m_cec_mh.messageId_pair);
}

Result Message::SetMessageTypeFlag(u8 messTypeFlag)
{
    m_cec_mh.messageTypeFlag = messTypeFlag;
    return ResultSuccess();
}

Result Message::SetSendMode(u8 sendMode)
{
    if (sendMode < 0U || sendMode > 3U)
    {
        return ResultInvalidArgument();
    }
    
    m_cec_mh.sendMode = sendMode;
    return ResultSuccess();
}

Result Message::SetSenderID(u64 senderId)
{
    m_cec_mh.senderId = senderId;
    return ResultSuccess();
}


Result Message::SetSendDate(const nn::fnd::DateTimeParameters& date)
{
    m_cec_mh.sendDate = date;
    return ResultSuccess();
}

Result Message::SetRecvDate(const nn::fnd::DateTimeParameters& date)
{
    m_cec_mh.recvDate = date;
    return ResultSuccess();
}

Result Message::SetCreateDate(const nn::fnd::DateTimeParameters& date)
{
    m_cec_mh.createDate = date;
    return ResultSuccess();
}

Result Message::SetSendCount(u8 sendCount)
{
    m_cec_mh.sendCount = sendCount;
    return ResultSuccess();
}

Result Message::SetPropagationCount(u8 propagationCount)
{
    m_cec_mh.propagationCount = propagationCount;
    return ResultSuccess();
}

Result Message::SetFlag_Unread(u8 flag)
{
    m_cec_mh.flagUnread = flag;
    return ResultSuccess();
}

Result Message::SetFlag_New(u8 flag)
{
    m_cec_mh.flagNew = flag;
    return ResultSuccess();
}

void Message::SetTag(u16 tag)
{
    m_cec_mh.tag = tag;
}

Result Message::SetExHeaderWithoutCalc(
    MessageExHeaderType exhType, size_t exhLen, const void* exhBody
)
{
    if (exhLen > CEC_EXHEADER_SIZE_MAX)
    {
        return ResultTooLarge();
    }
    if (exhBody == NULL)
    {
        return ResultInvalidArgument();
    }
    
    {
        const s8 nExHeader = numOfExHeader;
        for (int i = 0; i < nExHeader; ++i)
        {
            if(m_cec_mhex[i].exHeaderType == exhType)
            {
                m_cec_mhex[i].exHeaderLen = exhLen;
                m_cec_mhex[i].exHeaderData = static_cast<u8*>(const_cast<void*>(exhBody));
                return ResultSuccess();
            }
        }
    }

    NN_TASSERT_(numOfExHeader < CEC_EXHEADER_NUM_MAX);

    m_cec_mhex[numOfExHeader].exHeaderType = exhType;
    m_cec_mhex[numOfExHeader].exHeaderLen = exhLen;
    m_cec_mhex[numOfExHeader].exHeaderData = static_cast<u8*>(const_cast<void*>(exhBody));

    numOfExHeader++;
    return ResultSuccess();
}

Result Message::GetInfoText(const wchar_t** infoTextData, size_t* infoTextSize) const
{
    return GetExHeader(4U, infoTextSize, reinterpret_cast<void**>(const_cast<wchar_t**>(infoTextData)));
}

Result Message::SetExHeader(u32 exhType, size_t exhLen, const void* exhBody)
{
    MessageId messageId;
    GetMessageId(&messageId);
    if (m_flag_input || !messageId.IsEmpty())
    {
        return ResultNotAuthorized();
    }

    NN_UTIL_RETURN_IF_FAILED(SetExHeaderWithoutCalc(exhType, exhLen, exhBody));
    calcCecMessSize();
    return ResultSuccess();
}

Result Message::GetExHeader(u32 exhType, size_t* exhLen, void** exhBody) const
{
    NN_TASSERT_(exhLen);
    NN_TASSERT_(exhBody);

    const s8 nExHeader = numOfExHeader;

    if (nExHeader == 0)
    {
        *exhLen = 0;
        return ResultNoData();
    }

    for (int i = 0; i < nExHeader; ++i)
    {
        if (m_cec_mhex[i].exHeaderType == exhType)
        {
            *exhLen = m_cec_mhex[i].exHeaderLen;
            *exhBody = m_cec_mhex[i].exHeaderData;
            return ResultSuccess();
        }
    }
    *exhLen = 0;
    return ResultNoData();
}

Result Message::SetMessageBody(const void* dataBody, size_t size)
{
    if (!dataBody)
    {
        return ResultInvalidArgument();
    }

    if ((size & 0x3) != 0)
    {
        return ResultMisalignedSize();
    }
    
    if (size + sizeof(CecMessageHeader) > CEC_MESSSIZEMAX_DEFAULT || size <= 0)
    {
        return ResultTooLarge();
    }

    if (size > MESSAGE_BODY_SIZE_MAX)
    {
        return ResultTooLarge();
    }

    MessageId messageId;
    GetMessageId(&messageId);
    if (m_flag_input || !messageId.IsEmpty())
    {
        return ResultNotAuthorized();
    }
    
    {
        m_messBodyLen = size;
        m_pMessBody = static_cast<u8*>(const_cast<void*>(dataBody));
    }
    SetBodySize(m_messBodyLen);

    calcCecMessSize();
    return ResultSuccess();
}

u32 Message::GetMessageBody(void* dataBody, size_t size) const
{
    NN_TASSERT_(dataBody);
    if (size > m_messBodyLen)
    {
        size = m_messBodyLen;
    }

    if (m_pMessBody != NULL)
    {
        std::memcpy(dataBody, m_pMessBody, size);
    }
    return m_messBodyLen;
}

u32 Message::MakeMessageBinary(void* messData) const
{
    NN_TASSERT_(messData);

    u8* pMessageData = static_cast<u8*>(messData);

    std::memcpy(pMessageData, &m_cec_mh, sizeof(CecMessageHeader));
    pMessageData += sizeof(CecMessageHeader);

    const u32 nExHeader = numOfExHeader;
    for (int i = 0; i < nExHeader; ++i)
    { 
        std::memcpy(pMessageData, &m_cec_mhex[i], GetExHeaderCoreSize());
        pMessageData += GetExHeaderCoreSize();

        std::memcpy(pMessageData, m_cec_mhex[i].exHeaderData, m_cec_mhex[i].exHeaderLen);
        const u32 exHeaderBodySize = GetExHeaderBodySize(m_cec_mhex[i]);

        pMessageData += exHeaderBodySize;
    }

    if (m_pMessBody != NULL)
    {
        std::memcpy(pMessageData, m_pMessBody, m_messBodyLen);
        pMessageData += m_messBodyLen;
    }

    if (m_pHash)
    {
        std::memcpy(pMessageData, m_pHash, 32);
    }
    return m_cec_mh.messSize;
}

void Message::OutputMessageHeader(void* pHeaderBuf) const
{
    NN_TASSERT_(pHeaderBuf);
    std::memcpy(pHeaderBuf, &m_cec_mh, sizeof(CecMessageHeader));
}

u32 Message::calcCecMessSize()
{
    u32 headerSize = sizeof(m_cec_mh);
    {
        const u32 nExHeader = numOfExHeader;
        for (int i = 0; i < nExHeader; ++i)
        {
            headerSize += GetExHeaderCoreSize();
            headerSize += GetExHeaderBodySize(m_cec_mhex[i]);
        }
    }
    m_cec_mh.headerSize = headerSize;

    m_cec_mh.messSize = headerSize + m_messBodyLen + 32;

    return m_cec_mh.messSize;
}

} // namespace CTR
} // namespace cec
} // namespace nn
