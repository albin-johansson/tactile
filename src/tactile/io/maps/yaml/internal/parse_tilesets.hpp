#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include <tactile/io/maps/parse_error.hpp>
#include <tactile/io/maps/parse_ir.hpp>

#include "common.hpp"

#include <yaml-cpp/yaml.h>

namespace Tactile::IO {

[[nodiscard]] auto ParseTilesets(const YAML::Node& seq, const std::filesystem::path& dir)
    -> Expected<std::vector<TilesetData>, ParseError>;

}  // namespace Tactile::IO
