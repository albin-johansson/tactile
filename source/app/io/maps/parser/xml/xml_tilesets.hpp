#pragma once

#include <filesystem>  // path

#include <pugixml.hpp>

#include "io/maps/parser/parse_data.hpp"

namespace tactile::parsing {

[[nodiscard]] auto parse_tileset(pugi::xml_node node,
                                 ir::tileset_data& tilesetData,
                                 const std::filesystem::path& dir) -> parse_error;

}  // namespace tactile::parsing