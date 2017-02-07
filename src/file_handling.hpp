#ifndef FILE_HANDLING_HPP
#define FILE_HANDLING_HPP

#include <tuple>

#include <boost/filesystem.hpp>

inline std::tuple<boost::filesystem::path, boost::filesystem::path, boost::filesystem::path> get_files(const char* file_path)
{
  boost::filesystem::path tds_file(file_path);
  boost::filesystem::path pdb_file = tds_file.parent_path() / tds_file.stem().remove_trailing_separator().replace_extension(".pdb");
  boost::filesystem::path binary_file = tds_file.parent_path() / tds_file.stem().remove_trailing_separator().replace_extension(".exe");
  return std::make_tuple(tds_file, binary_file, pdb_file);
}

#endif
