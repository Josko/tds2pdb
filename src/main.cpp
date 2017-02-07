#include "tds2pdb.hpp"

#include <cstdint>
#include <iostream>

#include "file_handling.hpp"

/// Main entry point.
int32_t main(const int32_t argc, const char** argv)
{
  if (argc < 2)
  {
    std::cout << "Usage: "<< argv[0] << " <.tds file>\n";
    return EXIT_FAILURE;
  }

  boost::filesystem::path tds_file, binary_file, pdb_file;
  std::tie(tds_file, binary_file, binary_file) = get_files(argv[1]);

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
