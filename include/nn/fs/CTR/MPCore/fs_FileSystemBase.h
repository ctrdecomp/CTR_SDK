#pragma once

#include <nn/Result.h>
#include <nn/Handle.h>
#include <nn/fs/fs_Parameters.h>

namespace nn{
namespace fs{
    Result MountSaveData(const char* archiveName = "data:");

    Result FormatSaveData(size_t paxFiles, size_t maxDirectories, bool isDuplicated);
    Result CommitSaveData(const char* archiveName = "data:");

    Result MountRom(const char *archiveName, size_t maxFile, size_t maxDirectory, void *pWorkingMemory, size_t workingMemorySize, bool useCache);
    Result MountRom(size_t maxFile, size_t maxDirectory, void* pWorkingMemory, size_t workingMemorySize, bool useCache);
    Result MountSharedExtSaveData(const char* archiveName, bit32 id);
    Result MountSpecialArchive(const char* archiveName, bit32 archiveKind);
    Result MountContent(const char* archiveName, MediaType mediaType, TitleId titleId, ContentIdx contentIndex, size_t maxFile, size_t maxDirectory, void* workingMemory, size_t workingMemorySize, bool useCache);
    Result Unmount(const char* archiveName);

    Result MountSdmc(const char* archiveName = "sdmc:");

    // Archive Size
    Result GetTwlPhotoSize(s64* pTotal, s64* pFree);
    Result GetSdmcSize(s64* pTotal, s64* pFree);
    Result GetSaveDataFormatInfo(size_t* maxFiles, size_t* maxDirectories, bool* isDuplicateAll);
    s32 GetContentRequiredMemorySize(MediaType, TitleId, ContentIdx, size_t maxFile, size_t maxDirectory);

    // Memory Size
    s32 GetRomRequiredMemorySize(size_t maxFile, size_t maxDirectory, bool useCache);
    s32 GetRomRequiredMemorySizeImpl(size_t maxFile, size_t maxDirectory, bool useCache, ProgramDataPath* pContentPath);

    void InitializeLatencyEmulation();
    void ForceDisableLatencyEmulation();
}
}

