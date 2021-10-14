#pragma once

#include <filesystem>  // path

#include "common/expected.hpp"
#include "common/json.hpp"
#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileset(const JSON& json, const std::filesystem::path& dir)
    -> Expected<TilesetData, ParseError>;

}  // namespace Tactile::IO
