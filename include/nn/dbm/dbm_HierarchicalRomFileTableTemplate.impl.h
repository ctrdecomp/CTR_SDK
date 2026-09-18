#pragma once

#include <nn/dbm/dbm_HierarchicalRomFIleTableTemplate.h>

namespace nn{
namespace dbm{

template <typename DirectoryBucketStorage_,typename DirectoryEntryStorage_,typename FileBucketStorage_,typename FileEntryStorage_>
HierarchicalRomFileTableTemplate<DirectoryBucketStorage_,DirectoryEntryStorage_,FileBucketStorage_,FileEntryStorage_>::HierarchicalRomFileTableTemplate()
{
}

template <typename DirectoryBucketStorage_,typename DirectoryEntryStorage_,typename FileBucketStorage_,typename FileEntryStorage_>
Result HierarchicalRomFileTableTemplate<DirectoryBucketStorage_,DirectoryEntryStorage_,FileBucketStorage_,FileEntryStorage_>::Initialize(
        DirectoryBucketStorage* pDirectoryBucket,s64 offsetDirectoryBucket,u32 sizeDirectoryBucket, DirectoryEntryStorage* pDirectoryEntry,s64 offsetDirectoryEntry,u32 sizeDirectoryEntry,
        FileBucketStorage* pFileBucket,s64 offsetFileBucket,u32 sizeFileBucket, FileEntryStorage* pFileEntry,s64 offsetFileEntry,u32 sizeFileEntry)
{
    NN_NULL_TASSERT_(pDirectoryBucket);
    NN_NULL_TASSERT_(pDirectoryEntry);
    NN_NULL_TASSERT_(pFileBucket);
    NN_NULL_TASSERT_(pFileEntry);

    Result res;

    res = this->m_TableDirectory.Initialize(pDirectoryBucket,offsetDirectoryBucket,FileEntryMapTable::QueryBucketCount(sizeDirectoryBucket),pDirectoryEntry,offsetDirectoryEntry,sizeDirectoryEntry);

    if (res.IsFailure())
    {
        return res;
    }

    res = this->m_TableFile.Initialize(pFileBucket,offsetFileBucket,FileEntryMapTable::QueryBucketCount(sizeFileBucket),pFileEntry,offsetFileEntry,sizeFileEntry);

    if (res.IsFailure())
    {
        return res;
    }

    return  ResultSuccess();
}

template <typename DirectoryBucketStorage_,typename DirectoryEntryStorage_,typename FileBucketStorage_,typename FileEntryStorage_>
void HierarchicalRomFileTableTemplate<DirectoryBucketStorage_,DirectoryEntryStorage_,FileBucketStorage_,FileEntryStorage_>::Finalize(void){
    this->m_TableDirectory.Finalize();
    this->m_TableFile.Finalize();
}

template <typename DirectoryBucketStorage_,typename DirectoryEntryStorage_,typename FileBucketStorage_,typename FileEntryStorage_>
Result HierarchicalRomFileTableTemplate<DirectoryBucketStorage_,DirectoryEntryStorage_,FileBucketStorage_,FileEntryStorage_>::GetDirectoryEntry(StoragePosition* pPosition,DirectoryEntry* pEntry,const EntryKey& key) const
{
    NN_NULL_TASSERT_(pPosition);
    NN_NULL_TASSERT_(pEntry);

    Result res = this->m_TableDirectory.Get(pPosition, pEntry, key);
    if (res.IsFailure())
    {
        if (ResultKeyNotFound::Includes(res))
        {
            StoragePosition filePosition;
            FileEntry fileEntry;
            res = this->m_TableFile.Get(&filePosition, &fileEntry, key);
            if (res.IsFailure())
            {
                if (!ResultKeyNotFound::Includes(res))
                {
                    return res;
                }
                return fs::ResultDirectoryNotFound();
            }
            else
            {
                return fs::ResultInvalidOperation();
            }
        }
    }
    return res;
}

template <typename DirectoryBucketStorage_,typename DirectoryEntryStorage_,typename FileBucketStorage_,typename FileEntryStorage_>
Result HierarchicalRomFileTableTemplate<DirectoryBucketStorage_,DirectoryEntryStorage_,FileBucketStorage_,FileEntryStorage_>::GetGrandparent(
           StoragePosition* pPosition,EntryKey* pDirKey,DirectoryEntry* pDirEntry,StoragePosition pos,RomPathTool::RomEntryName name,const RomPathChar* pFullPath) const
{
    NN_NULL_TASSERT_(pPosition);
    NN_NULL_TASSERT_(pDirKey);
    NN_NULL_TASSERT_(pDirEntry);

    Result res;

    RomEntryKey granpaKey;
    DirectoryEntry granpaEntry;
    res = this->m_TableDirectory.GetByPosition(&granpaKey, &granpaEntry, NULL, NULL, pos);

    if (res.IsFailure())
    {
        return res;
    }
    pDirKey->key.parentDir = granpaKey.parentDir;

    res = RomPathTool::GetParentDirectoryName(&pDirKey->name, name, pFullPath);
    if (res.IsFailure())
    {
        return res;
    }

    res = GetDirectoryEntry(pPosition, pDirEntry, *pDirKey);
    if (res.IsFailure())
    {
        return res;
    }

    return ResultSuccess();
}

template <typename DirectoryBucketStorage_,typename DirectoryEntryStorage_,typename FileBucketStorage_,typename FileEntryStorage_>
Result HierarchicalRomFileTableTemplate<DirectoryBucketStorage_,DirectoryEntryStorage_,FileBucketStorage_,FileEntryStorage_>::FindOpen(FindPosition* pfi, const EntryKey& dirKey) const
{
    NN_NULL_TASSERT_(pfi);

    Result result;

    pfi->nextPositionDirectory = -1;
    pfi->nextPositionFile = -1;

    StoragePosition pos;
    DirectoryEntry dirEntry;
    result = this->GetDirectoryEntry(&pos, &dirEntry, dirKey);
    if (result.IsFailure())
    {
        return result;
    }

    pfi->nextPositionDirectory = dirEntry.posDirectory;
    pfi->nextPositionFile = dirEntry.posFile;

    return ResultSuccess();
}

template <typename DirectoryBucketStorage_,typename DirectoryEntryStorage_,typename FileBucketStorage_,typename FileEntryStorage_>
Result HierarchicalRomFileTableTemplate<DirectoryBucketStorage_,DirectoryEntryStorage_,FileBucketStorage_,FileEntryStorage_>::FindOpen(FindPosition* pfi, const RomPathChar* pFullPath) const
{
    NN_NULL_TASSERT_(pfi);
    NN_NULL_TASSERT_(pFullPath);

    Result result;

    EntryKey parentKey;
    DirectoryEntry parentEntry;
    EntryKey dirKey;
    result = this->FindDirectoryRecursive(&parentKey,&parentEntry,&dirKey,pFullPath);
    if (result.IsFailure())
    {
        return result;
    }

    return FindOpen(pfi, dirKey);
}

template <typename DirectoryBucketStorage_,typename DirectoryEntryStorage_,typename FileBucketStorage_,typename FileEntryStorage_>
Result HierarchicalRomFileTableTemplate<DirectoryBucketStorage_,DirectoryEntryStorage_,FileBucketStorage_,FileEntryStorage_>::FindParentDirRecursive(
    StoragePosition* pParentPosition,EntryKey* pParentDirKey, DirectoryEntry* pParentDirEntry,RomPathTool::PathParser& parser,const RomPathChar* pFullPath) const
{
    NN_NULL_TASSERT_(pParentPosition);
    NN_NULL_TASSERT_(pParentDirKey);
    NN_NULL_TASSERT_(pParentDirEntry);

    Result res;

    StoragePosition dirPosition = 0xC;
    EntryKey dirKey;
    DirectoryEntry dirEntry;

    dirKey.key.parentDir = 0xC;
    res = parser.GetNextDirectoryName(&dirKey.name);
    if (res.IsFailure())
    {
        return res;
    }

    res = this->GetDirectoryEntry(&dirPosition, &dirEntry, dirKey);
    if (res.IsFailure())
    {
        return res;
    }

    StoragePosition parentPosition = dirPosition;
    while (!parser.IsParseFinished())
    {
        EntryKey oldKey = dirKey;

        res = parser.GetNextDirectoryName(&dirKey.name);
        if (res.IsFailure())
        {
            return res;
        }

        if (RomPathTool::IsCurrentDirectory(dirKey.name))
        {
            dirKey = oldKey;
            continue;
        }

        else if (RomPathTool::IsParentDirectory(dirKey.name))
        {
            if (parentPosition == 0xC)
            {
                return fs::ResultInvalidOperation();
            }

            res = this->GetGrandparent(&parentPosition,&dirKey,&dirEntry,dirKey.key.parentDir,dirKey.name,pFullPath);
            if (res.IsFailure())
            {
                return res;
            }
        }
        else
        {
            dirKey.key.parentDir = parentPosition;
            res = this->GetDirectoryEntry(&dirPosition, &dirEntry, dirKey);
            if (res.IsFailure())
            {
                return res;
            }
    
            parentPosition = dirPosition;
        }
    }

    *pParentPosition = parentPosition;
    *pParentDirKey = dirKey;
    *pParentDirEntry = dirEntry;

    return ResultSuccess();
}

template <typename DirectoryBucketStorage_,typename DirectoryEntryStorage_,typename FileBucketStorage_,typename FileEntryStorage_>
Result HierarchicalRomFileTableTemplate<DirectoryBucketStorage_,DirectoryEntryStorage_,FileBucketStorage_,FileEntryStorage_>::FindDirectoryRecursive(
    EntryKey* pParentDirKey,DirectoryEntry* pParentDirEntry,EntryKey* pDirKey,const RomPathChar* pFullPath) const
{
    return this->FindPathRecursive(pParentDirKey,pParentDirEntry,pDirKey,true,pFullPath);
}

template <typename DirectoryBucketStorage_,typename DirectoryEntryStorage_,typename FileBucketStorage_,typename FileEntryStorage_>
Result HierarchicalRomFileTableTemplate<DirectoryBucketStorage_,DirectoryEntryStorage_,FileBucketStorage_,FileEntryStorage_>::OpenFile(FileInfo* pFileInfo, RomFileId fileId) const
{
    NN_NULL_TASSERT_(pFileInfo);

    Result res;

    FileEntry fileEntry;
    res = this->GetFileEntry(&fileEntry, fileId);
    if (res.IsFailure())
    {
        return res;
    }

    *pFileInfo = fileEntry.info;

    return ResultSuccess();
}

template <typename DirectoryBucketStorage_,typename DirectoryEntryStorage_,typename FileBucketStorage_,typename FileEntryStorage_>
Result HierarchicalRomFileTableTemplate<DirectoryBucketStorage_,DirectoryEntryStorage_,FileBucketStorage_,FileEntryStorage_>::GetFileEntry(StoragePosition* pPosition,FileEntry* pEntry,const EntryKey& key) const
{
    NN_NULL_TASSERT_(pPosition);
    NN_NULL_TASSERT_(pEntry);

    Result res = this->m_TableFile.Get(pPosition, pEntry, key);
    if (res.IsFailure())
    {
        if (ResultKeyNotFound::Includes(res))
        {
            StoragePosition dirPosition;
            DirectoryEntry dirEntry;
            res = this->m_TableDirectory.Get(&dirPosition, &dirEntry, key);
            if (res.IsFailure())
            {
                if (!ResultKeyNotFound::Includes(res))
                {
                    return res;
                }

                return ResultFileNotFound();
            }
            else
            {
                return fs::ResultInvalidOperation();
            }
        }
    }
    return res;
}

template <typename DirectoryBucketStorage_,typename DirectoryEntryStorage_,typename FileBucketStorage_,typename FileEntryStorage_>
Result HierarchicalRomFileTableTemplate<DirectoryBucketStorage_,DirectoryEntryStorage_,FileBucketStorage_,FileEntryStorage_>::GetFileEntry(FileEntry* pEntry,RomFileId fileId) const
{
    NN_NULL_TASSERT_(pEntry);

    StoragePosition pos = FileIdToPosition(fileId);

    RomEntryKey key;
    Result res = this->m_TableFile.GetByPosition(&key, pEntry, NULL, NULL, pos);
    if (res.IsFailure())
    {
        if (ResultKeyNotFound::Includes(res))
        {
            DirectoryEntry dirEntry;
            res = this->m_TableDirectory.GetByPosition(&key, &dirEntry, NULL, NULL, pos);
            if (res.IsFailure())
            {
                if (!ResultKeyNotFound::Includes(res))
                {
                    return res;
                }
                return ResultFileNotFound();
            }
            else
            {
                return fs::ResultInvalidOperation();
            }
        }
    }
    return res;
}

template <typename DirectoryBucketStorage_,typename DirectoryEntryStorage_,typename FileBucketStorage_,typename FileEntryStorage_>
Result HierarchicalRomFileTableTemplate<DirectoryBucketStorage_,DirectoryEntryStorage_,FileBucketStorage_,FileEntryStorage_>::OpenFile(FileInfo* pFileInfo, const EntryKey& fileKey) const
{
    NN_NULL_TASSERT_(pFileInfo);

    Result res;

    FileEntry fileEntry;
    StoragePosition pos;
    res = GetFileEntry(&pos, &fileEntry, fileKey);
    if (res.IsFailure())
    {
        return res;
    }

    *pFileInfo = fileEntry.info;

    return ResultSuccess();
}

template <typename DirectoryBucketStorage_,typename DirectoryEntryStorage_,typename FileBucketStorage_,typename FileEntryStorage_>
Result HierarchicalRomFileTableTemplate<DirectoryBucketStorage_,DirectoryEntryStorage_,FileBucketStorage_,FileEntryStorage_>::OpenFile(FileInfo* pFileInfo, const RomPathChar* pFullPath) const
{
    NN_NULL_TASSERT_(pFileInfo);
    NN_NULL_TASSERT_(pFullPath);

    Result res;

    EntryKey parentKey;
    DirectoryEntry parentEntry;
    EntryKey fileKey;
    res = FindFileRecursive(&parentKey,&parentEntry,&fileKey,pFullPath);
    if (res.IsFailure())
    {
        return res;
    }

    return OpenFile(pFileInfo, fileKey);
}

template <typename DirectoryBucketStorage_,typename DirectoryEntryStorage_,typename FileBucketStorage_,typename FileEntryStorage_>
Result HierarchicalRomFileTableTemplate<DirectoryBucketStorage_,DirectoryEntryStorage_,FileBucketStorage_,FileEntryStorage_>::FindNextDirectory(RomPathChar* pDirectoryName,FindPosition* pfi) const
{
    NN_NULL_TASSERT_(pfi);
    NN_NULL_TASSERT_(pDirectoryName);

    Result res;

    if (pfi->nextPositionDirectory == -1)
    {
        return  ResultFindFinished();
    }

    RomEntryKey key;
    DirectoryEntry entry;
    size_t length;
    res = this->m_TableDirectory.GetByPosition(&key, &entry, pDirectoryName, &length, pfi->nextPositionDirectory);
    if (res.IsFailure())
    {
        return res;
    }
    pDirectoryName[length / sizeof(RomPathChar)] = NULL;

    pfi->nextPositionDirectory = entry.posNext;

    return ResultSuccess();
}

template <typename DirectoryBucketStorage_,typename DirectoryEntryStorage_,typename FileBucketStorage_,typename FileEntryStorage_>
Result HierarchicalRomFileTableTemplate<DirectoryBucketStorage_,DirectoryEntryStorage_,FileBucketStorage_,FileEntryStorage_>::FindNextFile(RomPathChar* pFileName, FindPosition* pfi) const
{
    NN_NULL_TASSERT_(pfi);
    NN_NULL_TASSERT_(pFileName);

    Result result;

    if (pfi->nextPositionFile == -1)
    {
        return ResultFindFinished();
    }

    RomEntryKey key;
    FileEntry entry;
    size_t length = 0;
    result = this->m_TableFile.GetByPosition(&key, &entry, pFileName, &length, pfi->nextPositionFile);
    if (result.IsFailure())
    {
        return result;
    }
    pFileName[length / sizeof(RomPathChar)] = NULL;

    pfi->nextPositionFile = entry.posNext;

    return ResultSuccess();
}

template <typename DirectoryBucketStorage_,typename DirectoryEntryStorage_,typename FileBucketStorage_,typename FileEntryStorage_>
Result HierarchicalRomFileTableTemplate<DirectoryBucketStorage_,DirectoryEntryStorage_,FileBucketStorage_,FileEntryStorage_>::FindPathRecursive(EntryKey* pParentDirKey,DirectoryEntry* pParentDirEntry,
        EntryKey* pKey,bool bFindDir,const RomPathChar* pFullPath) const
{
    NN_NULL_TASSERT_(pParentDirKey);
    NN_NULL_TASSERT_(pParentDirEntry);
    NN_NULL_TASSERT_(pKey);
    NN_NULL_TASSERT_(pFullPath);

    Result result;

    RomPathTool::PathParser parser;
    result = parser.Initialize(pFullPath);
    if (result.IsFailure())
    {
        return result;
    }

    StoragePosition parentPosition = 0;
    result = this->FindParentDirRecursive(&parentPosition,pParentDirKey,pParentDirEntry,parser,pFullPath);
    if (result.IsFailure())
    {
        return result;
    }

    if (bFindDir)
    {
        RomPathTool::RomEntryName name;

        result = parser.GetAsDirectoryName(&name);
        if (result.IsFailure())
        {
            return result;
        }

        if (RomPathTool::IsCurrentDirectory(name))
        {
            *pKey = *pParentDirKey;

            if (pKey->key.parentDir != 0)
            {
                StoragePosition pos;
                result = this->GetGrandparent(&pos,pParentDirKey,pParentDirEntry,pKey->key.parentDir,pKey->name,pFullPath);
                if (result.IsFailure())
                {
                    return result;
                }
            }
        }
        else if (RomPathTool::IsParentDirectory(name))
        {
            if (parentPosition == 0)
            {
                return fs::ResultInvalidOperation();
            }

            StoragePosition pos;

            DirectoryEntry currEntry;
            result = this->GetGrandparent(&pos,pKey,&currEntry,pParentDirKey->key.parentDir,pParentDirKey->name,pFullPath);
            if (result.IsFailure())
            {
                return result;
            }

            if (pKey->key.parentDir != 0)
            {
                result = this->GetGrandparent(&pos,pParentDirKey,pParentDirEntry,pKey->key.parentDir,pKey->name,pFullPath);
                if (result.IsFailure())
                {
                    return result;
                }
            }
        }
        else
        {
            pKey->name = name;
            pKey->key.parentDir = (pKey->name.length > 0) ? parentPosition : 0;
        }
    }
    else
    {
        if (parser.IsDirectoryPath())
        {
            return fs::ResultInvalidOperation();
        }

        pKey->key.parentDir = parentPosition;
        result = parser.GetAsFileName(&pKey->name);
        if (result.IsFailure())
        {
            return result;
        }
    }

    return ResultSuccess();
}

template <typename DirectoryBucketStorage_,typename DirectoryEntryStorage_,typename FileBucketStorage_,typename FileEntryStorage_>
Result HierarchicalRomFileTableTemplate<DirectoryBucketStorage_,DirectoryEntryStorage_,FileBucketStorage_,FileEntryStorage_>::FindFileRecursive(
    EntryKey* pParentDirKey,DirectoryEntry* pParentDirEntry,EntryKey* pFileKey,const RomPathChar* pFullPath) const
{
    return FindPathRecursive(pParentDirKey,pParentDirEntry,pFileKey,false,pFullPath);
}

}
}