#pragma once

#include <filesystem>  // path

#include "../json_common.hpp"
#include "tactile_io.hpp"

namespace tactile::IO {

[[nodiscard]] auto SaveLayers(const Map& map, const std::filesystem::path& dir) -> JSON;

}  // namespace tactile::IO
