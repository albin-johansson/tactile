#pragma once

#include <filesystem>  // path

#include "common/expected.hpp"
#include "parsers/parse_error.hpp"
#include "parsers/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseJsonMap(const std::filesystem::path& path)
    -> Expected<MapData, ParseError>;

}  // namespace Tactile::IO
