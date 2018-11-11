#ifndef __SAFE_HANDLE_HPP__
#define __SAFE_HANDLE_HPP__

#include <handleapi.h>
#include <tchar.h>

#include <Safe/Safe.hpp>


namespace Safe
{
    template<typename T>
    struct Handle_Spec
    {
        using Type = HANDLE;

        static constexpr bool valid(const Type asset) noexcept
        {
            return NULL != asset && INVALID_HANDLE_VALUE != asset;
        }

        static void invalidate(Type & asset) noexcept
        {
            asset = NULL;
        }

        static bool acquire(Type & asset, HANDLE && copy)
        {
            if(valid(copy))
            {
                asset = copy;
                return true;
            }
            return false;
        }

        template<typename ...A>
        static bool acquire(Type & asset, A&& ...args)
        {
            return T::acquire(asset, std::forward<A>(args)...);
        }

        static bool release(const Type asset)
        {
            if(valid(asset))
            {
                return FALSE != ::CloseHandle(asset);
            }
            return false;
        }
    }; // struct Handle_Spec
    template<typename T>
    using Handle = Safe<Handle_Spec<T>>;

    template<typename T>
    inline static Handle<T> CloneHandle(const Handle<T> & src)
    {
        HANDLE dst = NULL;
        const auto result = ::DuplicateHandle
        (
              ::GetCurrentProcess()
            , src.get()
            , ::GetCurrentProcess()
            , &dst
            , 0
            , FALSE
            , DUPLICATE_SAME_ACCESS
        );
        if(FALSE != result)
        {
            return Handle<T>(target);
        }
        return Handle<T>(INVALID_HANDLE_VALUE);
    }

    inline static DWORD WaitFor(const HANDLE handle, const DWORD timeout = INFINITE)
    {
        return ::WaitForSingleObject(handle, timeout);
    }

    template<typename T>
    inline static DWORD WaitFor(const Handle<T> & handle, const DWORD timeout = INFINITE)
    {
        return WaitFor(handle.get(), timeout);
    }

    template<typename T> constexpr auto sizeOf()       noexcept {return sizeof(T)   ;}
    template<>           constexpr auto sizeOf<void>() noexcept {return sizeof(BYTE);}

    template<typename D>
    inline size_t Write(   const HANDLE    handle
                         , const D * const items
                         , const size_t    count )
    {
        const auto data = reinterpret_cast<const BYTE * const>(items);
        const auto size = count * sizeOf<D>();

        size_t totally_written = 0;
        for(   DWORD written = 0
             ; totally_written < size
             ; totally_written += written, written = 0 )
        {
            const auto okey = ::WriteFile(    handle
                                           ,  data + totally_written
                                           ,  static_cast<DWORD>(size - totally_written)
                                           , &written
                                           ,  NULL    );
            if(TRUE != okey || 0 == written)
            {
                totally_written += written;
                break;
            }
        }
        return totally_written;
    }

    // template<typename D>
    // static inline size_t Write(   const HANDLE       handle
    //                             , const D * const    data
    //                             , const size_t       size
    //                             , const LPOVERLAPPED position )
    // {
    //     if(!position)
    //     {
    //         return Write(handle, data, size);
    //     }
    //
    //     DWORD totally_written = 0;
    //     const auto okey = ::WriteFile(     handle
    //                                     ,  data
    //                                     ,  static_cast<DWORD>(size)
    //                                     , &totally_written
    //                                     ,  position  );
    //     return static_cast<size_t>(totally_written);
    // }

    template<typename D>
    inline auto Write(const HANDLE handle, const D & item)
    {
        return Write(handle, &item, 1);
    }

    template<typename D>
    inline auto WriteData(const HANDLE handle, const D & items)
    {
        return Write(handle, std::data(items), std::size(items));
    }

    template<typename T, typename ...A>
    inline auto Write(const Handle<T> & handle, A&& ...args)
    {
        return Write(handle.get(), std::forward<A>(args)...);
    }

    template<typename T, typename ...A>
    inline auto WriteData(const Handle<T> & handle, A&& ...args)
    {
        return WriteData(handle.get(), std::forward<A>(args)...);
    }

    template<typename D>
    inline size_t Read(const HANDLE handle, D * const items, const size_t count)
    {
        const auto data = reinterpret_cast<BYTE * const>(items);
        const auto size = count * sizeOf<D>();

        size_t totally_read = 0;
        for(   DWORD read = 0
             ; totally_read < size
             ; totally_read += read, read = 0 )
        {
            const auto okey = ::ReadFile(    handle
                                          ,  data + totally_read
                                          ,  static_cast<DWORD>(size - totally_read)
                                          , &read
                                          ,  NULL );
            if(TRUE != okey || 0 == read)
            {
                totally_read += read;
                break;
            }
        }
        return totally_read;
    }

    template<typename D>
    inline auto Read(const HANDLE handle, D & item)
    {
        return Read(handle, &item, 1);
    }

    template<typename D>
    inline auto ReadData(const HANDLE handle, D & items)
    {
        return Read(handle, std::data(items), std::size(items));
    }

    template<typename T, typename ...A>
    inline auto Read(const Handle<T> & handle, A&& ...args)
    {
        return Read(handle.get(), std::forward<A>(args)...);
    }

    template<typename T, typename ...A>
    inline auto ReadData(const Handle<T> & handle, A&& ...args)
    {
        return ReadData(handle.get(), std::forward<A>(args)...);
    }

} // namespace Safe

#endif // __SAFE_HANDLE_HPP__
