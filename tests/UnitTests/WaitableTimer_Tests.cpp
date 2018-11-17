#include "Common.h"

#include <Safe/WaitableTimer.hpp>

TEST(WaitableTimer, Create)
{
    auto actual = Safe::WaitableTimer(HANDLE(NULL));
    ASSERT_FALSE(actual);
}