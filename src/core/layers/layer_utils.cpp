#include "layer_utils.hpp"

#include "object_layer.hpp"
#include "tile_layer.hpp"

namespace tactile::core {

auto as_tile_layer(shared<ILayer>& layer) -> TileLayer*
{
  if (layer->Type() != LayerType::tile_layer)
  {
    return nullptr;
  }
  else
  {
    return dynamic_cast<TileLayer*>(layer.get());
  }
}

auto as_tile_layer(const shared<ILayer>& layer) -> const TileLayer*
{
  if (layer->Type() != LayerType::tile_layer)
  {
    return nullptr;
  }
  else
  {
    return dynamic_cast<const TileLayer*>(layer.get());
  }
}

auto as_object_layer(shared<ILayer>& layer) -> ObjectLayer*
{
  if (layer->Type() != LayerType::object_layer)
  {
    return nullptr;
  }
  else
  {
    return dynamic_cast<ObjectLayer*>(layer.get());
  }
}

auto as_object_layer(const shared<ILayer>& layer) -> const ObjectLayer*
{
  if (layer->Type() != LayerType::object_layer)
  {
    return nullptr;
  }
  else
  {
    return dynamic_cast<const ObjectLayer*>(layer.get());
  }
}

}  // namespace tactile::core
