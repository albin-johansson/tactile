#pragma once

#include "aliases/shared.hpp"
#include "object_layer.hpp"
#include "tile_layer.hpp"

namespace tactile {

[[nodiscard]] auto AsTileLayer(SharedLayer& layer) -> TileLayer*;

[[nodiscard]] auto AsTileLayer(const SharedLayer& layer) -> const TileLayer*;

[[nodiscard]] auto AsObjectLayer(SharedLayer& layer) -> ObjectLayer*;

[[nodiscard]] auto AsObjectLayer(const SharedLayer& layer)
    -> const ObjectLayer*;

}  // namespace tactile
