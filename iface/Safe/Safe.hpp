#ifndef __SAFE_SAFE_HPP__
#define __SAFE_SAFE_HPP__


namespace Safe
{
    template<typename T>
    class Safe
    {
    public:
        using Spec = T;
        using Type = typename Spec::Type;

    private:
        bool Owner = false;
        Type Asset;

    public:
        Safe(const Safe &) = delete;
        const Safe & operator = (const Safe &) = delete;

        template<typename ...A>
        Safe(A&& ...args)
        {
            Spec::invalidate(Asset);
            Owner = Spec::acquire(Asset, std::forward<A>(args)...);
            if(!Owner)
            {
                Spec::invalidate(Asset);
            }
        }

        Safe(Safe && copy)
        {
            if(!copy.Owner)
            {
                Spec::invalidate(Asset);
            }
            else
            {
                Asset = std::move(copy.Asset);
                Owner = true;
                copy.Owner = false;
            }
        }

        ~Safe()
        {
            release();
            Spec::invalidate(Asset);
        }

        const Safe & operator = (Safe && copy) 
        {
            if(this != &copy)
            {
                if(release())
                {
                    if(copy.Owner)
                    {
                        Asset = std::move(copy.Asset);
                        Owner = true;
                        copy.Owner = false;
                    }
                }
            }
            return *this;
        }

        bool release()
        {
            if(Owner)
            {
                if(Spec::release(Asset))
                {
                    Spec::invalidate(Asset);
                    Owner = false;
                }
            }
            return !Owner;
        }

        constexpr    bool owner() const {return Owner;}
        constexpr operator bool() const {return Owner;}

              Type & get()       {return Asset;}
        const Type & get() const {return Asset;}

    }; // class Safe

} // namespace Safe

#endif // __SAFE_SAFE_HPP__