#include "convert_document_to_ir.hpp"

#include <filesystem>  // absolute

#include <tactile_def.hpp>

#include "core/components/animation.hpp"
#include "core/components/fancy_tile.hpp"
#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/components/parent.hpp"
#include "core/components/property.hpp"
#include "core/components/property_context.hpp"
#include "core/components/texture.hpp"
#include "core/components/tileset.hpp"
#include "core/map.hpp"
#include "core/systems/tileset_system.hpp"
#include "editor/document.hpp"
#include "profile.hpp"

namespace Tactile {
namespace {

template <typename T>
void ConvertProperties(T& source,
                       const entt::registry& registry,
                       const PropertyContext& context)

{
  IO::ReserveProperties(source, context.properties.size());

  for (const auto propertyEntity : context.properties) {
    const auto& property = registry.get<Property>(propertyEntity);

    auto& data = IO::AddProperty(source);
    IO::SetName(data, property.name.c_str());

    switch (property.value.GetType().value()) {
      case PropertyType::String:
        IO::AssignString(data, property.value.AsString().c_str());
        break;

      case PropertyType::Integer:
        IO::AssignInt(data, property.value.AsInt());
        break;

      case PropertyType::Floating:
        IO::AssignFloat(data, property.value.AsFloat());
        break;

      case PropertyType::Boolean:
        IO::AssignBool(data, property.value.AsBool());
        break;

      case PropertyType::Color: {
        const auto& color = property.value.AsColor();
        IO::AssignColor(data, {color.red(), color.green(), color.blue(), color.alpha()});
        break;
      }

      case PropertyType::File:
        IO::AssignFile(data, property.value.AsFile().c_str());
        break;

      case PropertyType::Object:
        IO::AssignObject(data, property.value.AsObject());
        break;
    }
  }
}

void ConvertObject(IO::Object& ir,
                   const entt::registry& registry,
                   const entt::entity entity)
{
  const auto& object = registry.get<Object>(entity);
  IO::SetId(ir, object.id);
  IO::SetX(ir, object.x);
  IO::SetY(ir, object.y);
  IO::SetWidth(ir, object.width);
  IO::SetHeight(ir, object.height);
  IO::SetType(ir, object.type);
  IO::SetTag(ir, object.tag.c_str());
  IO::SetVisible(ir, object.visible);

  const auto& context = registry.get<PropertyContext>(entity);
  IO::SetName(ir, context.name.c_str());
  ConvertProperties(ir, registry, context);
}

void ConvertFancyTiles(IO::Tileset& ir,
                       const entt::registry& registry,
                       const Tileset& tileset)

{
  for (auto&& [entity, tile, ctx] : registry.view<FancyTile, PropertyContext>().each()) {
    if (tile.id >= tileset.first_id && tile.id <= tileset.last_id) {
      auto& tileData = IO::AddTile(ir);
      IO::SetId(tileData, Sys::ConvertToLocal(registry, tile.id).value());

      if (const auto* animation = registry.try_get<Animation>(entity)) {
        IO::ReserveAnimationFrames(tileData, animation->frames.size());

        for (const auto frameEntity : animation->frames) {
          const auto& frame = registry.get<AnimationFrame>(frameEntity);

          auto& frameData = IO::AddAnimationFrame(tileData);
          IO::SetTile(frameData, Sys::ConvertToLocal(registry, frame.tile).value());
          IO::SetDuration(frameData, static_cast<int32>(frame.duration.count()));
        }
      }

      IO::ReserveObjects(tileData, tile.objects.size());
      for (const auto objectEntity : tile.objects) {
        auto& objectData = IO::AddObject(tileData);
        ConvertObject(objectData, registry, objectEntity);
      }

      ConvertProperties(tileData, registry, ctx);
    }
  }
}

void ConvertTileset(IO::Tileset& ir,
                    const entt::registry& registry,
                    const entt::entity entity,
                    const Tileset& tileset)
{
  IO::SetFirstGlobalId(ir, tileset.first_id);
  IO::SetTileWidth(ir, tileset.tile_width);
  IO::SetTileHeight(ir, tileset.tile_height);
  IO::SetTileCount(ir, tileset.tile_count);
  IO::SetColumnCount(ir, tileset.column_count);

  const auto& texture = registry.get<Texture>(entity);
  IO::SetImagePath(ir, std::filesystem::absolute(texture.path).c_str());
  IO::SetImageWidth(ir, texture.width);
  IO::SetImageHeight(ir, texture.height);

  const auto& ctx = registry.get<PropertyContext>(entity);
  IO::SetName(ir, ctx.name.c_str());

  ConvertProperties(ir, registry, ctx);
  ConvertFancyTiles(ir, registry, tileset);
}

void ConvertLayer(IO::Layer& ir,
                  const entt::registry& registry,
                  const entt::entity entity,
                  const LayerTreeNode& node,
                  const usize nRows,
                  const usize nCols)
{
  const auto& layer = registry.get<Layer>(entity);
  const auto& context = registry.get<PropertyContext>(entity);

  IO::SetIndex(ir, node.index);
  IO::SetId(ir, layer.id);
  IO::SetOpacity(ir, layer.opacity);
  IO::SetVisible(ir, layer.visible);

  IO::SetName(ir, context.name.c_str());
  ConvertProperties(ir, registry, context);

  switch (layer.type) {
    case LayerType::TileLayer: {
      auto& tileLayer = IO::MarkAsTileLayer(ir);
      IO::ReserveTiles(tileLayer, nRows, nCols);

      const auto matrix = registry.get<TileLayer>(entity).matrix;
      for (usize row = 0; row < nRows; ++row) {
        for (usize col = 0; col < nCols; ++col) {
          IO::SetTile(tileLayer, row, col, matrix.at(row).at(col));
        }
      }

      break;
    }

    case LayerType::ObjectLayer: {
      const auto& objectLayer = registry.get<ObjectLayer>(entity);

      auto& objectLayerData = IO::MarkAsObjectLayer(ir);
      IO::ReserveObjects(objectLayerData, objectLayer.objects.size());

      for (const auto objectEntity : objectLayer.objects) {
        auto& object = IO::AddObject(objectLayerData);
        ConvertObject(object, registry, objectEntity);
      }

      break;
    }

    case LayerType::GroupLayer: {
      auto& groupLayer = IO::MarkAsGroupLayer(ir);
      IO::ReserveLayers(groupLayer, node.children.size());

      for (const auto child : node.children) {
        auto& childData = IO::AddLayer(groupLayer);
        const auto& childNode = registry.get<LayerTreeNode>(child);
        ConvertLayer(childData, registry, child, childNode, nRows, nCols);
      }

      break;
    }
  }
}

void ConvertLayers(IO::Map& ir, const entt::registry& registry)
{
  const auto& map = registry.ctx<Map>();

  IO::ReserveLayers(ir, registry.storage<LayerTreeNode>().size());
  for (auto&& [entity, node] : registry.view<LayerTreeNode>().each()) {
    const auto& parent = registry.get<Parent>(entity);
    if (parent.entity == entt::null) {
      auto& layerData = IO::AddLayer(ir);
      ConvertLayer(layerData, registry, entity, node, map.row_count, map.column_count);
    }
  }
}

void ConvertTilesets(IO::Map& ir, const entt::registry& registry)
{
  IO::ReserveTilesets(ir, registry.storage<Tileset>().size());
  for (auto&& [entity, tileset] : registry.view<Tileset>().each()) {
    auto& tilesetData = IO::AddTileset(ir);
    ConvertTileset(tilesetData, registry, entity, tileset);
  }
}

void ConvertMapAttributes(IO::Map& ir, const Document& document)
{
  const auto& map = document.registry.ctx<Map>();
  IO::SetPath(ir, std::filesystem::absolute(document.path).c_str());

  IO::SetNextLayerId(ir, map.next_layer_id);
  IO::SetNextObjectId(ir, map.next_object_id);

  IO::SetTileWidth(ir, map.tile_width);
  IO::SetTileHeight(ir, map.tile_height);

  IO::SetRowCount(ir, map.row_count);
  IO::SetColumnCount(ir, map.column_count);
}

}  // namespace

auto ConvertDocumentToIR(const Document& document) -> IO::MapPtr
{
  TACTILE_PROFILE_START;

  const auto& registry = document.registry;

  auto ir = IO::CreateMap();
  ConvertMapAttributes(*ir, document);

  ConvertTilesets(*ir, registry);
  ConvertLayers(*ir, registry);

  ConvertProperties(*ir, registry, registry.ctx<PropertyContext>());

  TACTILE_PROFILE_END("Converted document to intermediate representation");
  return ir;
}

}  // namespace Tactile
