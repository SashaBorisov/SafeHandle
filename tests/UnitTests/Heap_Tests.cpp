#include "Common.h"

#include <Safe/Heap.hpp>

TEST(Heap, Create)
{
    auto actual = Safe::Heap<0>();
}