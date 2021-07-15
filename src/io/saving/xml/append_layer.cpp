#include "append_layer.hpp"

#include <sstream>  // stringstream

#include "core/map/layers/object_layer.hpp"
#include "core/map/layers/tile_layer.hpp"
#include "io/preferences.hpp"
#include "io/saving/xml/append_properties.hpp"

namespace Tactile::IO {
namespace {

void AppendTileLayer(const layer_id id,
                     const TileLayer& layer,
                     pugi::xml_node& mapNode,
                     const std::filesystem::path& dir)
{
  auto node = mapNode.append_child("layer");
  node.append_attribute("id").set_value(id.get());
  node.append_attribute("name").set_value(layer.GetName().data());
  node.append_attribute("width").set_value(layer.GetColumnCount().get());
  node.append_attribute("height").set_value(layer.GetRowCount().get());

  AppendProperties(layer, node, dir);

  auto data = node.append_child("data");
  data.append_attribute("encoding").set_value("csv");

  const auto count = layer.GetTileCount();
  const auto nCols = layer.GetColumnCount().get();
  const auto readable = Prefs::GetHumanReadableOutput();

  std::stringstream stream;
  usize index = 0;
  layer.Each([&](const tile_id tile) {
    if (readable && index == 0)
    {
      stream << '\n';
    }

    stream << tile.get();
    if (index < count - 1)
    {
      stream << ',';
    }

    if (readable && (index + 1) % nCols == 0)
    {
      stream << '\n';
    }

    ++index;
  });

  const auto tileData = stream.str();
  data.text().set(tileData.c_str());
}

void AppendObjectLayer(const layer_id layerId,
                       const ObjectLayer& layer,
                       pugi::xml_node& mapNode,
                       const std::filesystem::path& dir)
{
  auto node = mapNode.append_child("objectgroup");
  node.append_attribute("id").set_value(layerId.get());
  node.append_attribute("name").set_value(layer.GetName().data());

  AppendProperties(layer, node, dir);

  for (const auto& [id, object] : layer)
  {
    auto objNode = node.append_child("object");
    objNode.append_attribute("id").set_value(id.get());

    if (!object.GetName().empty())
    {
      objNode.append_attribute("name").set_value(object.GetName().data());
    }

    if (const auto type = object.GetCustomType())
    {
      objNode.append_attribute("type").set_value(type->c_str());
    }

    objNode.append_attribute("x").set_value(object.GetX());
    objNode.append_attribute("y").set_value(object.GetY());

    if (object.GetWidth() != 0)
    {
      objNode.append_attribute("width").set_value(object.GetWidth());
    }

    if (object.GetHeight() != 0)
    {
      objNode.append_attribute("height").set_value(object.GetHeight());
    }

    if (!object.IsVisible())
    {
      objNode.append_attribute("visible").set_value(0);
    }
  }
}

}  // namespace

void AppendLayer(const layer_id id,
                 const ILayer& layer,
                 pugi::xml_node& mapNode,
                 const std::filesystem::path& dir)
{
  if (layer.GetType() == LayerType::TileLayer)
  {
    const auto& tileLayer = dynamic_cast<const TileLayer&>(layer);
    AppendTileLayer(id, tileLayer, mapNode, dir);
  }
  else if (layer.GetType() == LayerType::ObjectLayer)
  {
    const auto& objectLayer = dynamic_cast<const ObjectLayer&>(layer);
    AppendObjectLayer(id, objectLayer, mapNode, dir);
  }
  else
  {
    CENTURION_LOG_WARN("Did not recognize layer type when saving as XML!");
  }
}

}  // namespace Tactile::IO
