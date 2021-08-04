#pragma once

#include "aliases/shared.hpp"
#include "group_layer.hpp"
#include "object_layer.hpp"
#include "tile_layer.hpp"

namespace Tactile {

[[nodiscard]] auto AsTileLayer(SharedLayer& layer) -> TileLayer*;

[[nodiscard]] auto AsTileLayer(const SharedLayer& layer) -> const TileLayer*;

[[nodiscard]] auto AsObjectLayer(SharedLayer& layer) -> ObjectLayer*;

[[nodiscard]] auto AsObjectLayer(const SharedLayer& layer) -> const ObjectLayer*;

[[nodiscard]] auto AsGroupLayer(SharedLayer& layer) -> GroupLayer*;

[[nodiscard]] auto AsGroupLayer(const SharedLayer& layer) -> const GroupLayer*;

}  // namespace Tactile
