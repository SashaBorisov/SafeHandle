#ifndef __SAFE_FILE_MAPPING_HPP__
#define __SAFE_FILE_MAPPING_HPP__

#include <memoryapi.h>
#include <Safe/File.hpp>


namespace Safe
{
    constexpr DWORD HiDword(const QWORD value) noexcept
    {
        return static_cast<DWORD>((value >> 32) & 0xFFFFFFFF);
    }

    constexpr DWORD LoDword(const QWORD value) noexcept
    {
        return static_cast<DWORD>(value & 0xFFFFFFFF);
    }

    struct FileMappingHandle_Spec
    {
        static bool acquire(HANDLE & gem, const File & file
                                        , const LPSECURITY_ATTRIBUTES attrs
                                        , const DWORD   protect
                                        , const QWORD   max_size
                                        , const TCHAR * name     )
        {
            gem = ::CreateFileMapping
            (
                  file.get()
                , attrs
                , protect
                , HiDword(max_size)
                , LoDword(max_size)
                , name
            );
            return NULL != gem;
        }

        static bool acquire(HANDLE & gem, const File  & file
                                        , const DWORD   protect  = PAGE_READONLY
                                        , const QWORD   max_size = 0
                                        , const TCHAR * name     = NULL )
        {
            return acquire
            (
                gem, file
                   , NULL
                   , protect
                   , max_size
                   , name
            );
        }

    }; // struct FileMappingHandle_Spec
    using FileMapping = Handle<FileMappingHandle_Spec>;

    struct FileMappingView_Spec
    {
        using Type  = void *;
        using Asset = Type  ;

        static constexpr Asset & get(const Type & gem) noexcept
        {
            return const_cast<Asset &>(gem);
        }

        static constexpr bool valid(const Type gem) noexcept
        {
            return NULL != gem;
        }

        static void invalidate(Type & gem) noexcept
        {
            gem = NULL;
        }

        static bool acquire(Type & gem, const FileMapping & mapping
                                      , const DWORD         access  = FILE_MAP_READ
                                      , const QWORD         offset  = 0
                                      , const SIZE_T        size    = 0  )
        {
            gem = ::MapViewOfFile
            (
                  mapping.get()
                , access
                , HiDword(offset)
                , LoDword(offset)
                , size
            )
            return valid(gem);
        }

        static bool release(const Type gem)
        {
            return TRUE == ::UnmapViewOfFile(gem);
        }

    }; // struct FileMappingView_Spec
    using FileMappingView = Safe<FileMappingView_Spec>;

} // namespace Safe

#endif // __SAFE_FILE_MAPPING_HPP__
