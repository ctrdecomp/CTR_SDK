#pragma once

#include <nn/types.h>
#include <nn/cfg/CTR/cfg_LanguageCode.h>
#include <nn/cfg/CTR/cfg_UserInfo.h>
#include <nn/cfg/CTR/cfg_RegionCode.h>

namespace nn {
namespace cfg {
namespace CTR {

const char PORT_NAME_USER[] = "cfg:u";
const char PORT_NAME_INIT[] = "cfg:i";
const char PORT_NAME_SYSTEM[] = "cfg:s";
    
void Initialize();
void Finalize();
void GetBirthday(Birthday* pBirthday);
CfgRegionCode GetRegion();
CfgLanguageCode GetLanguage();
bit64 GetTransferableId(bit32 uniqueId);

struct LanguageCfgData
{
    u8 code;
};

extern LanguageCfgData LANGUAGE_CFG_DEFAULT;

struct DebugParamCfgData
{

    bool dlpDebug;
    union debugParam
    {
        u8 flags1;
        u8 param1;
    } param;
    u8 fsLatencyParam;
    u8 rsv;
};

enum ConfigKeyData
{
    CFG_KEY_STEREO_CAMERA = 0x50005,
    CFG_KEY_USER_SYSTEM_VOLUME = 0x70001,
    CFG_KEY_USER_BIRTHDAY = 0xa0001,
    CFG_KEY_USER_REGION = 0xa0002,
    CFG_KEY_USER_P2P_CEC  = 0xc0000,
    CFG_KEY_USER_EULA = 0xd0000,
    CFG_KEY_USER_DEBUG_DATA = 0x130000
};

}
}
}