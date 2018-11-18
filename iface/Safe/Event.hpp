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
            return acquire(gem, NULL
                              , manual  
                              , initial 
                              , name    );
        }
    }; // EventHandle_Spec
    using Event = Handle<EventHandle_Spec>;

    struct EventSetter_Spec
    {
        using Type  = HANDLE;
        using Asset = Type  ;

        static constexpr bool valid(const Type gem) noexcept
        {
            return NULL != gem;
        }

        static void invalidate(Type & gem) noexcept
        {
            gem = NULL;
        }

        static constexpr Asset & get(const Type & gem) noexcept
        {
            return const_cast<Asset &>(gem);
        }

        static bool acquire(HANDLE & gem, const Event & handle)
        {
            if(handle)
            {
                gem = handle.get();
                return valid(gem);
            }
            return false;
        }

        static bool release(const HANDLE gem)
        {
            if(valid(gem))
            {
                return FALSE != ::SetEvent(gem);
            }
            return false;
        }
    }; // struct EventSetter_Spec
    using EventSetter = Safe<EventSetter_Spec>;

} // namespace Safe

#endif // __SAFE_EVENT_HPP__