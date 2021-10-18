#pragma once

#include <filesystem>  // path

#include <tactile/io/maps/parse_error.hpp>
#include <tactile/io/maps/parse_ir.hpp>

#include "common/expected.hpp"
#include "io/json.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileset(const JSON& json, const std::filesystem::path& dir)
    -> Expected<TilesetData, ParseError>;

}  // namespace Tactile::IO
