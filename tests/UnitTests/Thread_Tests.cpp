#include "Common.h"

#include <Safe/Thread.hpp>

TEST(Thread, Create)
{
    auto actual = Safe::Thread(nullptr);
    ASSERT_FALSE(actual);
}