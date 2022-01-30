#pragma once

#include <filesystem>  // path

#include <yaml-cpp/yaml.h>

#include "io/maps/parser/parse_data.hpp"
#include "tactile_def.hpp"

namespace tactile::parsing {

[[nodiscard]] auto parse_tilesets(const YAML::Node& sequence,
                                  map_data& data,
                                  const std::filesystem::path& dir) -> parse_error;

}  // namespace tactile::parsing
