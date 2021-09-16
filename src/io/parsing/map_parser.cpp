#include "map_parser.hpp"

#include <filesystem>  // absolute

#include "io/parsing/json/parse_json_map.hpp"
#include "io/parsing/xml/parse_xml_map.hpp"
#include "utils/profile.hpp"

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
      mError = IO::ParseJsonMap(path, mData);
    }
    else if (extension == ".tmx") {
      mError = IO::ParseXmlMap(path, mData);
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
