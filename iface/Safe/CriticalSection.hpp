#pragma once

#include <Windows.h>

#include <Safe/Safe.hpp>

namespace Safe
{
    struct CriticalSection_Spec
    {
        using Type  = CRITICAL_SECTION;
        using Asset = Type;

        // static constexpr bool valid(Type&) noexcept
        // {
        //     return true;
        // }

        static constexpr void invalidate(Type &) noexcept {}

        static constexpr Asset & get(const Type & gem) noexcept
        {
            return const_cast<Asset &>(gem);
        }

        static bool acquire(Type & gem)
        {
            ::InitializeCriticalSection(&gem);
            return true;
        }

        static bool acquire(Type & gem, const DWORD spin_count)
        {
           return TRUE == ::InitializeCriticalSectionAndSpinCount(&gem, spin_count);
        }

        static bool release(Type & gem)
        {
            ::DeleteCriticalSection(&gem);
            return true;
        }

    }; // struct CriticalSection_Spec
    using CriticalSection = Safe<CriticalSection_Spec>;

    struct CriticalSectionLocker_Spec
    {
        using Type  = CRITICAL_SECTION *;
        using Asset = Type;

        // static constexpr bool valid(const Type gem) noexcept
        // {
        //     return NULL != gem;
        // }
        
        static constexpr Asset & get(const Type & gem) noexcept
        {
            return const_cast<Asset &>(gem);
        }

        static void invalidate(Type & gem) noexcept
        {
            gem = NULL;
        }

        static bool acquire(Type & gem, const Type guard)
        {
            if(NULL != guard)
            {    
                gem = guard;                
                ::EnterCriticalSection(gem);
                return true;
            }
            return false;    
        }

        static bool acquire(Type & gem, const CriticalSection & guard)
        {
            if(guard)
            {
                return acquire(gem, const_cast<Type>(&guard.get()));
            }
            return false;
        }

        static bool release(const Type gem)
        {
            ::LeaveCriticalSection(gem);
            return true;
        }

    }; // struct CriticalSectionLocker_Spec
    using CsLocker = Safe<CriticalSectionLocker_Spec>;

    struct CriticalSectionTryLocker_Spec
    {
        using Type  = CRITICAL_SECTION *;
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

        static bool acquire(Type & gem, const Type guard, const DWORD spin_count)
        {
            if(valid(guard))
            {    
                DWORD spin = 0;
                do
                {
                    if(TRUE == ::TryEnterCriticalSection(guard))
                    {
                        gem = guard;                
                        return true;
                    }
                    // YeldProcessor();
                } 
                while((++spin) < spin_count);
            }
            return false;    
        }

        static bool acquire(Type & gem, const Type guard)
        {
            if(valid(guard))
            {
                return acquire(gem, guard, static_cast<DWORD>(guard->SpinCount));
            }
            return false;
        }

        static bool acquire(Type & gem, const CriticalSection & guard, const DWORD spin_count)
        {
            if(guard)
            {
                return acquire(gem, const_cast<Type>(&guard.get()), spin_count);
            }
            return false;
        }

        static bool acquire(Type & gem, const CriticalSection & guard)
        {
            if(guard)
            {
                return acquire(gem, const_cast<Type>(&guard.get()));
            }
            return false;
        }

        static bool release(const Type gem)
        {
            ::LeaveCriticalSection(gem);
            return true;
        }

    }; // struct CriticalSectionTryLocker_Spec
    using CsTryLocker = Safe<CriticalSectionTryLocker_Spec>;

} // namespace Safe

