#include "Common.h"

#include <Safe/CoInitialize.hpp>

TEST(CoInitialize, Create1)
{
    auto coinit1 = Safe::CoInitialize();
    coinit1.release();
}

// TEST(CoInitialize, Create2)
// {
//     const auto coinit2 = Safe::CoInitialize();
// }