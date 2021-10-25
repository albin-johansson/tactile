#include "save_json.hpp"

#include <fstream>  // ofstream
#include <iomanip>  // setw

namespace Tactile::IO {

void SaveJson(const JSON& json, const std::filesystem::path& path, const bool indent)
{
  std::ofstream stream{path};

  if (indent) {
    stream << std::setw(2);
  }

  stream << json;
}

}  // namespace Tactile::IO
