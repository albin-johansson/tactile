#include "layer_utils.hpp"

#include "object_layer.hpp"
#include "tile_layer.hpp"

namespace tactile::core {

auto AsTileLayer(shared<ILayer>& layer) -> TileLayer*
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

auto AsTileLayer(const shared<ILayer>& layer) -> const TileLayer*
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

auto AsObjectLayer(shared<ILayer>& layer) -> ObjectLayer*
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

auto AsObjectLayer(const shared<ILayer>& layer) -> const ObjectLayer*
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
