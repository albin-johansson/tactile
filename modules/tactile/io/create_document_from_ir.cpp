#include "create_document_from_ir.hpp"

#include <algorithm>   // sort
#include <cstring>     // strcmp
#include <filesystem>  // path
#include <string>      // string

#include <entt/entt.hpp>  // registry
#include <fmt/format.h>   // format
#include <tactile_stdlib.hpp>

#include "core/components/animation.hpp"
#include "core/components/component.hpp"
#include "core/components/fancy_tile.hpp"
#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/components/property.hpp"
#include "core/components/attribute_context.hpp"
#include "core/components/tileset.hpp"
#include "core/map.hpp"
#include "core/systems/component_system.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/property_system.hpp"
#include "core/systems/registry_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/utils/texture_manager.hpp"

namespace tactile {
namespace {

[[nodiscard]] constexpr auto ToColor(const IO::Color& irColor) -> cen::color
{
  return {irColor.red, irColor.green, irColor.blue, irColor.alpha};
}

template <typename T>
void AddProperties(entt::registry& registry, const entt::entity entity, const T& source)
{
  const auto count = IO::GetPropertyCount(source);

  auto& context = (entity != entt::null) ? registry.get<attribute_context>(entity)
                                         : registry.ctx<attribute_context>();
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

      case PropertyType::Color:
        property.value = ToColor(IO::GetColor(propertyData));
        break;

      case PropertyType::File:
        property.value = std::filesystem::path{IO::GetFile(propertyData)};
        break;

      case PropertyType::Object:
        property.value = object_t{IO::GetObject(propertyData)};
        break;
    };

    context.properties.push_back(propertyEntity);
  }
}

template <typename T>
void AddComponents(entt::registry& registry, const entt::entity entity, const T& source)
{
  auto& context = (entity != entt::null) ? registry.get<attribute_context>(entity)
                                         : registry.ctx<attribute_context>();
  context.components.reserve(IO::GetComponentCount(source));

  IO::EachComponent(source, [&](const IO::Component& irComponent) {
    const auto defEntity = sys::find_component_def(registry, IO::GetName(irComponent));
    TACTILE_ASSERT(defEntity != entt::null);

    const auto def = registry.get<ComponentDef>(defEntity);

    const auto componentEntity = registry.create();
    context.components.push_back(componentEntity);

    auto& component = registry.emplace<Component>(componentEntity);
    component.type = def.id;

    IO::EachAttribute(irComponent, [&](const CStr attr) {
      switch (IO::GetAttributeType(irComponent, attr)) {
        case PropertyType::String:
          component.values[attr] = std::string{IO::GetString(irComponent, attr)};
          break;

        case PropertyType::Integer:
          component.values[attr] = IO::GetInt(irComponent, attr);
          break;

        case PropertyType::Floating:
          component.values[attr] = IO::GetFloat(irComponent, attr);
          break;

        case PropertyType::Boolean:
          component.values[attr] = IO::GetBool(irComponent, attr);
          break;

        case PropertyType::File:
          component.values[attr] = std::filesystem::path{IO::GetFile(irComponent, attr)};
          break;

        case PropertyType::Color:
          component.values[attr] = ToColor(IO::GetColor(irComponent, attr));
          break;

        case PropertyType::Object:
          component.values[attr] = object_t{IO::GetObject(irComponent, attr)};
          break;
      }
    });
  });
}

auto AddObject(entt::registry& registry, const IO::Object& irObject) -> entt::entity
{
  const auto objectEntity = registry.create();

  auto& object = registry.emplace<Object>(objectEntity);
  object.id = ObjectID{IO::GetId(irObject)};
  object.x = IO::GetX(irObject);
  object.y = IO::GetY(irObject);
  object.width = IO::GetWidth(irObject);
  object.height = IO::GetHeight(irObject);
  object.type = IO::GetType(irObject);
  object.visible = IO::IsVisible(irObject);

  if (const auto* tag = IO::GetTag(irObject); std::strcmp(tag, "") != 0) {
    object.tag = tag;
  }

  auto& context = sys::AddPropertyContext(registry, objectEntity);
  context.name = IO::GetName(irObject);

  AddProperties(registry, objectEntity, irObject);
  AddComponents(registry, objectEntity, irObject);

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
    frame.duration = cen::u32ms{IO::GetDuration(frameData)};

    animation.frames.push_back(frameEntity);
  }
}

void AddTileObjects(entt::registry& registry,
                    FancyTile& tile,
                    const IO::Tile& irTile,
                    const usize nObjects)
{
  tile.objects.reserve(nObjects);

  for (usize index = 0; index < nObjects; ++index) {
    const auto& objectData = IO::GetObject(irTile, index);
    const auto objectEntity = AddObject(registry, objectData);
    tile.objects.push_back(objectEntity);
  }
}

void MakeFancyTiles(entt::registry& registry,
                    TilesetCache& cache,
                    const IO::Tileset& irTileset)
{
  const auto firstGlobalId = IO::GetFirstGlobalId(irTileset);
  const auto count = IO::GetTileInfoCount(irTileset);
  for (usize index = 0; index < count; ++index) {
    const auto& irTileInfo = IO::GetTileInfo(irTileset, index);

    const auto tileEntity = registry.create();
    auto& tile = registry.emplace<FancyTile>(tileEntity);
    tile.id = TileID{firstGlobalId + IO::GetId(irTileInfo)};

    cache.tiles.try_emplace(tile.id, tileEntity);

    if (const auto nFrames = IO::GetAnimationFrameCount(irTileInfo); nFrames != 0) {
      AddAnimation(registry, tileEntity, firstGlobalId, irTileInfo, nFrames);
    }

    if (const auto nObjects = IO::GetObjectCount(irTileInfo); nObjects != 0) {
      AddTileObjects(registry, tile, irTileInfo, nObjects);
    }

    auto& context = sys::AddPropertyContext(registry, tileEntity);
    context.name = fmt::format("Tile {}", tile.id);
    AddProperties(registry, tileEntity, irTileInfo);
    AddComponents(registry, tileEntity, irTileInfo);
  }
}

void MakeTileset(entt::registry& registry,
                 TextureManager& textures,
                 const IO::Tileset& irTileset)
{
  const auto info = textures.Load(IO::GetImagePath(irTileset)).value();
  const auto entity = sys::make_tileset(registry,
                                        TileID{IO::GetFirstGlobalId(irTileset)},
                                        info,
                                        IO::GetTileWidth(irTileset),
                                        IO::GetTileHeight(irTileset));

  registry.get<attribute_context>(entity).name = IO::GetName(irTileset);
  AddProperties(registry, entity, irTileset);
  AddComponents(registry, entity, irTileset);

  auto& cache = registry.get<TilesetCache>(entity);
  MakeFancyTiles(registry, cache, irTileset);
}

void MakeObjectLayer(entt::registry& registry,
                     const entt::entity entity,
                     const IO::ObjectLayer& irObjectLayer)
{
  const auto count = IO::GetObjectCount(irObjectLayer);

  auto& objectLayer = registry.emplace<ObjectLayer>(entity);
  objectLayer.objects.reserve(count);

  for (usize index = 0; index < count; ++index) {
    const auto& objectData = IO::GetObject(irObjectLayer, index);
    const auto objectEntity = AddObject(registry, objectData);
    objectLayer.objects.push_back(objectEntity);
  }
}

auto MakeLayer(entt::registry& registry,
               const IO::Layer& irLayer,
               const entt::entity parent = entt::null) -> entt::entity
{
  const auto entity = sys::AddBasicLayer(registry,
                                         LayerID{IO::GetId(irLayer)},
                                         IO::GetType(irLayer),
                                         IO::GetName(irLayer),
                                         parent);

  auto& node = registry.get<LayerTreeNode>(entity);
  node.index = IO::GetIndex(irLayer);

  auto& layer = registry.get<Layer>(entity);
  layer.opacity = IO::GetOpacity(irLayer);
  layer.visible = IO::IsVisible(irLayer);

  const auto type = IO::GetType(irLayer);
  if (type == LayerType::TileLayer) {
    const auto& irTileLayer = IO::GetTileLayer(irLayer);
    const auto nRows = IO::GetRowCount(irTileLayer);
    const auto nCols = IO::GetColumnCount(irTileLayer);

    auto& tileLayer = registry.emplace<TileLayer>(entity);
    tileLayer.matrix = MakeTileMatrix(nRows, nCols);

    for (usize row = 0; row < nRows; ++row) {
      for (usize col = 0; col < nCols; ++col) {
        tileLayer.matrix[row][col] = TileID{IO::GetTile(irTileLayer, row, col)};
      }
    }
  }
  else if (type == LayerType::ObjectLayer) {
    const auto& irObjectLayer = IO::GetObjectLayer(irLayer);
    MakeObjectLayer(registry, entity, irObjectLayer);
  }
  else if (type == LayerType::GroupLayer) {
    registry.emplace<GroupLayer>(entity);

    const auto& irGroupLayer = IO::GetGroupLayer(irLayer);
    IO::EachLayer(irGroupLayer, [&](const IO::Layer& irSublayer) {
      MakeLayer(registry, irSublayer, entity);
    });
  }

  AddProperties(registry, entity, irLayer);
  AddComponents(registry, entity, irLayer);

  return entity;
}

void CreateLayers(Document& document, const IO::Map& irMap)
{
  IO::EachLayer(irMap,
                [&](const IO::Layer& irLayer) { MakeLayer(document.registry, irLayer); });

  sys::SortLayers(document.registry);

  if (!document.registry.storage<LayerTreeNode>().empty()) {
    auto& activeLayer = document.registry.ctx<ActiveLayer>();
    activeLayer.entity = document.registry.view<LayerTreeNode>().front();
  }
}

void CreateTilesets(Document& document, TextureManager& textures, const IO::Map& irMap)
{
  IO::EachTileset(irMap, [&](const IO::Tileset& irTileset) {
    MakeTileset(document.registry, textures, irTileset);
  });

  if (!document.registry.storage<Tileset>().empty()) {
    auto& activeTileset = document.registry.ctx<ActiveTileset>();
    activeTileset.entity = document.registry.view<Tileset>().front();
  }
}

void InitRootPropertyContext(Document& document)
{
  auto& context = document.registry.ctx<attribute_context>();
  context.name = document.path.filename().string();
}

void InitMapContext(MapInfo& map, const IO::Map& irMap)
{
  map.next_layer_id = LayerID{IO::GetNextLayerId(irMap)};
  map.next_object_id = ObjectID{IO::GetNextObjectId(irMap)};

  map.tile_width = IO::GetTileWidth(irMap);
  map.tile_height = IO::GetTileHeight(irMap);

  map.row_count = IO::GetRowCount(irMap);
  map.column_count = IO::GetColumnCount(irMap);
}

void InitComponentDefinitions(entt::registry& registry, const IO::Map& irMap)
{
  IO::EachComponentDef(irMap, [&](const IO::ComponentDef& irDef) {
    const auto* componentName = IO::GetName(irDef);

    const auto componentId = sys::make_component_def(registry, componentName);
    auto [entity, def] = sys::get_component_def(registry, componentId);

    IO::EachAttribute(irDef, [&](const CStr attr) {
      const auto type = IO::GetAttributeType(irDef, attr);
      switch (type) {
        case PropertyType::String:
          sys::make_component_attribute(registry,
                                        componentId,
                                        attr,
                                        std::string{IO::GetString(irDef, attr)});
          break;

        case PropertyType::Integer:
          sys::make_component_attribute(registry,
                                        componentId,
                                        attr,
                                        IO::GetInt(irDef, attr));
          break;

        case PropertyType::Floating:
          sys::make_component_attribute(registry,
                                        componentId,
                                        attr,
                                        IO::GetFloat(irDef, attr));
          break;

        case PropertyType::Boolean:
          sys::make_component_attribute(registry,
                                        componentId,
                                        attr,
                                        IO::GetBool(irDef, attr));
          break;

        case PropertyType::File:
          sys::make_component_attribute(registry,
                                        componentId,
                                        attr,
                                        std::filesystem::path{IO::GetFile(irDef, attr)});
          break;

        case PropertyType::Color:
          sys::make_component_attribute(registry,
                                        componentId,
                                        attr,
                                        ToColor(IO::GetColor(irDef, attr)));
          break;

        case PropertyType::Object:
          sys::make_component_attribute(registry,
                                        componentId,
                                        attr,
                                        object_t{IO::GetObject(irDef, attr)});
          break;
      }
    });
  });
}

}  // namespace

auto CreateDocumentFromIR(const IO::Map& irMap, TextureManager& textures) -> Document
{
  Document document;
  document.path = IO::GetPath(irMap);
  document.registry = sys::MakeRegistry();

  InitComponentDefinitions(document.registry, irMap);

  InitMapContext(document.registry.ctx<MapInfo>(), irMap);
  InitRootPropertyContext(document);

  AddProperties(document.registry, entt::null, irMap);
  AddComponents(document.registry, entt::null, irMap);

  CreateTilesets(document, textures, irMap);
  CreateLayers(document, irMap);

  return document;
}

}  // namespace tactile
