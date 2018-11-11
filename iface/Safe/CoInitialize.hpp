#ifndef __SAFE_CO_INITIALIZE_HPP__
#define  __SAFE_CO_INITIALIZE_HPP__

#include <combaseapi.h>
#include <Safe/Safe.hpp>

namespace Safe
{
    struct CoInitialize_Spec
    {
        using Type = HRESULT;

        static constexpr bool valid(const Type asset) noexcept
        {
            return SUCCEEDED(asset);
        }

        static void invalidate(Type & asset) noexcept
        {
            asset = E_FAIL;
        }

        static bool acquire(Type & asset)
        {
            asset = ::CoInitialize(NULL);
            return valid(asset);
        }

        static bool acquire(Type & asset, const DWORD coinit)
        {
            asset = ::CoInitializeEx(NULL, coinit);
            return valid(asset);
        }

        static bool release(const Type asset)
        {
            if(valid(asset))
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