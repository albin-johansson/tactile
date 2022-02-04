#pragma once

#include <filesystem>  // path

#include "io/maps/parser/parse_data.hpp"

namespace tactile::parsing {

[[nodiscard]] auto parse_yaml_map(const std::filesystem::path& path) -> parse_data;

}  // namespace tactile::parsing
