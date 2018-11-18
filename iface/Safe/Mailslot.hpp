#ifndef __SAFE_MAILSLOT_HPP__
#define __SAFE_MAILSLOT_HPP__

#include <WinBase.h>
#include <Safe/Handle.hpp>

namespace Safe
{
    struct MailslotHandle_Spec
    {
        static bool acquire(HANDLE & gem, const TCHAR * name
                                        , const DWORD   max_message_size
                                        , const DWORD   read_timeout
                                        , const LPSECURITY_ATTRIBUTES attrs = NULL)
        {
            gem = ::CreateMailslot
            (
                  name
                , max_message_size
                , read_timeout
                , attrs
            );
            return INVALID_HANDLE_VALUE != gem;
        }

    }; // struct MailslotHandle_Spec
    using Mailslot = Handle<MailslotHandle_Spec>;

} // namespace Safe

#endif // __SAFE_MAILSLOT_HPP__