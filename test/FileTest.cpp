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