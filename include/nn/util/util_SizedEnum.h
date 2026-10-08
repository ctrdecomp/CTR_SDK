#pragma once

namespace nn { 
namespace util {

template <typename StorageT, typename EnumT>
class SizedEnum
{
private:
    typedef SizedEnum<StorageT, EnumT> Self;
private:
    StorageT        m_EnumValue;
public:
    SizedEnum() {}
    SizedEnum(EnumT e) { Set(e); }

    operator EnumT() const { return Get(); }
    Self& operator =(EnumT e) { Set(e); return *this; }
    void Set(EnumT e) { m_EnumValue = static_cast<EnumT>(e); }
    EnumT Get() const { return static_cast<EnumT>(m_EnumValue); }
};

template <typename EnumT>
class SizedEnum1 : public SizedEnum<bit8, EnumT>
{
public:
    SizedEnum1() : SizedEnum<bit8, EnumT>() {}
    SizedEnum1(EnumT e) : SizedEnum<bit8, EnumT>(e) {}
};

template <typename EnumT>
class SizedEnum2 : public SizedEnum<bit16, EnumT>
{
public:
    SizedEnum2() : SizedEnum<bit16, EnumT>() {}
    SizedEnum2(EnumT e) : SizedEnum<bit16, EnumT>(e) {}
};

template <typename EnumT>
class SizedEnum4 : public SizedEnum<bit32, EnumT>
{
public:
    SizedEnum4() : SizedEnum<bit32, EnumT>() {}
    SizedEnum4(EnumT e) : SizedEnum<bit32, EnumT>(e) {}
};

}
}
