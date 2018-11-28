#pragma once

#include <WinBase.h>
#include <process.h>
#include <Safe/Handle.hpp>


namespace Safe
{
    struct ThreadHandle_Spec
    {
        using ProcedurePtr = _beginthreadex_proc_type;

        static bool acquire(HANDLE & gem, const ProcedurePtr          procedure
                                        ,       void         * const  argument   = nullptr
                                        , const unsigned int          init_flag  = 0
                                        ,       unsigned int * const  thread_id  = NULL 
                                        , const unsigned int          stack_size = 0   
                                        , const LPSECURITY_ATTRIBUTES security   = NULL )
        {
            if(nullptr == procedure)
            {
                gem = NULL;
            }
            else
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
            }
            return NULL != gem && INVALID_HANDLE_VALUE != gem;
        }

    };
    using Thread = Handle<ThreadHandle_Spec>;
    
} // namespace Safe

