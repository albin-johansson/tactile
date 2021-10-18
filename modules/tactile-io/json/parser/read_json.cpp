#include "read_json.hpp"

#include <fstream>  // ifstream

namespace Tactile::IO {

auto ReadJson(const std::filesystem::path& path) -> std::optional<JSON>
{
  try {
    std::ifstream stream{path};

    JSON json;
    stream >> json;

    return json;
  }
  catch (...) {
    return std::nullopt;
  }
}

}  // namespace Tactile
