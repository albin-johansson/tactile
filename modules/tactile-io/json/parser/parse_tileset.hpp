#pragma once

#include <filesystem>  // path

#include <expected.hpp>  // expected

#include "../json_common.hpp"
#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileset(const JSON& json, const std::filesystem::path& dir)
    -> tl::expected<TilesetData, ParseError>;

}  // namespace Tactile::IO
