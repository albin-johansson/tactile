#include "layer_utils.hpp"

#include "object_layer.hpp"
#include "tile_layer.hpp"

namespace tactile::core {

auto as_tile_layer(shared<layer>& layer) -> tile_layer*
{
  if (layer->type() != core::layer_type::tile_layer) {
    return nullptr;
  } else {
    return dynamic_cast<tile_layer*>(layer.get());
  }
}

auto as_tile_layer(const shared<layer>& layer) -> const tile_layer*
{
  if (layer->type() != core::layer_type::tile_layer) {
    return nullptr;
  } else {
    return dynamic_cast<const tile_layer*>(layer.get());
  }
}

auto as_object_layer(shared<layer>& layer) -> object_layer*
{
  if (layer->type() != core::layer_type::object_layer) {
    return nullptr;
  } else {
    return dynamic_cast<object_layer*>(layer.get());
  }
}

auto as_object_layer(const shared<layer>& layer) -> const object_layer*
{
  if (layer->type() != core::layer_type::object_layer) {
    return nullptr;
  } else {
    return dynamic_cast<const object_layer*>(layer.get());
  }
}

}  // namespace tactile::core
