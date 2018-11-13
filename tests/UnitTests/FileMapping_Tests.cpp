#include "Common.h"

#include <Safe/FileMapping.hpp>

TEST(FileMapping, Create)
{
    auto mapping = Safe::FileMapping(NULL);
}