#include "save_layers.hpp"

#include <utility>  // move
#include <variant>  // get

#include <tactile-base/tactile_error.hpp>

#include "save_object.hpp"
#include "save_properties.hpp"

namespace Tactile::IO {
namespace {

void AddCommonAttributes(JSON& json,
                         const LayerData& layer,
                         const std::filesystem::path& dir)
{
  json["id"] = layer.id;
  json["name"] = layer.name;
  json["opacity"] = layer.opacity;
  json["visible"] = layer.is_visible;
  json["x"] = 0;
  json["y"] = 0;

  if (!layer.properties.empty()) {
    json["properties"] = SaveProperties(layer.properties, dir);
  }
}

[[nodiscard]] auto SaveTileLayer(const LayerData& layer, const std::filesystem::path& dir)
    -> JSON
{
  auto json = JSON::object();
  AddCommonAttributes(json, layer, dir);

  const auto& tileLayer = std::get<TileLayerData>(layer.data);
  json["type"] = "tilelayer";
  json["width"] = tileLayer.tiles.at(0).size();
  json["height"] = tileLayer.tiles.size();

  auto data = JSON::array();

  for (const auto& row : tileLayer.tiles) {
    for (const auto tile : row) {
      data += tile.get();
    }
  }

  json["data"] = std::move(data);

  return json;
}

[[nodiscard]] auto SaveObjectLayer(const LayerData& layer,
                                   const std::filesystem::path& dir) -> JSON
{
  auto json = JSON::object();
  AddCommonAttributes(json, layer, dir);

  json["type"] = "objectgroup";

  auto objects = JSON::array();

  const auto& objectLayer = std::get<ObjectLayerData>(layer.data);
  for (const auto& object : objectLayer.objects) {
    objects += SaveObject(object, dir);
  }

  json["objects"] = std::move(objects);

  return json;
}

[[nodiscard]] auto SaveLayer(const LayerData& layer, const std::filesystem::path& dir)
    -> JSON;

[[nodiscard]] auto SaveGroupLayer(const LayerData& layer,
                                  const std::filesystem::path& dir)
{
  auto json = JSON::object();
  AddCommonAttributes(json, layer, dir);

  json["type"] = "group";

  auto layers = JSON::array();

  const auto& groupLayer = std::get<GroupLayerData>(layer.data);
  for (const auto& child : groupLayer.layers) {
    layers += SaveLayer(*child, dir);
  }

  json["layers"] = std::move(layers);

  return json;
}

[[nodiscard]] auto SaveLayer(const LayerData& layer, const std::filesystem::path& dir)
    -> JSON
{
  switch (layer.type) {
    case LayerType::TileLayer:
      return SaveTileLayer(layer, dir);

    case LayerType::ObjectLayer:
      return SaveObjectLayer(layer, dir);

    case LayerType::GroupLayer:
      return SaveGroupLayer(layer, dir);

    default:
      throw TactileError{"Did not recognize layer type!"};
  }
}

}  // namespace

auto SaveLayers(const std::vector<LayerData>& layers, const std::filesystem::path& dir)
    -> JSON
{
  auto json = JSON::array();

  for (const auto& layer : layers) {
    json += SaveLayer(layer, dir);
  }

  return json;
}

}  // namespace Tactile::IO
