#include "read_json.hpp"

#include <fstream>  // ifstream

namespace tactile::IO {

auto ReadJson(const std::filesystem::path& path) -> Maybe<JSON>
{
  try {
    std::ifstream stream{path};

    JSON json;
    stream >> json;

    return json;
  }
  catch (...) {
    return nothing;
  }
}

}  // namespace tactile::IO
