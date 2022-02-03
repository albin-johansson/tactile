#pragma once

#include <filesystem>  // path

#include <nlohmann/json_fwd.hpp>

#include "io/maps/ir.hpp"
#include "io/maps/parser/parse_error.hpp"

namespace tactile::parsing {

[[nodiscard]] auto parse_tilesets(const nlohmann::json& json,
                                  ir::map_data& mapData,
                                  const std::filesystem::path& dir) -> parse_error;

}  // namespace tactile::parsing
