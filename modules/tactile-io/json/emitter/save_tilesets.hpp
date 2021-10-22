#pragma once

#include <filesystem>  // path

#include "../json_common.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

struct EmitterOptions;

[[nodiscard]] auto SaveTilesets(const Map& map,
                                const std::filesystem::path& dir,
                                const EmitterOptions& options) -> JSON;

}  // namespace Tactile::IO
