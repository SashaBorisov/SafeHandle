#pragma once

#include <heapapi.h>
#include <Safe/Safe.hpp>


namespace Safe
{
    template<DWORD FLAGS>
    struct Heap_Spec
    {
        using Type  = HANDLE;
        using Asset = HANDLE;

        static constexpr Asset & get(const Type & gem) noexcept
        {
            return const_cast<Asset &>(gem);
        }

        static constexpr bool valid(const Type gem) noexcept
        {
            return NULL != gem;
        }

        static void invalidate(Type & gem) noexcept
        {
            gem = NULL;
        }

        static bool acquire(Type & gem, const SIZE_T initial_size = 0
                                      , const SIZE_T maximum_size = 0 )
        {
            gem = ::HeapCreate(FLAGS, initial_size, maximum_size);
            return valid(gem);
        }

        static bool release(const Type & gem)
        {
            return TRUE == ::HeapDestroy(gem);
        }

    }; // struct Heap_Spec
    template<DWORD FLAGS>
    using Heap = Safe<Heap_Spec<FLAGS>>;

    struct HeapLocker_Spec
    {
        using Type  = HANDLE;
        using Asset = Type  ;

        static constexpr Asset & get(const Type & gem) noexcept
        {
            return const_cast<Asset &>(gem);
        }

        static constexpr bool valid(const Type gem) noexcept
        {
            return NULL != gem;
        }

        static void invalidate(Type & gem) noexcept
        {
            gem = NULL;
        }

        template<DWORD FLAGS>
        static bool acquire(Type & gem, const Heap<FLAGS> & heap)
        {
            static_asset(0 == (FLAGS & HEAP_NO_SERIALIZE), "Locking is invalid for non serializable heap");
            if(heap)
            {
                if(TRUE == ::HeapLock(heap.get()))
                {
                    gem = heap.get();
                    return valid(gem);
                }
            }
            return false;
        }

        static bool release(const Type gem)
        {
            return TRUE == ::HeapUnlock(gem);
        }

    }; // struct HeapLocker_Spec
    using HeapLocker = Safe<HeapLocker_Spec>;

    struct ProcHeapChunk_Spec
    {
        using Type  = void *;
        using Asset = Type;

        static constexpr bool valid(const Type gem) noexcept
        {
            return NULL != gem;
        }

        static constexpr Asset & get(const Type & gem) noexcept
        {
            return const_cast<Asset &>(gem);
        }

        static void invalidate(Type & gem) noexcept
        {
            gem = NULL;
        }

        static bool acquire(Type & gem, const SIZE_T size)
        {
            gem = ::HeapAlloc(::GetProcessHeap(), 0, size);
            return valid(gem);
        }

        static bool release(const Type gem)
        {
            return TRUE == ::HeapFree(::GetProcessHeap(), 0, gem);
        }

    }; // struct ProcHeapChunk_Spec
    using ProcHeapChunk = Safe<ProcHeapChunk_Spec>;

} // namespace Safe

