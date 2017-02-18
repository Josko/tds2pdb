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
  const std::array<char, 6> in {0x00, 0x01, 0x02, 0x03, 0x04, 0x00};
  const boost::iostreams::array_source source {in.data(), in.size()};
  boost::iostreams::stream<boost::iostreams::array_source> input{source};

  EXPECT_EQ(readNumeric<int8_t>(input), 0);
  EXPECT_EQ(readNumeric<int8_t>(input), 1);
  EXPECT_EQ(readNumeric<int8_t>(input), 2);
  EXPECT_EQ(readNumeric<int8_t>(input), 3);
  EXPECT_EQ(readNumeric<int8_t>(input), 4);
}

TEST(StreamReaders, ReadNumericPreservesSignedness)
{
  const std::array<char, 2> in {static_cast<char>(0xFF), static_cast<char>(0XFF)};
  const boost::iostreams::array_source source {in.data(), in.size()};
  boost::iostreams::stream<boost::iostreams::array_source> input{source};

  EXPECT_EQ(readNumeric<int8_t>(input), -1);
  EXPECT_EQ(readNumeric<uint8_t>(input), 0xFF);
}

TEST(StreamReaders, ReadInt16)
{
  const std::array<char, 10> in {0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00};
  const boost::iostreams::array_source source {in.data(), in.size()};
  boost::iostreams::stream<boost::iostreams::array_source> input{source};

  EXPECT_EQ(readNumeric<int16_t>(input), 1);
  EXPECT_EQ(readNumeric<int16_t>(input), 2);
  EXPECT_EQ(readNumeric<int16_t>(input), 3);
  EXPECT_EQ(readNumeric<int16_t>(input), 4);
}

TEST(StreamReaders, ReadInt32)
{
  const std::array<char, 17> in {0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00};
  const boost::iostreams::array_source source {in.data(), in.size()};
  boost::iostreams::stream<boost::iostreams::array_source> input{source};

  EXPECT_EQ(readNumeric<int32_t>(input), 1);
  EXPECT_EQ(readNumeric<int32_t>(input), 2);
  EXPECT_EQ(readNumeric<int32_t>(input), 3);
  EXPECT_EQ(readNumeric<int32_t>(input), 4);
}

TEST(StreamReaders, ReadInt64)
{
  const std::array<char, 8> in {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  const boost::iostreams::array_source source {in.data(), in.size()};
  boost::iostreams::stream<boost::iostreams::array_source> input{source};

  EXPECT_EQ(readNumeric<uint64_t>(input), 1);
}

TEST(StreamReaders, ReadNumericPastEnding)
{
  const std::array<char, 1> in {0x00};
  const boost::iostreams::array_source source {in.data(), in.size()};
  boost::iostreams::stream<boost::iostreams::array_source> input{source};

  ASSERT_THROW({
    readNumeric<int64_t>(input);
  }, std::runtime_error);
}

TEST(StreamReaders, ReadStringPastEnding)
{
  const std::array<char, 1> in {0x00};
  const boost::iostreams::array_source source {in.data(), in.size()};
  boost::iostreams::stream<boost::iostreams::array_source> input{source};

  ASSERT_THROW({
    readString<5>(input);
  }, std::runtime_error);
}

TEST(StreamReaders, ReadString)
{
  std::istringstream input("abcd space UpPeRcAsE");

  EXPECT_EQ(readString<4>(input), "abcd");
  EXPECT_EQ(readString<6>(input), " space");
  EXPECT_EQ(readString<1>(input), " ");
  EXPECT_EQ(readString<9>(input), "UpPeRcAsE");
}

