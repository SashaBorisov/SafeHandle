#ifndef __SAFE_EVENT_HPP__
#define __SAFE_EVENT_HPP__

#include <synchapi.h>
#include <Safe/Handle.hpp>


namespace Safe
{
    struct EventHandle_Spec
    {
        static bool acquire(HANDLE & gem, const LPSECURITY_ATTRIBUTES sec_attrs
                                        , const bool    manual
                                        , const bool    initial
                                        , const TCHAR * name      )
        {
            gem = ::CreateEvent
            (
                  sec_attrs
                , manual  ? TRUE : FALSE
                , initial ? TRUE : FALSE
                , name
            );
            return NULL != gem;
        }

        static bool acquire(HANDLE & gem, const bool    manual  = false
                                        , const bool    initial = false
                                        , const TCHAR * name    = NULL  )
        {
            return acquire
            (
                  gem
                , NULL
                , manual  
                , initial 
                , name
            );
            return NULL != gem;
        }
    }; // EventHandle_Spec
    using Event = Handle<EventHandle_Spec>;

} // namespace Safe

#endif // __SAFE_EVENT_HPP__