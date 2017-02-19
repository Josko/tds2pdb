#include "tds_parser.hpp"
#include "file_handling.hpp"

#include <iostream>

/// @brief Parses out the available information out of the TDS file.
///
/// @param tds_file TDS file to be processed.
///
/// @return Object will all of the data stored.
///
/// @throw std::runtime_error in the event of an parsing error.
TdsFormat parse_tds(const fs::path tds_file)
{
  TdsFormat tds_format;

  try
  {
    BinaryFile tds(fs::canonical(tds_file).generic_string().c_str());

    const auto signature = read_string<4>(tds());
    if (signature != "FB0A")
      throw std::runtime_error("Wrong signature found! [" + signature + "]");

    const auto offset = read_numeric<int32_t>(tds());
    tds().seekg(offset);

    const auto dir_header_size = read_numeric<int16_t>(tds());
    assert(dir_header_size == 16);

    const auto dir_entry_size = read_numeric<int16_t>(tds());
    assert(dir_entry_size == 12);
  }
  catch (const std::runtime_error& e)
  {
    std::cout << "Failed parsing the TDS file: " << e.what() << '\n';
    throw std::runtime_error(e.what());
  }

  return tds_format;
}
