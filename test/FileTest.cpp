#include "gtest/gtest.h"

#include "file_handling.hpp"

TEST(FileHandling, AbsolutePaths)
{
  fs::path tds_file, binary_file, pdb_file;
  std::tie(tds_file, binary_file, pdb_file) = get_files("/data/test.tds");

  EXPECT_EQ(tds_file.generic_string(), "/data/test.tds");
  EXPECT_EQ(binary_file.generic_string(), "/data/test.exe");
  EXPECT_EQ(pdb_file.generic_string(), "/data/test.pdb");
}

TEST(FileHandling, RelativePaths)
{
  fs::path tds_file, binary_file, pdb_file;
  std::tie(tds_file, binary_file, pdb_file) = get_files("data/test.tds");

  EXPECT_EQ(tds_file.generic_string(), "data/test.tds");
  EXPECT_EQ(binary_file.generic_string(), "data/test.exe");
  EXPECT_EQ(pdb_file.generic_string(), "data/test.pdb");
}

class BinaryFileTest : public ::testing::Test
{
};

TEST_F(BinaryFileTest, NonExistingFileMustThrow)
{
  ASSERT_THROW({
    BinaryFile file("I don't exist for sure!");
  }, std::runtime_error);
}

TEST_F(BinaryFileTest, ExistingFileMustNotThrow)
{
  ASSERT_NO_THROW({
    BinaryFile file("tds2pdb_test");
  });
}

TEST(StreamReaders, ReadInt8)
{
  std::istringstream input("1234");

  EXPECT_EQ(readInteger<int8_t>(input), 1);
  EXPECT_EQ(readInteger<int8_t>(input), 2);
  EXPECT_EQ(readInteger<int8_t>(input), 3);
  EXPECT_EQ(readInteger<int8_t>(input), 4);
}

TEST(StreamReaders, ReadInt16)
{
  std::istringstream input("1234");

  EXPECT_EQ(readInteger<int16_t>(input), 121);
  EXPECT_EQ(readInteger<int16_t>(input), 34);
}

TEST(StreamReaders, ReadInt32)
{
  std::istringstream input("1234");

  EXPECT_EQ(readInteger<int32_t>(input), 1234);
}

TEST(StreamReaders, ReadString)
{
  std::istringstream input("abcd space UpPeRcAsE");

  EXPECT_EQ(readString<4>(input), "abcd");
  EXPECT_EQ(readString<6>(input), " space");
}

