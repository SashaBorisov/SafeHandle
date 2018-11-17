#ifndef __SAFE_WAITABLE_TIMER_HPP__
#define __SAFE_WAITABLE_TIMER_HPP__

#include <synchapi.h>
#include <Safe/Handle.hpp>


namespace Safe
{
    struct WaitableTimerHandle_Spec
    {
        static bool acquire(HANDLE & gem, const LPSECURITY_ATTRIBUTES attrs
                                        , const bool    manual
                                        , const TCHAR * name )
        {
            gem = ::CreateWaitableTimer
            (
                  attrs
                , manual ? TRUE : FALSE
                , name
            );
            return NULL != gem;
        }

        static bool acquire(HANDLE & gem, const bool    manual = false
                                        , const TCHAR * name   = NULL)
        {
            return acquire(gem, NULL
                              , manual
                              , name   );
        }

    }; // struct WaitableTimerHandle_Spec
    using WaitableTimer = Handle<WaitableTimerHandle_Spec>;

    struct PeriodicCallback_Spec
    {
        using Type  = HANDLE;
        using Asset = Type  ;

        // static constexpr const Asset & get(const Type & gem) noexcept
        // {
        //     return gem;
        // }

        static constexpr bool valid(const Type gem) noexcept
        {
            return NULL != gem;
        }

        static void invalidate(Type & gem) noexcept
        {
            gem = NULL;
        }

        static bool acquire(Type & gem, const WaitableTimer  & timer
                                      , const LONGLONG         due //const LARGE_INTEGER* lpDueTime,
                                      , const LONG             period
                                      , const PTIMERAPCROUTINE callback
                                      , const LPVOID           argument
                                      , const bool             resume)
        {
            if(timer)
            {
                LARGE_INTEGER due_time;
                due_time.QuadPart = due;
                const auto result = ::SetWaitableTimer
                (
                       timer.get()
                    , &due_time
                    ,  period
                    ,  callback
                    ,  argument
                    ,  resume ? TRUE : FALSE
                );
                if(FALSE != result)
                {
                    gem = timer.get();
                    return true;
                }
            }
            return false;
        }

        static bool release(const Type gem)
        {
            if(valid(gem))
            {
                return FALSE != ::CancelWaitableTimer(gem);
            }
            return false;
        }
    };
    

} // namespace Safe

#endif // __SAFE_WAITABLE_TIMER_HPP__