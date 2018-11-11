#ifndef __SAFE_EVENT_HPP__
#define __SAFE_EVENT_HPP__

#include <Safe/Handle.hpp>


namespace Safe
{
    struct EventHandle_Spec
    {
        static bool acquire(HANDLE & asset, const LPSECURITY_ATTRIBUTES sec_attrs
                                          , const BOOL    manual
                                          , const BOOL    initial
                                          , const TCHAR * name      )
        {
            asset = ::CreateEvent
            (
                  sec_attrs
                , manual
                , initial
                , name
            );
            return NULL != asset;
        }

        static bool acquire(HANDLE & asset, const BOOL    manual  = FALSE
                                          , const BOOL    initial = FALSE
                                          , const TCHAR * name    = NULL  )
        {
            asset = ::CreateEvent
            (
                  NULL
                , manual
                , initial
                , name
            );
            return NULL != asset;
        }
    }; // EventHandle_Spec
    using Event = Handle<EventHandle_Spec>;

} // namespace Safe

#endif // __SAFE_EVENT_HPP__