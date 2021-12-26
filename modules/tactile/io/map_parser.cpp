#include "map_parser.hpp"

#include <filesystem>  // absolute

#include <tactile_io.hpp>

#include "core/utils/profile.hpp"
#include "logging.hpp"

namespace Tactile {

MapParser::MapParser(const std::filesystem::path& path)
{
  LogInfo("Parsing map {}", path.c_str());
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
