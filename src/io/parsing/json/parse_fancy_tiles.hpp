#pragma once

#include <filesystem>  // path

#include "aliases/json.hpp"
#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseFancyTiles(const JSON& json,
                                   TilesetData& data,
                                   const std::filesystem::path& dir) -> ParseError;

}  // namespace Tactile::IO
