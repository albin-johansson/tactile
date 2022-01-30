#include "map_parser.hpp"

#include <exception>  // exception

#include "build.hpp"
#include "logging.hpp"
#include "profile.hpp"
#include "throw.hpp"
#include "yaml/yaml_parser.hpp"

namespace tactile::parsing {

void map_parser::parse_map(const std::filesystem::path& path)
{
  log_info("Parsing map {}", path);

  try {
    TACTILE_PROFILE_START

    // TODO determine extension and which parser to choose
    mData = parse_yaml_map(path);

    TACTILE_PROFILE_END("Parsed map")
  }
  catch (const std::exception& e) {
    log_error("Parser threw unhandled exception with message: '{}'\n", e.what());

    if constexpr (is_debug_build) {
      if (const auto* stacktrace = boost::get_error_info<trace_info>(e)) {
        print(fmt::color::orange, "{}\n", *stacktrace);
      }
    }

    mData.set_error(parse_error::unknown);
  }
  catch (...) {
    log_error("Parser threw non-exception value!");
    mData.set_error(parse_error::unknown);
  }
}

auto map_parser::data() const -> const parse_data&
{
  return mData;
}

auto map_parser::error() const -> parse_error
{
  return mData.error();
}

auto map_parser::is_okay() const -> bool
{
  return mData.error() == parse_error::none;
}

}  // namespace tactile::parsing