#ifndef __SAFE_THREAD_HPP__
#define __SAFE_THREAD_HPP__

#include <WinBase.h>
#include <process.h>
#include <Safe/Handle.hpp>


namespace Safe
{
    struct ThreadHandle_Spec
    {
        using ProcedurePtr = _beginthreadex_proc_type;

        static bool acquire(HANDLE & gem, const LPSECURITY_ATTRIBUTES security
                                        , const unsigned int          stack_size
                                        , const ProcedurePtr          procedure
                                        ,       void         * const  argument
                                        , const unsigned int          init_flag
                                        ,       unsigned int * const  thread_id  )
        {
            gem = reinterpret_cast<HANDLE>(::_beginthreadex
            (
                  security
                , stack_size
                , procedure
                , argument
                , init_flag
                , thread_id
            ));
            return NULL != gem && INVALID_HANDLE_VALUE != gem;
        }

        static bool acquire(HANDLE & gem, const unsigned int          stack_size
                                        , const ProcedurePtr          procedure
                                        ,       void         * const  argument  = NULL
                                        , const unsigned int          init_flag = 0
                                        ,       unsigned int * const  thread_id = NULL )
        {
            return acquire(gem, NULL
                              , stack_size
                              , procedure
                              , argument
                              , init_flag
                              , thread_id );
        }
    };
    using Thread = Handle<ThreadHandle_Spec>;
    
} // namespace Safe

#endif // __SAFE_THREAD_HPP__