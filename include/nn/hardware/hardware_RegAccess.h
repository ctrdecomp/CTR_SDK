#pragma once

//! @brief MRC to access the ARM Processors Registers.

#define HW_INST_MRC(a,b,c,d,e,v)    __asm { mrc a,b,v,c,d,e }

#define HW_GET_CP15_THREAD_ID_USER_READ_ONLY(v)         HW_INST_MRC(p15,0,c13,c0,3,v)
