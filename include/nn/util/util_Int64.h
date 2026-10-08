#pragma once

namespace nn {
namespace util {

template <typename Base, typename Tag = void>
struct Int64
{
    bit32 lo;
    bit32 hi;
    Int64() {}
    Int64(Base s) : lo(static_cast<bit32>(s)), hi(static_cast<bit32>(s >> 32))  {}
    operator Base() const { return (static_cast<Base>(hi) << 32) | static_cast<Base>(lo); }
    Int64& operator++() { Base lhs = *this; lhs++; *this = lhs; return *this; }
    Int64& operator--() { Base lhs = *this; lhs--; *this = lhs; return *this; }
    Int64& operator+=(Base rhs) { Base lhs = *this; lhs += rhs; *this = lhs; return *this; }
    Int64& operator-=(Base rhs) { Base lhs = *this; lhs -= rhs; *this = lhs; return *this; }
    Int64& operator*=(Base rhs) { Base lhs = *this; lhs *= rhs; *this = lhs; return *this; }
    Int64& operator/=(Base rhs) { Base lhs = *this; lhs /= rhs; *this = lhs; return *this; }
    Int64& operator%=(Base rhs) { Base lhs = *this; lhs %= rhs; *this = lhs; return *this; }
    Int64& operator|=(Base rhs) { Base lhs = *this; lhs |= rhs; *this = lhs; return *this; }
    Int64& operator&=(Base rhs) { Base lhs = *this; lhs &= rhs; *this = lhs; return *this; }
    Int64& operator^=(Base rhs) { Base lhs = *this; lhs ^= rhs; *this = lhs; return *this; }
    Int64& operator<<=(int rhs) { Base lhs = *this; lhs <<= rhs; *this = lhs; return *this; }
    Int64& operator>>=(int rhs) { Base lhs = *this; lhs >>= rhs; *this = lhs; return *this; }
    Int64 operator++(int) { Base lhs = *this; *this = lhs + 1; return Int64(lhs); }
    Int64 operator--(int) { Base lhs = *this; *this = lhs - 1; return Int64(lhs); }
    
    Int64(const Int64<Base, void>& other) : lo(other.lo), hi(other.hi) {}
#pragma push
#pragma diag_suppress 554
    operator Int64<Base, void>() const { return static_cast<Base>(*this); }
#pragma pop
    
    bool operator==(Int64 rhs) const { return this->hi == rhs.hi && this->lo == rhs.lo; }
    bool operator!=(Int64 rhs) const { return !(*this == rhs); }
    friend bool operator==(Int64 lhs, Base rhs) { return static_cast<Base>(lhs) == rhs; }
    friend bool operator!=(Int64 lhs, Base rhs) { return !(lhs == rhs); }
    friend bool operator==(Base lhs, Int64 rhs) { return lhs == static_cast<Base>(rhs); }
    friend bool operator!=(Base lhs, Int64 rhs) { return !(lhs == rhs); }
    
    bool operator<(Int64 rhs) const { return static_cast<Base>(*this) < static_cast<Base>(rhs); }
    bool operator>(Int64 rhs) const { return rhs < *this; }
    bool operator<(Base rhs) const { return static_cast<Base>(*this) < rhs; }
    bool operator>(Base rhs) const { return rhs < *this; }
    bool operator<=(Int64 rhs) const { return !(*this > rhs); }
    bool operator>=(Int64 rhs) const { return !(*this < rhs); }
    bool operator<=(Base rhs) const { return !(static_cast<Base>(*this) > rhs); }
    bool operator>=(Base rhs) const { return !(static_cast<Base>(*this) < rhs); }
};

}
}
