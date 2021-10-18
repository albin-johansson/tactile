#pragma once

#include <filesystem>  // path

#include <tactile/io/maps/parse_error.hpp>
#include <tactile/io/maps/parse_ir.hpp>

#include "common/expected.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseJsonMap(const std::filesystem::path& path)
    -> Expected<MapData, ParseError>;

}  // namespace Tactile::IO
