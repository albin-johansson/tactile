#pragma once

#include <filesystem>  // path

#include "common/expected.hpp"
#include "io/json.hpp"
#include "parsers/parse_error.hpp"
#include "parsers/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileset(const JSON& json, const std::filesystem::path& dir)
    -> Expected<TilesetData, ParseError>;

}  // namespace Tactile::IO
