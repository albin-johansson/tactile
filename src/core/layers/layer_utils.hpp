#pragma once

#include "layer.hpp"
#include "smart_pointers.hpp"

namespace tactile::core {

class TileLayer;
class ObjectLayer;

[[nodiscard]] auto AsTileLayer(Shared<ILayer>& layer) -> TileLayer*;

[[nodiscard]] auto AsTileLayer(const Shared<ILayer>& layer) -> const TileLayer*;

[[nodiscard]] auto AsObjectLayer(Shared<ILayer>& layer) -> ObjectLayer*;

[[nodiscard]] auto AsObjectLayer(const Shared<ILayer>& layer)
    -> const ObjectLayer*;

}  // namespace tactile::core
