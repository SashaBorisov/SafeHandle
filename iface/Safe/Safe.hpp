#ifndef __SAFE_SAFE_HPP__
#define __SAFE_SAFE_HPP__


namespace Safe
{
    template<typename T>
    class Safe
    {
    public:
        using Spec  = T;
        using Type  = typename Spec::Type ;
        using Asset = typename Spec::Asset;

    private:
        bool Owner = false;
        Type Gem;

    public:
        Safe(const Safe &) = delete;
        const Safe & operator = (const Safe &) = delete;

        template<typename ...A>
        Safe(A&& ...args)
        {
            Spec::invalidate(Gem);
            Owner = Spec::acquire(Gem, std::forward<A>(args)...);
            if(!Owner)
            {
                Spec::invalidate(Gem);
            }
        }

        Safe(Safe && copy)
        {
            if(!copy.Owner)
            {
                Spec::invalidate(Gem);
            }
            else
            {
                Gem        = std::move(copy.Gem);
                Owner      = true;
                copy.Owner = false;
            }
        }

        ~Safe()
        {
            release();
            Spec::invalidate(Gem);
        }

        const Safe & operator = (Safe && copy) 
        {
            if(this != &copy)
            {
                if(release())
                {
                    if(copy.Owner)
                    {
                        Gem = std::move(copy.Gem);
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
                if(Spec::release(Gem))
                {
                    Spec::invalidate(Gem);
                    Owner = false;
                }
            }
            return !Owner;
        }

        constexpr    bool owner() const {return Owner;}
        constexpr operator bool() const {return Owner;}

              Asset & get()       {return Spec::get(Gem);}
        const Asset & get() const {return Spec::get(Gem);}

    }; // class Safe

} // namespace Safe

#endif // __SAFE_SAFE_HPP__