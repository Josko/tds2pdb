#ifndef FILE_HANDLING_HPP
#define FILE_HANDLING_HPP

#include <tuple>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

inline std::tuple<fs::path, fs::path, fs::path> get_files(const char* file_path)
{
  fs::path tds_file(file_path);
  fs::path pdb_file = tds_file.parent_path() / tds_file.stem().remove_trailing_separator().replace_extension(".pdb");
  fs::path binary_file = tds_file.parent_path() / tds_file.stem().remove_trailing_separator().replace_extension(".exe");
  return std::make_tuple(tds_file, binary_file, pdb_file);
}

#endif
