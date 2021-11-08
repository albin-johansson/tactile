#include "save_layers.hpp"

#include <utility>  // move
#include <variant>  // get

#include <tactile_stdlib.hpp>

#include "save_object.hpp"
#include "save_properties.hpp"

namespace Tactile::IO {
namespace {

void AddCommonAttributes(JSON& json, const Layer& layer, const std::filesystem::path& dir)
{
  json["id"] = GetId(layer);
  json["name"] = GetName(layer);
  json["opacity"] = GetOpacity(layer);
  json["visible"] = IsVisible(layer);
  json["x"] = 0;
  json["y"] = 0;

  if (GetPropertyCount(layer) != 0) {
    json["properties"] = SaveProperties(layer, dir);
  }
}

[[nodiscard]] auto SaveTileLayer(const Layer& layer, const std::filesystem::path& dir)
    -> JSON
{
  auto json = JSON::object();
  AddCommonAttributes(json, layer, dir);

  const auto& tileLayer = GetTileLayer(layer);
  const auto nRows = GetRowCount(tileLayer);
  const auto nCols = GetColumnCount(tileLayer);

  json["type"] = "tilelayer";
  json["width"] = nCols;
  json["height"] = nRows;

  auto data = JSON::array();

  for (usize row = 0; row < nRows; ++row) {
    for (usize col = 0; col < nCols; ++col) {
      data += GetTile(tileLayer, row, col);
    }
  }

  json["data"] = std::move(data);

  return json;
}

[[nodiscard]] auto SaveObjectLayer(const Layer& layer, const std::filesystem::path& dir)
    -> JSON
{
  auto json = JSON::object();
  AddCommonAttributes(json, layer, dir);

  json["type"] = "objectgroup";

  auto objects = JSON::array();

  const auto& objectLayer = GetObjectLayer(layer);
  EachObject(objectLayer,
             [&](const Object& object) { objects += SaveObject(object, dir); });

  json["objects"] = std::move(objects);

  return json;
}

[[nodiscard]] auto SaveLayer(const Layer& layer, const std::filesystem::path& dir)
    -> JSON;

[[nodiscard]] auto SaveGroupLayer(const Layer& layer, const std::filesystem::path& dir)
{
  auto json = JSON::object();
  AddCommonAttributes(json, layer, dir);

  json["type"] = "group";

  auto layers = JSON::array();

  const auto& groupLayer = GetGroupLayer(layer);
  EachLayer(groupLayer, [&](const Layer& child) { layers += SaveLayer(child, dir); });

  json["layers"] = std::move(layers);

  return json;
}

[[nodiscard]] auto SaveLayer(const Layer& layer, const std::filesystem::path& dir) -> JSON
{
  switch (GetType(layer)) {
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

auto SaveLayers(const Map& map, const std::filesystem::path& dir) -> JSON
{
  auto json = JSON::array();
  EachLayer(map, [&](const Layer& layer) { json += SaveLayer(layer, dir); });

  return json;
}

}  // namespace Tactile::IO
