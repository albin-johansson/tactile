#include "save_layers.hpp"

#include <utility>  // move

#include "core/map/layers/layer_utils.hpp"
#include "core/tactile_error.hpp"
#include "io/saving/json/save_properties.hpp"

namespace Tactile::IO {
namespace {

void AddCommonAttributes(JSON& json,
                         const layer_id id,
                         const ILayer& layer,
                         const std::filesystem::path& dir)
{
  json["id"] = id.get();
  json["name"] = layer.GetName();
  json["opacity"] = layer.GetOpacity();
  json["visible"] = layer.IsVisible();
  json["x"] = 0;
  json["y"] = 0;

  if (layer.GetPropertyCount() > 0)
  {
    json["properties"] = SaveProperties(layer, dir);
  }
}

[[nodiscard]] auto SaveTileLayer(const layer_id id,
                                 const TileLayer& layer,
                                 const std::filesystem::path& dir) -> JSON
{
  auto json = JSON::object();
  AddCommonAttributes(json, id, layer, dir);

  json["type"] = "tilelayer";
  json["width"] = layer.GetColumnCount().get();
  json["height"] = layer.GetRowCount().get();

  auto data = JSON::array();
  layer.Each([&](const tile_id tile) { data += tile.get(); });

  json["data"] = std::move(data);

  return json;
}

[[nodiscard]] auto SaveObjectLayer(const layer_id id,
                                   const ObjectLayer& layer,
                                   const std::filesystem::path& dir) -> JSON
{
  auto json = JSON::object();
  AddCommonAttributes(json, id, layer, dir);

  json["type"] = "objectgroup";

  auto objects = JSON::array();
  layer.Each([&](const object_id id, const Object& object) {
    auto jsonObject = JSON::object();

    jsonObject["id"] = id.get();
    jsonObject["name"] = object.GetName();
    jsonObject["x"] = object.GetX();
    jsonObject["y"] = object.GetY();
    jsonObject["width"] = object.GetWidth();
    jsonObject["height"] = object.GetHeight();
    jsonObject["visible"] = object.IsVisible();
    jsonObject["type"] = object.GetCustomType().value_or("");
    jsonObject["rotation"] = 0;

    if (object.IsPoint())
    {
      jsonObject["point"] = true;
    }

    if (object.GetPropertyCount() > 0)
    {
      jsonObject["properties"] = SaveProperties(object, dir);
    }

    objects += std::move(jsonObject);
  });

  json["objects"] = std::move(objects);

  return json;
}

[[nodiscard]] auto SaveLayer(layer_id id,
                             const SharedLayer& layer,
                             const std::filesystem::path& dir) -> JSON;

[[nodiscard]] auto SaveGroupLayer(const layer_id id,
                                  const GroupLayer& layer,
                                  const std::filesystem::path& dir)
{
  auto json = JSON::object();
  AddCommonAttributes(json, id, layer, dir);

  json["type"] = "group";

  auto layers = JSON::array();
  for (const auto& [subid, sublayer] : layer)
  {
    layers += SaveLayer(subid, sublayer, dir);
  }

  json["layers"] = std::move(layers);

  return json;
}

[[nodiscard]] auto SaveLayer(const layer_id id,
                             const SharedLayer& layer,
                             const std::filesystem::path& dir) -> JSON
{
  switch (layer->GetType())
  {
    case LayerType::TileLayer:
      return SaveTileLayer(id, *AsTileLayer(layer), dir);

    case LayerType::ObjectLayer:
      return SaveObjectLayer(id, *AsObjectLayer(layer), dir);

    case LayerType::GroupLayer:
      return SaveGroupLayer(id, *AsGroupLayer(layer), dir);

    default:
      throw TactileError{"Did not recognize layer type when saving as JSON!"};
  }
}

}  // namespace

auto SaveLayers(const Map& map, const std::filesystem::path& dir) -> JSON
{
  auto json = JSON::array();

  for (const auto& [id, layer] : map)
  {
    json += SaveLayer(id, layer, dir);
  }

  return json;
}

}  // namespace Tactile::IO
