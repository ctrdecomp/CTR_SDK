// Filename: cec_MessageBox.cpp
//
// Project: Horizon

#include <cstring>
#include <cstdlib>

#include <nn/math.h>
#include <nn/os.h>
#include <nn/dbg.h>
#include <nn/cec.h>
#include <nn/fs.h>
#include <nn/os/os_Memory.h>
#include <nn/fnd.h>

#include <nn/cec/CTR/cec_Api.h>
#include <nn/cec/CTR/cec_CecAPI.h>
#include <nn/cec/CTR/cec_ControlSys.h>
#include <nn/cec/cec_Result.h>

#define CHECK_CURRENTCECTITLEID()                                                                                      \
    do                                                                                                                 \
    {                                                                                                                  \
        if (currentCecTitleId == 0)                                                                                    \
            return ResultNotAuthorized();                                                                              \
    } while (0)

#define TRACE()

#define DBG_PRINTF_ERR(format, args...) NN_LOG_ERROR_(format, ##args)

#define MEMCPY(dest, src, len) std::memcpy(dest, src, len)

namespace
{
bool IsInboxInfoBodyBufAllocated;
bool IsOutboxInfoBodyBufAllocated;
bool b_IsBoxOpened = false;
bool b_IsSuspended = false;

volatile bool b_IsAgreeEulaAppRequired = false;
} // namespace

namespace nn {
namespace cec {
namespace CTR {

nn::os::CriticalSection MessageBox::m_Cs = nn::WithInitialize();
nn::os::CriticalSection MessageBox::m_CsMem = nn::WithInitialize();

#define SCOPED_LOCK(m_Cs) nn::os::CriticalSection::ScopedLock locker(m_Cs)

MessageBox::MessageBox(bool NoInitialize)
{
    m_Initialized = false;
    IsInboxInfoBodyBufAllocated = false;
    IsOutboxInfoBodyBufAllocated = false;
    b_SkippedWriteInboxInfo = false;
    b_SkippedWriteOutboxInfo = false;

    std::memset(&tmp_cecInboxInfo, 0, sizeof(tmp_cecInboxInfo));
    std::memset(&tmp_cecOutboxInfo, 0, sizeof(tmp_cecOutboxInfo));

    if (!NoInitialize)
    {
        cecInboxInfo.magic16 = CEC_BOXINFO_MAGIC;
        cecOutboxInfo.magic16 = CEC_BOXINFO_MAGIC;
        cecOutboxIndex_header.magic16 = CEC_OUTBOXINDEX_MAGIC;

        Initialize();
    }
}

MessageBox::~MessageBox()
{
    Finalize();
}

Result MessageBox::Initialize()
{
    nn::Result result = ResultSuccess();
    if (m_Initialized == false)
    {
        currentCecTitleId = 0;
        if (CecControl::IsInitialized() == false)
        {
            DBG_PRINTF_ERR("%s(%d): ### CecControl is not Initialized...\n", __FUNCTION__, __LINE__);
        }
        if (CecControlSys::IsInitializedSys())
        {
            m_BoxAccessible = true;
        }
        else
        {
            m_BoxAccessible = false;
        }

        m_Initialized = true;
    }
    return result;
}

Result MessageBox::Finalize()
{
    Result result = ResultSuccess();
    currentCecTitleId = 0;

    if (b_IsBoxOpened)
    {
        CloseMessageBox();
    }

    FreeInboxInfoBodyBuf();
    FreeOutboxInfoBodyBuf();

    if (b_IsSuspended)
    {
        CecControl::StartScanning();
        b_IsSuspended = false;
    }

    if (m_Initialized)
    {
        m_Initialized = false;
    }

    return result;
}

Result MessageBox::FreeInboxInfoBodyBuf()
{
    nn::os::CriticalSection::ScopedLock locker(m_CsMem);
    if(IsInboxInfoBodyBufAllocated)
    {
        os_free(cecInboxInfoBuf);
        IsInboxInfoBodyBufAllocated = false;
    }
    return ResultSuccess();
}

Result MessageBox::FreeOutboxInfoBodyBuf()
{
    nn::os::CriticalSection::ScopedLock locker(m_CsMem);
    if(IsOutboxInfoBodyBufAllocated)
    {
        os_free(cecOutboxInfoBuf);
        IsOutboxInfoBodyBufAllocated = false;
    }
    return ResultSuccess();
}

Result MessageBox::WriteMessageBoxList()
{
    Result result;
    size_t filesize;

    OpenFile(currentCecTitleId, FILETYPE_CEC_BASE_DIR, FILEOPT_MKDIR, &filesize);

    result = OpenAndWriteFile(reinterpret_cast<u8 *>(&cecMessageBoxList), sizeof(cecMessageBoxList), currentCecTitleId,
        FILETYPE_MESSAGE_BOX_LIST, FILEOPT_WRITE | FILEOPT_NOCHECK);
    NN_UTIL_RETURN_IF_FAILED(result);

    result = OpenAndReadFile(reinterpret_cast<u8 *>(&cecMessageBoxList), sizeof(cecMessageBoxList), &filesize,
        currentCecTitleId, FILETYPE_MESSAGE_BOX_LIST, FILEOPT_READ | FILEOPT_NOCHECK);
    NN_UTIL_RETURN_IF_FAILED(result);

    return result;
}

size_t MessageBox::ReadMessageBoxList()
{
    Result result;
    size_t filesize = 0;

    result = OpenAndReadFile(reinterpret_cast<u8 *>(&cecMessageBoxList), sizeof(cecMessageBoxList), &filesize,
        currentCecTitleId, FILETYPE_MESSAGE_BOX_LIST, FILEOPT_READ | FILEOPT_NOCHECK);
    if (result.GetDescription() == Result::DESCRIPTION_INVALID_HANDLE)
    {
        DBG_PRINTF_ERR("%s(%d): DESCRIPTION_INVALID_HANDLE\n", __FUNCTION__, __LINE__);
        return 0;
    }
    if (filesize == 0)
    {
        WriteMessageBoxList();
        SetData(currentCecTitleId, NULL, 0, 5);
        return 0;
    }
    if (result.IsFailure() || cecMessageBoxList.magic16 != MESSAGEBOXLIST_MAGIC)
    {
        result = WriteMessageBoxList();
        if (result.IsFailure())
        {
            return 0;
        }
        return 0;
    }

    size_t tmp_filesize;
    NN_TASSERT_(cecMessageBoxList.DirNum <= MESSAGE_BOX_NUM_MAX);
    for (int i = 0; i < cecMessageBoxList.DirNum; i++)
    {
        result = OpenFile(Base64Str2CecTitleId(cecMessageBoxList.DirName[i]), FILETYPE_MESSAGE_BOX_DIR, 0, &tmp_filesize);
        if (result.IsFailure())
        {
            result = WriteMessageBoxList();
            if (result.IsFailure())
            {
                return 0;
            }
            break;
        }
    }
    return filesize;
}

size_t MessageBox::ReadMessageBoxList(void *bufOut, size_t size)
{
    NN_TASSERT_(bufOut);

    nn::Result result;
    size_t filesize;
    if (size < sizeof(cecMessageBoxList))
    {
        return 0;
    }
    if (!IsOpened())
    {
        return 0;
    }

    MessageBoxList messBoxList = {0};
    result = OpenAndReadFile(reinterpret_cast<u8 *>(&messBoxList), sizeof(MessageBoxList), &filesize, currentCecTitleId,
        FILETYPE_MESSAGE_BOX_LIST, FILEOPT_READ | FILEOPT_NOCHECK);
    if (result.GetDescription() == nn::Result::DESCRIPTION_INVALID_HANDLE)
    {
        DBG_PRINTF_ERR("%s(%d): DESCRIPTION_INVALID_HANDLE\n", __FUNCTION__, __LINE__);
        return 0;
    }

    NN_TASSERT_(result.IsSuccess());
    NN_TASSERT_(filesize >= sizeof(MessageBoxList));

    size_t tmp_filesize;

    NN_TASSERT_(messBoxList.DirNum <= MESSAGE_BOX_NUM_MAX);
    for (int i = 0; i < messBoxList.DirNum; i++)
    {
        result = OpenFile(Base64Str2CecTitleId(messBoxList.DirName[i]), FILETYPE_MESSAGE_BOX_DIR, 0, &tmp_filesize);
        if (result.IsFailure())
        {

            result = WriteMessageBoxList();
            if (result.IsFailure())
            {
                return 0;
            }
            break;
        }
    }

    std::memcpy(&bufOut, &messBoxList, sizeof(cecMessageBoxList));

    return filesize;
}

Result MessageBox::CheckEulaParentalControl()
{
    Result result;

    {
        bool isDaemonEulaAgreed;
        result = ReadData(reinterpret_cast<u8 *>(&isDaemonEulaAgreed), sizeof(isDaemonEulaAgreed), 2, 0, 0);
        NN_UTIL_RETURN_IF_FAILED(result);
        if (!isDaemonEulaAgreed)
        {
            return ResultNotAgreeEula();
        }
    }

    {
        bool isParentalControled;
        result = ReadData(reinterpret_cast<u8 *>(&isParentalControled), sizeof(isParentalControled), 3, 0, 0);
        NN_UTIL_RETURN_IF_FAILED(result);

        if (isParentalControled)
        {
            return ResultParentalControlCec();
        }
    }

    if (!b_IsAgreeEulaAppRequired)
    {
        result = IsAgreeEulaAppRequired();
        b_IsAgreeEulaAppRequired = result.IsSuccess();
    }
    if (b_IsAgreeEulaAppRequired == false)
    {
        if (result == ResultBufferFull())
        {
            return result;
        }
        return ResultNotAgreeEula();
    }
    return ResultSuccess();
}

nn::Result MessageBox::OpenMessageBox(const u32 cecTitleId, const u32 privateId)
{
    SCOPED_LOCK(m_Cs);

    nn::Result result;

    result = CecControl::Suspend();
    NN_RESULT_ASSERT_(result);
    b_IsSuspended = true;

    struct MessageBoxInfo tmp_messageboxinfo;
    currentCecTitleId = cecTitleId;

    result = ReadMessageBoxInfo(&tmp_messageboxinfo, cecTitleId);

    if (result.IsFailure())
    {
        currentCecTitleId = 0;
        CloseMessageBox(true);
        if (b_IsSuspended)
        {
            CecControl::StartScanning();
        }

        return result;
    }

    if (tmp_messageboxinfo.privateId != privateId)
    {
        currentCecTitleId = 0;
        CloseMessageBox(true);
        if (b_IsSuspended)
        {
            CecControl::StartScanning();
        }
        return ResultNotAuthorized();
    }

    b_SkippedWriteInboxInfo = false;
    b_SkippedWriteOutboxInfo = false;

    std::memcpy(&cecMessageBoxInfo, &tmp_messageboxinfo, sizeof(MessageBoxInfo));
    result = ReadInBoxInfo();
    if (result.IsFailure())
    {
        currentCecTitleId = 0;
        CloseMessageBox(true);
        if (b_IsSuspended)
        {
            CecControl::StartScanning();
        }
        return result;
    }

    result = ReadOutBoxInfo();
    if (result.IsFailure())
    {
        currentCecTitleId = 0;
        CloseMessageBox(true);
        if (b_IsSuspended)
        {
            CecControl::StartScanning();
        }
        return result;
    }

    ReadOutBoxIndex();

    cecMessageBoxInfo.flag1 = 0;
    cecMessageBoxInfo.flag2 = 0;
    cecMessageBoxInfo.lastOpened = nn::fnd::DateTime::GetNow().GetParameters();

    b_IsBoxOpened = true;

    return ResultSuccess();
}

bool MessageBox::IsOpened() const
{
    if (currentCecTitleId != 0)
    {
        return true;
    }
    return false;
}

void MessageBox::CloseMessageBox()
{
    CloseMessageBox(false);
}

void MessageBox::CloseMessageBox(bool bWithoutCommit)
{
    SCOPED_LOCK(m_Cs);
    if (!bWithoutCommit)
    {
        CommitMessageBox();
    }

    currentCecTitleId = 0;
    std::memset(&cecInboxInfo, 0, sizeof(cecInboxInfo));
    std::memset(&cecOutboxInfo, 0, sizeof(cecOutboxInfo));
    std::memset(cecInboxInfo_body, 0, sizeof(cecInboxInfo_body));
    std::memset(cecOutboxInfo_body, 0, sizeof(cecOutboxInfo_body));
    std::memset(&cecOutboxIndex_header, 0, sizeof(cecOutboxIndex_header));
    std::memset(cecOutboxIndex, 0, sizeof(cecOutboxIndex));

    FreeInboxInfoBodyBuf();
    FreeOutboxInfoBodyBuf();

    cecInboxInfo.magic16 = CEC_BOXINFO_MAGIC;
    cecOutboxInfo.magic16 = CEC_BOXINFO_MAGIC;
    cecOutboxIndex_header.magic16 = CEC_OUTBOXINDEX_MAGIC;

    if (!bWithoutCommit)
    {
        CecControl::StartScanning();
        b_IsSuspended = false;
    }
    b_IsBoxOpened = false;
}

Result MessageBox::CreateMessageBox(const u32 cecTitleId, const u32 privateId, const char *hmacKey,
    const void *icon, size_t iconSize, const wchar_t *name, size_t nameSize,
    size_t inboxSizeMax, size_t outboxSizeMax, size_t inboxMessNumMax,
    size_t outboxMessNumMax, size_t messageSizeMax)
{

    Result result;

    CecControl::Suspend();
    b_IsSuspended = true;

    currentCecTitleId = cecTitleId;
    CHECK_CURRENTCECTITLEID();

    if ((inboxSizeMax + outboxSizeMax > CEC_INBOX_SIZE_DEFAULT + CEC_OUTBOX_SIZE_DEFAULT) ||
        (inboxMessNumMax > CEC_INBOX_MESSNUM_DEFAULT) || (outboxMessNumMax > CEC_OUTBOX_MESSNUM_DEFAULT) ||
        (messageSizeMax > CEC_MESSSIZEMAX_DEFAULT) || (inboxMessNumMax == 0) || (outboxMessNumMax == 0) ||
        (inboxSizeMax == 0) || (outboxSizeMax == 0) || (messageSizeMax == 0))
    {
        if (b_IsSuspended)
        {
            CecControl::StartScanning();
        }
        currentCecTitleId = 0;
        return ResultInvalidArgument();
    }

    if (hmacKey == NULL)
    {
        currentCecTitleId = 0;
        return ResultInvalidArgument();
    }

    if (1)
    {
        u32 boxNum = GetMessageBoxNum(0);
        for (int i = 0; i < boxNum; i++)
        {
            u32 dirCecTitleId = Base64Str2CecTitleId(cecMessageBoxList.DirName[i]);
            if (((dirCecTitleId >> 8) & 0x00ffffff) == ((cecTitleId >> 8) & 0x00ffffff))
            {
                if (b_IsSuspended)
                {
                    CecControl::StartScanning();
                }
                currentCecTitleId = 0;
                return ResultBoxAlreadyExists();
            }
        }

        boxNum = GetMessageBoxNum(MESSAGE_BOX_FLAG_APPLICATION);
        if (boxNum >= CEC_BOXNUMMAX_APPLICATION)
        {
            if (b_IsSuspended)
            {
                CecControl::StartScanning();
            }
            currentCecTitleId = 0;
            return ResultBoxNumFull();
        }
    }

    cecInboxInfo.boxSizeMax = inboxSizeMax;
    cecInboxInfo.messNumMax = inboxMessNumMax;
    cecOutboxInfo.boxSizeMax = outboxSizeMax;
    cecOutboxInfo.messNumMax = outboxMessNumMax;
    cecInboxInfo.messSizeMax = messageSizeMax;
    cecOutboxInfo.messSizeMax = messageSizeMax;

    size_t tmp_size;
    result = OpenFile(currentCecTitleId, FILETYPE_MESSAGE_BOX_DIR, FILEOPT_MKDIR, &tmp_size);
    NN_RESULT_ASSERT_(result);
    CreateInBox(currentCecTitleId);
    CreateOutBox(currentCecTitleId);

    std::memset(&cecMessageBoxInfo, 0, sizeof(MessageBoxInfo));
    cecMessageBoxInfo.magic16 = MESSAGEBOXINFO_MAGIC;
    cecMessageBoxInfo.cecTitleId = cecTitleId;
    cecMessageBoxInfo.isActive = true;
    cecMessageBoxInfo.privateId = privateId;

    cecMessageBoxInfo.MessageBoxInfoFlag = MESSAGE_BOX_FLAG_APPLICATION;

    std::memcpy(cecMessageBoxInfo.hmacKey, hmacKey, MESSAGE_HMAC_KEYLEN);

    cecMessageBoxInfo.lastOpened = nn::fnd::DateTime::GetNow().GetParameters();

    result = WriteMessageBoxInfo();
    if (result.IsFailure())
    {
        DeleteMessageBox(currentCecTitleId);
        if (b_IsSuspended)
        {
            CecControl::StartScanning();
        }
        currentCecTitleId = 0;
        return result;
    }
    result = WriteMessageBoxList();
    if (result.IsFailure())
    {
        DeleteMessageBox(currentCecTitleId);
        if (b_IsSuspended)
        {
            CecControl::StartScanning();
        }
        currentCecTitleId = 0;
        return result;
    }

    result = SetMessageBoxName(name, nameSize);
    if (result.IsFailure())
    {
        DeleteMessageBox(currentCecTitleId);
        if (b_IsSuspended)
        {
            CecControl::StartScanning();
        }
        currentCecTitleId = 0;
        return result;
    }

    result = SetMessageBoxIcon(icon, iconSize);
    if (result.IsFailure())
    {
        DeleteMessageBox(currentCecTitleId);
        if (b_IsSuspended)
        {
            CecControl::StartScanning();
        }
        currentCecTitleId = 0;
        return result;
    }

    b_IsBoxOpened = true;

    return ResultSuccess();
}

Result MessageBox::CreateMessageBox(const u32 cecTitleId, const u32 privateId, const char *hmacKey,
    size_t inboxSizeMax, size_t outboxSizeMax, size_t inboxMessNumMax,
    size_t outboxMessNumMax, size_t messageSizeMax)
{
    u32 dummy_Icon = 0x00000000;
    wchar_t dummy_Name[] = L"(No Name)";
    return CreateMessageBox(cecTitleId, privateId, hmacKey, &dummy_Icon, sizeof(dummy_Icon), dummy_Name,
        sizeof(dummy_Name), inboxSizeMax, outboxSizeMax, inboxMessNumMax, outboxMessNumMax, messageSizeMax);
}

void MessageBox::CreateInBox(const u32 cecTitleId)
{
    size_t tmp_size;
    OpenFile(cecTitleId,FILETYPE_MESSAGE_INBOX_DIR, FILEOPT_MKDIR, &tmp_size);
    WriteInBoxInfo();
}

void MessageBox::CreateOutBox(const u32 cecTitleId)
{
    size_t tmp_size;
    OpenFile(cecTitleId, FILETYPE_MESSAGE_OUTBOX_DIR, FILEOPT_MKDIR, &tmp_size);
    WriteOutBoxInfo();
    ResetOutBoxIndex();
}

u32 MessageBox::GetMessageBoxNum(MessageBoxFlag flag)
{
    u32 dirCount = 0;
    nn::Result result;
    if (ReadMessageBoxList() == 0)
    {
        return 0;
    }
    for (int i = 0; i < cecMessageBoxList.DirNum; i++)
    {
        MessageBoxInfo boxInfo;
        result = ReadMessageBoxInfo(&boxInfo, Base64Str2CecTitleId(cecMessageBoxList.DirName[i]));
        if (result.IsSuccess() && (((boxInfo.MessageBoxInfoFlag & flag) & ~MESSAGE_BOX_FLAG_HIDDEN) || flag == 0))
        {
            dirCount++;
        }
    }
    return dirCount;
}

Result MessageBox::DeleteMessageBox()
{
    CHECK_CURRENTCECTITLEID();
    Result result;

    result = Delete(currentCecTitleId, FILETYPE_MESSAGE_BOX_DIR, 0, 0, 0);

    result = WriteMessageBoxList();
    currentCecTitleId = 0;
    CloseMessageBox();

    return result;
}

Result MessageBox::DeleteMessageBox(const u32 cecTitleId)
{
    Result result;

    result = Delete(cecTitleId, FILETYPE_MESSAGE_BOX_DIR, 0, 0, 0);

    result = WriteMessageBoxList();

    CloseMessageBox();

    return result;
}

nn::Result MessageBox::ReadMessageBoxInfo() const
{
    CHECK_CURRENTCECTITLEID();

    nn::Result result;
    size_t readLen = 0;

    result = OpenAndReadFile(reinterpret_cast<u8 *>(const_cast<MessageBoxInfo *>(&cecMessageBoxInfo)),
        sizeof(struct MessageBoxInfo), &readLen, currentCecTitleId, FILETYPE_MESSAGE_BOX_INFO,
        FILEOPT_READ | FILEOPT_NOCHECK);
    if (readLen == 0)
    {
        return ResultNoData();
    }
    if (result.IsFailure())
    {
        return result;
    }

    return result;
}

nn::Result MessageBox::ReadMessageBoxInfo(struct MessageBoxInfo *outbuf, u32 cecTitleId) const
{
    NN_TASSERT_(outbuf);

    if (cecTitleId == 0)
    {
        return ResultInvalidArgument();
    }

    nn::Result result;
    size_t readLen = 0;

    std::memset(outbuf, 0, sizeof(MessageBoxInfo));
    result = OpenAndReadFile(reinterpret_cast<u8 *>(outbuf), sizeof(struct MessageBoxInfo), &readLen, cecTitleId,
                             nn::cec::CTR::FILETYPE_MESSAGE_BOX_INFO,
                             nn::cec::CTR::FILEOPT_READ | nn::cec::CTR::FILEOPT_NOCHECK);
    if (result.GetDescription() == nn::Result::DESCRIPTION_INVALID_HANDLE)
    {
        DBG_PRINTF_ERR("%s(%d): DESCRIPTION_INVALID_HANDLE\n", __FUNCTION__, __LINE__);
        return result;
    }

    if (readLen == 0)
    {
        result = ResultNoData();
    }
    else
    {
        NN_TASSERTMSG_(readLen == sizeof(struct MessageBoxInfo), "result = %08x / %08x != %08x",
                       result.GetPrintableBits(), readLen, sizeof(struct MessageBoxInfo));
    }
    return result;
}

Result MessageBox::ReadMessageBoxInfo(struct MessageBoxInfo *outbuf) const
{
    return ReadMessageBoxInfo(outbuf, currentCecTitleId);
}

Result MessageBox::GetMessageBoxData(const u32 datatype, void *dataBuf, const size_t dataBufSize) const
{
    size_t readLen = 0;
    CHECK_CURRENTCECTITLEID();
    nn::Result result = ResultInvalidArgument();

    if (dataBuf == NULL || dataBufSize == 0)
    {
        return ResultInvalidArgument();
    }

    if (datatype == FILETYPE_BOXDATA_FLAG_1)
    {
        std::memcpy(dataBuf, &cecMessageBoxInfo.flag1, dataBufSize);
        return ResultSuccess();
    }

    if (datatype == FILETYPE_BOXDATA_FLAG_2)
    {
        std::memcpy(dataBuf, &cecMessageBoxInfo.flag2, dataBufSize);
        return ResultSuccess();
    }

    if (datatype == FILETYPE_BOXDATA_FLAG_3)
    {
        std::memcpy(dataBuf, &cecMessageBoxInfo.flag3, dataBufSize);
        return ResultSuccess();
    }

    if (datatype == FILETYPE_BOXDATA_FLAG_4)
    {
        std::memcpy(dataBuf, &cecMessageBoxInfo.flag4, dataBufSize);
        return ResultSuccess();
    }

    if (datatype > FILETYPE_BOXDATA_START && datatype < FILETYPE_BOXDATA_END)
    {
        result = OpenAndReadFile(reinterpret_cast<u8 *>(dataBuf), dataBufSize, &readLen, currentCecTitleId, datatype, FILEOPT_READ);
        if (result.IsFailure())
        {
            return result;
        }
        return ResultSuccess();
    }
    return result;
}

Result MessageBox::GetMessageBoxName(wchar_t *dataBuf, size_t dataBufSize) const
{
    return GetMessageBoxData(BOXDATA_TYPE_NAME_1, dataBuf, dataBufSize);
}

Result MessageBox::GetMessageBoxIcon(void *dataBuf, size_t dataBufSize) const
{
    return GetMessageBoxData(BOXDATA_TYPE_ICON, dataBuf, dataBufSize);
}

size_t MessageBox::GetMessageBoxDataSize(const u32 datatype) const
{
    size_t filesize = 0;

    if (datatype == FILETYPE_BOXDATA_FLAG_1)
    {
        return sizeof(cecMessageBoxInfo.flag1);
    }

    if (datatype == FILETYPE_BOXDATA_FLAG_2)
    {
        return sizeof(cecMessageBoxInfo.flag2);
    }

    if(datatype == FILETYPE_BOXDATA_FLAG_3)
    {
        return sizeof(cecMessageBoxInfo.flag3);
    }

    if (datatype == FILETYPE_BOXDATA_FLAG_4)
    {
        return sizeof(cecMessageBoxInfo.flag4);
    }

    if (datatype > FILETYPE_BOXDATA_START && datatype < FILETYPE_BOXDATA_END)
    {
        OpenFile(currentCecTitleId, datatype, FILEOPT_READ, &filesize);
    }
    return filesize;
}

nn::Result MessageBox::SetMessageBoxData(const u32 datatype, const void *data, const size_t dataSize)
{
    nn::Result result;
    CHECK_CURRENTCECTITLEID();
    if (CEC_BOX_DATA_SIZE_MAX < dataSize)
    {
        return ResultTooLarge();
    }

    if (datatype == FILETYPE_BOXDATA_FLAG_1)
    {
        std::memcpy(&cecMessageBoxInfo.flag1, reinterpret_cast<u8 *>(const_cast<void *>(data)),
            sizeof(cecMessageBoxInfo.flag1));
        return WriteMessageBoxInfo();
    }

    if (datatype == FILETYPE_BOXDATA_FLAG_2)
    {
        std::memcpy(&cecMessageBoxInfo.flag2, reinterpret_cast<u8 *>(const_cast<void *>(data)),
            sizeof(cecMessageBoxInfo.flag2));
        return WriteMessageBoxInfo();
    }

    if (datatype == FILETYPE_BOXDATA_FLAG_3)
    {
        std::memcpy(&cecMessageBoxInfo.flag3, reinterpret_cast<u8 *>(const_cast<void *>(data)),
            sizeof(cecMessageBoxInfo.flag3));
        return WriteMessageBoxInfo();
    }

    if (datatype == FILETYPE_BOXDATA_FLAG_4)
    {
        std::memcpy(&cecMessageBoxInfo.flag4, reinterpret_cast<u8 *>(const_cast<void *>(data)),
            sizeof(cecMessageBoxInfo.flag4));
        return WriteMessageBoxInfo();
    }

    if (datatype > FILETYPE_BOXDATA_START && datatype < FILETYPE_BOXDATA_END)
    {

        if (dataSize == 0 || data == NULL)
        {
            result = Delete(currentCecTitleId, datatype, 0, 0, 0);
            return result;
        }

        result = OpenAndWriteFile(reinterpret_cast<u8 *>(const_cast<void *>(data)), dataSize, currentCecTitleId,
            datatype, FILEOPT_WRITE | FILEOPT_NOCHECK);
        if (result.IsFailure())
        {
            return result;
        }
    }
    else
    {
        result = ResultInvalidArgument();
    }
    return result;
}

Result MessageBox::SetMessageBoxName(const wchar_t *data, size_t dataSize)
{
    if (dataSize > 64 * 2)
    {
        return ResultInvalidData();
    }
    return SetMessageBoxData(BOXDATA_TYPE_NAME_1, data, dataSize);
}

Result MessageBox::SetMessageBoxIcon(const void *data, size_t dataSize)
{
    return SetMessageBoxData(BOXDATA_TYPE_ICON, data, dataSize);
}

Result MessageBox::WriteMessageBoxInfo()
{
    CHECK_CURRENTCECTITLEID();

    Result result;

    result = OpenAndWriteFile(reinterpret_cast<u8 *>(&cecMessageBoxInfo), sizeof(struct MessageBoxInfo),
        currentCecTitleId, FILETYPE_MESSAGE_BOX_INFO, FILEOPT_WRITE | nn::cec::CTR::FILEOPT_NOCHECK);
    return result;
}

nn::Result MessageBox::GetBoxInfo(struct CecBoxInfoHeader *boxinfo, CecMessageHeader **boxInfoBody,
    CecBoxType boxType) const
{
    SCOPED_LOCK(m_Cs);
    NN_TASSERT_(boxinfo);
    NN_TASSERT_(boxInfoBody);

    if (boxType == CEC_BOXTYPE_INBOX)
    {
        if (cecInboxInfoBuf == NULL || IsInboxInfoBodyBufAllocated == false)
        {
            return ResultNoData();
        }
        std::memcpy(boxinfo, &cecInboxInfo, sizeof(CecBoxInfoHeader));
        std::memcpy(boxInfoBody, cecInboxInfo_body, sizeof(cecInboxInfo_body));
    }
    else
    {
        NN_TASSERT_(boxType == CEC_BOXTYPE_OUTBOX);
        if (cecOutboxInfoBuf == NULL || IsOutboxInfoBodyBufAllocated == false)
        {
            return ResultNoData();
        }
        std::memcpy(boxinfo, &cecOutboxInfo, sizeof(CecBoxInfoHeader));
        std::memcpy(boxInfoBody, cecOutboxInfo_body, sizeof(cecOutboxInfo_body));
    }
    return ResultSuccess();
}

nn::Result MessageBox::ReadBoxInfo(struct CecBoxInfoHeader *boxinfo, CecMessageHeader **boxInfoBody, u8 *infoBodyBuf,
                                   CecBoxType boxType)
{
    SCOPED_LOCK(m_Cs);
    NN_TASSERT_(boxinfo);
    NN_TASSERT_(boxInfoBody);

    NN_UNUSED_VAR(infoBodyBuf);
    CHECK_CURRENTCECTITLEID();

    u8 *p_boxinfo;
    struct CecMessageHeader tmp_cec_mh;
    nn::Result result;
    size_t readLen = 0;
    size_t filesize = 0;

    u8 *pBodyBuf;

    if (boxType == CEC_BOXTYPE_INBOX)
    {

        result = OpenFile(currentCecTitleId, FILETYPE_INBOX_INFO, FILEOPT_READ, &filesize);
    }
    else
    {

        NN_TASSERT_(boxType == CEC_BOXTYPE_OUTBOX);
        result = OpenFile(currentCecTitleId, FILETYPE_OUTBOX_INFO, FILEOPT_READ, &filesize);
    }

    if (filesize == 0)
    {
        return ResultNoData();
    }

    if ((boxType == CEC_BOXTYPE_INBOX && IsInboxInfoBodyBufAllocated == true) ||
        (boxType == CEC_BOXTYPE_OUTBOX && IsOutboxInfoBodyBufAllocated == true))
    {
        boxinfo->messNum = 0;

        if (boxType == CEC_BOXTYPE_INBOX)
        {
            FreeInboxInfoBodyBuf();
        }
        else
        {
            FreeOutboxInfoBodyBuf();
        }
    }

    if (boxType == CEC_BOXTYPE_INBOX)
    {
        result = AllocInboxInfoBodyBuf(filesize);
        if (result.IsFailure())
        {
            boxinfo->messNum = 0;
            return ResultBufferFull();
        }
        result = OpenAndReadFile(cecInboxInfoBuf, filesize, &readLen, currentCecTitleId, FILETYPE_INBOX_INFO,
            FILEOPT_READ | FILEOPT_NOCHECK);
        pBodyBuf = cecInboxInfoBuf;
    }
    else
    {
        result = AllocOutboxInfoBodyBuf(filesize);
        if (result.IsFailure())
        {
            boxinfo->messNum = 0;
            return ResultBufferFull();
        }
        result =
            OpenAndReadFile(cecOutboxInfoBuf, filesize, &readLen, currentCecTitleId, FILETYPE_OUTBOX_INFO,
                FILEOPT_READ | FILEOPT_NOCHECK);
        pBodyBuf = cecOutboxInfoBuf;
    }

    if (result.IsFailure() || readLen == 0)
    {
        boxinfo->messNum = 0;
        if (boxType == CEC_BOXTYPE_INBOX)
        {
            FreeInboxInfoBodyBuf();
        }
        else
        {
            FreeOutboxInfoBodyBuf();
        }
        return ResultNoData();
    }

    if (readLen < sizeof(struct CecBoxInfoHeader))
    {
        return nn::cec::ResultInvalidData();
    }
    std::memcpy(reinterpret_cast<u8 *>(boxinfo), pBodyBuf, sizeof(struct CecBoxInfoHeader));
    if (readLen != boxinfo->boxInfoSize ||
        readLen < sizeof(struct CecBoxInfoHeader) + sizeof(struct CecMessageHeader) * boxinfo->messNum)
    {
        return ResultInvalidData();
    }

    p_boxinfo = pBodyBuf + sizeof(struct CecBoxInfoHeader);

    for (int i = 0; i < boxinfo->messNum; i++)
    {

        std::memcpy(reinterpret_cast<u8 *>(&tmp_cec_mh), p_boxinfo, sizeof(struct CecMessageHeader));

        if (tmp_cec_mh.magic16 != MESSAGE_MAGIC || tmp_cec_mh.cecTitleId != currentCecTitleId)
        {
            return ResultInvalidData();
        }

        boxInfoBody[i] = reinterpret_cast<CecMessageHeader *>(p_boxinfo);
        p_boxinfo += sizeof(struct CecMessageHeader);
    }

    if (filesize > 0)
    {
        result = ResultSuccess();
    }
    else
    {
        result = ResultNoData();
    }

    return result;
}

Result MessageBox::WriteBoxInfo(CecBoxType boxType, CecBoxInfoHeader &boxInfo, CecMessageHeader **boxInfoBody)
{
    CHECK_CURRENTCECTITLEID();

    NN_UNUSED_VAR(boxInfoBody);

    Result result;
    if (boxType == CEC_BOXTYPE_INBOX)
    {
        result = OpenAndWriteFile(reinterpret_cast<u8 *>(&boxInfo), sizeof(CecBoxInfoHeader), currentCecTitleId,
            FILETYPE_INBOX_INFO, FILEOPT_WRITE | FILEOPT_NOCHECK);
        if (result.IsFailure())
        {
            return result;
        }
        result = ReadInBoxInfo();
        if (result.IsFailure())
        {
        }
        b_SkippedWriteInboxInfo = false;
    }
    else
    {
        NN_TASSERT_(boxType == CEC_BOXTYPE_OUTBOX);
        result = OpenAndWriteFile(reinterpret_cast<u8 *>(&boxInfo), sizeof(CecBoxInfoHeader), currentCecTitleId, FILETYPE_OUTBOX_INFO, FILEOPT_WRITE | FILEOPT_NOCHECK);
        if (result.IsFailure())
        {
            return result;
        }

        result = ReadOutBoxInfo();
        if (result.IsFailure())
        {
        }
    }
    return result;
}

Result MessageBox::ReadInBoxInfo()
{
    return ReadBoxInfo(&cecInboxInfo, cecInboxInfo_body, cecInboxInfoBuf, CEC_BOXTYPE_INBOX);
}

Result MessageBox::WriteInBoxInfo()
{
    return WriteBoxInfo(CEC_BOXTYPE_INBOX, cecInboxInfo, cecInboxInfo_body);
}

Result MessageBox::ReadOutBoxInfo()
{
    return ReadBoxInfo(&cecOutboxInfo, cecOutboxInfo_body, cecOutboxInfoBuf, CEC_BOXTYPE_OUTBOX);
}

Result MessageBox::WriteOutBoxInfo()
{
    return WriteBoxInfo(CEC_BOXTYPE_OUTBOX, cecOutboxInfo, cecOutboxInfo_body);
}

u32 MessageBox::ResetOutBoxIndex()
{

    nn::Result result;
    result = SetData(currentCecTitleId, NULL, 0, 0);
    if (result.IsFailure())
    {
        return 0;
    }
    return ReadOutBoxIndex();
}

u32 MessageBox::RoundOutBoxIndex(const MessageId &messageId)
{

    nn::Result result;
    result = SetData(currentCecTitleId, messageId.GetBinary(), CEC_SIZEOF_MESSAGEID, 1);
    if (result.IsFailure())
    {
        return 0;
    }
    return ReadOutBoxIndex();
}

u32 MessageBox::AppendOutBoxIndex(const MessageId &messageId)
{
    nn::Result result;
    result = SetData(currentCecTitleId, messageId.GetBinary(), CEC_SIZEOF_MESSAGEID, 2);
    if (result.IsFailure())
    {
        return 0;
    }
    return ReadOutBoxIndex();
}

u32 MessageBox::RemoveOutBoxIndex(const MessageId &messageId)
{
    nn::Result result;
    result = SetData(currentCecTitleId, messageId.GetBinary(), CEC_SIZEOF_MESSAGEID, 3);
    if (result.IsFailure())
    {
        return 0;
    }
    return ReadOutBoxIndex();
}

u32 MessageBox::ReadOutBoxIndex()
{

    if (!IsOpened())
    {
        return 0;
    }

    u8 *p_filebuf;
    nn::Result result;
    size_t readLen = 0;
    size_t filesize = 0;

    result = OpenFile(currentCecTitleId, nn::cec::CTR::FILETYPE_OUTBOX_INDEX, nn::cec::CTR::FILEOPT_READ, &filesize);
    if (result.GetDescription() == nn::Result::DESCRIPTION_INVALID_HANDLE)
    {
        DBG_PRINTF_ERR("%s(%d): DESCRIPTION_INVALID_HANDLE\n", __FUNCTION__, __LINE__);
        return 0;
    }

    if (filesize == 0 || result <= nn::fs::ResultNotFound())
    {

        result = SetData(currentCecTitleId, NULL, 0, 0);
        if (result.IsFailure())
        {
            return 0;
        }
    }

    u8 *cecOutboxIndexBuf = reinterpret_cast<u8 *>(os_malloc(sizeof(CecOutBoxIndexHeader) + CEC_SIZEOF_MESSAGEID * CEC_OUTBOX_MESSNUM_DEFAULT));

    if (cecOutboxIndexBuf == NULL)
    {
        cecOutboxIndex_header.messageNum = 0;
        return 0;
    }

    result = OpenAndReadFile(cecOutboxIndexBuf, filesize, &readLen, currentCecTitleId, nn::cec::CTR::FILETYPE_OUTBOX_INDEX,
                        nn::cec::CTR::FILEOPT_READ | nn::cec::CTR::FILEOPT_NOCHECK);
    if (result.IsFailure())
    {
        DBG_PRINTF_ERR("%s(%d): OpenAndReadFile Failed result[0x%08x]\n", __FUNCTION__, __LINE__, result.GetPrintableBits());
        os_free(cecOutboxIndexBuf);
        return 0;
    }

    p_filebuf = cecOutboxIndexBuf;
    if (readLen > 0)
    {
        std::memcpy(reinterpret_cast<u8 *>(&cecOutboxIndex_header), p_filebuf, sizeof(CecOutBoxIndexHeader));
    }

    if (cecOutboxIndex_header.magic16 != CEC_OUTBOXINDEX_MAGIC ||
        (filesize - sizeof(CecOutBoxIndexHeader)) / CEC_SIZEOF_MESSAGEID != cecOutboxIndex_header.messageNum)
    {

        result = SetData(currentCecTitleId, NULL, 0, 0);
        if (result.IsFailure())
        {
            os_free(cecOutboxIndexBuf);
            return 0;
        }

        result =
            OpenFile(currentCecTitleId, FILETYPE_OUTBOX_INDEX, FILEOPT_READ, &filesize);
        if (result.IsFailure())
        {
            cecOutboxIndex_header.messageNum = 0;
            os_free(cecOutboxIndexBuf);
            return 0;
        }

        result = OpenAndReadFile(cecOutboxIndexBuf, filesize, &readLen, currentCecTitleId,
            FILETYPE_OUTBOX_INDEX, FILEOPT_READ | FILEOPT_NOCHECK);
        if (result.IsFailure())
        {
            cecOutboxIndex_header.messageNum = 0;
            os_free(cecOutboxIndexBuf);
            return 0;
        }

        p_filebuf = cecOutboxIndexBuf;
        std::memcpy(reinterpret_cast<u8 *>(&cecOutboxIndex_header), p_filebuf, sizeof(CecOutBoxIndexHeader));
        if (cecOutboxIndex_header.magic16 != CEC_OUTBOXINDEX_MAGIC ||
            (filesize - sizeof(CecOutBoxIndexHeader)) / CEC_SIZEOF_MESSAGEID != cecOutboxIndex_header.messageNum)
        {
            cecOutboxIndex_header.messageNum = 0;
            os_free(cecOutboxIndexBuf);
            return 0;
        }
    }

    p_filebuf += sizeof(CecOutBoxIndexHeader);
    NN_TASSERT_(cecOutboxIndex_header.messageNum <= CEC_OUTBOX_MESSNUM_DEFAULT);
    for (int i = 0; i < cecOutboxIndex_header.messageNum; i++)
    {
        std::memcpy(cecOutboxIndex[i], p_filebuf, CEC_SIZEOF_MESSAGEID);
        p_filebuf += CEC_SIZEOF_MESSAGEID;
    }

    os_free(cecOutboxIndexBuf);

    return cecOutboxIndex_header.messageNum;
}

Result MessageBox::WriteOutBoxIndex()
{
    Result result;
    size_t filesize = 0;
    result = OpenFile(currentCecTitleId, FILETYPE_OUTBOX_INDEX, FILEOPT_READ, &filesize);
    if (filesize == 0)
    {
        ResetOutBoxIndex();
    }
    return ResultSuccess();
}

Result MessageBox::CommitMessageBox()
{
    if (IsOpened())
    {
        if (b_SkippedWriteInboxInfo)
        {
            WriteInBoxInfo();
        }
        if (b_SkippedWriteOutboxInfo)
        {
            WriteOutBoxInfo();
        }
        if (b_IsBoxOpened)
        {
            size_t filesize;
            OpenFile(currentCecTitleId, FILETYPE_BOXDATA_PROGRAM_ID, FILEOPT_WRITE | FILEOPT_NOCHECK, &filesize);
            WriteMessageBoxInfo();
        }
    }

    Result result = SetData(currentCecTitleId, NULL, 0, 5);
    return result;
}

Result MessageBox::ReadMessage(void *bufOut, const size_t bufLen, const CecBoxType boxType, const MessageId &messageId)
{
    nn::Result result;
    CHECK_CURRENTCECTITLEID();

    if (bufOut == NULL || bufLen < sizeof(CecMessageHeader))
    {
        return ResultInvalidArgument();
    }

    size_t filesize = 0;
    if (boxType == CEC_BOXTYPE_OUTBOX)
    {
        result = ReadMessageFile(currentCecTitleId, static_cast<u8>(CEC_BOXTYPE_OUTBOX), messageId.GetBinary(),
            CEC_SIZEOF_MESSAGEID, &filesize, reinterpret_cast<u8 *>(bufOut), bufLen);
        if (result.IsFailure())
        {
            return result;
        }
    }
    else
    {
        NN_TASSERT_(boxType == CEC_BOXTYPE_INBOX);

        result = ReadMessageFile(currentCecTitleId, static_cast<u8>(CEC_BOXTYPE_INBOX), messageId.GetBinary(),
            CEC_SIZEOF_MESSAGEID, &filesize, reinterpret_cast<u8 *>(bufOut), bufLen);

        if (result.IsFailure())
        {
            return result;
        }
    }

    if (filesize > 0)
    {
        result = ResultSuccess();
    }
    else
    {
        result = ResultNoData();
    }

    return result;
}

Result MessageBox::ReadMessage(Message &cecMessage, void *buf, const size_t bufLen, const CecBoxType boxType,
    const MessageId &messageId)
{
    Result result;
    result = ReadMessage(buf, bufLen, boxType, messageId);
    if (result.IsFailure())
    {
        return result;
    }

    return cecMessage.InputMessage(buf, bufLen);
}

Result MessageBox::WriteMessage(const void *message, const size_t messageLen, const CecBoxType boxType,
    MessageId &messageIdOut)
{
    Message cecMessage;
    Result result;

    result = cecMessage.InputMessage(reinterpret_cast<const u8 *>(message), messageLen);
    if (result.IsSuccess())
    {
        return WriteMessage(cecMessage, boxType, messageIdOut);
    }
    else
    {
        return result;
    }
}

Result MessageBox::WriteMessage(const Message &cecMessage, const CecBoxType boxType, MessageId &messageIdOut)
{
    return MessageBox::WriteMessage(cecMessage, boxType, messageIdOut, true);
}

Result MessageBox::WriteMessage(const Message &cecMessage, const CecBoxType boxType, MessageId &messageIdOut,
    bool b_withWriteBoxInfo)
{
    SCOPED_LOCK(m_Cs);

    u8 flag_exists = 0;
    Result result;

    MessageId tmp_messId;
    s32 increasingSize = 0;
    s32 increasingNum = 0;

    if (!m_BoxAccessible)
    {
        result = CheckEulaParentalControl();
        if (result.IsFailure())
        {
            return result;
        }
    }

    if (currentCecTitleId == 0)
    {
        DBG_PRINTF_ERR("%s(%d): Err. Not opened mess box\n", __FUNCTION__, __LINE__);
        return ResultNotAuthorized();
    }

    if (cecMessage.GetBodySize() == 0)
    {
        return ResultNoData();
    }

    void *exhbody = NULL;
    u32 exhlen;
    result = cecMessage.GetIcon(&exhbody, &exhlen);
    if (result.IsFailure())
    {
        return ResultNoData();
    }

    if (currentCecTitleId != cecMessage.GetCecTitleId())
    {
        return ResultInvalidId();
    }
    cecMessage.GetMessageId(&tmp_messId);

    CecMessageHeader messHeaderOuts;

    cecMessage.OutputMessageHeader(&messHeaderOuts);

    if (tmp_messId.IsEmpty())
    {

        messHeaderOuts.createDate = nn::fnd::DateTime::GetNow().GetParameters();
    }
    else
    {
        if (MessageExists(boxType, tmp_messId))
        {
            flag_exists = 1;
        }
        else
        {
            flag_exists = 0;
        }
    }

    if (cecMessage.GetSendCount() > 1 && cecMessage.GetPropagationCount() > 1)
    {
        return ResultInvalidCombination();
    }

    u32 BoxSizeMax;
    u32 BoxSize;
    u32 BoxMessNumMax;
    u32 BoxMessNum;
    if (boxType == CEC_BOXTYPE_INBOX)
    {
        if (b_SkippedWriteInboxInfo == false)
        {
            BoxSizeMax = cecInboxInfo.boxSizeMax;
            BoxSize = cecInboxInfo.boxSize;
            BoxMessNumMax = cecInboxInfo.messNumMax;
            BoxMessNum = cecInboxInfo.messNum;
        }
        else
        {
            BoxSizeMax = tmp_cecInboxInfo.boxSizeMax;
            BoxSize = tmp_cecInboxInfo.boxSize;
            BoxMessNumMax = tmp_cecInboxInfo.messNumMax;
            BoxMessNum = tmp_cecInboxInfo.messNum;
        }

        if (cecInboxInfo.messSizeMax > 0 && cecMessage.GetMessSize() > cecInboxInfo.messSizeMax)
        {
            return ResultMessTooLarge();
        }
    }
    else
    {
        NN_TASSERT_(boxType == CEC_BOXTYPE_OUTBOX);
        if (b_SkippedWriteOutboxInfo == false)
        {
            BoxSizeMax = cecOutboxInfo.boxSizeMax;
            BoxSize = cecOutboxInfo.boxSize;
            BoxMessNumMax = cecOutboxInfo.messNumMax;
            BoxMessNum = cecOutboxInfo.messNum;
        }
        else
        {
            BoxSizeMax = tmp_cecOutboxInfo.boxSizeMax;
            BoxSize = tmp_cecOutboxInfo.boxSize;
            BoxMessNumMax = tmp_cecOutboxInfo.messNumMax;
            BoxMessNum = tmp_cecOutboxInfo.messNum;
        }

        if (cecOutboxInfo.messSizeMax > 0 && cecMessage.GetMessSize() > cecOutboxInfo.messSizeMax)
        {
            return ResultMessTooLarge();
        }
    }

    if (flag_exists == 0)
    {
        if (BoxSizeMax > 0 && BoxSize + cecMessage.GetMessSize() > BoxSizeMax)
        {
            NN_LOG_("%sboxinfo BoxSize(%d)/BoxSizeMax(%d) mess(%d)\n", (boxType == CEC_BOXTYPE_OUTBOX) ? "Out" : "In",
                   BoxSize, BoxSizeMax, cecMessage.GetMessSize());
            return ResultBoxSizeFull();
        }
        increasingSize = cecMessage.GetMessSize();
    }
    else
    {
        u32 existsMessSize;
        u32 existsMessIndex = GetMessageIndex(boxType, const_cast<u8 *>(tmp_messId.GetBinary()));
        if (existsMessIndex < 0xffffffff)
        {
            existsMessSize = GetMessageSize(boxType, existsMessIndex);
            if (BoxSizeMax > 0 && BoxSize - existsMessSize + cecMessage.GetMessSize() > BoxSizeMax)
            {
                NN_LOG_("boxinfo BoxSize(%d)/BoxSizeMax(%d) mess(%d) existsMessSize(%d)\n", BoxSize, BoxSizeMax,
                       cecMessage.GetMessSize(), existsMessSize);
                return ResultBoxSizeFull();
            }
            increasingSize = (cecMessage.GetMessSize() - existsMessSize);
        }
        else
        {
        }
    }

    if (BoxMessNumMax > 0 && flag_exists == 0 && BoxMessNum + 1 > BoxMessNumMax)
    {
        return ResultBoxMessNumFull();
    }
    if (flag_exists == 0)
    {
        increasingNum++;
    }

    u8 *bufWrite;

    bufWrite = reinterpret_cast<u8 *>(os_malloc(cecMessage.GetMessSize(), 4));
    if (bufWrite == NULL)
    {
        return ResultBufferFull();
    }

    cecMessage.MakeMessageBinary(bufWrite);
    std::memcpy(bufWrite, reinterpret_cast<u8 *>(&messHeaderOuts), sizeof(CecMessageHeader));
    u8 tmp_messIdBin[CEC_SIZEOF_MESSAGEID] = {0};
    if (!tmp_messId.IsEmpty())
    {
        tmp_messId.GetBinary(tmp_messIdBin);
    }

    Result writeResult = WriteMessageFileWithHmac(currentCecTitleId, boxType, tmp_messIdBin, CEC_SIZEOF_MESSAGEID, bufWrite,
        cecMessage.GetMessSize(), reinterpret_cast<u8 *>(cecMessageBoxInfo.hmacKey));

    if (writeResult.IsFailure())
    {
    }

    if (b_withWriteBoxInfo)
    {
        if (boxType == CEC_BOXTYPE_OUTBOX)
        {
            result = WriteOutBoxInfo();
        }
        else
        {
            NN_TASSERT_(boxType == CEC_BOXTYPE_INBOX);
            result = WriteInBoxInfo();
        }
        if (result.IsFailure())
        {
            if (bufWrite != NULL)
                os_free(bufWrite);
            return result;
        }
    }
    else
    {
        if (boxType == CEC_BOXTYPE_OUTBOX)
        {
            if (b_SkippedWriteOutboxInfo == false)
            {
                std::memcpy(&tmp_cecOutboxInfo, &cecOutboxInfo, sizeof(tmp_cecOutboxInfo));
            }
            b_SkippedWriteOutboxInfo = true;
            if (writeResult.IsSuccess())
            {
                tmp_cecOutboxInfo.boxSize += increasingSize;
                tmp_cecOutboxInfo.messNum += increasingNum;
            }
        }
        else
        {
            NN_TASSERT_(boxType == CEC_BOXTYPE_INBOX);
            if (b_SkippedWriteInboxInfo == false)
            {
                std::memcpy(&tmp_cecInboxInfo, &cecInboxInfo, sizeof(tmp_cecInboxInfo));
            }
            b_SkippedWriteInboxInfo = true;
            if (writeResult.IsSuccess())
            {
                tmp_cecInboxInfo.boxSize += increasingSize;
                tmp_cecInboxInfo.messNum += increasingNum;
            }
        }
    }

    if (writeResult.IsSuccess())
    {
        if (tmp_messId.IsEmpty())
        {
            tmp_messId = MessageId(tmp_messIdBin);
            messageIdOut = tmp_messId;
        }

        if (boxType == CEC_BOXTYPE_OUTBOX && flag_exists == 0)
        {
            AppendOutBoxIndex(tmp_messId);
        }
    }

    if (bufWrite != NULL)
        os_free(bufWrite);
    return writeResult;
}

Result MessageBox::DeleteMessage(const CecBoxType boxType, const MessageId &messageId)
{
    return DeleteMessage(boxType, messageId, true);
}

Result MessageBox::DeleteMessage(const CecBoxType boxType, const MessageId &messageId, bool b_withWriteBoxInfo)
{
    SCOPED_LOCK(m_Cs);
    Result result;
    CHECK_CURRENTCECTITLEID();

    if (boxType == CEC_BOXTYPE_OUTBOX)
    {
        u32 targetMessSize = 0;
        u32 targetMessIndex = GetMessageIndex(boxType, const_cast<u8 *>(messageId.GetBinary()));
        if (targetMessIndex < 0xffffffff)
        {
            targetMessSize = GetMessageSize(boxType, targetMessIndex);
        }

        result = Delete(currentCecTitleId, FILETYPE_OUTBOX_MESSAGE, static_cast<u8>(boxType), messageId.GetBinary(),
            CEC_SIZEOF_MESSAGEID);
        if (result.IsFailure())
        {
            if (b_withWriteBoxInfo)
            {

                WriteOutBoxInfo();
            }
            else
            {
                if (b_SkippedWriteOutboxInfo == false)
                {
                    std::memcpy(&tmp_cecOutboxInfo, &cecOutboxInfo, sizeof(tmp_cecOutboxInfo));
                }
                b_SkippedWriteOutboxInfo = true;
            }
            return result;
        }

        if (b_withWriteBoxInfo)
        {
            result = WriteOutBoxInfo();
            NN_UTIL_RETURN_IF_FAILED(result);
        }
        else
        {
            if (b_SkippedWriteOutboxInfo == false)
            {
                std::memcpy(&tmp_cecOutboxInfo, &cecOutboxInfo, sizeof(tmp_cecOutboxInfo));
            }
            b_SkippedWriteOutboxInfo = true;
            tmp_cecOutboxInfo.boxSize -= targetMessSize;
            tmp_cecOutboxInfo.messNum--;
        }

        RemoveOutBoxIndex(messageId);
    }
    else
    {
        NN_TASSERT_(boxType == CEC_BOXTYPE_INBOX);
        u32 targetMessSize = 0;
        u32 targetMessIndex = GetMessageIndex(boxType, const_cast<u8 *>(messageId.GetBinary()));
        if (targetMessIndex < 0xffffffff)
        {
            targetMessSize = GetMessageSize(boxType, targetMessIndex);
        }
        result = Delete(currentCecTitleId, FILETYPE_INBOX_MESSAGE, static_cast<u8>(boxType), messageId.GetBinary(),
                        CEC_SIZEOF_MESSAGEID);
        if (result.IsFailure())
        {
            if (b_withWriteBoxInfo)
            {

                WriteInBoxInfo();
            }
            else
            {
                if (b_SkippedWriteInboxInfo == false)
                {
                    std::memcpy(&tmp_cecInboxInfo, &cecInboxInfo, sizeof(tmp_cecInboxInfo));
                }
                b_SkippedWriteInboxInfo = true;
            }
            return result;
        }

        if (b_withWriteBoxInfo)
        {
            result = WriteInBoxInfo();
            NN_UTIL_RETURN_IF_FAILED(result);
        }
        else
        {
            if (b_SkippedWriteInboxInfo == false)
            {
                std::memcpy(&tmp_cecInboxInfo, &cecInboxInfo, sizeof(tmp_cecInboxInfo));
            }
            b_SkippedWriteInboxInfo = true;
            tmp_cecInboxInfo.boxSize -= targetMessSize;
            tmp_cecInboxInfo.messNum--;
        }
    }
    return result;
}

Result MessageBox::DeleteAllMessages(const CecBoxType boxType)
{
    SCOPED_LOCK(m_Cs);
    Result result;
    CHECK_CURRENTCECTITLEID();

    if (boxType == CEC_BOXTYPE_OUTBOX)
    {
        result = Delete(currentCecTitleId, FILETYPE_MESSAGE_OUTBOX_DIR, static_cast<u8>(boxType), 0, 0);
        if (result.IsSuccess())
        {

            CreateOutBox(currentCecTitleId);
        }
    }
    else
    {
        result = Delete(currentCecTitleId, FILETYPE_MESSAGE_INBOX_DIR, static_cast<u8>(boxType), 0, 0);
        if (result.IsSuccess())
        {

            CreateInBox(currentCecTitleId);
        }
    }
    return result;
}

u32 MessageBox::DeleteSentMessages()
{
    nn::Result result;
    if (currentCecTitleId == 0)
    {
        return 0;
    }

    int j = 0;
    u32 deletedMessageNum = 0;
    u32 messNum = GetBoxMessageNum(CEC_BOXTYPE_OUTBOX);
    for (j = 0; j < messNum; j++)
    {
        MessageId messId;
        result = GetMessageId(&messId, CEC_BOXTYPE_OUTBOX, j);
        if (result.IsSuccess())
        {
            if (GetMessageSendCount(CEC_BOXTYPE_OUTBOX, j) == 0)
            {
                result = Delete(currentCecTitleId, FILETYPE_OUTBOX_MESSAGE, static_cast<u8>(CEC_BOXTYPE_OUTBOX),
                                messId.GetBinary(), CEC_SIZEOF_MESSAGEID);
                if (result.IsFailure())
                {
                    break;
                }
                result = SetData(currentCecTitleId, messId.GetBinary(), CEC_SIZEOF_MESSAGEID, 3);
                deletedMessageNum++;
            }
        }
    }
    WriteOutBoxInfo();
    ReadOutBoxIndex();
    return deletedMessageNum;
}

u32 MessageBox::GetInBoxGroupNum() const
{
    u32 count = 0;
    int i, j;

    for (i = 0; i < cecInboxInfo.messNum; i++)
    {
        if (cecInboxInfo_body[i]->groupId == 0)
        {
            count++;
            continue;
        }
        for (j = 0; j < i; j++)
        {
            if (cecInboxInfo_body[i]->groupId == cecInboxInfo_body[j]->groupId)
            {
                break;
            }
        }
        if (i == j)
        {
            count++;
        }
    }
    return count;
}

u32 MessageBox::GetOutBoxGroupNum() const
{
    u32 count = 0;
    int i, j;

    for (i = 0; i < cecOutboxInfo.messNum; i++)
    {
        if (cecOutboxInfo_body[i]->groupId == 0)
        {
            count++;
            continue;
        }
        for (j = 0; j < i; j++)
        {
            if (cecOutboxInfo_body[i]->groupId == cecOutboxInfo_body[j]->groupId)
            {
                break;
            }
        }
        if (i == j)
        {
            count++;
        }
    }
    return count;
}

Result MessageBox::SetInBoxGroupNumMax(u32 num)
{
    CHECK_CURRENTCECTITLEID();
    Result result;
    if (cecInboxInfo.messNumMax < num || num < 1)
    {
        return ResultInvalidArgument();
    }
    cecInboxInfo.groupNumMax = num;
    result = WriteMessageBoxInfo();
    if (result.IsFailure())
    {
        return result;
    }
    return result;
}

Result MessageBox::SetOutBoxGroupNumMax(u32 num)
{
    CHECK_CURRENTCECTITLEID();
    Result result;
    if (cecOutboxInfo.messNumMax < num || num < 1)
    {
        return ResultInvalidArgument();
    }
    cecOutboxInfo.groupNumMax = num;
    result = WriteMessageBoxInfo();
    if (result.IsFailure())
    {
        return result;
    }
    return result;
}

Result MessageBox::SetBoxGroupNumMax(u32 num, CecBoxType boxType)
{
    if (boxType == CEC_BOXTYPE_INBOX)
    {
        return SetInBoxGroupNumMax(num);
    }
    else
    {
        NN_TASSERT_(boxType == CEC_BOXTYPE_OUTBOX);
        return SetOutBoxGroupNumMax(num);
    }
}

u32 MessageBox::GetInBoxSessionNum()
{
    u32 count = 0;
    int i, j;

    for (i = 0; i < cecInboxInfo.messNum; i++)
    {
        if (cecInboxInfo_body[i]->sessionId == 0)
        {
            count++;
            continue;
        }
        for (j = 0; j < i; j++)
        {
            if (cecInboxInfo_body[i]->sessionId == cecInboxInfo_body[j]->sessionId)
            {
                break;
            }
        }
        if (i == j)
        {
            count++;
        }
    }
    return count;
}

#if 1

u32 MessageBox::GetInBoxMessHeader(CecMessageHeader &messHeader, const MessageId &messageId) const
{
    for (int i = 0; i < cecInboxInfo.messNum; i++)
    {
        if (messageId.IsEqual(cecInboxInfo_body[i]->messageId))
        {
            GetInBoxMessHeaderByIndex(messHeader, i);
            return 0;
        }
    }
    return 1;
}

u32 MessageBox::GetInBoxMessHeaderByIndex(CecMessageHeader &messHeader, const u32 messIndex) const
{
    MEMCPY(reinterpret_cast<u8 *>(&messHeader), reinterpret_cast<u8 *>(cecInboxInfo_body[messIndex]),
           sizeof(CecMessageHeader));
    return 0;
}

u8 *MessageBox::GetInBoxMessIdByIndex(const u32 messIndex)
{
    if (messIndex < cecInboxInfo.messNum)
    {
        return cecInboxInfo_body[messIndex]->messageId;
    }
    return 0;
}

u32 MessageBox::GetOutBoxMessHeader(CecMessageHeader &messHeader, const MessageId &messageId) const
{

    for (int i = 0; i < cecOutboxInfo.messNum; i++)
    {
        if (messageId.IsEqual(cecOutboxInfo_body[i]->messageId))
        {
            MEMCPY(reinterpret_cast<u8 *>(&messHeader), reinterpret_cast<u8 *>(cecOutboxInfo_body[i]),
                   sizeof(CecMessageHeader));
            return 0;
        }
    }
    return 1;
}

u32 MessageBox::GetOutBoxMessHeaderByIndex(CecMessageHeader &messHeader, const u32 messIndex) const
{
    if (messIndex < cecOutboxInfo.messNum)
    {
        return GetOutBoxMessHeader(messHeader, MessageId(cecOutboxIndex[messIndex]));
    }
    return 1;
}

u8 *MessageBox::GetOutBoxMessIdByIndex(const u32 messIndex)
{
    if (messIndex < cecOutboxInfo.messNum)
    {
        return cecOutboxIndex[messIndex];
    }
    return 0;
}

size_t MessageBox::GetBodySizeByMessId(CecBoxType boxType, const MessageId &messageId) const
{
    CecMessageHeader messHeader;
    u32 ret = 1;
    if (boxType == CEC_BOXTYPE_INBOX)
    {
        ret = MessageBox::GetInBoxMessHeader(messHeader, messageId);
    }
    else
    {
        NN_TASSERT_(boxType == CEC_BOXTYPE_OUTBOX);
        ret = MessageBox::GetOutBoxMessHeader(messHeader, messageId);
    }
    if (ret == 0)
    {
        return messHeader.bodySize;
    }
    else
    {
        return 0;
    }
}

#endif

CecMessageHeader *MessageBox::GetMessHeaderByMessId(const CecBoxType boxType, const MessageId &messId) const
{
    if (boxType == CEC_BOXTYPE_INBOX)
    {
        for (int i = 0; i < cecInboxInfo.messNum; i++)
        {
            if (messId.IsEqual(cecInboxInfo_body[i]->messageId))
            {
                return cecInboxInfo_body[i];
            }
        }
    }
    else
    {
        NN_TASSERT_(boxType == CEC_BOXTYPE_OUTBOX);
        for (int i = 0; i < cecOutboxInfo.messNum; i++)
        {
            if (messId.IsEqual(cecOutboxInfo_body[i]->messageId))
            {
                return cecOutboxInfo_body[i];
            }
        }
    }
    return NULL;
}

CecMessageHeader *MessageBox::GetMessHeader(const CecBoxType boxType, const u32 messIndex) const
{
    if (boxType == CEC_BOXTYPE_INBOX)
    {
        if (messIndex < cecInboxInfo.messNum)
        {
            return cecInboxInfo_body[messIndex];
        }
    }
    else
    {
        NN_TASSERT_(boxType == CEC_BOXTYPE_OUTBOX);
        if (messIndex < cecOutboxInfo.messNum)
        {
            if (cecOutboxIndex_header.messageNum == cecOutboxInfo.messNum)
            {
                return GetMessHeaderByMessId(CEC_BOXTYPE_OUTBOX, MessageId(cecOutboxIndex[messIndex]));
            }
            else
            {
                return cecOutboxInfo_body[messIndex];
            }
        }
    }
    return NULL;
}

bool MessageBox::MessageExists(CecBoxType boxType, const MessageId &messageId) const
{

    u32 ret = 1;

    MessageBox& self = const_cast<MessageBox&>(*this);

    if (boxType == CEC_BOXTYPE_INBOX)
    {
        ret = self.GetMessageIndex(CEC_BOXTYPE_INBOX, const_cast<u8*>(messageId.GetBinary()));
    }
    else
    {
        NN_TASSERT_(boxType == CEC_BOXTYPE_OUTBOX);
        ret = self.GetMessageIndex(CEC_BOXTYPE_OUTBOX, const_cast<u8*>(messageId.GetBinary()));
    }

    if (ret == 0xffffffff)
    {
        return false;
    }
    else
    {
        return true;
    }
}

u32 MessageBox::GetMessageSize(const CecBoxType boxType, const u32 messIndex) const
{
    const CecMessageHeader *p_CecMessHeader;

    p_CecMessHeader = GetMessHeader(boxType, messIndex);
    if (p_CecMessHeader == NULL)
    {
        return 0;
    }
    return p_CecMessHeader->messSize;
}

u32 MessageBox::GetMessageBodySize(const CecBoxType boxType, const u32 messIndex) const
{
    const CecMessageHeader *p_CecMessHeader;

    p_CecMessHeader = GetMessHeader(boxType, messIndex);
    if (p_CecMessHeader == NULL)
    {
        return 0;
    }
    return p_CecMessHeader->bodySize;
}

u32 MessageBox::GetMessageGroupId(const CecBoxType boxType, const u32 messIndex) const
{
    const CecMessageHeader *p_CecMessHeader;

    p_CecMessHeader = GetMessHeader(boxType, messIndex);
    if (p_CecMessHeader == NULL)
    {
        return 0;
    }
    return p_CecMessHeader->groupId;
}

u32 MessageBox::GetMessageSessionId(const CecBoxType boxType, const u32 messIndex) const
{
    const CecMessageHeader *p_CecMessHeader;

    p_CecMessHeader = GetMessHeader(boxType, messIndex);
    if (p_CecMessHeader == NULL)
    {
        return 0;
    }
    return p_CecMessHeader->sessionId;
}

MessageTypeFlag MessageBox::GetMessageTypeFlag(const CecBoxType boxType, const u32 messIndex) const
{
    const CecMessageHeader *p_CecMessHeader;

    p_CecMessHeader = GetMessHeader(boxType, messIndex);
    if (p_CecMessHeader == NULL)
    {
        return 0;
    }
    return p_CecMessHeader->messageTypeFlag;
}

SendMode MessageBox::GetMessageSendMode(const CecBoxType boxType, const u32 messIndex) const
{
    const CecMessageHeader *p_CecMessHeader;

    p_CecMessHeader = GetMessHeader(boxType, messIndex);
    if (p_CecMessHeader == NULL)
    {
        return 0;
    }
    return p_CecMessHeader->sendMode;
}

u8 MessageBox::GetMessageSendCount(const CecBoxType boxType, const u32 messIndex) const
{
    const CecMessageHeader *p_CecMessHeader;

    p_CecMessHeader = GetMessHeader(boxType, messIndex);
    if (p_CecMessHeader == NULL)
    {
        return 0;
    }
    return p_CecMessHeader->sendCount;
}

u8 MessageBox::GetMessagePropagationCount(const CecBoxType boxType, const u32 messIndex) const
{
    const CecMessageHeader *p_CecMessHeader;

    p_CecMessHeader = GetMessHeader(boxType, messIndex);
    if (p_CecMessHeader == NULL)
    {
        return 0;
    }
    return p_CecMessHeader->propagationCount;
}

bit8 MessageBox::GetMessageFlag_Unread(const CecBoxType boxType, const u32 messIndex) const
{
    const CecMessageHeader *p_CecMessHeader;

    p_CecMessHeader = GetMessHeader(boxType, messIndex);
    if (p_CecMessHeader == NULL)
    {
        return 0;
    }
    return p_CecMessHeader->flagUnread;
}

bit8 MessageBox::GetMessageFlag_New(const CecBoxType boxType, const u32 messIndex) const
{
    const CecMessageHeader *p_CecMessHeader;

    p_CecMessHeader = GetMessHeader(boxType, messIndex);
    if (p_CecMessHeader == NULL)
    {
        return 0;
    }
    return p_CecMessHeader->flagNew;
}

bit16 MessageBox::GetMessageTag(const CecBoxType boxType, const u32 messIndex) const
{
    const CecMessageHeader *p_CecMessHeader;

    p_CecMessHeader = GetMessHeader(boxType, messIndex);
    if (p_CecMessHeader == NULL)
    {
        return 0;
    }
    return p_CecMessHeader->tag;
}

nn::fnd::DateTimeParameters MessageBox::GetMessageSendDate(const CecBoxType boxType, const u32 messIndex) const
{
    const CecMessageHeader *p_CecMessHeader;

    p_CecMessHeader = GetMessHeader(boxType, messIndex);
    if (p_CecMessHeader == NULL)
    {
        nn::fnd::DateTime minDate = nn::fnd::DateTime::MIN_DATETIME;
        return minDate.GetParameters();
    }
    return p_CecMessHeader->sendDate;
}

nn::fnd::DateTimeParameters MessageBox::GetMessageRecvDate(const CecBoxType boxType, const u32 messIndex) const
{
    const CecMessageHeader *p_CecMessHeader;

    p_CecMessHeader = GetMessHeader(boxType, messIndex);
    if (p_CecMessHeader == NULL)
    {
        nn::fnd::DateTime minDate = nn::fnd::DateTime::MIN_DATETIME;
        return minDate.GetParameters();
    }
    return p_CecMessHeader->recvDate;
}

nn::fnd::DateTimeParameters MessageBox::GetMessageCreateDate(const CecBoxType boxType, const u32 messIndex) const
{
    const CecMessageHeader *p_CecMessHeader;

    p_CecMessHeader = GetMessHeader(boxType, messIndex);
    if (p_CecMessHeader == NULL)
    {
        nn::fnd::DateTime minDate = nn::fnd::DateTime::MIN_DATETIME;
        return minDate.GetParameters();
    }
    return p_CecMessHeader->createDate;
}

MessageId MessageBox::GetMessageIdPair(const CecBoxType boxType, const u32 messIndex)
{
    const CecMessageHeader *p_CecMessHeader;
    static u8 error[] = "ERROR   ";

    p_CecMessHeader = GetMessHeader(boxType, messIndex);
    if (p_CecMessHeader == NULL)
    {
        return MessageId(error);
    }
    MessageId messageIdPair(p_CecMessHeader->messageId_pair);
    if (messageIdPair.IsEmpty())
    {
        return MessageId(error);
    }

    return messageIdPair;
}

Result MessageBox::GetMessageIdPair(MessageId *messId, const CecBoxType boxType, const u32 messIndex)
{
    const CecMessageHeader *p_CecMessHeader;

    p_CecMessHeader = GetMessHeader(boxType, messIndex);
    if (p_CecMessHeader == NULL)
    {
        return ResultNoData();
    }
    MessageId messageIdPair(p_CecMessHeader->messageId_pair);
    if (messageIdPair.IsEmpty())
    {
        return ResultNoData();
    }

    if (messId != NULL)
    {
        *messId = messageIdPair;
    }
    return nn::ResultSuccess();
}

MessageId MessageBox::GetMessageId(const CecBoxType boxType, const u32 messIndex)
{
    const CecMessageHeader *p_CecMessHeader;
    static u8 error[] = "ERROR   ";
    p_CecMessHeader = GetMessHeader(boxType, messIndex);
    if (p_CecMessHeader == NULL || p_CecMessHeader->messageId == NULL)
    {
        return MessageId(error);
    }
    return MessageId(p_CecMessHeader->messageId);
}

Result MessageBox::GetMessageId(MessageId *messId, const CecBoxType boxType, const u32 messIndex)
{
    const CecMessageHeader *p_CecMessHeader;

    p_CecMessHeader = GetMessHeader(boxType, messIndex);
    if (p_CecMessHeader == NULL || p_CecMessHeader->messageId == NULL)
    {
        return ResultNoData();
    }

    if (messId != NULL)
    {
        *messId = MessageId(p_CecMessHeader->messageId);
    }
    return nn::ResultSuccess();
}

u32 MessageBox::GetMessageIndex(CecBoxType boxType, u8 *messId)
{
    NN_TASSERT_(messId);
    u32 messNum = GetBoxMessageNum(boxType);
    for (u32 i = 0; i < messNum; i++)
    {
        if (std::memcmp(messId, GetMessageId(boxType, i).GetBinary(), CEC_SIZEOF_MESSAGEID) == 0)
        {
            return i;
        }
    }
    return 0xffffffff;
}

u32 MessageBox::GetMessageIndex(CecBoxType boxType, MessageId &messId)
{
    u32 messNum = GetBoxMessageNum(boxType);
    for (u32 i = 0; i < messNum; i++)
    {
        if (messId.IsEqual(GetMessageId(boxType, i).GetBinary()))
        {
            return i;
        }
    }
    return 0xffffffff;
}

Result MessageBox::IsAgreeEulaAppRequired() const
{
    // no
    return ResultSuccess();
}

Result MessageBox::OpenFile(u32 cecTitleId, u32 dataType, u32 option, size_t *filesize) const
{
    Result result;
    s32 retry = 0;
    while (retry < 3)
    {
        result = detail::Open(cecTitleId, dataType, option, filesize);
        if (result.IsFailure() && retry < 2)
        {
            if (result == ResultStateBusy())
            {
                CecControl::StopScanning(true);
                b_IsSuspended = true;

                retry++;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    return result;
}

Result MessageBox::ReadFile(void *readBuf, size_t readBufLen, size_t *readLen) const
{
    Result result;
    result = detail::Read(readLen, reinterpret_cast<u8 *>(readBuf), readBufLen);

    if (result == ResultStateBusy())
    {
        CecControl::StopScanning(true);
        b_IsSuspended = true;

        result = detail::Read(readLen, reinterpret_cast<u8 *>(readBuf), readBufLen);
        if (result.IsFailure())
        {
        }
    }
    return result;
}

Result MessageBox::WriteFile(const void *writeBuf, const size_t writeBufLen) const
{
    Result result;
    result = detail::Write(reinterpret_cast<const u8 *>(writeBuf), writeBufLen);

    if (result == ResultStateBusy())
    {
        CecControl::StopScanning(true);
        b_IsSuspended = true;

        result = detail::Write(reinterpret_cast<const u8 *>(writeBuf), writeBufLen);
        if (result.IsFailure())
        {
        }
    }
    return result;
}

Result MessageBox::ReadMessageFile(u32 cecTitleId, u8 boxType, const u8 *pMessId, size_t messIdLen,
    size_t *pReadLen, u8 *pReadBuf, size_t len)
{
    Result result;
    result = detail::ReadMessage(cecTitleId, boxType, pMessId, messIdLen, pReadLen, pReadBuf, len);
    if (result.IsFailure())
    {
        if (result == nn::cec::ResultStateBusy())
        {
            CecControl::StopScanning(true);
            b_IsSuspended = true;

            result = detail::ReadMessage(cecTitleId, boxType, pMessId, messIdLen, pReadLen, pReadBuf, len);
            if (result.IsFailure())
            {
            }
        }
    }
    return result;
}

Result MessageBox::WriteMessageFileWithHmac(u32 cecTitleId, u8 boxType, u8 *pMessId, size_t messIdLen,
    const u8 *pWriteBuf, size_t len, u8 *pHmac)
{
    nn::Result result;

    result = detail::WriteMessageWithHmac(cecTitleId, boxType, pMessId, messIdLen, pWriteBuf, len, pHmac);
    if (result.IsFailure())
    {

        if (result == nn::cec::ResultStateBusy())
        {
            CecControl::StopScanning(true);
            b_IsSuspended = true;

            result = detail::WriteMessageWithHmac(cecTitleId, boxType, pMessId, messIdLen, pWriteBuf, len, pHmac);
            if (result.IsFailure())
            {
            }
        }
    }
    return result;
}

Result MessageBox::Delete(u32 cecTitleId, const u32 dataType, u8 boxType, const u8 *pMessId, size_t messIdLen)
{
    Result result;

    result = detail::Delete(cecTitleId, dataType, boxType, pMessId, messIdLen);
    if (result.IsFailure())
    {
        if (result == nn::cec::ResultStateBusy())
        {
            CecControl::StopScanning(true);
            b_IsSuspended = true;

            result = detail::Delete(cecTitleId, dataType, boxType, pMessId, messIdLen);
        }
    }
    return result;
}

Result MessageBox::SetData(u32 cecTitleId, const u8 *pSetBuf, size_t len, u32 option)
{
    Result result;

    result = detail::SetData(cecTitleId, pSetBuf, len, option);
    if (result.IsFailure())
    {
        if (result == nn::cec::ResultStateBusy())
        {
            CecControl::StopScanning(true);
            b_IsSuspended = true;

            result = detail::SetData(cecTitleId, pSetBuf, len, option);
            if (result.IsFailure())
            {
            }
        }
    }
    return result;
}

Result MessageBox::ReadData(u8 *pReadBuf, size_t len, u32 option, const u8 optionData[], size_t optionDataLen)
{
    Result result;

    result = detail::ReadData(pReadBuf, len, option, optionData, optionDataLen);
    if (result.IsFailure())
    {

        if (result == nn::cec::ResultStateBusy())
        {
            CecControl::StopScanning(true);
            b_IsSuspended = true;

            result = detail::ReadData(pReadBuf, len, option, optionData, optionDataLen);
            if (result.IsFailure())
            {
            }
        }
    }
    return result;
}

} // namespace CTR
} // namespace cec
} // namespace nn

