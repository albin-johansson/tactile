#include "convert_document_to_ir.hpp"

#include <memory>  // make_unique

#include "core/components/animation.hpp"
#include "core/components/attribute_context.hpp"
#include "core/components/component.hpp"
#include "core/components/fancy_tile.hpp"
#include "core/components/parent.hpp"
#include "core/components/property.hpp"
#include "core/components/texture.hpp"
#include "core/components/tileset.hpp"
#include "core/map.hpp"
#include "core/systems/component_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "editor/document.hpp"
#include "profile.hpp"

namespace tactile {
namespace {

void _convert_attribute_context(ir::attribute_context_data& data,
                                const comp::attribute_context& context,
                                const entt::registry& registry)
{
  for (const auto propertyEntity : context.properties) {
    const auto& property = registry.get<comp::property>(propertyEntity);
    data.properties[property.name] = property.value;
  }

  for (const auto componentEntity : context.components) {
    const auto& component = registry.get<comp::component>(componentEntity);
    const auto type = sys::get_component_def_name(registry, component.type);

    auto& attributes = data.components[type];
    for (const auto& [attrName, attrValue] : component.values) {
      attributes[attrName] = attrValue;
    }
  }
}

void _convert_object(ir::object_data& data,
                     const comp::object& object,
                     const comp::attribute_context& context,
                     const entt::registry& registry)
{
  data.id = object.id;
  data.type = object.type;

  data.x = object.x;
  data.y = object.y;
  data.width = object.width;
  data.height = object.height;

  data.tag = object.tag;
  data.visible = object.visible;

  data.name = context.name;
  _convert_attribute_context(data.context, context, registry);
}

void _convert_layer(ir::layer_data& data,
                    usize index,
                    const entt::registry& registry,
                    entt::entity entity);

void _convert_object_layer(ir::object_layer_data& data,
                           const entt::registry& registry,
                           const entt::entity entity)
{
  const auto& objectLayer = registry.get<comp::object_layer>(entity);
  data.objects.reserve(objectLayer.objects.size());

  for (const auto objectEntity : objectLayer.objects) {
    const auto& object = registry.get<comp::object>(objectEntity);
    const auto& context = registry.get<comp::attribute_context>(objectEntity);

    auto& objectData = data.objects.emplace_back();
    _convert_object(objectData, object, context, registry);
  }
}

void _convert_group_layer(ir::group_layer_data& data,
                          const entt::registry& registry,
                          const entt::entity entity)
{
  const auto& node = registry.get<comp::layer_tree_node>(entity);
  data.children.reserve(node.children.size());

  usize index = 0;
  for (const auto childEntity : node.children) {
    auto& childLayerData = data.children.emplace_back(std::make_unique<ir::layer_data>());
    _convert_layer(*childLayerData, index, registry, childEntity);
    ++index;
  }
}

void _convert_layer(ir::layer_data& data,
                    const usize index,
                    const entt::registry& registry,
                    const entt::entity entity)
{
  const auto& layer = registry.get<comp::layer>(entity);

  data.index = index;

  data.id = layer.id;
  data.type = layer.type;
  data.opacity = layer.opacity;
  data.visible = layer.visible;

  switch (layer.type) {
    case layer_type::tile_layer: {
      const auto& tileLayer = registry.get<comp::tile_layer>(entity);

      auto& tileLayerData = data.data.emplace<ir::tile_layer_data>();
      tileLayerData.tiles = tileLayer.matrix;

      const auto& mapInfo = registry.ctx<MapInfo>();
      tileLayerData.row_count = mapInfo.row_count;
      tileLayerData.col_count = mapInfo.column_count;

      break;
    }
    case layer_type::object_layer: {
      auto& objectLayerData = data.data.emplace<ir::object_layer_data>();
      _convert_object_layer(objectLayerData, registry, entity);
      break;
    }
    case layer_type::group_layer: {
      auto& groupLayerData = data.data.emplace<ir::group_layer_data>();
      _convert_group_layer(groupLayerData, registry, entity);
      break;
    }
  }

  const auto& context = registry.get<comp::attribute_context>(entity);
  data.name = context.name;
  _convert_attribute_context(data.context, context, registry);
}

void _convert_layers(ir::map_data& data, const entt::registry& registry)
{
  usize index = 0;

  /* Only iterate top-level layers */
  for (auto&& [entity, node] : registry.view<comp::layer_tree_node>().each()) {
    const auto& parent = registry.get<comp::parent>(entity);
    if (parent.entity == entt::null) {
      auto& layerData = data.layers.emplace_back();
      _convert_layer(layerData, index, registry, entity);
      ++index;
    }
  }
}

void _convert_fancy_tile_animation(ir::fancy_tile_data& data,
                                   const comp::animation& animation,
                                   const entt::registry& registry)
{
  data.frames.reserve(animation.frames.size());

  for (const auto frameEntity : animation.frames) {
    const auto& frame = registry.get<comp::animation_frame>(frameEntity);

    auto& frameData = data.frames.emplace_back();
    frameData.local_id = sys::convert_to_local(registry, frame.tile).value();
    frameData.duration_ms = frame.duration.count();
  }
}

void _convert_fancy_tiles(ir::tileset_data& data,
                          const comp::tileset& tileset,
                          const entt::registry& registry)
{
  for (auto&& [entity, tile, context] :
       registry.view<comp::fancy_tile, comp::attribute_context>().each()) {
    if (tile.id >= tileset.first_id && tile.id <= tileset.last_id) {
      const bool interesting = registry.all_of<comp::animation>(entity) ||
                               !context.properties.empty() ||
                               !context.properties.empty() || !tile.objects.empty();

      if (interesting) {
        const auto local = sys::convert_to_local(registry, tile.id).value();
        auto& tileData = data.fancy_tiles[local];

        if (const auto* animation = registry.try_get<comp::animation>(entity)) {
          _convert_fancy_tile_animation(tileData, *animation, registry);
        }

        if (!context.properties.empty() || !context.components.empty()) {
          _convert_attribute_context(tileData.context, context, registry);
        }

        if (!tile.objects.empty()) {
          for (const auto objectEntity : tile.objects) {
            const auto& object = registry.get<comp::object>(objectEntity);
            const auto& objectContext =
                registry.get<comp::attribute_context>(objectEntity);

            auto& objectData = tileData.objects.emplace_back();
            _convert_object(objectData, object, objectContext, registry);
          }
        }
      }
    }
  }
}

void _convert_tilesets(ir::map_data& data, const entt::registry& registry)
{
  for (auto&& [entity, tileset, texture, context] :
       registry.view<comp::tileset, comp::texture, comp::attribute_context>().each()) {
    auto& tilesetData = data.tilesets.emplace_back();
    tilesetData.name = context.name;

    tilesetData.first_tile = tileset.first_id;
    tilesetData.tile_width = tileset.tile_width;
    tilesetData.tile_height = tileset.tile_height;
    tilesetData.tile_count = tileset.tile_count;
    tilesetData.column_count = tileset.column_count;

    tilesetData.image_path = texture.path;
    tilesetData.image_width = texture.width;
    tilesetData.image_height = texture.height;

    _convert_fancy_tiles(tilesetData, tileset, registry);
  }
}

void _convert_component_definitions(ir::map_data& data, const entt::registry& registry)
{
  for (auto&& [entity, def] : registry.view<comp::component_def>().each()) {
    auto& attributes = data.component_definitions[def.name];
    for (const auto& [attrName, attrValue] : def.attributes) {
      attributes[attrName] = attrValue;
    }
  }
}

void _convert_basic_map_info(ir::map_data& data, const MapInfo& mapInfo)
{
  data.row_count = mapInfo.row_count;
  data.col_count = mapInfo.column_count;

  data.tile_width = mapInfo.tile_width;
  data.tile_height = mapInfo.tile_height;

  data.next_layer_id = mapInfo.next_layer_id;
  data.next_object_id = mapInfo.next_object_id;
}

}  // namespace

auto convert_document_to_ir(const Document& document) -> ir::map_data
{
  TACTILE_PROFILE_START

  const auto& registry = document.registry;

  ir::map_data data;
  _convert_basic_map_info(data, registry.ctx<MapInfo>());

  _convert_component_definitions(data, registry);
  _convert_tilesets(data, registry);

  data.layers.reserve(registry.storage<comp::layer>().size());
  _convert_layers(data, registry);

  _convert_attribute_context(data.context,
                             registry.ctx<comp::attribute_context>(),
                             registry);

  TACTILE_PROFILE_END("Converted document to IR")
  return data;
}

}  // namespace tactile