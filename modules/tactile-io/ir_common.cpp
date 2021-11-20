#include "ir_common.hpp"

#include <filesystem>  // exists, path, is_regular_file

namespace Tactile::IO {

auto DeduceFormat(const CPathStr str) -> MapFormat
{
  if (!str) {
    return MapFormat::Unsupported;
  }

  const std::filesystem::path path = str;
  if (path.empty() ||
      (std::filesystem::exists(path) && !std::filesystem::is_regular_file(path)))
  {
    return MapFormat::Unsupported;
  }

  const auto ext = path.extension();
  if (ext == ".yaml" || ext == ".yml") {
    return MapFormat::Yaml;
  }
  else if (ext == ".xml" || ext == ".tmx") {
    return MapFormat::Xml;
  }
  else if (ext == ".json") {
    return MapFormat::Json;
  }
  else {
    return MapFormat::Unsupported;
  }
}

}  // namespace Tactile::IO