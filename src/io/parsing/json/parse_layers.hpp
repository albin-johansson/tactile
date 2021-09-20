#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include "aliases/json.hpp"
#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseLayers(const JSON& json,
                               std::vector<LayerData>& layers,
                               const std::filesystem::path& dir) -> ParseError;

}  // namespace Tactile::IO
