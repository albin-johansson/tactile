#include "read_json.hpp"

#include <fstream>  // ifstream

namespace Tactile {

auto ReadJson(const std::filesystem::path& path) -> nlohmann::json
{
  std::ifstream stream{path};

  nlohmann::json json;
  stream >> json;

  return json;
}

}  // namespace Tactile
