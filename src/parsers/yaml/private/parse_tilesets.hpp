#pragma once

#include <yaml-cpp/yaml.h>

#include <filesystem>  // path
#include <vector>      // vector

#include "common.hpp"
#include "parsers/parse_error.hpp"
#include "parsers/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTilesets(const YAML::Node& seq, const std::filesystem::path& dir)
    -> Expected<std::vector<TilesetData>, ParseError>;

}  // namespace Tactile::IO
