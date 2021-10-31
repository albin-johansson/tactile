#include "map_parser.hpp"

#include <filesystem>  // absolute

#include <tactile-io/parser.hpp>

#include "core/utils/profile.hpp"

namespace Tactile {

MapParser::MapParser(const std::filesystem::path& path)
{
  cen::log::info("Parsing map %s", path.string().c_str());
  try {
    TACTILE_PROFILE_START;
    mData = IO::ParseMap(path, &mError);
    TACTILE_PROFILE_END("Parsed map");
  }
  catch (...) {
    mError = IO::ParseError::Unknown;
  }
}

}  // namespace Tactile
