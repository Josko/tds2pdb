#include "tds_parser.hpp"
#include "file_handling.hpp"

#include <iostream>

TdsFormat parse_tds(const fs::path tds_file)
{
  TdsFormat tds_format;

  try
  {
    BinaryFile tds(fs::canonical(tds_file).generic_string().c_str());

    const auto signature = readString<4>(tds());
    if (signature != "FB0A")
      throw std::runtime_error("Wrong signature found! [" + signature + "]");
  }
  catch (const std::runtime_error& e)
  {
    std::cout << "Failed parsing the TDS file: " << e.what() << '\n';
    return tds_format;
  }

  return tds_format;
}
