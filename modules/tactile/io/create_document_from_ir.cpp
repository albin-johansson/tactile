#include "create_document_from_ir.hpp"

#include <algorithm>  // sort
#include <cstring>    // strcmp
#include <format>     // format
#include <variant>    // get

#include <tactile-io/ir.hpp>

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

template <typename T>
void AddProperties(entt::registry& registry, const entt::entity entity, const T& source)
{
  const auto count = IO::GetPropertyCount(source);

  auto& context = (entity != entt::null) ? registry.get<PropertyContext>(entity)
                                         : registry.ctx<PropertyContext>();
  context.properties.reserve(count);

  for (usize index = 0; index < count; ++index) {
    const auto& propertyData = IO::GetProperty(source, index);

    const auto propertyEntity = registry.create();
    auto& property = registry.emplace<Property>(propertyEntity);
    property.name = IO::GetName(propertyData);

    switch (IO::GetType(propertyData)) {
      case PropertyType::String:
        property.value = std::string{IO::GetString(propertyData)};
        break;

      case PropertyType::Integer:
        property.value = IO::GetInt(propertyData);
        break;

      case PropertyType::Floating:
        property.value = IO::GetFloat(propertyData);
        break;

      case PropertyType::Boolean:
        property.value = IO::GetBool(propertyData);
        break;

      case PropertyType::Color: {
        const auto color = IO::GetColor(propertyData);
        property.value = cen::color{color.red, color.green, color.blue, color.alpha};
        break;
      }

      case PropertyType::File:
        property.value = std::filesystem::path{IO::GetFile(propertyData)};
        break;

      case PropertyType::Object:
        property.value = ObjectRef{IO::GetObject(propertyData)};
        break;
    };

    context.properties.push_back(propertyEntity);
  }
}

auto AddObject(entt::registry& registry, const IO::Object& data) -> entt::entity
{
  const auto objectEntity = registry.create();

  auto& object = registry.emplace<Object>(objectEntity);
  object.id = ObjectID{IO::GetId(data)};
  object.x = IO::GetX(data);
  object.y = IO::GetY(data);
  object.width = IO::GetWidth(data);
  object.height = IO::GetHeight(data);
  object.type = IO::GetType(data);
  object.visible = IO::IsVisible(data);

  if (const auto tag = IO::GetTag(data); std::strcmp(tag, "") != 0) {
    object.custom_type = tag;
  }

  auto& context = Sys::AddPropertyContext(registry, objectEntity);
  context.name = IO::GetName(data);
  AddProperties(registry, objectEntity, data);

  return objectEntity;
}

void AddAnimation(entt::registry& registry,
                  const entt::entity tileEntity,
                  const int32 firstGlobalId,
                  const IO::Tile& tileData,
                  const usize nFrames)
{
  auto& animation = registry.emplace<Animation>(tileEntity);
  animation.frames.reserve(nFrames);

  for (usize index = 0; index < nFrames; ++index) {
    const auto& frameData = IO::GetAnimationFrame(tileData, index);

    const auto frameEntity = registry.create();

    auto& frame = registry.emplace<AnimationFrame>(frameEntity);
    frame.tile = TileID{firstGlobalId + IO::GetTile(frameData)};
    frame.duration = cen::milliseconds<uint32>{IO::GetDuration(frameData)};

    animation.frames.push_back(frameEntity);
  }
}

void AddTileObjects(entt::registry& registry,
                    FancyTile& tile,
                    const IO::Tile& tileData,
                    const usize nObjects)
{
  tile.objects.reserve(nObjects);

  for (usize index = 0; index < nObjects; ++index) {
    const auto& objectData = IO::GetObject(tileData, index);
    const auto objectEntity = AddObject(registry, objectData);
    tile.objects.push_back(objectEntity);
  }
}

void MakeFancyTiles(entt::registry& registry,
                    TilesetCache& cache,
                    const IO::Tileset& data)
{
  const auto firstGlobalId = IO::GetFirstGlobalId(data);
  const auto count = IO::GetTileInfoCount(data);
  for (usize index = 0; index < count; ++index) {
    const auto& tileData = IO::GetTileInfo(data, index);

    const auto tileEntity = registry.create();
    auto& tile = registry.emplace<FancyTile>(tileEntity);
    tile.id = TileID{firstGlobalId + IO::GetId(tileData)};

    cache.tiles.try_emplace(tile.id, tileEntity);

    if (const auto nFrames = IO::GetAnimationFrameCount(tileData); nFrames != 0) {
      AddAnimation(registry, tileEntity, firstGlobalId, tileData, nFrames);
    }

    if (const auto nObjects = IO::GetObjectCount(tileData); nObjects != 0) {
      AddTileObjects(registry, tile, tileData, nObjects);
    }

    auto& context = Sys::AddPropertyContext(registry, tileEntity);
    context.name = std::format("Tile {}", tile.id.get());
    AddProperties(registry, tileEntity, tileData);
  }
}

void MakeTileset(entt::registry& registry, const IO::Tileset& data)
{
  const auto info = LoadTexture(IO::GetImagePath(data)).value();
  const auto entity = Sys::MakeTileset(registry,
                                       TileID{IO::GetFirstGlobalId(data)},
                                       info,
                                       IO::GetTileWidth(data),
                                       IO::GetTileHeight(data));

  registry.get<PropertyContext>(entity).name = IO::GetName(data);
  AddProperties(registry, entity, data);

  auto& cache = registry.get<TilesetCache>(entity);
  MakeFancyTiles(registry, cache, data);
}

void MakeObjectLayer(entt::registry& registry,
                     const entt::entity entity,
                     const IO::ObjectLayer& data)
{
  const auto count = IO::GetObjectCount(data);

  auto& objectLayer = registry.emplace<ObjectLayer>(entity);
  objectLayer.objects.reserve(count);

  for (usize index = 0; index < count; ++index) {
    const auto& objectData = IO::GetObject(data, index);
    const auto objectEntity = AddObject(registry, objectData);
    objectLayer.objects.push_back(objectEntity);
  }
}

auto MakeLayer(entt::registry& registry,
               const IO::Layer& data,
               const entt::entity parent = entt::null) -> entt::entity
{
  const auto entity = Sys::AddBasicLayer(registry,
                                         LayerID{IO::GetId(data)},
                                         IO::GetType(data),
                                         IO::GetName(data),
                                         parent);

  auto& node = registry.get<LayerTreeNode>(entity);
  node.index = IO::GetIndex(data);

  auto& layer = registry.get<Layer>(entity);
  layer.opacity = IO::GetOpacity(data);
  layer.visible = IO::IsVisible(data);

  const auto type = IO::GetType(data);
  if (type == LayerType::TileLayer) {
    const auto& tileLayerData = IO::GetTileLayer(data);
    const auto nRows = IO::GetRowCount(tileLayerData);
    const auto nCols = IO::GetColumnCount(tileLayerData);

    auto& tileLayer = registry.emplace<TileLayer>(entity);
    tileLayer.matrix = MakeTileMatrix(nRows, nCols);

    for (int32 row = 0; row < nRows; ++row) {
      for (int32 col = 0; col < nCols; ++col) {
        tileLayer.matrix[row][col] = TileID{IO::GetTile(tileLayerData, row, col)};
      }
    }
  }
  else if (type == LayerType::ObjectLayer) {
    const auto& objectLayerData = IO::GetObjectLayer(data);
    MakeObjectLayer(registry, entity, objectLayerData);
  }
  else if (type == LayerType::GroupLayer) {
    registry.emplace<GroupLayer>(entity);

    const auto& groupLayerData = IO::GetGroupLayer(data);
    const auto count = IO::GetLayerCount(groupLayerData);

    for (usize index = 0; index < count; ++index) {
      const auto& layerData = IO::GetLayer(groupLayerData, index);
      MakeLayer(registry, layerData, entity);
    }
  }

  AddProperties(registry, entity, data);

  return entity;
}

}  // namespace

auto CreateDocumentFromIR(const IO::Map& data) -> Document
{
  Document document;
  document.path = IO::GetAbsolutePath(data);
  document.registry = Sys::MakeRegistry();

  {
    auto& map = document.registry.ctx<Map>();
    map.next_layer_id = LayerID{IO::GetNextLayerId(data)};
    map.next_object_id = ObjectID{IO::GetNextObjectId(data)};
    map.tile_width = IO::GetTileWidth(data);
    map.tile_height = IO::GetTileHeight(data);
    map.row_count = IO::GetRowCount(data);
    map.column_count = IO::GetColumnCount(data);
  }

  {
    auto& context = document.registry.ctx<PropertyContext>();
    context.name = document.path.filename().string();
  }

  AddProperties(document.registry, entt::null, data);

  const auto nTilesets = IO::GetTilesetCount(data);
  for (usize index = 0; index < nTilesets; ++index) {
    const auto& tilesetData = IO::GetTileset(data, index);
    MakeTileset(document.registry, tilesetData);
  }

  if (nTilesets != 0) {
    auto& activeTileset = document.registry.ctx<ActiveTileset>();
    activeTileset.entity = document.registry.view<Tileset>().front();
  }

  const auto nLayers = IO::GetLayerCount(data);
  for (usize index = 0; index < nLayers; ++index) {
    const auto& layerData = IO::GetLayer(data, index);
    MakeLayer(document.registry, layerData);
  }

  Sys::SortLayers(document.registry);

  if (nLayers != 0) {
    auto& activeLayer = document.registry.ctx<ActiveLayer>();
    activeLayer.entity = document.registry.view<LayerTreeNode>().front();
  }

  return document;
}

}  // namespace Tactile::IO
