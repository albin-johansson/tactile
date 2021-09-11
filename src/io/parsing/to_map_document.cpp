#include "to_map_document.hpp"

#include <algorithm>  // sort
#include <entt.hpp>   // registry
#include <format>     // format
#include <variant>    // get

#include "core/components/animation.hpp"
#include "core/components/fancy_tile.hpp"
#include "core/components/group_layer.hpp"
#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/components/object_layer.hpp"
#include "core/components/parent.hpp"
#include "core/components/property.hpp"
#include "core/components/property_context.hpp"
#include "core/components/tile_layer.hpp"
#include "core/components/tileset.hpp"
#include "core/map.hpp"
#include "core/systems/layer_system.hpp"
#include "core/systems/property_system.hpp"
#include "core/systems/registry_factory_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "io/parsing/parse_ir.hpp"
#include "utils/load_texture.hpp"

namespace Tactile::IO {
namespace {

void AddProperties(entt::registry& registry,
                   const entt::entity entity,
                   const std::vector<PropertyData>& data)
{
  auto& context = (entity != entt::null) ? registry.get<PropertyContext>(entity)
                                         : registry.ctx<PropertyContext>();
  for (const auto& propertyData : data)
  {
    const auto propertyEntity = registry.create();

    auto& property = registry.emplace<Property>(propertyEntity);
    property.name = propertyData.name;
    property.value = propertyData.property;

    context.properties.push_back(propertyEntity);
  }
}

void MakeTileset(entt::registry& registry,
                 const TilesetID id,
                 const TilesetData& data)
{
  const auto info = LoadTexture(data.absolute_image_path).value();
  const auto tilesetEntity = Sys::MakeTileset(registry,
                                              id,
                                              data.first_id,
                                              info,
                                              data.tile_width,
                                              data.tile_height);

  {
    auto& context = registry.get<PropertyContext>(tilesetEntity);
    context.name = data.name;
  }

  AddProperties(registry, tilesetEntity, data.properties);

  auto& cache = registry.get<TilesetCache>(tilesetEntity);
  for (const auto& tileData : data.tiles)
  {
    const auto tileEntity = registry.create();

    auto& tile = registry.emplace<FancyTile>(tileEntity);
    tile.id = data.first_id + tileData.id;

    cache.tiles.try_emplace(tile.id, tileEntity);

    if (!tileData.animation.empty())
    {
      auto& animation = registry.emplace<Animation>(tileEntity);
      for (const auto& frameData : tileData.animation)
      {
        const auto frameEntity = registry.create();

        auto& frame = registry.emplace<AnimationFrame>(frameEntity);
        frame.tile = data.first_id + frameData.tile;
        frame.duration = cen::milliseconds<uint32>{frameData.duration};

        animation.frames.push_back(frameEntity);
      }
    }

    auto& context = Sys::AddPropertyContext(registry, tileEntity);
    context.name = std::format("Tile {}", tile.id.get());
    AddProperties(registry, tileEntity, tileData.properties);
  }
}

void MakeObjectLayer(entt::registry& registry,
                     const entt::entity entity,
                     const ObjectLayerData& data)
{
  auto& objectLayer = registry.emplace<ObjectLayer>(entity);

  for (const auto& objectData : data.objects)
  {
    const auto objectEntity = objectLayer.objects.emplace_back(registry.create());

    auto& object = registry.emplace<Object>(objectEntity);
    object.id = objectData.id;
    object.x = objectData.x;
    object.y = objectData.y;
    object.width = objectData.width;
    object.height = objectData.height;
    object.type = objectData.type;
    object.visible = objectData.visible;

    if (!objectData.custom_type.empty())
    {
      object.custom_type = objectData.custom_type;
    }

    auto& context = Sys::AddPropertyContext(registry, objectEntity);
    context.name = objectData.name;

    // TODO properties
  }
}

auto MakeLayer(entt::registry& registry,
               const LayerData& data,
               entt::entity parent = entt::null) -> entt::entity
{
  const auto entity =
      Sys::AddBasicLayer(registry, data.id, data.type, data.name, parent);

  auto& layer = registry.get<Layer>(entity);
  layer.index = data.index;
  layer.opacity = data.opacity;
  layer.visible = data.is_visible;

  if (data.type == LayerType::TileLayer)
  {
    const auto& tileLayerData = std::get<TileLayerData>(data.data);

    auto& tileLayer = registry.emplace<TileLayer>(entity);
    tileLayer.matrix = tileLayerData.tiles;
  }
  else if (data.type == LayerType::ObjectLayer)
  {
    const auto& objectLayerData = std::get<ObjectLayerData>(data.data);
    MakeObjectLayer(registry, entity, objectLayerData);
  }
  else if (data.type == LayerType::GroupLayer)
  {
    registry.emplace<GroupLayer>(entity);

    const auto& groupLayerData = std::get<GroupLayerData>(data.data);
    for (const auto& layerData : groupLayerData.layers)
    {
      MakeLayer(registry, *layerData, entity);
    }
  }

  AddProperties(registry, entity, data.properties);

  return entity;
}

}  // namespace

auto ToMapDocument(const MapData& data) -> Document
{
  Document document;
  document.path = data.absolute_path;
  document.registry = Sys::MakeRegistry();

  auto& map = document.registry.ctx<Map>();
  map.next_layer_id = data.next_layer_id;
  map.next_object_id = data.next_object_id;
  map.tile_width = data.tile_width;
  map.tile_height = data.tile_height;
  map.row_count = AsRow(data.row_count);
  map.column_count = AsColumn(data.column_count);

  auto& context = document.registry.ctx<PropertyContext>();
  context.name = data.absolute_path.filename().string();

  AddProperties(document.registry, entt::null, data.properties);

  for (TilesetID id{1}; const auto& tilesetData : data.tilesets)
  {
    MakeTileset(document.registry, id, tilesetData);
    ++id;
  }

  if (!data.tilesets.empty())
  {
    auto& activeTileset = document.registry.ctx<ActiveTileset>();
    activeTileset.entity = document.registry.view<Tileset>().front();
  }

  for (const auto& layerData : data.layers)
  {
    MakeLayer(document.registry, layerData);
  }

  Sys::SortLayers(document.registry);

  if (!data.layers.empty())
  {
    auto& activeLayer = document.registry.ctx<ActiveLayer>();
    activeLayer.entity = document.registry.view<Layer>().front();
  }

  return document;
}

}  // namespace Tactile::IO
