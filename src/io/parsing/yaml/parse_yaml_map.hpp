#pragma once

#include <filesystem>  // path

#include "common/expected.hpp"
#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseYamlMap(const std::filesystem::path& path)
    -> Expected<MapData, ParseError>;

}  // namespace Tactile::IO
