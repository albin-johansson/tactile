#include "append_layer.hpp"

#include <sstream>  // stringstream
#include <variant>  // get

#include <tactile-base/tactile_error.hpp>

#include "append_object.hpp"
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
                     const std::filesystem::path& dir,
                     const bool humanReadableOutput)
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

  std::stringstream stream;
  usize index = 0;

  for (const auto& row : tileLayer.tiles) {
    for (const auto tile : row) {
      if (humanReadableOutput && index == 0) {
        stream << '\n';
      }

      stream << tile;
      if (index < count - 1) {
        stream << ',';
      }

      if (humanReadableOutput && (index + 1) % nCols == 0) {
        stream << '\n';
      }

      ++index;
    }
  }

  const auto tileData = stream.str();
  data.text().set(tileData.c_str());
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
                      const std::filesystem::path& dir,
                      const bool humanReadableOutput)
{
  const auto& groupLayer = std::get<GroupLayerData>(layer.data);

  auto node = mapNode.append_child("group");
  AppendCommonLayerAttributes(node, layer);

  for (const auto& childLayer : groupLayer.layers) {
    AppendLayer(node, *childLayer, dir, humanReadableOutput);
  }
}

}  // namespace

void AppendLayer(pugi::xml_node mapNode,
                 const LayerData& layer,
                 const std::filesystem::path& dir,
                 const bool humanReadableOutput)
{
  switch (layer.type) {
    case LayerType::TileLayer: {
      AppendTileLayer(mapNode, layer, dir, humanReadableOutput);
      break;
    }
    case LayerType::ObjectLayer: {
      AppendObjectLayer(mapNode, layer, dir);
      break;
    }
    case LayerType::GroupLayer: {
      AppendGroupLayer(mapNode, layer, dir, humanReadableOutput);
      break;
    }
    default:
      throw TactileError{"Did not recognize layer type when saving as XML!"};
  }
}

}  // namespace Tactile::IO
