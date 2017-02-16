#ifndef FILE_HANDLING_HPP
#define FILE_HANDLING_HPP

#include <tuple>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

/// Wrapper class that helps with file RAII and
/// helper methods for reading certain types.
class BinaryFile final
{
private:
  std::ifstream in_stream;

public:
  /// Constructs and acquired a stream to read the file.
  explicit BinaryFile(const char* file_name)
  {
    in_stream.open(file_name, std::ifstream::binary);

    if (!in_stream.is_open())
      throw std::runtime_error("Error opening binary file: " + std::string(file_name));
  }

  ~BinaryFile()
  {
    if (in_stream.is_open())
      in_stream.close();
  }

  inline std::istream& operator()()
  {
    return in_stream;
  }
};

/// Signed integer implementation.
template<typename T, typename = std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>>
inline T readInteger(std::istream& in_stream)
{
  if (!in_stream)
    throw std::runtime_error("Reading signed integer value failed!");

  T buf;
  in_stream.read(reinterpret_cast<char*>(&buf), sizeof(T));
  return buf;
}

/// Unsigned integer implementation.
template<typename T, typename = std::enable_if<std::is_integral<T>::value && std::is_unsigned<T>::value>>
inline T readUInteger(std::istream& in_stream)
{
  if (!in_stream)
    throw std::runtime_error("Reading unsigned integer value failed!");

  std::array<char, std::numeric_limits<T>::digits10 + 1> buffer{};
  in_stream.read(buffer, sizeof(T));
  return static_cast<T>(strtoul(buffer, nullptr, 10));
}

/// Floating point implementation.
template<typename T, typename = std::enable_if<std::is_floating_point<T>::value>>
inline T readDouble(std::istream& in_stream)
{
  if (!in_stream)
    throw std::runtime_error("Reading floating point value failed!");

  std::array<char, std::numeric_limits<T>::digits10 + 1> buffer{};
  in_stream.read(buffer, sizeof(T));
  return atof(buffer);
}

/// String implementation.
template<const std::size_t N>
inline std::string readString(std::istream& in_stream)
{
  if (!in_stream)
    throw std::runtime_error("Reading string value failed!");

  std::array<char, N + 1> buffer{};
  in_stream.read(buffer.data(), N);
  return std::string(buffer.data());
}

/// Helper function for determining the path trio:
/// Path of the TDS file (file we convert from)
/// Path of the EXE/DLL file (file we convert for)
/// Path of the PDB file (file we conver to)
inline std::tuple<fs::path, fs::path, fs::path> get_files(const char* file_path)
{
  auto base_path = fs::path(file_path);
  fs::path tds_file = base_path.parent_path() / base_path.stem().remove_trailing_separator().replace_extension(".tds");
  fs::path pdb_file = tds_file.parent_path() / tds_file.stem().remove_trailing_separator().replace_extension(".pdb");
  fs::path binary_file = tds_file.parent_path() / tds_file.stem().remove_trailing_separator().replace_extension(".exe");
  return std::make_tuple(tds_file, binary_file, pdb_file);
}

#endif
