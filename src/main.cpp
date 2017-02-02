#include "tds2pdb.hpp"

#include <cstdint>
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>

/// Main entry point.
int32_t main(const int32_t argc, const char** argv)
{
  if (argc < 2)
  {
    std::cout << "Usage: "<< argv[0] << " <.tds file>\n";
    return EXIT_FAILURE;
  }

  const boost::filesystem::path tds_file(argv[1]);
  const boost::filesystem::path pdb_file = tds_file.parent_path() / tds_file.stem().remove_trailing_separator().replace_extension(".pdb");
  boost::filesystem::path binary_file = tds_file.parent_path() / tds_file.stem().remove_trailing_separator().replace_extension(".exe");

  // If the .exe file does not exist, try a .dll file instead.
  if (boost::filesystem::exists(binary_file) == false)
  {
    binary_file.replace_extension(".dll");

    if (boost::filesystem::exists(binary_file) == false)
    {
      std::cout << "Error: there are no .dll or .exe files with the stem " << binary_file.stem().generic_string() << '\n';
      return EXIT_FAILURE;
    }
  }

  // auto tds_format = parse_tds(tds_file);

  return EXIT_SUCCESS;
}
