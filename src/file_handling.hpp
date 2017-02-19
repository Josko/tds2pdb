#ifndef FILE_HANDLING_HPP
#define FILE_HANDLING_HPP

#include <tuple>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

/// \file

/// @brief Wrapper class that helps with file RAII and
/// helper methods for reading certain types.
class BinaryFile final
{
private:
  std::ifstream in_stream;

public:
  /// @Brief Constructs and acquired a stream to read the file.
  ///
  /// @param file_name File path of the file to be opened.
  ///
  /// @throw std::runtime_error in the event that the file could not be opened.
  explicit BinaryFile(const char* file_name)
  {
    in_stream.open(file_name, std::ifstream::binary);

    if (!in_stream.is_open())
      throw std::runtime_error("Error opening binary file: " + std::string(file_name));
  }

  /// @brief Closes and releases he BinaryFile and
  /// it's associated resources.
  ~BinaryFile()
  {
    if (in_stream.is_open())
      in_stream.close();
  }

  /// @brief Conversion operator
  ///
  /// @return std::istream&
  inline std::istream& operator()()
  {
    return in_stream;
  }
};

/// @brief Reads the appropriate number of bytes from the input stream and returns them as
/// the numeric type specified.
///
/// @param in_stream Input stream to read from.
///
/// @return Extracted numeric value.
///
/// @throw std::runtime_error when the input stream is not in a valid state.
/// @throw std::runtime_error when the input stream has ran out of bytes.
template<typename T, typename = std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value>>
inline T read_numeric(std::istream& in_stream)
{
  if (!in_stream)
    throw std::runtime_error("Reading numeric value failed - input stream is not ok!");

  T buf;
  in_stream.read(reinterpret_cast<char*>(&buf), sizeof(T));

  if (in_stream.eof() && in_stream.fail())
    throw std::runtime_error("Reading numeric value failed - input stream ran out of data prematurely!");

  return buf;
}

/// @brief Reads the appropriate number of bytes from the input stream and returns them as
/// a string.
///
/// @param in_stream Input stream to read from.
///
/// @return Extracted string with the specified length.
///
/// @throw std::runtime_error when the input stream is not in a valid state or when then
/// the input stream has ran out of bytes.
template<const std::size_t N>
inline std::string read_string(std::istream& in_stream)
{
  if (!in_stream)
    throw std::runtime_error("Reading string value failed - input stream is not ok!");

  std::array<char, N + 1> buffer{};
  in_stream.read(buffer.data(), N);

  if (in_stream.eof() && in_stream.fail())
    throw std::runtime_error("Reading string value failed - input stream ran out of data prematurely!");

  return std::string(buffer.data());
}

/// @brif Helper function for determining the path trio.
///
/// @param file_path File path that specifies the stem of the taret files (Example: Stem of the file test.exe is just "test")
///
/// @return Tuple containing thee paths:
/// - Path of the TDS file (file we convert from):
/// - Path of the EXE/DLL file (file we convert for)
/// - Path of the PDB file (file we conver to)
inline std::tuple<fs::path, fs::path, fs::path> get_files(const char* file_path)
{
  auto base_path = fs::path(file_path);
  fs::path tds_file = base_path.parent_path() / base_path.stem().remove_trailing_separator().replace_extension(".tds");
  fs::path pdb_file = tds_file.parent_path() / tds_file.stem().remove_trailing_separator().replace_extension(".pdb");
  fs::path binary_file = tds_file.parent_path() / tds_file.stem().remove_trailing_separator().replace_extension(".exe");
  return std::make_tuple(tds_file, binary_file, pdb_file);
}

#endif
