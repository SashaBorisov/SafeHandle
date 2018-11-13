#ifndef __SAFE_FILE_HPP__
#define __SAFE_FILE_HPP__

#include <Safe/Handle.hpp>

namespace Safe
{
    struct FileHandle_Spec
    {
        static bool acquire(HANDLE & asset, const TCHAR * path
                                          , const DWORD   access
                                          , const DWORD   share_mode
                                          , const LPSECURITY_ATTRIBUTES sec_attrs
                                          , const DWORD   disposition
                                          , const DWORD   flags
                                          , const HANDLE  template_file   )
        {
            asset = ::CreateFile
            (
                  path
                , access
                , share_mode
                , sec_attrs
                , disposition
                , flags
                , template_file
            );
            return INVALID_HANDLE_VALUE != asset;
        }

        static bool acquire(HANDLE & asset, const TCHAR * path
                                          , const DWORD   access
                                          , const DWORD   share_mode
                                          , const LPSECURITY_ATTRIBUTES sec_attrs
                                          , const DWORD   disposition
                                          , const DWORD   flags         )
        {
            return acquire(asset, path
                                , access
                                , share_mode
                                , sec_attrs
                                , disposition
                                , flags
                                , NULL    );
        }

        static bool acquire(HANDLE & asset, const TCHAR * path
                                          , const DWORD   access
                                          , const DWORD   share_mode
                                          , const DWORD   disposition
                                          , const DWORD   flags         )
        {
            return acquire(asset, path
                                , access
                                , share_mode
                                , NULL
                                , disposition
                                , flags
                                , NULL     );
        }

        static bool acquire(HANDLE & asset, const TCHAR * path
                                          , const DWORD   access      = FILE_GENERIC_READ
                                          , const DWORD   share_mode  = FILE_SHARE_READ
                                          , const DWORD   disposition = OPEN_EXISTING )
        {
            return acquire(asset, path
                                , access
                                , share_mode
                                , NULL
                                , disposition
                                , FILE_ATTRIBUTE_NORMAL
                                , NULL    );
        }

    }; // struct FileHandle_Spec    
    using File = Handle<FileHandle_Spec>;

} // namespace Safe

#endif // __SAFE_FILE_HPP__