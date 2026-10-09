#pragma once

#include <nn/fnd.h>
#include <nn/cec/CTR/cec_Types.h>

namespace nn{
namespace cec{
namespace CTR{

struct CecMessageHeader;

#define CEC_EXHEADER_NUM_MAX    (16)
#define CEC_EXHEADER_SIZE_MAX   (8*1024)

struct CecMessageExHeader
{
    u32  exHeaderType;
    u32  exHeaderLen;
    u8*  exHeaderData;
};

#define LOAD_FLAG_POINTER   1
#define LOAD_FLAG_MALLOC    0

class Message
{
private:
    struct CecMessageHeader m_cec_mh;
    struct CecMessageExHeader m_cec_mhex[CEC_EXHEADER_NUM_MAX] NN_ATTRIBUTE_ALIGN(4);
    s8 numOfExHeader;
    u8 m_flag_pointer;
    u8 m_flag_input;
    u8 pad;
    u8* m_messBody;
    u32 m_messBodyLen;
    u8* m_pMessBody;
    u8* m_pHash;
    u32 m_hashSize;
    u8 m_hmacKey[MESSAGE_HMAC_KEYLEN];

    u32 calcCecMessSize();
    Result Init_Message();
    Result SetExHeaderWithoutCalc(MessageExHeaderType exhType, size_t exhLen, const void* exhBody);
public:
    Message();
    Message(const void* messData, size_t messSize);
    ~Message();

    Result NewMessage(TitleId cecTitleId, u32 groupId, MessageTypeFlag messageTypeFlag, SendMode sendMode, u8 sendCount, u8 propagationCount, const void* icon, size_t iconSize,
        const wchar_t* infoTextData, size_t infoTextSize);
    Result NewMessage(u32 cecTitleId, u32 groupId, MessageTypeFlag messageTypeFlag,SendMode sendMode, u8 sendCount,u8 propagationCount);

    Result  SetCecTitleId(TitleId cecTitleId);
    TitleId GetCecTitleId() const{ return m_cec_mh.cecTitleId; }

    Result  SetGroupID(u32 groupId);
    u32     GetGroupID() const { return m_cec_mh.groupId; }

    Result  SetSessionID(u32 sessionId);
    u32     GetSessionID() const{ return m_cec_mh.sessionId; }

    Result  SetMessSize(u32 messSize);
    u32     GetMessSize() const { return m_cec_mh.messSize; }

    Result  SetHeaderSize(u32 headerSize);
    u32     GetHeaderSize() const{ return m_cec_mh.headerSize; }
    
    Result  SetBodySize(u32 bodySize);
    u32     GetBodySize() const { return m_cec_mh.bodySize; }

    Result  SetMessageVersion(u32 messVersion);
    u32     GetMessageVersion() const { return m_cec_mh.messVersion; }

    Result    SetMessageId_Pair(const MessageId& messIdPair);
    MessageId GetMessageId_Pair(MessageId* messIdPair) const;

    Result          SetMessageTypeFlag(u8 messTypeFlag);
    MessageTypeFlag GetMessageTypeFlag() const{ return m_cec_mh.messageTypeFlag;}

    Result   SetSendMode(SendMode sendMode);
    SendMode GetSendMode() const{ return m_cec_mh.sendMode; }

    Result SetSenderID(u64 senderId);
    u64    GetSenderID() const{ return m_cec_mh.senderId; }

    Result SetSendDate(const nn::fnd::DateTimeParameters& date);
    nn::fnd::DateTimeParameters GetSendDate() const{ return m_cec_mh.sendDate; }

    Result SetRecvDate(const nn::fnd::DateTimeParameters& date);
    nn::fnd::DateTimeParameters GetRecvDate() const{ return m_cec_mh.recvDate; }

    Result SetCreateDate(const nn::fnd::DateTimeParameters& date);
    nn::fnd::DateTimeParameters GetCreateDate() const { return m_cec_mh.createDate; }

    Result  SetSendCount(const u8 sendCount);
    u8      GetSendCount() const{ return m_cec_mh.sendCount;}

    Result  SetPropagationCount(const u8 propagtionCount);
    u8      GetPropagationCount() const{ return m_cec_mh.propagationCount; }

    Result  SetFlag_Unread(const u8 flagUnreadCount);
    u8      GetFlag_Unread() const{ return m_cec_mh.flagUnread; }

    Result  SetFlag_New(const u8 flagNewCount);
    u8      GetFlag_New() const{ return m_cec_mh.flagNew; }

    void    SetTag(const u16 tagCount);
    bit16   GetTag() const{ return m_cec_mh.tag;}

    Result  SetExHeader(MessageExHeaderType exhType, size_t exhLen, const void* exhBody);
    Result  GetExHeader(MessageExHeaderType exhType, size_t* exhLen, void** exhBody) const;

    inline Result SetIcon(void* iconData, size_t iconSize){ return SetExHeader(2, iconSize, iconData); }
    inline Result GetIcon(void** iconData, size_t* iconSize) const{ return GetExHeader(2, iconSize, iconData); }

    inline Result SetInfoText(const wchar_t* infoTextData, size_t infoTextSize){ return SetExHeader(4 , infoTextSize, infoTextData);}
    Result        GetInfoText(const wchar_t** infoTextData, size_t* infoTextSize) const;

    Result SetMessageBody(const void* dataBody, size_t size);
    u32    GetMessageBody(void* dataBody, size_t size) const;

    Result InputMessage(const void* mess, size_t size);
    u32    MakeMessageBinary(void* messData) const;
    void   OutputMessageHeader(void* pHeaderBuf) const;  
    
    Result    SetMessageId(const MessageId& messId);
    MessageId GetMessageId(MessageId* messId) const;
};



} // namespace CTR
} // namespace cec
} // namespace nn

