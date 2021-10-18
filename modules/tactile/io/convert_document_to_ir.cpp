#include "convert_document_to_ir.hpp"

#include <filesystem>  // absolute
#include <memory>      // make_unique
#include <vector>      // vector

#include <tactile-base/tactile_std.hpp>

#include "tactile/core/components/animation.hpp"
#include "tactile/core/components/fancy_tile.hpp"
#include "tactile/core/components/layer.hpp"
#include "tactile/core/components/layer_tree_node.hpp"
#include "tactile/core/components/object.hpp"
#include "tactile/core/components/object_layer.hpp"
#include "tactile/core/components/parent.hpp"
#include "tactile/core/components/property.hpp"
#include "tactile/core/components/property_context.hpp"
#include "tactile/core/components/texture.hpp"
#include "tactile/core/components/tile_layer.hpp"
#include "tactile/core/components/tileset.hpp"
#include "tactile/core/map.hpp"
#include "tactile/core/systems/tileset_system.hpp"
#include "tactile/editor/document.hpp"

namespace Tactile {
namespace {

[[nodiscard]] auto ConvertProperties(const entt::registry& registry,
                                     const PropertyContext& context)
    -> std::vector<IO::PropertyData>
{
  std::vector<IO::PropertyData> props;
  props.reserve(context.properties.size());

  for (const auto propertyEntity : context.properties) {
    const auto& property = registry.get<Property>(propertyEntity);

    auto& data = props.emplace_back();
    data.name = property.name;
    data.property = property.value;
  }

  return props;
}

[[nodiscard]] auto ConvertFancyTiles(const entt::registry& registry,
                                     const Tileset& tileset) -> std::vector<IO::TileData>
{
  std::vector<IO::TileData> tiles;
  tiles.reserve(16);

  for (auto&& [entity, tile, ctx] : registry.view<FancyTile, PropertyContext>().each()) {
    if (tile.id >= tileset.first_id && tile.id <= tileset.last_id) {
      auto& data = tiles.emplace_back();
      data.id = Sys::ConvertToLocal(registry, tile.id).value();

      if (const auto* animation = registry.try_get<Animation>(entity)) {
        data.animation.reserve(animation->frames.size());

        for (const auto frameEntity : animation->frames) {
          const auto& frame = registry.get<AnimationFrame>(frameEntity);

          auto& frameData = data.animation.emplace_back();
          frameData.tile = Sys::ConvertToLocal(registry, frame.tile).value();
          frameData.duration = frame.duration.count();  // TODO make uint32
        }
      }

      data.properties = ConvertProperties(registry, ctx);
    }
  }

  return tiles;
}

[[nodiscard]] auto ConvertTileset(const entt::registry& registry,
                                  const entt::entity entity,
                                  const Tileset& tileset) -> IO::TilesetData
{
  IO::TilesetData data;
  data.first_id = tileset.first_id;
  data.tile_width = tileset.tile_width;
  data.tile_height = tileset.tile_height;
  data.tile_count = tileset.tile_count;
  data.column_count = tileset.column_count;

  const auto& texture = registry.get<Texture>(entity);
  data.absolute_image_path = std::filesystem::absolute(texture.path);
  data.image_width = texture.width;
  data.image_height = texture.height;

  const auto& ctx = registry.get<PropertyContext>(entity);
  data.name = ctx.name;
  data.properties = ConvertProperties(registry, ctx);

  data.tiles = ConvertFancyTiles(registry, tileset);

  return data;
}

[[nodiscard]] auto ConvertObject(const entt::registry& registry,
                                 const entt::entity entity) -> IO::ObjectData
{
  IO::ObjectData data;

  const auto& object = registry.get<Object>(entity);
  data.id = object.id;
  data.x = object.x;
  data.y = object.y;
  data.width = object.width;
  data.height = object.height;
  data.type = object.type;
  data.custom_type = object.custom_type;
  data.visible = object.visible;

  const auto& context = registry.get<PropertyContext>(entity);
  data.name = context.name;
  data.properties = ConvertProperties(registry, context);

  return data;
}

[[nodiscard]] auto ConvertLayer(const entt::registry& registry,
                                const entt::entity entity,
                                const LayerTreeNode& node,
                                const int32 nRows,
                                const int32 nCols) -> IO::LayerData
{
  const auto& layer = registry.get<Layer>(entity);
  const auto& context = registry.get<PropertyContext>(entity);

  IO::LayerData data;
  data.index = node.index;
  data.id = layer.id;
  data.type = layer.type;
  data.opacity = layer.opacity;
  data.is_visible = layer.visible;

  data.name = context.name;
  data.properties = ConvertProperties(registry, context);

  switch (data.type) {
    case LayerType::TileLayer: {
      auto& tileLayer = data.data.emplace<IO::TileLayerData>();

      tileLayer.row_count = nRows;
      tileLayer.col_count = nCols;
      tileLayer.tiles = registry.get<TileLayer>(entity).matrix;

      break;
    }

    case LayerType::ObjectLayer: {
      const auto& objectLayer = registry.get<ObjectLayer>(entity);

      auto& objectLayerData = data.data.emplace<IO::ObjectLayerData>();
      objectLayerData.objects.reserve(objectLayer.objects.size());

      for (const auto objectEntity : objectLayer.objects) {
        objectLayerData.objects.push_back(ConvertObject(registry, objectEntity));
      }

      break;
    }

    case LayerType::GroupLayer: {
      auto& groupLayer = data.data.emplace<IO::GroupLayerData>();
      groupLayer.layers.reserve(node.children.size());

      for (const auto child : node.children) {
        const auto& childNode = registry.get<LayerTreeNode>(child);
        groupLayer.layers.push_back(std::make_unique<IO::LayerData>(
            ConvertLayer(registry, child, childNode, nRows, nCols)));
      }

      break;
    }
  }

  return data;
}

}  // namespace

auto ConvertDocumentToIR(const Document& document) -> IO::MapData
{
  const auto& registry = document.registry;
  const auto& map = registry.ctx<Map>();

  IO::MapData data;
  data.absolute_path = std::filesystem::absolute(document.path);

  data.next_layer_id = map.next_layer_id;
  data.next_object_id = map.next_object_id;

  data.tile_width = map.tile_width;
  data.tile_height = map.tile_height;

  data.row_count = map.row_count;
  data.column_count = map.column_count;

  data.tilesets.reserve(registry.size<Tileset>());
  for (auto&& [entity, tileset] : registry.view<Tileset>().each()) {
    data.tilesets.push_back(ConvertTileset(registry, entity, tileset));
  }

  data.layers.reserve(registry.size<LayerTreeNode>());  // Will overestimate, oh well
  for (auto&& [entity, node] : registry.view<LayerTreeNode>().each()) {
    const auto& parent = registry.get<Parent>(entity);
    if (parent.entity == entt::null) {
      data.layers.push_back(
          ConvertLayer(registry, entity, node, data.row_count, data.column_count));
    }
  }

  data.properties = ConvertProperties(registry, registry.ctx<PropertyContext>());

  return data;
}

}  // namespace Tactile
