#pragma once 

#include <nn/types.h>
#include <nn/fs/fs_Types.h>

namespace nn{
namespace dbm{

typedef wchar_t RomPathChar;

typedef s32 RomFileId;

namespace detail{

struct S64
{
private:
    u32 mvalue[2];

public:
    inline s64 Get(void) const
    {
        return *reinterpret_cast<const s64*>(&mvalue);
    }
};

struct RomFileInfo
{
    S64 offset;
    S64 size;
};

struct RomFileSystemInformation
{
    u32 size;
    u32 offsetBucketDirectory;
    u32 sizeBucketDirectory;
    u32 offsetDirectoryEntry;
    u32 sizeDirectoryEntry;
    u32 offsetBucketFile;
    u32 sizeBucketFile;
    u32 offsetFileEntry;
    u32 sizeFileEntry;
    u32 offsetFileBody;
};
    
}
}
}