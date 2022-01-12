/*****************************************************************************
*                           Klepsydra Streaming Modules
*              Copyright (C) 2020-2022  Klepsydra Technologies GmbH
*                            All Rights Reserved.
*
*  This file is subject to the terms and conditions defined in
*  file 'LICENSE.md', which is part of this source code package.
*
*  NOTICE:  All information contained herein is, and remains the property of Klepsydra
*  Technologies GmbH and its suppliers, if any. The intellectual and technical concepts
*  contained herein are proprietary to Klepsydra Technologies GmbH and its suppliers and
*  may be covered by Swiss and Foreign Patents, patents in process, and are protected by
*  trade secret or copyright law. Dissemination of this information or reproduction of
*  this material is strictly forbidden unless prior written permission is obtained from
*  Klepsydra Technologies GmbH.
*
*****************************************************************************/

#pragma once

#include <cassert>
#include <stdlib.h>
#include <string>
#include <utility>

#ifdef _WIN32
#include <malloc.h>
#endif

#ifdef __MINGW32__
#include <mm_malloc.h>
#endif

namespace kpsr {
namespace streaming {

template<typename T, std::size_t alignment>
class AlignedAllocator
{
public:
    typedef T value_type;
    typedef T *pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T &reference;
    typedef const T &const_reference;
    typedef const T *const_pointer;

    template<typename U>
    struct rebind
    {
        typedef AlignedAllocator<U, alignment> other;
    };

    AlignedAllocator() {}

    template<typename U>
    AlignedAllocator(const AlignedAllocator<U, alignment> &)
    {}

    const_pointer address(const_reference value) const { return std::addressof(value); }

    pointer address(reference value) const { return std::addressof(value); }

    pointer allocate(size_type size, const void * = nullptr)
    {
        void *p = aligned_alloc(alignment, sizeof(T) * size);
        assert((p != 0) && (size > 0));
        return static_cast<pointer>(p);
    }

    size_type max_size() const { return ~static_cast<std::size_t>(0) / sizeof(T); }

    void deallocate(pointer ptr, size_type) { aligned_free(ptr); }

    template<class U, class V>
    void construct(U *ptr, const V &value)
    {
        void *p = ptr;
        ::new (p) U(value);
    }

    template<class U, class... Args>
    void construct(U *ptr, Args &&...args)
    {
        void *p = ptr;
        ::new (p) U(std::forward<Args>(args)...);
    }

    template<class U>
    void construct(U *ptr)
    {
        void *p = ptr;
        ::new (p) U();
    }

    template<class U>
    void destroy(U *ptr)
    {
        ptr->~U();
    }

private:
    void *aligned_alloc(size_type align, size_type size) const
    {
#if defined(_MSC_VER)
        return ::_aligned_malloc(size, align);
#elif defined(__ANDROID__)
        return ::memalign(align, size);
#elif defined(__MINGW32__)
        return ::_mm_malloc(size, align);
#else // posix assumed
        void *p;
        if (::posix_memalign(&p, align, size) != 0) {
            p = 0;
        }
        return p;
#endif
    }

    void aligned_free(pointer ptr)
    {
#if defined(_MSC_VER)
        ::_aligned_free(ptr);
#elif defined(__MINGW32__)
        ::_mm_free(ptr);
#else
        ::free(ptr);
#endif
    }
};

template<typename T1, typename T2, std::size_t alignment>
inline bool operator==(const AlignedAllocator<T1, alignment> &,
                       const AlignedAllocator<T2, alignment> &)
{
    return true;
}

template<typename T1, typename T2, std::size_t alignment>
inline bool operator!=(const AlignedAllocator<T1, alignment> &,
                       const AlignedAllocator<T2, alignment> &)
{
    return false;
}

} // namespace streaming
} // namespace kpsr
