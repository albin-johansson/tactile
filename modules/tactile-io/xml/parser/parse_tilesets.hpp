#pragma once

#include <filesystem>  // path

#include <pugixml.hpp>  // xml_node

#include "parse_error.hpp"
#include "tactile_io.hpp"

namespace tactile::IO {

[[nodiscard]] auto ParseTilesets(pugi::xml_node root,
                                 Map& map,
                                 const std::filesystem::path& dir) -> ParseError;

}  // namespace tactile::IO
