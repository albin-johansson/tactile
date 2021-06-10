#pragma once

#include "layer.hpp"
#include "smart_pointers.hpp"

namespace tactile::core {

class TileLayer;
class ObjectLayer;

[[nodiscard]] auto as_tile_layer(shared<ILayer>& layer) -> TileLayer*;

[[nodiscard]] auto as_tile_layer(const shared<ILayer>& layer)
    -> const TileLayer*;

[[nodiscard]] auto as_object_layer(shared<ILayer>& layer) -> ObjectLayer*;

[[nodiscard]] auto as_object_layer(const shared<ILayer>& layer)
    -> const ObjectLayer*;

}  // namespace tactile::core
