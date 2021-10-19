#include "append_layer.hpp"

#include <sstream>  // stringstream
#include <variant>  // get

#include <tactile-base/tactile_error.hpp>

#include "append_properties.hpp"

namespace Tactile::IO {
namespace {

void AppendCommonLayerAttributes(pugi::xml_node node, const LayerData& layer)
{
  node.append_attribute("id").set_value(layer.id);
  node.append_attribute("name").set_value(layer.name.c_str());

  if (layer.opacity != 1.0f) {
    node.append_attribute("opacity").set_value(layer.opacity);
  }

  if (!layer.is_visible) {
    node.append_attribute("visible").set_value(0);
  }
}

void AppendTileLayer(pugi::xml_node mapNode,
                     const LayerData& layer,
                     const std::filesystem::path& dir)
{
  const auto& tileLayer = std::get<TileLayerData>(layer.data);

  auto node = mapNode.append_child("layer");
  AppendCommonLayerAttributes(node, layer);

  const auto nCols = tileLayer.tiles.at(0).size();
  const auto nRows = tileLayer.tiles.size();
  node.append_attribute("width").set_value(nCols);
  node.append_attribute("height").set_value(nRows);

  AppendProperties(node, layer.properties, dir);

  auto data = node.append_child("data");
  data.append_attribute("encoding").set_value("csv");

  const auto count = nRows * nCols;
  const auto readable = true;  // Prefs::GetHumanReadableOutput(); TODO

  std::stringstream stream;
  usize index = 0;

  for (const auto& row : tileLayer.tiles) {
    for (const auto tile : row) {
      if (readable && index == 0) {
        stream << '\n';
      }

      stream << tile.get();
      if (index < count - 1) {
        stream << ',';
      }

      if (readable && (index + 1) % nCols == 0) {
        stream << '\n';
      }

      ++index;
    }
  }

  const auto tileData = stream.str();
  data.text().set(tileData.c_str());
}

void AppendObject(pugi::xml_node layerNode,
                  const ObjectData& object,
                  const std::filesystem::path& dir)
{
  auto node = layerNode.append_child("object");
  node.append_attribute("id").set_value(object.id);

  if (!object.name.empty()) {
    node.append_attribute("name").set_value(object.name.c_str());
  }

  if (!object.custom_type.empty()) {
    node.append_attribute("type").set_value(object.custom_type.c_str());
  }

  node.append_attribute("x").set_value(object.x);
  node.append_attribute("y").set_value(object.y);

  if (object.width != 0) {
    node.append_attribute("width").set_value(object.width);
  }

  if (object.height != 0) {
    node.append_attribute("height").set_value(object.height);
  }

  if (!object.visible) {
    node.append_attribute("visible").set_value(0);
  }

  if (object.type == ObjectType::Point) {
    node.append_child("point");
  }
  else if (object.type == ObjectType::Ellipse) {
    node.append_child("ellipse");
  }

  AppendProperties(node, object.properties, dir);
}

void AppendObjectLayer(pugi::xml_node mapNode,
                       const LayerData& layer,
                       const std::filesystem::path& dir)
{
  const auto& objectLayer = std::get<ObjectLayerData>(layer.data);

  auto node = mapNode.append_child("objectgroup");
  AppendCommonLayerAttributes(node, layer);
  AppendProperties(node, layer.properties, dir);

  for (const auto& object : objectLayer.objects) {
    AppendObject(node, object, dir);
  }
}

void AppendGroupLayer(pugi::xml_node mapNode,
                      const LayerData& layer,
                      const std::filesystem::path& dir)
{
  const auto& groupLayer = std::get<GroupLayerData>(layer.data);

  auto node = mapNode.append_child("group");
  AppendCommonLayerAttributes(node, layer);

  for (const auto& childLayer : groupLayer.layers) {
    AppendLayer(node, *childLayer, dir);
  }
}

}  // namespace

void AppendLayer(pugi::xml_node mapNode,
                 const LayerData& layer,
                 const std::filesystem::path& dir)
{
  switch (layer.type) {
    case LayerType::TileLayer: {
      AppendTileLayer(mapNode, layer, dir);
      break;
    }
    case LayerType::ObjectLayer: {
      AppendObjectLayer(mapNode, layer, dir);
      break;
    }
    case LayerType::GroupLayer: {
      AppendGroupLayer(mapNode, layer, dir);
      break;
    }
    default:
      throw TactileError{"Did not recognize layer type when saving as XML!"};
  }
}

}  // namespace Tactile::IO
