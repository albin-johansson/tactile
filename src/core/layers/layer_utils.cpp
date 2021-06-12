#include "layer_utils.hpp"

#include "object_layer.hpp"
#include "tile_layer.hpp"

namespace tactile::core {

auto AsTileLayer(Shared<ILayer>& layer) -> TileLayer*
{
  if (layer->Type() != LayerType::TileLayer)
  {
    return nullptr;
  }
  else
  {
    return dynamic_cast<TileLayer*>(layer.get());
  }
}

auto AsTileLayer(const Shared<ILayer>& layer) -> const TileLayer*
{
  if (layer->Type() != LayerType::TileLayer)
  {
    return nullptr;
  }
  else
  {
    return dynamic_cast<const TileLayer*>(layer.get());
  }
}

auto AsObjectLayer(Shared<ILayer>& layer) -> ObjectLayer*
{
  if (layer->Type() != LayerType::ObjectLayer)
  {
    return nullptr;
  }
  else
  {
    return dynamic_cast<ObjectLayer*>(layer.get());
  }
}

auto AsObjectLayer(const Shared<ILayer>& layer) -> const ObjectLayer*
{
  if (layer->Type() != LayerType::ObjectLayer)
  {
    return nullptr;
  }
  else
  {
    return dynamic_cast<const ObjectLayer*>(layer.get());
  }
}

}  // namespace tactile::core
