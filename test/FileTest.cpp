#include "gtest/gtest.h"
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

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
  char acArray[6] = "\x00\x01\x02\x03\x04";
  boost::iostreams::array_source source{acArray, sizeof(acArray)};
  boost::iostreams::stream<boost::iostreams::array_source> input{source};
  
  EXPECT_EQ(readInteger<int8_t>(input), 0);
  EXPECT_EQ(readInteger<int8_t>(input), 1);
  EXPECT_EQ(readInteger<int8_t>(input), 2);
  EXPECT_EQ(readInteger<int8_t>(input), 3);
  EXPECT_EQ(readInteger<int8_t>(input), 4);
}

TEST(StreamReaders, ReadInt16)
{
  char acArray[9] = "\x01\x00\x02\x00\x03\x00\x04\x00";
  boost::iostreams::array_source source{acArray, sizeof(acArray)};
  boost::iostreams::stream<boost::iostreams::array_source> input{source};

  EXPECT_EQ(readInteger<int16_t>(input), 1);
  EXPECT_EQ(readInteger<int16_t>(input), 2);
  EXPECT_EQ(readInteger<int16_t>(input), 3);
  EXPECT_EQ(readInteger<int16_t>(input), 4);
}

TEST(StreamReaders, ReadInt32)
{
  char acArray[17] = "\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00\x04\x00\x00\x00";
  boost::iostreams::array_source source{acArray, sizeof(acArray)};
  boost::iostreams::stream<boost::iostreams::array_source> input{source};

  EXPECT_EQ(readInteger<int32_t>(input), 1);
  EXPECT_EQ(readInteger<int32_t>(input), 2);
  EXPECT_EQ(readInteger<int32_t>(input), 3);
  EXPECT_EQ(readInteger<int32_t>(input), 4);
}

TEST(StreamReaders, ReadString)
{
  std::istringstream input("abcd space UpPeRcAsE");

  EXPECT_EQ(readString<4>(input), "abcd");
  EXPECT_EQ(readString<6>(input), " space");
}

