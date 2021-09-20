#pragma once

#include <filesystem>  // path

#include "aliases/json.hpp"
#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseObjectLayer(const JSON& json,
                                    LayerData& layer,
                                    const std::filesystem::path& dir) -> ParseError;

}  // namespace Tactile::IO
