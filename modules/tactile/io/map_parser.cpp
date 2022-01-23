#include "map_parser.hpp"

#include <filesystem>  // absolute

#include <tactile_io.hpp>

#include "logging.hpp"
#include "profile.hpp"

namespace tactile {

MapParser::MapParser(const std::filesystem::path& path)
{
  LogInfo("Parsing map {}", path);
  try {
    TACTILE_PROFILE_START
    mData = IO::ParseMap(path, &mError);
    TACTILE_PROFILE_END("Parsed map")
  }
  catch (...) {
    mError = IO::ParseError::Unknown;
  }
}

}  // namespace tactile
