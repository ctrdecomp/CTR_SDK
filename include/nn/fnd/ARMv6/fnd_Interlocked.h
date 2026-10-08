#pragma once

#include <nn/util/util_TypeTraits.h>
#include <nn/Result.h>

namespace nn {
namespace fnd {
namespace ARMv6 {
namespace detail {

template <typename T>
struct LoadStoreRegEx{
        static T LoadRegEx (volatile T* ptr)
        {
                return (T)__ldrex (ptr);
        }
        static int StoreRegEx (T val, volatile T* ptr)
        {
                return __strex (val, ptr);
        }
};

template <>
struct LoadStoreRegEx<s64>{
        static s64 LoadRegEx (volatile s64* ptr)
        {
                return __ldrexd (ptr);
        }
        static int StoreRegEx (s64 val, volatile s64* ptr)
        {
                return __strexd (val, ptr);
        }
};

} // namespace detail

class Interlocked{
        template <typename T, size_t Size = sizeof (T)>
        struct AtomicStorageSelecter;

        template <typename T>
        struct AtomicStorageSelecter<T, 8>{
                typedef s64 Type;
        };

        template <typename T>
        struct AtomicStorageSelecter<T, 4>{
                typedef s32 Type;
        };

        template <typename T>
        struct AtomicStorageSelecter<T, 2>{
                typedef s16 Type;
        };

        template <typename T>
        struct AtomicStorageSelecter<T, 1>{
                typedef s8 Type;
        };

public:
        template <typename T, typename UpdateFunc>
        static bool AtomicUpdate (volatile T* p,UpdateFunc& update,typename nn::util::enable_if<sizeof (T) <= 4>::type* = 0){

                typedef typename AtomicStorageSelecter<T>::Type StorageType;
                typedef detail::LoadStoreRegEx<StorageType>     LoadStoreType;

                union U {
                        StorageType raw;
                        T           v;
                } u;

                while (true) {
                        u.raw = LoadStoreType::LoadRegEx ((volatile StorageType*&)(p));

                        if (!update (u.v)) {
                                __clrex ();
                                return false;
                        }

                        if (LoadStoreType::StoreRegEx (u.raw, (volatile StorageType*&)(p)) == 0) {
                                return true;
                        }
                }
        }
};

} // namespace ARMv6
} // namespace fnd
} // namespace nn
