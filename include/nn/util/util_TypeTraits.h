#pragma once

#include <nn/types.h>

namespace nn { 
namespace util {

template <class T, T v>
struct integral_constant
{
    typedef integral_constant<T, v> type;
    typedef T                       value_type;
    static const value_type         value = v;
};

typedef integral_constant<bool, true>  true_type;
typedef integral_constant<bool, false> false_type;

template <class>
struct is_void : public false_type {};

template <>
struct is_void<void> : public true_type {};

namespace detail {
    template <class T>
    class AlignmentHack 
    {
        char c;
        T x;
    };
}

template <class T>
struct alignment_of : public integral_constant<size_t, __alignof__(T)> {};

namespace detail {
    template <size_t> struct AlignmentType {};
    template <> struct AlignmentType<1> { typedef u8  type; };
    template <> struct AlignmentType<2> { typedef u16 type; };
    template <> struct AlignmentType<4> { typedef u32 type; };
    template <> struct AlignmentType<8> { typedef u64 type; };
}

template <size_t Size, size_t Align>
struct aligned_storage
{
private:
    union UnionType
    {
        char c[Size];
        typename detail::AlignmentType<Align>::type a;
    };
public:
    typedef UnionType type;
};

template <bool, class T = void> struct enable_if {};
template <class T> struct enable_if<true, T> { typedef T type; };

template <class From, class To>
struct is_convertible
{
private:
    typedef char T1;
    struct T2 { char dummy[2]; };
    static T1   IsConvertibleTest(To);
    static T2   IsConvertibleTest(...);
    static From MakeFrom();
public:
    typedef bool value_type;
    static const bool value = sizeof(IsConvertibleTest(MakeFrom())) == sizeof(T1);
};

template <class T, class U> struct is_same              : public false_type {};
template <class T>          struct is_same<T, T>        : public true_type  {};

template <class Base, class Derived>
struct is_base_of : public integral_constant<bool,
    is_convertible<const Derived*, const Base*>::value &&
    !is_same<const Base*, const void*>::value> {};

}} // namespace nn::util

