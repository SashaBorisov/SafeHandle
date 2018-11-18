#ifndef __SAFE_SEMAPHORE_HPP__
#define __SAFE_SEMAPHORE_HPP__

#include <synchapi.h>
#include <Safe/Handle.hpp>

namespace Safe
{
    struct SemaphoreHandle_Spec
    {
        static bool acquire(HANDLE & gem, const LPSECURITY_ATTRIBUTES attrs
                                        , const LONG    initial_count
                                        , const LONG    maximum_count
                                        , const TCHAR * name          )
        {
            gem = ::CreateSemaphore
            (
                  attrs
                , initial_count
                , maximum_count
                , name
            );
            return NULL != gem;
        }

        static bool acquire(HANDLE & gem, const LONG    initial_count
                                        , const LONG    maximum_count
                                        , const TCHAR * name = NULL   )
        {
            return acquire(gem, NULL
                              , initial_count
                              , maximum_count
                              , name          );
        }

    }; // struct SemaphoreHandle_Spec
    using Semaphore = Handle<SemaphoreHandle_Spec>;

    struct SemaphoreLocker_Spec
    {
        using Spec = SemaphoreHandle_Spec;

        static bool release(const HANDLE gem)
        {
            return FALSE != ::ReleaseSemaphore(gem, 1, NULL);
        }
    };
    using SemaphoreLocker = HandleLocker<SemaphoreLocker_Spec>;

} // namespace Safe

#endif // __SAFE_SEMAPHORE_HPP__