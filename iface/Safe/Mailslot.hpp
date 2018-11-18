#ifndef __SAFE_MAILSLOT_HPP__
#define __SAFE_MAILSLOT_HPP__

#include <WinBase.h>
#include <Safe/Handle.hpp>

namespace Safe
{
    struct MailslotHandle_Spec
    {
        static bool acquire(HANDLE & gem,  _In_     LPCWSTR lpName,
    _In_     DWORD nMaxMessageSize,
    _In_     DWORD lReadTimeout,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes)
        {
            gem = ::CreateMailslot
            (

            );
            return INVALID_HANDLE_VALUE != gem;
        }

    }; // struct MailSlotHandle_Spec

} // namespace Safe

#endif // __SAFE_MAILSLOT_HPP__