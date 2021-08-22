#include "read_json.hpp"

#include <fstream>  // ifstream

namespace Tactile {

auto ReadJson(const std::filesystem::path& path) -> JSON
{
  std::ifstream stream{path};

  JSON json;
  stream >> json;

  return json;
}

}  // namespace Tactile
