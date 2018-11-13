#include "Common.h"

#include <Safe/FileMapping.hpp>

TEST(FileMapping, Create)
{
    auto file    = Safe::File(__T("FileMapping.test"));
    auto mapping = Safe::FileMapping(file);
}