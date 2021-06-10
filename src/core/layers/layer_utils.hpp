#pragma once

#include "layer.hpp"
#include "smart_pointers.hpp"

namespace tactile::core {

class TileLayer;
class ObjectLayer;

[[nodiscard]] auto AsTileLayer(shared<ILayer>& layer) -> TileLayer*;

[[nodiscard]] auto AsTileLayer(const shared<ILayer>& layer) -> const TileLayer*;

[[nodiscard]] auto AsObjectLayer(shared<ILayer>& layer) -> ObjectLayer*;

[[nodiscard]] auto AsObjectLayer(const shared<ILayer>& layer)
    -> const ObjectLayer*;

}  // namespace tactile::core
