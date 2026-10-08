#pragma once

#include <nn/types.h>

namespace nn {
namespace util {

template <typename T, typename V>
class FlagsEnum
{
private:
    T m_FlagsValue;
public:
    FlagsEnum () {}
    FlagsEnum (T e): 
        m_FlagsValue (e)
    {
    }
    FlagsEnum (V e): 
        m_FlagsValue (e)
    {}

    T    Get () const { return this->m_FlagsValue; }
    void Set (T e) const { this->m_FlagsValue = e; }

    operator T () { return Get (); }
};

template <typename V>
struct FlagsEnum1 : public FlagsEnum<bit8, V>
{
    FlagsEnum1 () : FlagsEnum<bit8, V> () {}
    FlagsEnum1 (V e) : FlagsEnum<bit8, V> (e) {}
};

template <typename V>
struct FlagsEnum2 : public FlagsEnum<bit16, V>
{
    FlagsEnum2 () : FlagsEnum<bit16, V> () {}
    FlagsEnum2 (V e) : FlagsEnum<bit16, V> (e) {}
};

template <typename V>
struct FlagsEnum4 : public FlagsEnum<bit32, V>
{
    FlagsEnum4 () : FlagsEnum<bit32, V> () {}
    FlagsEnum4 (V e) : FlagsEnum<bit32, V> (e) {}
};

}
}
