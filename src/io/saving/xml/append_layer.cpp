#include "append_layer.hpp"

#include "core/map/layers/object_layer.hpp"
#include "core/map/layers/tile_layer.hpp"

namespace Tactile::IO {
namespace {

void AppendTileLayer(const TileLayer& layer,
                     pugi::xml_node& mapNode,
                     const std::filesystem::path& dir)
{
  // TODO
}

void AppendObjectLayer(const ObjectLayer& layer,
                       pugi::xml_node& mapNode,
                       const std::filesystem::path& dir)
{
  // TODO
}

}  // namespace

void AppendLayer(const ILayer& layer,
                 pugi::xml_node& mapNode,
                 const std::filesystem::path& dir)
{
  if (layer.GetType() == LayerType::TileLayer)
  {
    const auto& tileLayer = dynamic_cast<const TileLayer&>(layer);
    AppendTileLayer(tileLayer, mapNode, dir);
  }
  else if (layer.GetType() == LayerType::ObjectLayer)
  {
    const auto& objectLayer = dynamic_cast<const ObjectLayer&>(layer);
    AppendObjectLayer(objectLayer, mapNode, dir);
  }
  else
  {
    CENTURION_LOG_WARN("Did not recognize layer type when saving as XML!");
  }
}

}  // namespace Tactile::IO
