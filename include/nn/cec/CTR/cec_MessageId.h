#pragma once

#include <nn/types.h>

namespace nn {
namespace cec {
namespace CTR {

typedef u8 CECMessageId[8];

class MessageId
{
public:
    static const size_t SIZE = 8;
    static const size_t ENCODED_SIZE = 12;

    MessageId();
    MessageId(const MessageId& msgId){}

    explicit MessageId(const u8 msgId[SIZE]);
    explicit MessageId(const char* msgId);
    explicit MessageId(CECMessageId msgId);

    const u8* GetBinary(void) const{ return m_data; }
    void GetBinary(u8 msgId[SIZE]) const;

    bool IsEqual(const u8 msgId[SIZE]) const;
    bool IsEmpty() const;
private:
    u8 m_data[SIZE];
    static  char s_buffer[SIZE * 2 + 1];
};

} // namespace CTR
} // namespace cec
} // namespace nn
