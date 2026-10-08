#pragma once

#define CEC_INBOX_SIZE_DEFAULT         (512*1024)
#define CEC_OUTBOX_SIZE_DEFAULT        (512*1024)
#define CEC_INBOX_MESSNUM_DEFAULT      (99)
#define CEC_OUTBOX_MESSNUM_DEFAULT     (99)
#define CEC_MESSSIZEMAX_DEFAULT        (100*1024)

#define CEC_BOX_DATA_SIZE_MAX           (10*1024)

#define CEC_BOXNUMMAX_APPLICATION      (12)
#define CEC_BOXNUMMAX_SYSTEM           (6) 

#define CEC_BOXNUMMAX_SYSTEM2          (6)
#define MESSAGE_BOX_NUM_MAX     (CEC_BOXNUMMAX_APPLICATION + CEC_BOXNUMMAX_SYSTEM + CEC_BOXNUMMAX_SYSTEM2)

#define CEC_BOXDIR_NAMELEN_MAX (16)

#include <nn/cec/CTR/cec_Api.h>
namespace nn  {
namespace cec {
namespace CTR {

typedef bit8 MessageBoxFlag;

    static const bit8 MESSAGE_BOX_FLAG_APPLICATION  =  (0x1);

    static const bit8 MESSAGE_BOX_FLAG_SYSTEM  =  (0x2);

    static const bit8 MESSAGE_BOX_FLAG_SYSTEM2  =  (0x4);

    static const bit8 MESSAGE_BOX_FLAG_HIDDEN  =  (0x80);

enum CecBoxType
{
    CEC_BOXTYPE_INBOX  = (0x0),
    CEC_BOXTYPE_OUTBOX = (0x1)
};

#define MESSAGEBOXLIST_MAGIC 0x6868
#define CEC_BOX_VERSION 0x00000001

struct MessageBoxList
{
    u16 magic16;
    u16 __PADDING__;
    u32 CecBoxVersion;
    u32 DirNum;
    u8 DirName[MESSAGE_BOX_NUM_MAX][CEC_BOXDIR_NAMELEN_MAX];
};

#define MESSAGEBOXINFO_MAGIC 0x6363

struct MessageBoxInfo
{
    u16 magic16;
    u16 __PADDING_1__;
    u32 cecTitleId;
    u32 privateId;
    MessageBoxFlag MessageBoxInfoFlag;
    bool isActive;
    u16 __PADDING_2__;
    char  hmacKey[MESSAGE_HMAC_KEYLEN];
    u32 MessageBoxInfoSize;
    nn::fnd::DateTimeParameters lastOpened;
    u8 flag1;
    u8 flag2;
    u8 flag3;
    u8 flag4;
    nn::fnd::DateTimeParameters lastReceived;
    u8  reserved[16];
};

#define CEC_BOXINFO_MAGIC 0x6262
#define CEC_SIZEOF_BOXINFO_HEADER (sizeof(struct CecBoxInfoHeader))

struct CecBoxInfoHeader
{
    u16 magic16;
    u16 __PADDING_1__;
    u32 boxInfoSize;
    u32 boxSizeMax;
    u32 boxSize;
    u32 messNumMax;
    u32 messNum;
    u32 groupNumMax;
    u32 messSizeMax;
};

#define CEC_OUTBOXINDEX_MAGIC 0x6767

struct CecOutBoxIndexHeader
{
    u16 magic16;
    u16 __PADDING_1__;
    size_t messageNum;
};

class MessageBox
{
protected:
    u32 currentCecTitleId;

    bool m_Initialized;
    bool m_BoxAccessible;

    bool b_SkippedWriteInboxInfo;
    bool b_SkippedWriteOutboxInfo;

    u8* cecBoxBuf;

    struct MessageBoxList cecMessageBoxList;
    struct MessageBoxInfo cecMessageBoxInfo;

    struct CecBoxInfoHeader cecInboxInfo;
    CecMessageHeader* cecInboxInfo_body[CEC_INBOX_MESSNUM_DEFAULT];
    u8* cecInboxInfoBuf;
    struct CecBoxInfoHeader cecOutboxInfo;
    CecMessageHeader* cecOutboxInfo_body[CEC_OUTBOX_MESSNUM_DEFAULT];
    u8* cecOutboxInfoBuf;

    CecOutBoxIndexHeader cecOutboxIndex_header;
    CECMessageId cecOutboxIndex[CEC_OUTBOX_MESSNUM_DEFAULT];

    static nn::os::CriticalSection m_Cs;
    static nn::os::CriticalSection m_CsMem;

    struct CecBoxInfoHeader tmp_cecInboxInfo;
    struct CecBoxInfoHeader tmp_cecOutboxInfo;
public:

    explicit MessageBox(bool NoInitialize = false);
    virtual ~MessageBox();
protected:
    Result Initialize();
    Result Finalize();

    Result WriteMessageBoxList();
    size_t ReadMessageBoxList();

 public:
    u32 GetMessageBoxNum(MessageBoxFlag flag);
    size_t ReadMessageBoxList(void* bufOut, size_t size);

    inline u32 GetCurrentCecTitleId() const{return currentCecTitleId;}

    Result CreateMessageBox(
        const TitleId cecTitleId, const u32 privateId,
        const char* hmacKey, const void* icon, size_t iconSize,
        const wchar_t* name, size_t  nameSize, size_t inboxSizeMax = CEC_INBOX_SIZE_DEFAULT,
        size_t outboxSizeMax = CEC_OUTBOX_SIZE_DEFAULT,
        size_t inboxMessNumMax = CEC_INBOX_MESSNUM_DEFAULT,
        size_t outboxMessNumMax = CEC_OUTBOX_MESSNUM_DEFAULT,
        size_t messageSizeMax = CEC_MESSSIZEMAX_DEFAULT);

    Result CreateMessageBox(
        const TitleId cecTitleId, const u32 privateId, const char* hmacKey,
        size_t inboxSizeMax = CEC_INBOX_SIZE_DEFAULT,
        size_t outboxSizeMax = CEC_OUTBOX_SIZE_DEFAULT,
        size_t inboxMessNumMax = CEC_INBOX_MESSNUM_DEFAULT,
        size_t outboxMessNumMax = CEC_OUTBOX_MESSNUM_DEFAULT,
        size_t messageSizeMax = CEC_MESSSIZEMAX_DEFAULT);

    void CreateInBox(const TitleId cecTitleId);
    void CreateOutBox(const TitleId cecTitleId);

    Result DeleteMessageBox();
    Result DeleteMessageBox(const TitleId cecTitleId);
    Result OpenMessageBox(const TitleId cecTitleId, const u32 privateId);

    void CloseMessageBox(bool bWithoutCommit);
    void CloseMessageBox();
    bool IsOpened() const;

    inline Result SetBoxActivate(bool active)
    {
        cecMessageBoxInfo.isActive = active;
        return WriteMessageBoxInfo();
    }

    inline bool GetBoxActivate()
    {
        return cecMessageBoxInfo.isActive;
    }

    Result ReadMessageBoxInfo(struct MessageBoxInfo* outbuf) const;

    Result GetBoxInfo(struct CecBoxInfoHeader* boxinfo, CecMessageHeader** boxInfoBody, CecBoxType boxType) const;
    Result ReadBoxInfo(struct CecBoxInfoHeader* boxinfo, CecMessageHeader** boxInfoBody, u8* buf, CecBoxType boxType);

    Result ReadMessageBoxInfo(struct MessageBoxInfo* outbuf, TitleId cecTitleId) const;
    Result WriteBoxInfo(CecBoxType boxType ,CecBoxInfoHeader& boxInfo , CecMessageHeader** boxInfoBody);

protected:
    Result ReadMessageBoxInfo() const;
    Result WriteMessageBoxInfo();

    nn::Result ReadInBoxInfo();

    Result WriteInBoxInfo();
    Result ReadOutBoxInfo();
    Result WriteOutBoxInfo();

public:

    Result ReadMessage(Message& cecMessage, void* buf, const size_t bufLen, const CecBoxType boxType, const MessageId& messageId);
    Result ReadMessage(void* bufOut, const size_t bufLen, const CecBoxType boxType, const MessageId& messageId);

    Result WriteMessage(const Message& cecMessage,const CecBoxType boxType, MessageId& messageIdOut);
    Result WriteMessage(const Message& cecMessage,const CecBoxType boxType, MessageId& messageIdOut, bool withWriteBoxInfo);

    Result  WriteMessage(const Message& cecMessage,const CecBoxType boxType)
    {
        MessageId messageIdOut;
        return WriteMessage(cecMessage, boxType, messageIdOut);
    }

    Result WriteMessage(const void* message, const size_t messageLen ,const CecBoxType boxType, MessageId& messageIdOut);

    Result DeleteMessage(const CecBoxType boxType, const MessageId& messageId);
    Result DeleteMessage(const CecBoxType boxType, const MessageId& messageId, bool withWriteBoxInfo);
    Result DeleteAllMessages(const CecBoxType boxType);
    u32 DeleteSentMessages();

    Result CommitMessageBox();
protected:
    u8 ReadInBoxMessage(void* bufOut, const size_t messageLen, const MessageId& messageId);
    u8 WriteInBoxMessage(const void* message, const size_t messageLen, MessageId* pMessageIdOut);
    u8 WriteInBoxMessage(Message& cecMessage, MessageId* pMessageId);
    u8 DeleteInBoxMessage(const CECMessageId messageId);
    u8 ReadOutBoxMessage(void* bufOut, const size_t messageLen, const MessageId& messageId);
    u8 WriteOutBoxMessage(const void* message, const size_t messageLen, MessageId* pMessageIdOut);
    u8 WriteOutBoxMessage(Message& cecMessage, MessageId* pMessageIdOut);
    u8 DeleteOutBoxMessage(const MessageId& messageId);

    Result CheckEulaParentalControl();

public:

    Result GetMessageBoxData(u32 datatype, void* dataBuf, size_t dataBufSize) const;
    size_t GetMessageBoxDataSize(const u32 datatype) const;
    Result SetMessageBoxData(const u32 datatype, const void* data, const size_t dataSize);
    Result SetMessageBoxName(const wchar_t* data, size_t dataSize);
    Result SetMessageBoxIcon(const void* data, size_t dataSize);
    Result GetMessageBoxName(wchar_t* dataBuf, size_t dataBufSize) const;
    Result GetMessageBoxIcon(void* dataBuf, size_t dataBufSize) const;

private:
    u32 GetInBoxGroupNum() const;
    u32 GetOutBoxGroupNum() const;

    inline MessageBoxFlag GetBoxFlag()
    {
        return cecMessageBoxInfo.MessageBoxInfoFlag;
    }

protected:
    inline u32 GetInBoxMessNumMax()
    {
        return cecInboxInfo.messNumMax;
    }

    inline u32 GetInBoxMessNum()
    {
        return cecInboxInfo.messNum;
    }

    inline u32 GetOutBoxMessNumMax()
    {
        return cecOutboxInfo.messNumMax;
    }

    inline u32 GetOutBoxMessNum()
    {
        return cecOutboxInfo.messNum;
    }

protected:
    inline u32 GetInBoxSizeMax()
    {
        return cecInboxInfo.boxSizeMax;
    }

    inline u32 GetInBoxSize()
    {
        return cecInboxInfo.boxSize;
    }
    inline u32 GetOutBoxSizeMax()
    {
        return cecOutboxInfo.boxSizeMax;
    }

    inline u32 GetOutBoxSize()
    {
        return cecOutboxInfo.boxSize;
    }

    inline u32 GetInBoxMessSizeMax()
    {
        return cecInboxInfo.messSizeMax;
    }

    inline u32 GetOutBoxMessSizeMax()
    {
        return cecOutboxInfo.messSizeMax;
    }

 public:

    inline u32 GetBoxSizeMax(CecBoxType boxType)
    {
        if(boxType == CEC_BOXTYPE_INBOX)
        {
            return GetInBoxSizeMax();
        }
        else
        {
            return GetOutBoxSizeMax();
        }
    }

    inline u32 GetBoxSize(CecBoxType boxType)
    {
        if(boxType == CEC_BOXTYPE_INBOX)
        {
            return GetInBoxSize();
        }
        else
        {
            return GetOutBoxSize();
        }
    }

    inline u32 GetBoxMessNumMax(CecBoxType boxType)
    {
        return GetBoxMessageNumMax(boxType);
    }

    inline u32 GetBoxMessageNumMax(CecBoxType boxType)
    {
        if(boxType == CEC_BOXTYPE_INBOX)
        {
            return GetInBoxMessNumMax();
        }
        else
        {
            return GetOutBoxMessNumMax();
        }
    }

    inline u32 GetBoxMessNum(CecBoxType boxType)
    {
        return GetBoxMessageNum(boxType);
    };

    inline u32 GetBoxMessageNum(CecBoxType boxType)
    {
        if(boxType == CEC_BOXTYPE_INBOX)
        {
            return GetInBoxMessNum();
        }
        else
        {
            return GetOutBoxMessNum();
        }
    }

private:
    inline u32 GetInBoxGroupNumMax()
    {
        return cecInboxInfo.groupNumMax;
    }
    u32 GetInBoxGroupNum();

    inline u32 GetOutBoxGroupNumMax()
    {
        return cecOutboxInfo.groupNumMax;
    }

    u32     GetOutBoxGroupNum();

public:
    inline u32 GetBoxGroupNumMax(CecBoxType boxType)
    {
        if(boxType == CEC_BOXTYPE_INBOX)
        {
            return GetInBoxGroupNumMax();
        }
        else
        {
            return GetOutBoxGroupNumMax();
        }
    }

    inline u32 GetBoxGroupNum(CecBoxType boxType)
    {
        if(boxType == CEC_BOXTYPE_INBOX)
        {
            return GetInBoxGroupNum();
        }
        else
        {
            return GetOutBoxGroupNum();
        }
    }

    Result     SetBoxGroupNumMax(u32 num, CecBoxType boxType);
protected:
    Result SetInBoxGroupNumMax(u32 num);
    Result SetOutBoxGroupNumMax(u32 num);

    u32 GetInBoxSessionNum();

public:
    inline u32 GetBoxSessionNum(CecBoxType boxType)
    {
        if(boxType == CEC_BOXTYPE_INBOX)
        {
            return GetInBoxSessionNum();
        }
        else
        {
            return 0;
        }
    }

protected:
    u32 GetInBoxMessHeader(CecMessageHeader& messHeader, const MessageId& messageId) const;
    u32 GetInBoxMessHeaderByIndex(CecMessageHeader& messHeader, u32 messIndex) const;

public:
    u8* GetInBoxMessIdByIndex(u32 messIndex);
    u8* GetOutBoxMessIdByIndex(u32 messIndex) ;

protected:
    u32 GetOutBoxMessHeader(CecMessageHeader& messHeader, const MessageId& messageId) const;
    u32 GetOutBoxMessHeaderByIndex(CecMessageHeader& messHeader, u32 messIndex) const;
public:
    size_t GetBodySizeByMessId(CecBoxType boxType, const MessageId& messageId) const;
protected:

    CecMessageHeader* GetMessHeaderByMessId(const CecBoxType boxType, const MessageId& messId) const;
public:

    CecMessageHeader* GetMessHeader(const CecBoxType boxType, const u32 messIndex) const;

    bool MessageExists(CecBoxType boxType, const MessageId& messageId) const;

public:
    inline u32 GetMessageMessSize(const CecBoxType boxType, const u32 messIndex)
    {
        return GetMessageSize(boxType, messIndex);
    }

    u32 GetMessageSize(const CecBoxType boxType, const u32 messIndex) const;

    u32 GetMessageBodySize(const CecBoxType boxType, const u32 messIndex) const;

    u32 GetMessageGroupId(const CecBoxType boxType, const u32 messIndex) const;

    u32 GetMessageSessionId(const CecBoxType boxType, const u32 messIndex) const;

    inline MessageTypeFlag GetMessageMessTypeFlag(const CecBoxType boxType, const u32 messIndex)
    {
        return GetMessageTypeFlag(boxType, messIndex);
    };

    MessageTypeFlag GetMessageTypeFlag(const CecBoxType boxType, const u32 messIndex) const;

    SendMode GetMessageSendMode(const CecBoxType boxType, const u32 messIndex) const;

    u8 GetMessageSendCount(const CecBoxType boxType, const u32 messIndex) const;

    u8 GetMessagePropagationCount(const CecBoxType boxType, const u32 messIndex) const;

    bit8 GetMessageFlag_Unread(const CecBoxType boxType, const u32 messIndex) const;

    bit8 GetMessageFlag_New(const CecBoxType boxType, const u32 messIndex) const;

    bit16 GetMessageTag(const CecBoxType boxType, const u32 messIndex) const;

    nn::fnd::DateTimeParameters GetMessageSendDate(const CecBoxType boxType, const u32 messIndex) const;

    nn::fnd::DateTimeParameters GetMessageRecvDate(const CecBoxType boxType, const u32 messIndex) const;

    nn::fnd::DateTimeParameters GetMessageCreateDate(const CecBoxType boxType, const u32 messIndex) const;

    u32 GetMessageIndex(CecBoxType boxType, MessageId& messId);
    u32 GetMessageIndex(CecBoxType boxType, u8* messId);

    Result GetMessageIdPair(MessageId* messId, const CecBoxType boxType, const u32 messIndex);
    MessageId GetMessageIdPair(const CecBoxType boxType, const u32 messIndex);

    Result GetMessageId(MessageId* messId, const CecBoxType boxType, const u32 messIndex);
    MessageId GetMessageId(const CecBoxType boxType, const u32 messIndex);

    inline Result GetMessageMessIdPair(MessageId* messId, const CecBoxType boxType, const u32 messIndex)
    {
        return GetMessageIdPair(messId, boxType, messIndex);
    };

    inline MessageId GetMessageMessIdPair(const CecBoxType boxType, const u32 messIndex)
    {
        return GetMessageIdPair(boxType, messIndex);
    }

    inline Result GetMessageMessId(MessageId* messId, const CecBoxType boxType, const u32 messIndex)
    {
        return GetMessageId(messId, boxType, messIndex);
    }

    inline MessageId GetMessageMessId(const CecBoxType boxType, const u32 messIndex)
    {
        return GetMessageId(boxType, messIndex);
    }

    inline u32 GetMessIndex(CecBoxType boxType, MessageId& messId)
    {
        return GetMessageIndex(boxType, messId);
    };

    inline u32 GetMessIndex(CecBoxType boxType, u8* messId)
    {
        return GetMessageIndex(boxType, messId);
    }

    u32 AppendOutBoxIndex(const MessageId& messageId);
    u32 RemoveOutBoxIndex(const MessageId& messageId);
    u32 RoundOutBoxIndex(const MessageId& messageId);
protected:

    u32 ResetOutBoxIndex();
    u32 ReadOutBoxIndex();

    Result AllocInboxInfoBodyBuf(size_t size);
    Result AllocOutboxInfoBodyBuf(size_t size);
    Result FreeInboxInfoBodyBuf();
    Result FreeOutboxInfoBodyBuf();

    Result WriteOutBoxIndex();
    Result IsAgreeEulaAppRequired() const;
protected:

    virtual Result OpenFile(const u32 cecTitleId, const u32 dataType, const u32 option, size_t* filesize ) const ;
    virtual Result ReadFile(void* readBuf, size_t readBufLen, size_t* readLen) const;
    virtual Result WriteFile(const void* writeBuf, const size_t writeBufLen) const;
    virtual Result ReadMessageFile(
        u32 cecTitleId, u8 boxType, const u8* pMessId,
        size_t messIdLen, size_t* pReadLen, u8* pReadBuf,
        size_t len);

    virtual Result ReadMessageFileWithHmac(
        u32 cecTitle, u8 boxType, const u8* pMessId,
        size_t messIdLen, size_t* pReadLen, u8* pReadBuf,
        size_t len, u8* pHmac);

    virtual Result WriteMessageFile(
        u32 cecTitleId, u8 boxType, u8* pMessId,
        size_t messIdLen, const u8* pWriteBuf, size_t len);

    virtual Result WriteMessageFileWithHmac(
        u32 cecTitleId, u8 boxType, u8* pMessId,
        size_t messIdLen, const u8* pWriteBuf, size_t len, u8* pHmac);

    virtual Result Delete(u32 cecTitleId, u32 dataType, u8 boxType, const u8 pMessId[], size_t messIdLen );
    virtual Result SetData(u32 cecTitleId, const u8* pSetBuf, size_t len, u32 option);
    virtual Result ReadData(u8 *pReadBuf, size_t len, u32 option, const u8 optionData[], size_t optionDataLen);
    virtual Result OpenAndWriteFile(const u8 pWriteBuf[], size_t writeBufLen, u32 cecTitleId, u32 dataType, u32 option) const;
    virtual Result OpenAndReadFile(u8 pReadBuf[], size_t readBufLen, size_t* pReadLen, u32 cecTitleId, u32 dataType, u32 option) const;

};

} // namespace CTR
} // namespace cec
} // namespace nn
