#ifndef __SAFE_CRITICAL_SECTION_HPP__
#define __SAFE_CRITICAL_SECTION_HPP__

#include <Windows.h>

#include <Safe/Safe.hpp>

namespace Safe
{
    struct CriticalSection_Spec
    {
        using Type = CRITICAL_SECTION;

        // static constexpr bool valid(Type&) noexcept
        // {
        //     return true;
        // }

        static constexpr void invalidate(Type &) noexcept {}

        static bool acquire(Type & asset)
        {
            ::InitializeCriticalSection(&asset);
            return true;
        }

        static bool acquire(Type & asset, const DWORD spin_count)
        {
           return TRUE == ::InitializeCriticalSectionAndSpinCount(&asset, spin_count);
        }

        static bool release(Type & asset)
        {
            ::DeleteCriticalSection(&asset);
            return true;
        }

    }; // struct CriticalSection_Spec
    using CriticalSection = Safe<CriticalSection_Spec>;

    struct CriticalSectionLocker_Spec
    {
        using Type = CRITICAL_SECTION *;

        // static constexpr bool valid(const Type asset) noexcept
        // {
        //     return NULL != asset;
        // }

        static void invalidate(Type & asset) noexcept
        {
            asset = NULL;
        }

        static bool acquire(Type & asset, const Type guard)
        {
            if(NULL != guard)
            {    
                asset = guard;                
                ::EnterCriticalSection(asset);
                return true;
            }
            return false;    
        }

        static bool acquire(Type & asset, const CriticalSection & guard)
        {
            if(guard)
            {
                return acquire(asset, const_cast<Type>(&guard.get()));
            }
            return false;
        }

        static bool release(const Type asset)
        {
            ::LeaveCriticalSection(asset);
            return true;
        }

    }; // struct CriticalSectionLocker_Spec
    using CsLocker = Safe<CriticalSectionLocker_Spec>;

} // namespace Safe

#endif // __SAFE_CRITICAL_SECTION_HPP__