#pragma once

#include "nn/types.h"

namespace nn {
namespace ro {

    enum FixLevel{
        FIX_LEVEL_0,
        FIX_LEVEL_1,
        FIX_LEVEL_2,
        FIX_LEVEL_3,
        FIX_LEVEL_NONE = FIX_LEVEL_0, 
        FIX_LEVEL_INTERNAL = FIX_LEVEL_1,
        FIX_LEVEL_INTERNAL_IMPORT = FIX_LEVEL_2,
        FIX_LEVEL_INTERNAL_IMPORT_EXPORT = FIX_LEVEL_3,
        FIX_LEVEL_MAX_BITS = (1u << 31)
    };

    struct SizeInfo
    {
        uptr    m_Fix0End;
        uptr    m_Fix1End;
        uptr    m_Fix2End;
        uptr    m_Fix3End;
        size_t  m_BufferSize;
    };

    struct RegionInfo{
        uptr    m_MapBegin;
        size_t  m_MapSize;
        uptr    m_CroBegin;
        size_t  m_CroSize;
        uptr    m_DataBssBegin;
        size_t  m_DataBssSize;
        uptr    m_CodeBegin;
        size_t  m_CodeSize;
    };

} // end of namespace ro
} // end of namespace nn