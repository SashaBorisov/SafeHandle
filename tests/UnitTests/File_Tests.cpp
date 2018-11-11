#include "Common.h"
#include <string>
#include <vector>
#include <Safe/File.hpp>


TEST(File, ReadWrite)
{
    const auto path       = __T("File.Create.test");
    const char text[]     = u8"текст";
    const auto text_size  = std::size  (text );
    const auto text2      = std::string(text );
    const auto text2_size = std::size  (text2);

    {
        auto writer = Safe::File(path, FILE_GENERIC_WRITE, 0, CREATE_ALWAYS);
        ASSERT_TRUE(writer);

        ASSERT_EQ(sizeof(text_size) , Safe::Write(writer, text_size ));
        ASSERT_EQ(text_size         , Safe::Write(writer, text      ));

        ASSERT_EQ(sizeof(text2_size), Safe::Write(writer, text2_size));
        ASSERT_EQ(text2_size        , Safe::WriteData(writer, text2 ));
    }

    {
        auto reader = Safe::File(path, FILE_GENERIC_READ, 0, OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE);
        ASSERT_TRUE(reader);

        auto actual_text_size = text_size - text_size;
        ASSERT_EQ(sizeof(text_size) , Safe::Read(reader, actual_text_size));
        ASSERT_EQ(text_size, actual_text_size);

        char actual_text[std::size(text)] = {0};
        ASSERT_EQ(text_size, Safe::Read(reader, actual_text));
        ASSERT_EQ(0, std::memcmp(text, actual_text, sizeof(text)));

        auto actual_text2_size = text2_size - text2_size;
        ASSERT_EQ(sizeof(text2_size), Safe::Read(reader, actual_text2_size));
        ASSERT_EQ(text2_size, actual_text2_size);

        auto actual_text2 = std::vector<char>(text2_size, 0);
        ASSERT_EQ(text2_size, Safe::ReadData(reader, actual_text2));
        ASSERT_EQ(0, std::memcmp(std::data(text2), std::data(actual_text2), text2_size));
    }
}
