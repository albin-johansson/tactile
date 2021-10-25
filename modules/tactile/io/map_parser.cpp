#include "map_parser.hpp"

#include <filesystem>  // absolute

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

    mData = IO::ParseMap(path, &mError);

    TACTILE_PROFILE_END("Parsed map");
  }
  catch (...) {
    mError = IO::ParseError::Unknown;
  }
}

}  // namespace Tactile::IO
