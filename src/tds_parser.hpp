#ifndef TDS_PARSER_HPP
#define TDS_PARSER_HPP

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

class TdsFormat
{
private:

public:
  TdsFormat() = default;
  ~TdsFormat() = default;
};

TdsFormat parse_tds(const fs::path tds_file);

#endif
