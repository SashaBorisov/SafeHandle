#include "Common.h"
#include <thread>
#include <Safe/Event.hpp>


TEST(Event, Create)
{
    const auto actual = Safe::Event();
    ASSERT_TRUE(actual);
}

TEST(Event, Notify)
{
    const auto event = Safe::Event();
    ASSERT_TRUE(event);
    
    auto job = std::thread([&event]()->void
    {
        Sleep(50);
        ::SetEvent(event.get());
    });

    ASSERT_EQ(WAIT_OBJECT_0, Safe::WaitFor(event, 1000));

    job.join();
}