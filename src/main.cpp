#include "tds2pdb.hpp"

#include <cstdint>
#include <iostream>

#include <boost/program_options.hpp>

#include "file_handling.hpp"
#include "tds_parser.hpp"

namespace po = boost::program_options;

/// \file

/// @brief Main entry point. Users specifies the files to be processed via the arguments.
int32_t main(const int32_t argc, const char** argv)
{
  std::string input_file;
  // Option parsing.
  po::options_description desc("Generic options");
  desc.add_options()
    ("input-file,i", po::value<std::string>(&input_file)->required(), "input file")
    ("input-folder,f", po::value<std::string>(), "input folder")
    ("verbose,v", "verbose output")
    ("version", "print version string")
    ("help", "produce help message");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);

  if (vm.count("help"))
  {
    desc.print(std::cout, 40);
    return EXIT_SUCCESS;
  }
  else if (vm.count("version"))
  {
    std::cout << argv[0] << " version: " << TDS2PDB_VERSION_MAJOR << "." << TDS2PDB_VERSION_MINOR << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  // Beginning of useful work.
  boost::filesystem::path tds_file, binary_file, pdb_file;
  std::tie(tds_file, binary_file, pdb_file) = get_files(input_file.c_str());

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

  const auto tds_format = parse_tds(tds_file);

  return EXIT_SUCCESS;
}
