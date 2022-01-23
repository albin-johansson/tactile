#pragma once

#include <pugixml.hpp>  // xml_node

#include "parse_error.hpp"
#include "tactile_io.hpp"

namespace tactile::IO {

[[nodiscard]] auto ParseProperties(pugi::xml_node node, Map& map) -> ParseError;
[[nodiscard]] auto ParseProperties(pugi::xml_node node, Tileset& tileset) -> ParseError;
[[nodiscard]] auto ParseProperties(pugi::xml_node node, Tile& tile) -> ParseError;
[[nodiscard]] auto ParseProperties(pugi::xml_node node, Layer& layer) -> ParseError;
[[nodiscard]] auto ParseProperties(pugi::xml_node node, Object& object) -> ParseError;

}  // namespace tactile::IO
