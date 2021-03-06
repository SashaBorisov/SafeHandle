#pragma once

#include <synchapi.h>
#include <Safe/Handle.hpp>


namespace Safe
{
    struct MutexHandle_Spec
    {
        static bool acquire(HANDLE & gem, const LPSECURITY_ATTRIBUTES attrs
                                        , const bool    initial_owner
                                        , const TCHAR * name   )
        {
            gem = ::CreateMutex
            (
                  attrs
                , initial_owner ? TRUE : FALSE
                , name
            );
            return NULL != gem;
        }

        static bool acquire(HANDLE & gem, const bool    initial_owner = false
                                        , const TCHAR * name          = NULL )
        {
            return acquire(gem, NULL
                              , initial_owner
                              , name         );
        }

    }; // struct MutexHandle_Spec
    using Mutex = Handle<MutexHandle_Spec>;

    struct MutexLocker_Spec
    {
        using Spec = MutexHandle_Spec;

        static bool release(const HANDLE gem)
        {
            return FALSE != ::ReleaseMutex(gem);
        }
    };
    using MutexLocker = HandleLocker<MutexLocker_Spec>;

} // namespace Safe


