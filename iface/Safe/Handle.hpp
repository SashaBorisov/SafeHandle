#pragma once

#include <handleapi.h>
#include <tchar.h>

#include <Safe/Safe.hpp>


namespace Safe
{
    template<typename T>
    struct Handle_Spec
    {
        using Type  = HANDLE;
        using Asset = Type  ;

        static constexpr bool valid(const Type gem) noexcept
        {
            return NULL != gem && INVALID_HANDLE_VALUE != gem;
        }

        static void invalidate(Type & gem) noexcept
        {
            gem = NULL;
        }

        static constexpr Asset & get(const Type & gem) noexcept
        {
            return const_cast<Asset &>(gem);
        }

        static bool acquire(Type & gem, const HANDLE copy) noexcept
        {
            if(valid(copy))
            {
                gem = copy;
                return true;
            }
            return false;
        }

        template<typename ...A>
        static bool acquire(Type & gem, A&& ...args)
        {
            return T::acquire(gem, std::forward<A>(args)...);
        }

        static bool release(const Type gem)
        {
            if(valid(gem))
            {
                return FALSE != ::CloseHandle(gem);
            }
            return false;
        }
    }; // struct Handle_Spec
    template<typename T>
    using Handle = Safe<Handle_Spec<T>>;

    // inline static DWORD WaitFor(const HANDLE handle, const DWORD timeout = INFINITE)
    // {
    //     return ::WaitForSingleObject(handle, timeout);
    // }

    template<typename T> //, typename ...A>
    inline auto WaitFor(const Handle<T> & handle, const DWORD timeout = INFINITE) // A&& ..args)
    {
        return ::WaitForSingleObject(handle.get(), timeout); //std::forward<A>(args)...);
    }

    template<typename T>
    struct HandleLocker_Spec
    {
        using Type  = HANDLE;
        using Asset = Type  ;

        static constexpr bool valid(const Type gem) noexcept
        {
            return NULL != gem && INVALID_HANDLE_VALUE != gem;
        }

        static void invalidate(Type & gem) noexcept
        {
            gem = NULL;
        }

        static constexpr Asset & get(const Type & gem) noexcept
        {
            return const_cast<Asset &>(gem);
        }

        static bool acquire(HANDLE & gem, const Handle<typename T::Spec> & handle
                                        , const DWORD timeout = INFINITE )
        {
            if(handle)
            {
                if(WaitFor(handle, timeout))
                {
                    gem = handle.get();
                    return true;
                }
            }
            return false;
        }

        static bool release(const HANDLE gem)
        {
            if(valid(gem))
            {
                return T::release(gem);
            }
            return false;
        }
    }; // struct HandleLocker_Spec
    template<typename T>
    using HandleLocker = Safe<HandleLocker_Spec<T>>;

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



    template<typename T> constexpr auto sizeOf()       noexcept {return sizeof(T)   ;}
    template<>           constexpr auto sizeOf<void>() noexcept {return sizeof(BYTE);}

    template<typename I>
    inline size_t Write(const HANDLE handle, const I * const items, const size_t count)
    {
        const auto data = reinterpret_cast<const BYTE * const>(items);
        const auto size = count * sizeOf<I>();

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

    // template<typename I>
    // static inline size_t Write(   const HANDLE       handle
    //                             , const I * const    data
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

    template<typename I>
    inline auto Write(const HANDLE handle, const I & item)
    {
        return Write(handle, &item, 1);
    }

    template<typename I>
    inline auto WriteData(const HANDLE handle, const I & items)
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

    template<typename I>
    inline size_t Read(const HANDLE handle, I * const items, const size_t count)
    {
        const auto data = reinterpret_cast<BYTE * const>(items);
        const auto size = count * sizeOf<I>();

        size_t totally_read = 0;
        for(DWORD read = 0; totally_read < size; totally_read += read, read = 0)
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

    template<typename I>
    inline auto Read(const HANDLE handle, I & item)
    {
        return Read(handle, &item, 1);
    }

    template<typename I>
    inline auto ReadData(const HANDLE handle, I & items)
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


