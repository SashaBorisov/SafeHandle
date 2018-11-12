#ifndef __SAFE_CO_INITIALIZE_HPP__
#define  __SAFE_CO_INITIALIZE_HPP__

#include <combaseapi.h>
#include <Safe/Safe.hpp>

namespace Safe
{
    struct CoInitialize_Spec
    {
        using Type  = HRESULT;
        using Asset = Type;

        static constexpr bool valid(const Type gem) noexcept
        {
            return SUCCEEDED(gem);
        }

        static constexpr Asset & get(const Type & gem) noexcept
        {
            return const_cast<Asset &>(gem);
        }

        static void invalidate(Type & gem) noexcept
        {
            gem = E_FAIL;
        }

        static bool acquire(Type & gem)
        {
            gem = ::CoInitialize(NULL);
            return valid(gem);
        }

        static bool acquire(Type & gem, const DWORD coinit)
        {
            gem = ::CoInitializeEx(NULL, coinit);
            return valid(gem);
        }

        static bool release(const Type gem)
        {
            if(valid(gem))
            {
                ::CoUninitialize();
                return true;
            }
            return false;
        }

    }; // struct CoInitialize_Spec
    using CoInitialize = Safe<CoInitialize_Spec>;
}

#endif //  __SAFE_CO_INITIALIZE_HPP__