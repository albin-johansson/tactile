#pragma once

#include "layer.hpp"
#include "smart_pointers.hpp"

namespace tactile::core {

class tile_layer;
class object_layer;

[[nodiscard]] auto as_tile_layer(shared<ILayer>& layer) -> tile_layer*;

[[nodiscard]] auto as_tile_layer(const shared<ILayer>& layer)
    -> const tile_layer*;

[[nodiscard]] auto as_object_layer(shared<ILayer>& layer) -> object_layer*;

[[nodiscard]] auto as_object_layer(const shared<ILayer>& layer)
    -> const object_layer*;

}  // namespace tactile::core
