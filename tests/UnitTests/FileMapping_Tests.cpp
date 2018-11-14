#include "Common.h"

#include <Safe/FileMapping.hpp>

TEST(FileMapping, ReadWrite)
{
    const auto path       = __T("FileMapping.Create.test");
    const char text[]     = u8"текст";
    const auto text_size  = std::size  (text );
    const auto text2      = std::string(text );
    const auto text2_size = std::size  (text2);
    const auto total_size = sizeof(text_size) + text_size 
                          + sizeof(text2_size) + text2_size;
    {
        auto file  = Safe::File(path, GENERIC_WRITE | GENERIC_READ
                                    , 0
                                    , CREATE_ALWAYS );
        auto e = GetLastError();
        ASSERT_TRUE(file);

        auto mapping = Safe::FileMapping(file, PAGE_READWRITE, total_size);
        e = GetLastError();
        ASSERT_TRUE(mapping);

        auto view = Safe::FileMappingView(mapping, FILE_MAP_WRITE, 0, total_size);
        e = GetLastError();
        ASSERT_TRUE(view);

        auto pointer = reinterpret_cast<BYTE *>(view.get());

        std::memcpy(pointer, &text_size, sizeof(text_size));
        pointer += sizeof(text_size);

        std::memcpy(pointer, std::data(text), text_size);
        pointer += text_size;

        std::memcpy(pointer, &text2_size, sizeof(text2_size));
        pointer += sizeof(text2_size);

        std::memcpy(pointer, std::data(text2), text2_size);
        pointer += text2_size;
    }

    {
        auto file  = Safe::File(path, GENERIC_READ
                                    , 0
                                    , OPEN_EXISTING
                                    , FILE_FLAG_DELETE_ON_CLOSE );
        auto e = GetLastError();
        ASSERT_TRUE(file);

        auto mapping = Safe::FileMapping(file, PAGE_READONLY, total_size);
        e = GetLastError();
        ASSERT_TRUE(mapping);

        auto view = Safe::FileMappingView(mapping, FILE_MAP_READ, 0, total_size);
        e = GetLastError();
        ASSERT_TRUE(view);

        auto pointer = reinterpret_cast<BYTE *>(view.get());

        ASSERT_EQ(0, std::memcmp(pointer, &text_size, sizeof(text_size)));
        pointer += sizeof(text_size);

        ASSERT_EQ(0, std::memcmp(pointer, std::data(text), text_size));
        pointer += text_size;

        ASSERT_EQ(0, std::memcmp(pointer, &text2_size, sizeof(text2_size)));
        pointer += sizeof(text2_size);

        ASSERT_EQ(0, std::memcmp(pointer, std::data(text2), text2_size));
        pointer += text2_size;
    }
}