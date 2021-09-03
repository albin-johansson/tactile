#include "save_layers.hpp"

#include <utility>  // move

#include "core/components/group_layer.hpp"
#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/components/object_layer.hpp"
#include "core/components/property_context.hpp"
#include "core/components/tile_layer.hpp"
#include "core/tactile_error.hpp"
#include "save_properties.hpp"

namespace Tactile::IO {
namespace {

void AddCommonAttributes(JSON& json,
                         const entt::registry& registry,
                         const entt::entity entity,
                         const Layer& layer,
                         const std::filesystem::path& dir)
{
  const auto& ctx = registry.get<PropertyContext>(entity);

  json["id"] = layer.id.get();
  json["name"] = ctx.name;
  json["opacity"] = layer.opacity;
  json["visible"] = layer.visible;
  json["x"] = 0;
  json["y"] = 0;

  if (!ctx.properties.empty())
  {
    json["properties"] = SaveProperties(registry, entity, dir);
  }
}

[[nodiscard]] auto SaveTileLayer(const entt::registry& registry,
                                 const entt::entity entity,
                                 const Layer& layer,
                                 const std::filesystem::path& dir) -> JSON
{
  auto json = JSON::object();
  AddCommonAttributes(json, registry, entity, layer, dir);

  const auto& tileLayer = registry.get<TileLayer>(entity);
  json["type"] = "tilelayer";
  json["width"] = tileLayer.matrix.at(0).size();
  json["height"] = tileLayer.matrix.size();

  auto data = JSON::array();

  for (const auto& row : tileLayer.matrix)
  {
    for (const auto tile : row)
    {
      data += tile.get();
    }
  }

  json["data"] = std::move(data);

  return json;
}

[[nodiscard]] auto SaveObjectLayer(const entt::registry& registry,
                                   const entt::entity entity,
                                   const Layer& layer,
                                   const std::filesystem::path& dir) -> JSON
{
  auto json = JSON::object();
  AddCommonAttributes(json, registry, entity, layer, dir);

  json["type"] = "objectgroup";

  auto objects = JSON::array();

  const auto& objectLayer = registry.get<ObjectLayer>(entity);
  for (const auto objectEntity : objectLayer.objects)
  {
    const auto& object = registry.get<Object>(objectEntity);
    const auto& ctx = registry.get<PropertyContext>(objectEntity);

    auto jsonObject = JSON::object();
    jsonObject["id"] = object.id.get();
    jsonObject["name"] = ctx.name;
    jsonObject["x"] = object.x;
    jsonObject["y"] = object.y;
    jsonObject["width"] = object.width;
    jsonObject["height"] = object.height;
    jsonObject["visible"] = object.visible;
    jsonObject["type"] = object.custom_type;
    jsonObject["rotation"] = 0;

    if (object.type == ObjectType::Point)
    {
      jsonObject["point"] = true;
    }

    if (!ctx.properties.empty())
    {
      // TODO jsonObject["properties"] = SaveProperties(object, dir);
    }

    objects += std::move(jsonObject);
  }

  json["objects"] = std::move(objects);

  return json;
}

[[nodiscard]] auto SaveLayer(const entt::registry& registry,
                             entt::entity entity,
                             const Layer& layer,
                             const std::filesystem::path& dir) -> JSON;

[[nodiscard]] auto SaveGroupLayer(const entt::registry& registry,
                                  const entt::entity entity,
                                  const Layer& layer,
                                  const std::filesystem::path& dir)
{
  auto json = JSON::object();
  AddCommonAttributes(json, registry, entity, layer, dir);

  json["type"] = "group";

  auto layers = JSON::array();

  const auto& groupLayer = registry.get<GroupLayer>(entity);
  for (const auto child : groupLayer.layers)
  {
    const auto& childLayer = registry.get<Layer>(child);
    layers += SaveLayer(registry, child, childLayer, dir);
  }

  json["layers"] = std::move(layers);

  return json;
}

[[nodiscard]] auto SaveLayer(const entt::registry& registry,
                             const entt::entity entity,
                             const Layer& layer,
                             const std::filesystem::path& dir) -> JSON
{
  switch (layer.type)
  {
    case LayerType::TileLayer:
      return SaveTileLayer(registry, entity, layer, dir);

    case LayerType::ObjectLayer:
      return SaveObjectLayer(registry, entity, layer, dir);

    case LayerType::GroupLayer:
      return SaveGroupLayer(registry, entity, layer, dir);

    default:
      throw TactileError{"Did not recognize layer type when saving as JSON!"};
  }
}

}  // namespace

auto SaveLayers(const entt::registry& registry, const std::filesystem::path& dir)
    -> JSON
{
  auto json = JSON::array();

  for (auto&& [entity, layer] : registry.view<Layer>().each())
  {
    json += SaveLayer(registry, entity, layer, dir);
  }

  return json;
}

}  // namespace Tactile::IO
