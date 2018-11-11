#ifndef __SAFE_MUTEX_HPP__
#define __SAFE_MUTEX_HPP__

#include <Safe/Handle.hpp>


namespace Safe
{
    struct MutexHandle_Spec
    {
        static bool acquire(HANDLE & asset, const LPSECURITY_ATTRIBUTES attrs
                                          , const BOOL    initial_owner
                                          , const TCHAR * name   )
        {
            asset = ::CreateMutex
            (
                  attrs
                , initial_owner
                , name
            );
            return NULL != asset;
        }

        static bool acquire(HANDLE & asset, const BOOL    initial_owner = FALSE
                                          , const TCHAR * name          = NULL )
        {
            return acquire
            (
                  asset
                , NULL
                , initial_owner
                , name
            );
            return NULL != asset;
        }

    }; // struct MutexHandle_Spec
    using Mutex = Handle<MutexHandle_Spec>;

} // namespace Safe

#endif // __SAFE_MUTEX_HPP__
