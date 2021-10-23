#include "map_parser.hpp"

#include <filesystem>  // absolute
#include <utility>     // move

#include <tactile-io/parser.hpp>

#include "core/utils/profile.hpp"

namespace Tactile {

MapParser::MapParser(const std::filesystem::path& path)
{
  try {
    TACTILE_PROFILE_START;

    CENTURION_LOG_INFO("Parsing map at \"%s\"...", path.string().c_str());
    const auto absolute = std::filesystem::absolute(path);

    if (!std::filesystem::exists(absolute)) {
      mError = IO::ParseError::MapDoesNotExist;
      return;
    }

    const auto extension = path.extension();
    if (extension == ".json") {
      if (auto data = IO::ParseJsonMap(path, &mError)) {
        mData = std::move(data);
      }
    }
    else if (extension == ".tmx" || extension == ".xml") {
      if (auto data = IO::ParseXmlMap(path, &mError)) {
        mData = std::move(data);
      }
    }
    else if (extension == ".yaml" || extension == ".yml") {
      if (auto data = IO::ParseYamlMap(path, &mError)) {
        mData = std::move(data);
      }
    }
    else {
      mError = IO::ParseError::MapUnsupportedExtension;
    }

    TACTILE_PROFILE_END("Parsed map");
  }
  catch (...) {
    mError = IO::ParseError::Unknown;
  }
}

}  // namespace Tactile::IO
