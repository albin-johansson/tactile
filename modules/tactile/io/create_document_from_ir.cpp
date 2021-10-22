#include "create_document_from_ir.hpp"

#include <algorithm>  // sort
#include <format>     // format
#include <variant>    // get

#include <tactile-io/parse_ir.hpp>

#include <entt.hpp>  // registry

#include "core/components/animation.hpp"
#include "core/components/fancy_tile.hpp"
#include "core/components/group_layer.hpp"
#include "core/components/layer.hpp"
#include "core/components/layer_tree_node.hpp"
#include "core/components/object.hpp"
#include "core/components/object_layer.hpp"
#include "core/components/parent.hpp"
#include "core/components/property.hpp"
#include "core/components/property_context.hpp"
#include "core/components/tile_layer.hpp"
#include "core/components/tileset.hpp"
#include "core/map.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/property_system.hpp"
#include "core/systems/registry_factory_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/utils/load_texture.hpp"

namespace Tactile {
namespace {

void AddProperties(entt::registry& registry,
                   const entt::entity entity,
                   const std::vector<IO::PropertyData>& data)
{
  auto& context = (entity != entt::null) ? registry.get<PropertyContext>(entity)
                                         : registry.ctx<PropertyContext>();
  for (const auto& propertyData : data) {
    const auto propertyEntity = registry.create();

    auto& property = registry.emplace<Property>(propertyEntity);
    property.name = propertyData.name;
    property.value = propertyData.value;

    context.properties.push_back(propertyEntity);
  }
}

auto AddObject(entt::registry& registry, const IO::ObjectData& data) -> entt::entity
{
  const auto objectEntity = registry.create();

  auto& object = registry.emplace<Object>(objectEntity);
  object.id = ObjectID{data.id};
  object.x = data.x;
  object.y = data.y;
  object.width = data.width;
  object.height = data.height;
  object.type = data.type;
  object.visible = data.visible;

  if (!data.tag.empty()) {
    object.custom_type = data.tag;
  }

  auto& context = Sys::AddPropertyContext(registry, objectEntity);
  context.name = data.name;
  AddProperties(registry, objectEntity, data.properties);

  return objectEntity;
}

void MakeFancyTiles(entt::registry& registry,
                    TilesetCache& cache,
                    const IO::TilesetData& data)
{
  for (const auto& tileData : data.tiles) {
    const auto tileEntity = registry.create();

    auto& tile = registry.emplace<FancyTile>(tileEntity);
    tile.id = TileID{data.first_id + tileData.id};

    cache.tiles.try_emplace(tile.id, tileEntity);

    if (!tileData.animation.empty()) {
      auto& animation = registry.emplace<Animation>(tileEntity);
      for (const auto& frameData : tileData.animation) {
        const auto frameEntity = registry.create();

        auto& frame = registry.emplace<AnimationFrame>(frameEntity);
        frame.tile = TileID{data.first_id + frameData.tile};
        frame.duration = cen::milliseconds<uint32>{frameData.duration};

        animation.frames.push_back(frameEntity);
      }
    }

    if (!tileData.objects.empty()) {
      tile.objects.reserve(tileData.objects.size());
      for (const auto& objectData : tileData.objects) {
        const auto objectEntity = AddObject(registry, objectData);
        tile.objects.push_back(objectEntity);
      }
    }

    auto& context = Sys::AddPropertyContext(registry, tileEntity);
    context.name = std::format("Tile {}", tile.id.get());
    AddProperties(registry, tileEntity, tileData.properties);
  }
}

void MakeTileset(entt::registry& registry, const IO::TilesetData& data)
{
  const auto info = LoadTexture(data.absolute_image_path).value();
  const auto entity = Sys::MakeTileset(registry,
                                       TileID{data.first_id},
                                       info,
                                       data.tile_width,
                                       data.tile_height);

  registry.get<PropertyContext>(entity).name = data.name;
  AddProperties(registry, entity, data.properties);

  auto& cache = registry.get<TilesetCache>(entity);
  MakeFancyTiles(registry, cache, data);
}

void MakeObjectLayer(entt::registry& registry,
                     const entt::entity entity,
                     const IO::ObjectLayerData& data)
{
  auto& objectLayer = registry.emplace<ObjectLayer>(entity);

  for (const auto& objectData : data.objects) {
    const auto objectEntity = AddObject(registry, objectData);
    objectLayer.objects.push_back(objectEntity);
  }
}

auto MakeLayer(entt::registry& registry,
               const IO::LayerData& data,
               entt::entity parent = entt::null) -> entt::entity
{
  const auto entity =
      Sys::AddBasicLayer(registry, LayerID{data.id}, data.type, data.name, parent);

  auto& node = registry.get<LayerTreeNode>(entity);
  node.index = data.index;

  auto& layer = registry.get<Layer>(entity);
  layer.opacity = data.opacity;
  layer.visible = data.is_visible;

  if (data.type == LayerType::TileLayer) {
    const auto& tileLayerData = std::get<IO::TileLayerData>(data.data);

    auto& tileLayer = registry.emplace<TileLayer>(entity);
    tileLayer.matrix = tileLayerData.tiles;
  }
  else if (data.type == LayerType::ObjectLayer) {
    const auto& objectLayerData = std::get<IO::ObjectLayerData>(data.data);
    MakeObjectLayer(registry, entity, objectLayerData);
  }
  else if (data.type == LayerType::GroupLayer) {
    registry.emplace<GroupLayer>(entity);

    const auto& groupLayerData = std::get<IO::GroupLayerData>(data.data);
    for (const auto& layerData : groupLayerData.layers) {
      MakeLayer(registry, *layerData, entity);
    }
  }

  AddProperties(registry, entity, data.properties);

  return entity;
}

}  // namespace

auto CreateDocumentFromIR(const IO::MapData& data) -> Document
{
  Document document;
  document.path = data.absolute_path;
  document.registry = Sys::MakeRegistry();

  {
    auto& map = document.registry.ctx<Map>();
    map.next_layer_id = LayerID{data.next_layer_id};
    map.next_object_id = ObjectID{data.next_object_id};
    map.tile_width = data.tile_width;
    map.tile_height = data.tile_height;
    map.row_count = data.row_count;
    map.column_count = data.column_count;
  }

  {
    auto& context = document.registry.ctx<PropertyContext>();
    context.name = data.absolute_path.filename().string();
  }

  AddProperties(document.registry, entt::null, data.properties);

  for (const auto& tilesetData : data.tilesets) {
    MakeTileset(document.registry, tilesetData);
  }

  if (!data.tilesets.empty()) {
    auto& activeTileset = document.registry.ctx<ActiveTileset>();
    activeTileset.entity = document.registry.view<Tileset>().front();
  }

  for (const auto& layerData : data.layers) {
    MakeLayer(document.registry, layerData);
  }

  Sys::SortLayers(document.registry);

  if (!data.layers.empty()) {
    auto& activeLayer = document.registry.ctx<ActiveLayer>();
    activeLayer.entity = document.registry.view<LayerTreeNode>().front();
  }

  return document;
}

}  // namespace Tactile::IO
