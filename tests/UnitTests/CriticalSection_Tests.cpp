#include "Common.h"

#include <thread>
#include <Safe/CriticalSection.hpp>

TEST(CriticalSection, Create)
{
    {
        auto actual1 = Safe::CriticalSection();
        ASSERT_TRUE(actual1);
    }

    {
        auto actual2 = Safe::CriticalSection(4000);
        ASSERT_TRUE(actual2);
    }
}

constexpr auto CSTEST_LOCKER_REPEATS_COUNT = 1000;

void CsTestLockerProc(   const size_t step
                       , const Safe::CriticalSection * guard
                       , volatile size_t * counter)
{
    for(auto i = 0; i < CSTEST_LOCKER_REPEATS_COUNT; ++i)
    {
        const auto lock = Safe::CsLocker(*guard);
        ASSERT_TRUE(lock);
        ASSERT_TRUE(*guard);

        (*counter) += step;
    }
}

TEST(CriticalSection, Locker)
{

    auto guard = Safe::CriticalSection(4000);
    ASSERT_TRUE(guard);

    volatile size_t actual = 0;

    size_t step = 0;
    std::thread pool[4];
    for(auto & job : pool)
    {
        job = std::thread(CsTestLockerProc, ++step, &guard, &actual);
    }

    for(auto & job: pool)
    {
        if(job.joinable())
        {
            job.join();
        }
    }

    constexpr auto pool_size = std::size(pool);
    constexpr auto expected  = ((1 + pool_size) / 2) 
                             *  (pool_size + (0 == (pool_size % 2) ? 1 : 0)) 
                             * CSTEST_LOCKER_REPEATS_COUNT;
    ASSERT_EQ(expected, actual);
}