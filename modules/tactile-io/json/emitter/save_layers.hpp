#pragma once

#include <filesystem>  // path

#include "../json_common.hpp"
#include "ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveLayers(const Map& map, const std::filesystem::path& dir) -> JSON;

}  // namespace Tactile::IO
