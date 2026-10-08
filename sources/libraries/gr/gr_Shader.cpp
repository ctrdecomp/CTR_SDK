// Filename: gr_Shader.cpp
//
// Project: Horizon

#include <nn/gr/CTR/gr_Shader.h>

namespace nn{
namespace gr{
namespace CTR{

Shader::Shader():
    m_VtxShaderIndex(0),
    m_GeoShaderIndex(-1),
    m_ExeImageInfoNum(0),
    m_InstructionCount(0),
    m_SwizzleCount(0),
    m_DrawMode(PICA_DATA_DRAW_TRIANGLES),
    m_VtxShaderBoolMapUniform(0),
    m_GeoShaderBoolMapUniform(0),
    m_CmdCacheOutAttrNum(0)
{
    for (s32 shader_index = 0; shader_index < EXE_IMAGE_MAX; shader_index++)
    {
        m_CmdCacheConstNumArray[shader_index] = 0;
    }
}

void Shader::SetupBinary(const void* shader_binary, const s32 vtx_shader_index, const s32 geo_shader_index)
{
    const bit32* binary = reinterpret_cast<const bit32*>(shader_binary);
    NN_ASSERT_(binary != NULL);

    NN_ASSERT_(*binary == 0x424C5644);
    ++binary;

    NN_ASSERT_(*binary < EXE_IMAGE_MAX);

    m_ExeImageInfoNum = *binary;
    ++binary;

    m_VtxShaderBoolMapUniform = 0;
    m_GeoShaderBoolMapUniform = 0;

    for (s32 i = 0; i < m_ExeImageInfoNum; ++i)
    {
        m_ExeImageInfo[i] = reinterpret_cast< const ExeImageInfo* >( (u8*)shader_binary + *binary );
        NN_ASSERT_(m_ExeImageInfo[i]->signature == 0x454c5644);
        ++binary;
    }
                
    const bit32* package_info = binary;
    NN_ASSERT_(*binary ==0x504C5644);
    ++binary;
    ++binary;

    m_Instruction = reinterpret_cast<const bit32*>((u8*)package_info + *binary);
    ++binary;

    m_InstructionCount = *binary;
    ++binary;

    const bit32* swizzle = reinterpret_cast<const bit32*>((u8*)package_info + *binary);
    ++binary;

    m_SwizzleCount = *binary;
    NN_ASSERT_(m_SwizzleCount < SWIZZLE_PATTERN_MAX);
    ++binary;

    for (s32 i = 0; i < m_SwizzleCount; i++)
    {
        m_Swizzle[i] = swizzle[i * 2];
    }

    PicaDataDrawMode drawMode  = m_DrawMode;

    MakeShaderConstCommandCache_();
    SetShaderIndex(vtx_shader_index, geo_shader_index);
                
    if (!this->IsEnableGeoShader())
    {
        m_DrawMode = drawMode;
    }
}

void Shader::SetShaderIndex(const s32 vtx_shader_index, const s32 geo_shader_index)
{
    this->CheckVtxShaderIndex_(vtx_shader_index);
    this->CheckGeoShaderIndex_(geo_shader_index);
                
    m_VtxShaderIndex = vtx_shader_index;
    m_GeoShaderIndex = geo_shader_index;

    if(this->IsEnableGeoShader())
    {
        m_DrawMode = PICA_DATA_DRAW_GEOMETRY_PRIMITIVE;
    }
               
    this->MakeShaderOutAttrCommandCache_();
}

bool Shader::SearchBindSymbol(BindSymbol* symbol, const char* name) const
{
    const s32 shader_index = (symbol->shaderType == BindSymbol::SHADER_TYPE_GEOMETRY )
        ? GetGeoShaderIndex() : GetVtxShaderIndex();

    NN_ASSERT_(0 <= shader_index && shader_index < m_ExeImageInfoNum);

    const ExeImageInfo* exe_info = m_ExeImageInfo[ shader_index ];

    struct BindSymbolInfo 
    { 
        u32 nameIndex; 
        u32 regIndex; 
    };
    const BindSymbolInfo* bind_symbol_info = reinterpret_cast<const BindSymbolInfo*>(
        reinterpret_cast<const u8*>(exe_info) + exe_info->bindSymbolOffset);

    const char* string = reinterpret_cast<const char*>(
        reinterpret_cast<const u8*>(exe_info) + exe_info->stringOffset);

    u32 namelen = std::strlen(name);
    for (s32 i = 0; i < exe_info->bindSymbolCount; ++i)
    {
        const BindSymbolInfo& info = bind_symbol_info[i];

        if (std::strncmp(name, &string[info.nameIndex], namelen) != 0) 
        {
            continue;
        }
        if (string[info.nameIndex + namelen] != '\0' &&  string[info.nameIndex + namelen] != '.') 
        {
            continue;
        }

        symbol->name  = &string[info.nameIndex];
        symbol->start = (info.regIndex & 0x0000ffff);
        symbol->end   = (info.regIndex & 0xffff0000) >> 16;

        if (136 <= symbol->start)
        {
            return false;
        }
        else if (120 <= symbol->start)
        {
            symbol->start -= 120;
            symbol->end   -= 120;

            return symbol->symbolType == BindSymbol::SYMBOL_TYPE_BOOL;
        }
        else if (112 <= symbol->start)
        {
            symbol->start -= 112;
            symbol->end   -= 112;

            return symbol->symbolType == BindSymbol::SYMBOL_TYPE_INTEGER;
        }
        else if (16 <= symbol->start)
        {
            symbol->start -= 16;
            symbol->end   -= 16;

            return symbol->symbolType == BindSymbol::SYMBOL_TYPE_FLOAT;
        }
        else
        {
            return symbol->symbolType == BindSymbol::SYMBOL_TYPE_INPUT;
        }
    }

    return false;
}

bit32* Shader::MakeLoadCommand_(bit32* command, const bit32  load_reg, const bit32* src_buffer_ptr, const u32  src_data_num) const
{
    const s32 WRITE_MAX = 128;

    u32 rest = src_data_num;

    while (true)
    {
        if (rest <= WRITE_MAX)
        {
            *command++ = *src_buffer_ptr++;
            *command++ = PICA_CMD_HEADER_BURST(load_reg, rest);
            std::memcpy(command, src_buffer_ptr, (rest - 1) * sizeof(bit32));
            command += rest - 1;

            if ((rest & 1) == 0)
            {
                *command++ = PADDING_DATA;
            }
            break;
        }
        else
        {
            *command++ = *src_buffer_ptr++;
            *command++ = PICA_CMD_HEADER_BURST(load_reg, WRITE_MAX);
            std::memcpy(command, src_buffer_ptr, (WRITE_MAX - 1) * sizeof(bit32));

            command += WRITE_MAX - 1;
            src_buffer_ptr += WRITE_MAX - 1;

            rest -= WRITE_MAX;
            if ((WRITE_MAX & 1) == 0)
            {
                *command++ = PADDING_DATA;
            }
        }
    }

    return command;
}

bit32* Shader::MakeOutAttrCommand_(bit32* command,
                                   const s32 vtx_shader_index,
                                   const s32 geo_shader_index)
{
    s32 shader_index = vtx_shader_index;

    bool is_geometry_shader = false;
    if (0 <= GetGeoShaderIndex())
    {
        is_geometry_shader = true;
        shader_index = geo_shader_index;
    }

    NN_ASSERT_(0 <= shader_index && shader_index < m_ExeImageInfoNum);

    const s32 OUT_ATTR_INDEX_MAX     = 7;
    const s32 OUT_ATTR_DIMENTION_MAX = 4;
    const s32 OUT_ATTR_BUFFER_MAX    = 16 * 4;
    const s32 VS_OUT_ATTR_INDEX_MAX  = 16;

    struct OutmapInfo
    {
        u16 type;
        u16 index;
        u16 mask;
        u16 reserve;
    };

    u32 outNum = 0;
    bit32 useTex = 0;
    bit32 clock = 0;
    bit32 outMask = 0;
    bit32 attr[OUT_ATTR_INDEX_MAX];

    {
        const ExeImageInfo* exe_info = m_ExeImageInfo[shader_index];

        OutmapInfo outmap_buffer[OUT_ATTR_BUFFER_MAX];
        s32 outMapBufferCount = 0;

        if (is_geometry_shader && (exe_info->outputMaps & 0x01))
        {
            bit32 gs_copy_mask = 0;
            bit32 vs_copy_mask = 0;

            const OutmapInfo* outmapInfo =
                reinterpret_cast<const OutmapInfo*>(
                    reinterpret_cast<const u8*>(exe_info) + exe_info->outMapOffset);

            NN_ASSERT_(0 <= vtx_shader_index && vtx_shader_index < m_ExeImageInfoNum);
            const ExeImageInfo* vtx_exe_info = m_ExeImageInfo[vtx_shader_index];

            const OutmapInfo* vtxOutmapInfo =
                reinterpret_cast<const OutmapInfo*>(
                    reinterpret_cast<const u8*>(vtx_exe_info) + vtx_exe_info->outMapOffset);

            NN_ASSERT_(outMapBufferCount < OUT_ATTR_BUFFER_MAX);
            for (s32 g = 0; g < exe_info->outMapCount; ++g)
            {
                if (outmapInfo[g].type >= 0 &&
                    outmapInfo[g].type < 9 &&
                    outmapInfo[g].type != 7)
                {
                    for (s32 v = 0; v < vtx_exe_info->outMapCount; ++v)
                    {
                        if (vtxOutmapInfo[v].type >= 0 &&
                            vtxOutmapInfo[v].type < 9 &&
                            vtxOutmapInfo[v].type != 7)
                        {
                            if (outmapInfo[g].type == vtxOutmapInfo[v].type)
                            {
                                NN_ASSERT_(outMapBufferCount < OUT_ATTR_INDEX_MAX);

                                outmap_buffer[outMapBufferCount].type =
                                    outmapInfo[g].type;
                                outmap_buffer[outMapBufferCount].index =
                                    outMapBufferCount;
                                outmap_buffer[outMapBufferCount].mask =
                                    outmapInfo[g].mask;

                                gs_copy_mask |= 1 << g;
                                vs_copy_mask |= 1 << v;
                                ++outMapBufferCount;
                            }
                        }
                    }
                }
            }

            for (s32 g = 0; g < exe_info->outMapCount; ++g)
            {
                if (!(gs_copy_mask & (1 << g)) &&
                    outmapInfo[g].type >= 0 &&
                    outmapInfo[g].type < 9 &&
                    outmapInfo[g].type != 7)
                {
                    NN_ASSERT_(outMapBufferCount < OUT_ATTR_BUFFER_MAX);

                    outmap_buffer[outMapBufferCount].type = outmapInfo[g].type;
                    outmap_buffer[outMapBufferCount].index = outMapBufferCount;
                    outmap_buffer[outMapBufferCount].mask = outmapInfo[g].mask;
                    ++outMapBufferCount;
                }
            }

            for (s32 v = 0; v < vtx_exe_info->outMapCount; ++v)
            {
                if (!(vs_copy_mask & (1 << v)) &&
                    vtxOutmapInfo[v].type >= 0 &&
                    vtxOutmapInfo[v].type < 9 &&
                    vtxOutmapInfo[v].type != 7)
                {
                    NN_ASSERT_(outMapBufferCount < OUT_ATTR_BUFFER_MAX);

                    outmap_buffer[outMapBufferCount].type = vtxOutmapInfo[v].type;
                    outmap_buffer[outMapBufferCount].index = outMapBufferCount;
                    outmap_buffer[outMapBufferCount].mask = vtxOutmapInfo[v].mask;
                    ++outMapBufferCount;
                }
            }
        }
        else
        {
            const OutmapInfo* outmapInfo =
                reinterpret_cast<const OutmapInfo*>(
                    reinterpret_cast<const u8*>(exe_info) + exe_info->outMapOffset);

            for (s32 i = 0; i < exe_info->outMapCount; ++i)
                outmap_buffer[i] = outmapInfo[i];

            outMapBufferCount = exe_info->outMapCount;
        }

        for (s32 index = 0; index < OUT_ATTR_INDEX_MAX; ++index)
        {
            attr[index] = 0x1f1f1f1f;

            for (s32 i = 0; i < outMapBufferCount; ++i)
            {
                bit32 c = 0;

                for (s32 j = 0;
                     outmap_buffer[i].index == index && j < OUT_ATTR_DIMENTION_MAX;
                     ++j)
                {
                    if ((outmap_buffer[i].mask & (1 << j)) == 0)
                        continue;

                    s32 value = 0x1f;

                    switch (outmap_buffer[i].type)
                    {
                    case 0:
                        value = 0x00 + c++;
                        if (c == 2)
                            clock |= 1 << 0;
                        break; // position

                    case 1:
                        value = 0x04 + c++;
                        clock |= 1 << 24;
                        break; // quaternion

                    case 2:
                        value = 0x08 + c++;
                        clock |= 1 << 1;
                        break; // color

                    case 3:
                        if (c < 2)
                            value = 0x0c + c++;
                        useTex = 1;
                        clock |= 1 << 8;
                        break; // texcoord0

                    case 4:
                        value = 0x10;
                        useTex = 1;
                        clock |= 1 << 16;
                        break; // texcoord0w

                    case 5:
                        if (c < 2)
                            value = 0x0e + c++;
                        useTex = 1;
                        clock |= 1 << 9;
                        break; // texcoord1

                    case 6:
                        if (c < 2)
                            value = 0x16 + c++;
                        useTex = 1;
                        clock |= 1 << 10;
                        break; // texcoord2

                    case 8:
                        if (c < 3)
                            value = 0x12 + c++;
                        clock |= 1 << 24;
                        break; // view
                    }

                    attr[index] =
                        attr[index] & ~(0xff << (j * 8)) | value << (j * 8);
                }
            }

            if (attr[index] != 0x1f1f1f1f)
            {
                outMask |= 1 << index;
                ++outNum;
            }
        }
    }

    if (is_geometry_shader)
    {
        u32 vtxOutNum = 0;
        bit32 vtxOutMask = 0;
        bit32 vtxAttr[VS_OUT_ATTR_INDEX_MAX];

        const ExeImageInfo* exe_info = m_ExeImageInfo[vtx_shader_index];
        const OutmapInfo* outmapInfo =
            reinterpret_cast<const OutmapInfo*>(
                reinterpret_cast<const u8*>(exe_info) + exe_info->outMapOffset);

        for (s32 index = 0; index < VS_OUT_ATTR_INDEX_MAX; ++index)
        {
            vtxAttr[index] = 0x1f1f1f1f;

            for (s32 i = 0; i < exe_info->outMapCount; ++i)
            {
                u32 c = 0;

                for (s32 j = 0;
                     outmapInfo[i].index == index && j < OUT_ATTR_DIMENTION_MAX;
                     ++j)
                {
                    if ((outmapInfo[i].mask & (1 << j)) == 0)
                        continue;

                    s32 value = 0x1f;

                    switch (outmapInfo[i].type)
                    {
                    case 0:
                        value = 0x00 + c++;
                        break; // position

                    case 1:
                        value = 0x04 + c++;
                        break; // quaternion

                    case 2:
                        value = 0x08 + c++;
                        break; // color

                    case 3:
                        if (c < 2)
                            value = 0x0c + c++;
                        break; // texcoord0

                    case 4:
                        value = 0x10;
                        break; // texcoord0w

                    case 5:
                        if (c < 2)
                            value = 0x0e + c++;
                        break; // texcoord1

                    case 6:
                        if (c < 2)
                            value = 0x16 + c++;
                        break; // texcoord2

                    case 8:
                        if (c < 3)
                            value = 0x12 + c++;
                        break; // view

                    case 9:
                        value = 0xff;
                        break;
                    }

                    vtxAttr[index] =
                        vtxAttr[index] & ~(0xff << (j * 8)) | value << (j * 8);
                }
            }

            if (vtxAttr[index] != 0x1f1f1f1f)
            {
                vtxOutMask |= 1 << index;
                ++vtxOutNum;
            }
        }

        bit32 gsDataMode = m_ExeImageInfo[geo_shader_index]->gsDataMode;

        if (gsDataMode == 1)
            *command++ = 0x80000000;
        else
            *command++ = 0x00000000;

        *command++ = PICA_CMD_HEADER_SINGLE_BE(PICA_REG_DRAW_MODE0, 0xa);

        *command++ = 0x00000000;
        *command++ = PICA_CMD_HEADER_SINGLE_BE(PICA_REG_DRAW_MODE1, 0x3);

        *command++ = 0x08000000 |
                     (gsDataMode == 0 ? 0x0000 : 0x0100) |
                     vtxOutNum - 1;
        *command++ = PICA_CMD_HEADER_SINGLE_BE(PICA_REG_GS_ATTR_NUM, 0xb);

        *command++ = 0x7fff0000 | m_ExeImageInfo[geo_shader_index]->mainAddress;
        *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_GS_START_ADDR);

        *command++ = outMask;
        *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_GS_OUT_REG_MASK);

        *command++ = 0x7fff0000 | m_ExeImageInfo[vtx_shader_index]->mainAddress;
        *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_VS_START_ADDR);

        *command++ = vtxOutMask;
        *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_VS_OUT_REG_MASK);

        *command++ = vtxOutNum - 1;
        *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_VS_OUT_REG_NUM2);

        *command++ = 0x76543210;
        *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_GS_ATTR_IN_REG_MAP0);

        *command++ = 0xfedcba98;
        *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_GS_ATTR_IN_REG_MAP1);

        if (gsDataMode == 1 &&
            m_ExeImageInfo[geo_shader_index]->gsPatchSize != 0)
        {
            *command++ = m_ExeImageInfo[geo_shader_index]->gsPatchSize - 1;
            *command++ = PICA_CMD_HEADER_SINGLE_BE(PICA_REG_GS_MISC_REG1, 0x1);
        }

        if (gsDataMode == 2)
        {
            gsDataMode |= 0x01 << 24;
            gsDataMode |= m_ExeImageInfo[geo_shader_index]->gsVertexStartIndex << 16;
            gsDataMode |= (vtxOutNum - 1) << 12;
            gsDataMode |= (m_ExeImageInfo[geo_shader_index]->gsVertexNum - 1) << 8;
        }

        *command++ = gsDataMode;
        *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_GS_MISC_REG0);

        *command++ = vtxOutNum - 1;
        *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_VS_OUT_REG_NUM1);
    }
    else
    {
        *command++ = 0x0;
        *command++ = PICA_CMD_HEADER_SINGLE_BE(PICA_REG_DRAW_MODE0, 0x8);

        *command++ = 0x0;
        *command++ = PICA_CMD_HEADER_SINGLE_BE(PICA_REG_DRAW_MODE1, 0x1);

        *command++ = 0xa0000000;
        *command++ = PICA_CMD_HEADER_SINGLE_BE(PICA_REG_GS_ATTR_NUM, 0xb);

        *command++ = 0x7fff0000 | m_ExeImageInfo[vtx_shader_index]->mainAddress;
        *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_VS_START_ADDR);

        *command++ = outMask;
        *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_VS_OUT_REG_MASK);

        *command++ = outNum - 1;
        *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_VS_OUT_REG_NUM2);

        *command++ = 0;
        *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_GS_MISC_REG0);

        *command++ = outNum - 1;
        *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_VS_OUT_REG_NUM1);
    }

    {
        *command++ = outNum - 1;
        *command++ = PICA_CMD_HEADER_SINGLE_BE(PICA_REG_VS_OUT_REG_NUM3, 0x1);

        *command++ = outNum;
        *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_VS_OUT_REG_NUM0);

        outNum = 0;

        for (s32 index = 0; index < OUT_ATTR_INDEX_MAX; ++index)
        {
            if (attr[index] != 0x1f1f1f1f)
            {
                *command++ = attr[index];
                *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_VS_OUT_ATTR0 + outNum);
                ++outNum;
            }
        }

        for (s32 index = outNum; index < OUT_ATTR_INDEX_MAX; ++index)
        {
            *command++ = attr[index];
            *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_VS_OUT_ATTR0 + index);
        }
    }

    *command++ = useTex;
    *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_VS_OUT_ATTR_MODE);

    *command++ = clock;
    *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_VS_OUT_ATTR_CLK);

    if (is_geometry_shader)
    {
        *command++ = 0;
        *command++ = PICA_CMD_HEADER_SINGLE_BE(PICA_REG_VS_OUT_REG_NUM3, 8);
    }

    return command;
}

bit32* Shader::MakeDummyCommand_(bit32* command, const bit32 load_reg, const u32 dataNum)
{
    *command++ = 0;
    *command++ = PICA_CMD_HEADER_BURST_BE(load_reg, dataNum, 0);

    for (s32 i = 0; i < dataNum - (dataNum & 1); ++i)
    {
        *command++ = PADDING_DATA;
    }

    return command;
}

void Shader::MakeShaderConstCommandCache_()
{
    for (s32 shader_index = 0; shader_index < m_ExeImageInfoNum; shader_index++)
    {
        m_CmdCacheConstNumArray[shader_index] = this->MakeConstRgCommand_(m_CmdCacheConstArray[shader_index], shader_index) - m_CmdCacheConstArray[shader_index];
        NN_ASSERT_(m_CmdCacheConstNumArray[shader_index] <= CONST_REG_COMMAND_MAX);
    }
}

void Shader::MakeShaderOutAttrCommandCache_()
{
    m_CmdCacheOutAttrNum = this->MakeOutAttrCommand_(m_CmdCacheOutAttrArray, m_VtxShaderIndex, m_GeoShaderIndex)- m_CmdCacheOutAttrArray;
}

bit32* Shader::MakeFullCommand(bit32* command) const
{
    {
        command = MakePrepareCommand(command);
    }

    if (this->IsEnableGeoShader())
    {                     
        command = this->MakeGeoProgramCommand(command);
        command = this->MakeGeoSwizzleCommand(command);
        command = this->MakeGeoConstRgCommand(command);
        command = this->MakeGeoBoolMapCommand(command);
    }            

    {
        command = this->MakeVtxProgramCommand(command);
        command = this->MakeVtxSwizzleCommand(command);
        command = this->MakeVtxConstRgCommand(command);
        command = this->MakeVtxBoolMapCommand(command);
    }

    {
        command = this->MakeOutAttrCommand(command);
    }
                
    return command;
}

bit32* Shader::MakeDisableCommand(bit32* command)
{
    const bool isEnableGeometryShader = false;
    const PicaDataDrawMode drawMode = PICA_DATA_DRAW_TRIANGLES;

    command = MakeShaderModeCommand_(command, isEnableGeometryShader, drawMode);

    return command;
}

bit32* Shader::MakeShaderCommand(bit32* command, const bool isMakePrepareCommand) const
{
    if (isMakePrepareCommand)
    {
        command = this->MakePrepareCommand(command);
    }

    if (this->IsEnableGeoShader())
    {
        command = this->MakeGeoConstRgCommand(command);
        command = this->MakeGeoBoolMapCommand(command);
    }

    {
        command = this->MakeVtxConstRgCommand(command);
        command = this->MakeVtxBoolMapCommand(command);
    }

    {
        command = this->MakeOutAttrCommand(command);
    }

    return command;
}

bit32* Shader::MakePrepareCommand(bit32* command) const
{
    bool isEnableGeoShader = this->IsEnableGeoShader();
    PicaDataDrawMode drawMode = m_DrawMode;

    command = this->MakeShaderModeCommand_(command, isEnableGeoShader, drawMode);

    return command;
}

bit32* Shader::MakeVtxProgramCommand(bit32* command) const{
    s32 shader_index   = this->GetVtxShaderIndex();
    bit32 reg_addr     = PICA_REG_VS_PROG_ADDR;
    bit32 reg_load     = PICA_REG_VS_PROG_DATA0;
    bit32 reg_end      = PICA_REG_VS_PROG_UPDATE_END;

    {
        *command++ = 0;
        *command++ = PICA_CMD_HEADER_SINGLE(reg_addr);
    }

    {
        NN_ASSERT_(0 <= shader_index && shader_index < m_ExeImageInfoNum);
                    
        const ExeImageInfo* exe_info = m_ExeImageInfo[shader_index];

        u32 instructionCount = m_InstructionCount;
        if (instructionCount > 512)
        {
            instructionCount = 512;
        }

        command = this->MakeLoadCommand_(command, reg_load, m_Instruction, m_InstructionCount < 512 ? m_InstructionCount : 512);
    }

    {
        *command++ = 1;
        *command++ = PICA_CMD_HEADER_SINGLE(reg_end);
    }

    return command;
}

bit32* Shader::MakeGeoProgramCommand(bit32* command) const{
    s32 shader_index   = this->GetGeoShaderIndex();
    bit32 reg_addr     = PICA_REG_GS_PROG_ADDR;
    bit32 reg_load     = PICA_REG_GS_PROG_DATA0;
    bit32 reg_end      = PICA_REG_GS_PROG_UPDATE_END;

    {
        *command++ = 0;
        *command++ = PICA_CMD_HEADER_SINGLE(reg_addr);
    }

    {
        NN_ASSERT_((0 <= shader_index) && (shader_index < m_ExeImageInfoNum));
                        
        const ExeImageInfo* exe_info = m_ExeImageInfo[shader_index];

        NN_UNUSED_VAR(exe_info);

        command = this->MakeLoadCommand_(command, reg_load, m_Instruction, m_InstructionCount);
    }

    {
        *command++ = 1;
        *command++ = PICA_CMD_HEADER_SINGLE( reg_end );
    }

    return command;
}

bit32* Shader::MakeShaderModeCommand_(bit32* command, const bool isEnableGeoShader, const PicaDataDrawMode drawMode)
{
    { 
        if (isEnableGeoShader)
        {
            *command++ = PICA_DATA_DRAW_GEOMETRY_PRIMITIVE << 8;
        }
        else
        {
            *command++ = drawMode << 8;
        }

        *command++ = PICA_CMD_HEADER_SINGLE_BE( PICA_REG_VS_OUT_REG_NUM3, 2 );
    }

    {
        command = MakeDummyCommand_(command, PICA_REG_VS_OUT_REG_NUM2, DUMMY_DATA_NUM_251);
    }

    {
        command = MakeDummyCommand_(command, PICA_REG_VERTEX_ATTR_ARRAYS_BASE_ADDR, DUMMY_DATA_NUM_200);
    }

    {
        *command++ = isEnableGeoShader ? 2 : 0;
        *command++ = PICA_CMD_HEADER_SINGLE_BE(PICA_REG_DRAW_MODE0, 1);
    }

    {
        command = MakeDummyCommand_(command, PICA_REG_VERTEX_ATTR_ARRAYS_BASE_ADDR, DUMMY_DATA_NUM_200);
    }

    { 
        *command++ = isEnableGeoShader ? 1 : 0;
        *command++ = PICA_CMD_HEADER_SINGLE_BE(PICA_REG_VS_COM_MODE, 1);
    }

    return command;
}

void Shader::CheckVtxShaderIndex_(const s32 vtx_shader_index)
{
    NN_ASSERT_((0 <= vtx_shader_index) && (vtx_shader_index < this->GetShaderNum()));
    NN_ASSERT_(!m_ExeImageInfo[vtx_shader_index]->isGeoShader);
}

void Shader::CheckGeoShaderIndex_(const s32 geo_shader_index)
{
    NN_UNUSED_VAR(geo_shader_index);

    NN_ASSERT_(m_GeoShaderIndex < GetShaderNum());

    if (geo_shader_index > - 1)
    {
        NN_ASSERT_(m_ExeImageInfo[geo_shader_index]->isGeoShader);
    }
}

bit32* Shader::MakeConstRgCommand_(bit32* command, const s32 shader_index)
{
    bit32  reg_float     = PICA_REG_VS_FLOAT_ADDR;
    bit32  reg_integer   = PICA_REG_VS_INT0;
    bit32* boolMap       = &m_VtxShaderBoolMapUniform;

    bool is_geometry_shader = m_ExeImageInfo[shader_index]->isGeoShader;
    if (is_geometry_shader)
    {
        reg_float = PICA_REG_GS_FLOAT_ADDR;
        reg_integer = PICA_REG_GS_INT0;
        boolMap = &m_GeoShaderBoolMapUniform;
    }

    NN_ASSERT_((0 <= shader_index) && (shader_index < m_ExeImageInfoNum));
    const ExeImageInfo* exe_info = m_ExeImageInfo[shader_index];

    struct SetupInfo
    {
        u16 type;
        u16 index;
        bit32 value[4];
    };

    const SetupInfo* setupInfo = reinterpret_cast<const SetupInfo*>(reinterpret_cast<const u8*>(exe_info) + exe_info->setupOffset);

    for (int i = 0; i < exe_info->setupCount; ++i)
    {
        const SetupInfo& info = setupInfo[i];
        const bit32* value = info.value;

        switch (info.type)
        {
        case 0:
            *boolMap |= (info.value[0] << info.index) & (1 << info.index);
            break;
        case 1:
            *command++ = value[0] | value[1] <<  8 | value[2] << 16 | value[3] << 24;
            *command++ = PICA_CMD_HEADER_SINGLE(reg_integer + info.index);
             break;
        case 2:
            *command++ = info.index;
            *command++ = PICA_CMD_HEADER_BURSTSEQ(reg_float, 4);
            *command++ = (value[3] <<  8 & 0xffffff00) | (value[2] >> 16 & 0x000000ff);
            *command++ = (value[2] << 16 & 0xffff0000) | (value[1] >>  8 & 0x0000ffff);
            *command++ = (value[1] << 24 & 0xff000000) | (value[0] >>  0 & 0x00ffffff);
            *command++ = PADDING_DATA;
            break;
        }
    }

    return command;
}
        
}
}
}
