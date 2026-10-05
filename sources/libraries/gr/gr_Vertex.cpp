// Filename: gr_Vertex.cpp
//
// Project: Horizon

#include <nn/gr/CTR/gr_Vertex.h>
#include <nn/gx/CTR/gx_Vram.h>
#include <nn/gx/CTR/gx_CommandAccess.h>

namespace nn {
namespace gr {
namespace CTR {

const uptr scBaseAddr = nngxGetPhysicalAddr(nngxGetVramStartAddr(nn::gx::CTR::MEM_VRAMA)) >> 3;

void Vertex::LoadArray::CheckDisable()
{
    if (!IsEnable())
    {
        return;
    }

    for (u32 index = 0; index < VERTEX_ATTRIBUTE_MAX; ++index)
    {
        if (bind[index] != -1)
        {
            return;
        }
    }

    physicalAddr = NULL;

    for (u32 index = 0; index < VERTEX_ATTRIBUTE_MAX; ++index)
    {
        byte[index] = 0;
    }
}

void Vertex::LoadArray::DisableAll()
{
    physicalAddr = NULL;

    for (u32 index = 0; index < VERTEX_ATTRIBUTE_MAX; index++)
    {

        type[index] = PICA_DATA_SIZE_1_BYTE;
        byte[index] = 0;
        bind[index] = -1;
    }
}

void Vertex::DisableAttr_(const bit32 bind_reg){    
    NN_ASSERT_(bind_reg < VERTEX_ATTRIBUTE_MAX);

    if (!m_IsEnableReg[bind_reg]) return;

    m_CmdCacheVertexNum = 0;
                
    if (m_AttrConst[bind_reg].IsEnable())
    {
        m_AttrConst[bind_reg].Disable();

        m_IsEnableReg[bind_reg] = false;
        return;
    }

    for (int i = 0; i < VERTEX_ATTRIBUTE_MAX; ++i)
    {
        if (!m_LoadArray[i].IsEnable()) continue;

        for (int j = 0; j < VERTEX_ATTRIBUTE_MAX; ++j)
        {
            if (m_LoadArray[i].bind[j] != bind_reg) continue;
                        
            m_LoadArray[i].bind[j] = -1;                       
            m_IsEnableReg[bind_reg] = false;

            m_LoadArray[i].CheckDisable();
                        
            return;
        }
    }

    NN_ASSERT_(m_IsEnableReg[bind_reg] == false);
}

void Vertex::EnableAttrAsArray(const BindSymbolVSInput& symbol, const uptr physical_addr, const PicaDataVertexAttrType type )
{
    const bit32 bind_reg = symbol.start;
    const u32 byte     = PicaDataVertexAttrTypeToByteSize(type);

    NN_ASSERT_(bind_reg < VERTEX_ATTRIBUTE_MAX);

    DisableAttr_(bind_reg);

    LoadArray* array = NULL;
    for (int i = 0; i < VERTEX_ATTRIBUTE_MAX; ++i)
    {
        if (m_LoadArray[i].IsEnable() == false)
        {
            array = &m_LoadArray[i];
            break;
        }
    }
    NN_ASSERT_(array != NULL);

    m_IsEnableReg[bind_reg] = true;
    array->physicalAddr     = physical_addr;
    array->type[0]          = type;
    array->bind[0]          = bind_reg;
    array->byte[0]          = byte;

    for (int i = 1; i < VERTEX_ATTRIBUTE_MAX; ++i)
    {
        array->bind[i] = -1;
        array->byte[i] = 0;
    }

    m_CmdCacheVertexNum = 0;
}

bit32* Vertex::MakeDrawCommand(bit32* command, const IndexStream& index_stream) const
{
    bit32 fmt = index_stream.isUnsignedByte ? 0x00000000 : 0x80000000;
    uptr addr = index_stream.physicalAddr - scBaseAddr * 8;

    *command++ = 0x1;
    *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_START_DRAW_FUNC1);

    *command++ = fmt | addr;
    *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_INDEX_ARRAY_ADDR_OFFSET);

    *command++ = index_stream.drawVtxNum;
    *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_DRAW_VERTEX_NUM);

    *command++ = 0x00000000;
    *command++ = PICA_CMD_HEADER_SINGLE_BE(PICA_REG_START_DRAW_FUNC0, 0x1);

    *command++ = 0x00000001;
    *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_START_DRAW_ELEMENT);

    *command++ = 0x00000001;
    *command++ = PICA_CMD_HEADER_SINGLE_BE(PICA_REG_START_DRAW_FUNC0, 0x1);

    *command++ = 0x00000001;
    *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_VERTEX_FUNC);

    for (u32 index = 0; index < 2; index++)
    {
        *command++ = 0x0;
        *command++ = PICA_CMD_HEADER_SINGLE_BE(PICA_REG_DRAW_MODE2, 0x8);
    }

    return command;
}

bit32* Vertex::MakeEnableAttrCommand_(bit32* command) const
{
#if defined(NN_GR_VERTEX_DUMP)
    bit32* start = command;
#endif

    bit32* reg0x2b9 = command++;
    *command++ = 0x000b02b9;

    bit32* reg0x242 = command++;
    *command++ = 0x00010242;

    bit32* bind_reg_command = command;

    *command++ = 0;
    *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_VS_ATTR_IN_REG_MAP0);

    *command++ = 0;
    *command++ = PICA_CMD_HEADER_SINGLE(PICA_REG_VS_ATTR_IN_REG_MAP1);

    bit32* type = &command[2];
    bit32& fixed_attr_mask = command[3];

    *command++ = scBaseAddr;
    bit32* headerBaseAddr = command++;

    *command++ = 0;

    bit32* reg0x202 = command++;

    int input_index = 0;
    int array_num = 0;

    for (int i = 0; i < VERTEX_ATTRIBUTE_MAX; ++i)
    {
        const LoadArray& vtx_array = m_LoadArray[i];

        if (!vtx_array.IsEnable())
            continue;

        ++array_num;

        int total_byte = 0;
        int elem_num = 0;
        int elem[2] = {0, 0};

        for (int j = 0; j < VERTEX_ATTRIBUTE_MAX; ++j)
        {
            if (vtx_array.byte[j] == 0)
                break;

            total_byte += vtx_array.byte[j];

            if (vtx_array.bind[j] >= 0)
            {
                elem[j / 8] |= input_index << (4 * (j % 8));
                type[input_index / 8] |=
                    vtx_array.type[j] << (4 * (input_index % 8));

                bind_reg_command[(input_index >> 3) << 1] &=
                    ~(0xf << (4 * (input_index % 8)));

                bind_reg_command[(input_index >> 3) << 1] |=
                    vtx_array.bind[j] << (4 * (input_index % 8));

                ++input_index;

#if defined(NN_BUILD_DEBUG)
                NN_LOG("+ 0x%08x 0x%08x 0x%08x\n",
                       vtx_array.byte[j], elem[0], elem[1]);
#endif
            }
            else
            {
                elem[j / 8] |=
                    ((vtx_array.byte[j] >> 2) + 0xb) <<
                    (4 * (j % 8));

#if defined(NN_BUILD_DEBUG)
                NN_LOG("- 0x%08x 0x%08x 0x%08x\n",
                       vtx_array.byte[j] >> 2, elem[0], elem[1]);
#endif
            }

            ++elem_num;
        }

        *command++ = vtx_array.physicalAddr - scBaseAddr * 8;
        *command++ = elem[0];
        *command++ = elem[1] | total_byte << 16 | elem_num << 28;
    }

    *headerBaseAddr =
        PICA_CMD_HEADER_BURSTSEQ(
            PICA_REG_VERTEX_ATTR_ARRAYS_BASE_ADDR,
            3 + 3 * array_num
        );

    if (array_num % 2)
        *command++ = 0;

    for (int bind_reg = VERTEX_ATTRIBUTE_MAX - 1; bind_reg >= 0; --bind_reg)
    {
        const AttrConst& vtxConst = m_AttrConst[bind_reg];

        if (!vtxConst.IsEnable())
            continue;

        *command++ = input_index;
        *command++ = PICA_CMD_HEADER_BURSTSEQ(PICA_REG_VS_FIXED_ATTR, 4);

        *command++ =
            ((Float32ToFloat24(vtxConst.param[3]) << 8) & 0xffffff00) |
            ((Float32ToFloat24(vtxConst.param[2]) >> 16) & 0x000000ff);

        *command++ =
            ((Float32ToFloat24(vtxConst.param[2]) << 16) & 0xffff0000) |
            ((Float32ToFloat24(vtxConst.param[1]) >> 8) & 0x0000ffff);

        *command++ =
            ((Float32ToFloat24(vtxConst.param[1]) << 24) & 0xff000000) |
            ((Float32ToFloat24(vtxConst.param[0]) >> 0) & 0x00ffffff);

        *command++ = 0;

        bind_reg_command[(input_index / 8) * 2] &=
            ~(0xf << (4 * (input_index % 8)));

        bind_reg_command[(input_index / 8) * 2] |=
            bind_reg << (4 * (input_index % 8));

        fixed_attr_mask |= 1 << (input_index + 16);

        ++input_index;
    }

    *reg0x2b9 = (input_index - 1) | 0xa0000000;
    *reg0x242 = input_index - 1;
    *reg0x202 |= (input_index - 1) << 28;

#if defined(NN_BUILD_DEBUG)
    static int a = 0;

    if (++a == 1)
    {
        for (bit32* i = start; i != command; i += 2)
        {
            NN_LOG("0x%08x 0x%08x\n", *i, *(i + 1));
        }
    }
#endif

    return command;
}

}
}
}