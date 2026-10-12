// Filename: cfg_Default.cpp
//
// Project: Horizon

#include <nn/cfg/CTR/detail/cfg_Default.h>
#include <nn/cfg/CTR/detail/cfg_DataStructures.h>

#include <nn/CTR.h>

namespace nn {
namespace cfg {
namespace CTR {
namespace detail {

const MenuInfoCfgData MENU_INFO_CFG_DEFAULT =
{
    nn::CTR::MakeProgramId(48, 130, 2)
};
const MenuInfoCfgData MENU_INFO_CFG_DEFAULT_US =
{
    nn::CTR::MakeProgramId(48, 143, 2)
};

const MenuInfoCfgData MENU_INFO_CFG_DEFAULT_EU =
{
    nn::CTR::MakeProgramId(48, 152, 2)
};

const MenuInfoCfgData MENU_INFO_CFG_DEFAULT_CN =
{
    nn::CTR::MakeProgramId(48, 161, 2)
};

const MenuInfoCfgData MENU_INFO_CFG_DEFAULT_KR =
{
    nn::CTR::MakeProgramId(48, 169, 2)
};

const MenuInfoCfgData MENU_INFO_CFG_DEFAULT_TW =
{
    nn::CTR::MakeProgramId(48, 177, 2)
};

const nn::cfg::CTR::Birthday BIRTHDAY_CFG_DEFAULT =
{
    1, 1
};

const nn::cfg::CTR::LanguageCfgData LANGUAGE_CFG_DEFAULT =
{
    nn::cfg::CTR::CFG_LANGUAGE_JAPANESE
};

}
}
}
}

