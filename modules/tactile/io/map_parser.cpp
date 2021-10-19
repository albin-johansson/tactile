#include "map_parser.hpp"

#include <filesystem>  // absolute

#include <tactile-io/parser.hpp>

#include "core/utils/profile.hpp"
#include "parsers/xml/parse_xml_map.hpp"

namespace Tactile::IO {

MapParser::MapParser(const std::filesystem::path& path)
{
  try {
    TACTILE_PROFILE_START;

    CENTURION_LOG_INFO("Parsing map at \"%s\"...", path.string().c_str());
    mData.absolute_path = std::filesystem::absolute(path);

    if (!std::filesystem::exists(mData.absolute_path)) {
      mError = ParseError::MapDoesNotExist;
      return;
    }

    const auto extension = path.extension();
    if (extension == ".json") {
      if (auto data = IO::ParseJsonMap(path, &mError)) {
        mData = std::move(*data);
      }
    }
    else if (extension == ".tmx" || extension == ".xml") {
      mError = IO::ParseXmlMap(path, mData);
    }
    else if (extension == ".yaml" || extension == ".yml") {
      if (auto data = IO::ParseYamlMap(path, &mError)) {
        mData = std::move(*data);
      }
    }
    else {
      mError = ParseError::MapUnsupportedExtension;
    }

    TACTILE_PROFILE_END("Parsed map");
  }
  catch (...) {
    mError = ParseError::Unknown;
  }
}

}  // namespace Tactile::IO
