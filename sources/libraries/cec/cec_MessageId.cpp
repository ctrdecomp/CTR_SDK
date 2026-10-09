// Filename: cec_MessageId.cpp
//
// Project: Horizon

#include <cstring>
#include <nn/nstd.h>
#include <nn/cec/CTR/cec_MessageId.h>
#include <nn/Assert.h>

namespace nn {
namespace cec {
namespace CTR {

MessageId::MessageId(void)
{
    memset(m_data, 0x00, SIZE);
}

MessageId::MessageId(const u8 msgId[SIZE])
{
    if(msgId != NULL)
    {
        memcpy(m_data, msgId, SIZE);
    }
    else
    {
        memset(m_data, 0x00, SIZE);
    }
}

MessageId::MessageId(CECMessageId msgId)
{
    if(msgId != NULL)
    {
        memcpy(m_data, msgId, SIZE);
    }
    else
    {
        memset(m_data, 0x00, SIZE);
    }
}

bool MessageId::IsEqual(const u8 msgId[SIZE]) const
{
    if(msgId != NULL)
    {
        return memcmp(msgId, m_data, SIZE) == 0;
    }
    else
    {
        return false;
    }
}

void MessageId::GetBinary(u8 msgId[SIZE]) const
{
    NN_TASSERT_(msgId);

    memcpy(msgId, m_data, SIZE);
}

bool MessageId::IsEmpty() const
{
    for (s32 i = 0; i < SIZE; ++i)
    {
        if (m_data[i] != 0)
        {
            return false;
        }
    }
    return true;
}

} // namespace CTR
} // namespace cec
} // namespace nn

