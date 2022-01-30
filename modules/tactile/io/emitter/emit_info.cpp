#include "emit_info.hpp"

#include <utility>  // move
#include <vector>   // vector

#include "assert.hpp"
#include "core/components/animation.hpp"
#include "core/components/attribute_context.hpp"
#include "core/components/component.hpp"
#include "core/components/fancy_tile.hpp"
#include "core/components/layer.hpp"
#include "core/components/parent.hpp"
#include "core/components/tileset.hpp"
#include "core/map.hpp"
#include "core/systems/component_system.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/object_layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/systems/object_system.hpp"
#include "core/systems/property_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "tactile_stdlib.hpp"
#include "throw.hpp"

namespace tactile::emitter {
namespace {

template <typename T>
void _each_object(const entt::registry& registry,
                  const std::vector<entt::entity>& objects,
                  const T& func)
{
  for (const auto objectEntity : objects) {
    const auto& object = registry.get<comp::object>(objectEntity);
    const auto& context = registry.get<comp::attribute_context>(objectEntity);

    ObjectData data;
    data.id = object.id;

    data.x = object.x;
    data.y = object.y;
    data.width = object.width;
    data.height = object.height;

    data.name = context.name;
    data.tag = object.tag;

    data.visible = object.visible;

    func(data);
  }
}

}  // namespace

EmitInfo::EmitInfo(std::filesystem::path destination, const entt::registry& registry)
    : mDestinationFile{std::move(destination)}
    , mDestinationDir{mDestinationFile.parent_path()}
    , mRegistry{&registry}
{}

auto EmitInfo::component_def_count() const -> usize
{
  return mRegistry->storage<comp::component_def>().size();
}

auto EmitInfo::component_def_name(const usize index) const -> const std::string&
{
  const auto entity = mRegistry->storage<comp::component_def>().at(index);
  if (entity != entt::null) {
    const auto& def = mRegistry->get<comp::component_def>(entity);
    return def.name;
  }
  else {
    ThrowTraced(TactileError{"Invalid component definition index!"});
  }
}

auto EmitInfo::component_def_attr_count(const usize index) const -> usize
{
  const auto entity = mRegistry->storage<comp::component_def>().at(index);
  if (entity != entt::null) {
    const auto& def = mRegistry->get<comp::component_def>(entity);
    return def.attributes.size();
  }
  else {
    ThrowTraced(TactileError{"Invalid component definition index!"});
  }
}

void EmitInfo::each_component_def_attrs(const component_def_attr_visitor& func,
                                        const usize definitionIndex) const
{
  const auto entity = mRegistry->storage<comp::component_def>().at(definitionIndex);
  if (entity != entt::null) {
    const auto& def = mRegistry->get<comp::component_def>(entity);
    for (const auto& [attrName, attrValue] : def.attributes) {
      func(attrName, attrValue);
    }
  }
  else {
    ThrowTraced(TactileError{"Invalid component definition index!"});
  }
}

auto EmitInfo::tileset_count() const -> usize
{
  return mRegistry->storage<comp::tileset>().size();
}

void EmitInfo::each_tileset(const tileset_visitor& func) const
{
  for (auto&& [entity, tileset, context, texture] :
       mRegistry->view<comp::tileset, comp::attribute_context, comp::texture>().each()) {
    TilesetData data;

    data.id = tileset.id;
    data.first_tile = tileset.first_id;

    data.tile_width = tileset.tile_width;
    data.tile_height = tileset.tile_height;
    data.tile_count = tileset.tile_count;
    data.column_count = tileset.column_count;

    data.name = context.name;

    data.image_path = texture.path;
    data.image_width = texture.width;
    data.image_height = texture.height;

    func(data);
  }
}

auto EmitInfo::tileset_context(const TilesetID id) const -> ContextID
{
  const auto entity = sys::get_tileset_entity(*mRegistry, id);
  return mRegistry->get<comp::attribute_context>(entity).id;
}

auto EmitInfo::tileset_fancy_tile_count(TilesetID id) const -> usize
{
  usize count = 0;
  each_tileset_fancy_tile(id, [&count](const FancyTileData&) { ++count; });
  return count;
}

void EmitInfo::each_tileset_fancy_tile(const TilesetID id,
                                       const fancy_tile_visitor& func) const
{
  const auto tilesetEntity = sys::get_tileset_entity(*mRegistry, id);
  const auto& tileset = mRegistry->get<comp::tileset>(tilesetEntity);

  for (auto&& [entity, tile, context] :
       mRegistry->view<comp::fancy_tile, comp::attribute_context>().each()) {
    const bool inTileset = tile.id >= tileset.first_id && tile.id <= tileset.last_id;
    if (inTileset) {
      const bool worthSaving = mRegistry->all_of<comp::animation>(entity) ||
                               !context.properties.empty() || !tile.objects.empty();
      if (worthSaving) {
        FancyTileData data;
        data.global_id = tile.id;
        data.local_id = sys::convert_to_local(*mRegistry, tile.id).value();
        data.context_id = context.id;

        func(data);
      }
    }
  }
}

auto EmitInfo::fancy_tile_object_count(const TileID id) const -> usize
{
  usize count = 0;
  each_fancy_tile_object(id, [&count](const ObjectData&) { ++count; });
  return count;
}

void EmitInfo::each_fancy_tile_object(const TileID id, const object_visitor& func) const
{
  const auto entity = sys::get_tile_entity(*mRegistry, id);
  const auto& tile = mRegistry->get<comp::fancy_tile>(entity);
  _each_object(*mRegistry, tile.objects, func);
}

auto EmitInfo::fancy_tile_animation_frame_count(const TileID id) const -> usize
{
  const auto entity = sys::get_tile_entity(*mRegistry, id);

  if (const auto* animation = mRegistry->try_get<comp::animation>(entity)) {
    return animation->frames.size();
  }

  return 0;
}

void EmitInfo::each_fancy_tile_animation_frame(const TileID id,
                                               const animation_frame_visitor& func) const
{
  const auto entity = sys::get_tile_entity(*mRegistry, id);

  const auto& animation = mRegistry->get<comp::animation>(entity);
  for (const auto frameEntity : animation.frames) {
    const auto& frame = mRegistry->get<comp::animation_frame>(frameEntity);

    AnimationFrameData data;
    data.local_tile = sys::convert_to_local(*mRegistry, frame.tile).value();
    data.duration_ms = frame.duration.count();

    func(data);
  }
}

auto EmitInfo::component_count(const ContextID id) const -> usize
{
  return sys::get_component_count(*mRegistry, id);
}

void EmitInfo::each_component(const ContextID id, const component_visitor& func) const
{
  const auto& context = sys::GetContext(*mRegistry, id);
  for (const auto componentEntity : context.components) {
    const auto& component = mRegistry->get<comp::component>(componentEntity);
    const auto type = sys::get_component_def_name(*mRegistry, component.type);
    func(type, component.values);
  }
}

auto EmitInfo::root_context() const -> ContextID
{
  return mRegistry->ctx<comp::attribute_context>().id;
}

auto EmitInfo::layer_count(const Maybe<LayerID> id) const -> usize
{
  if (id) {
    const auto entity = sys::get_layer_entity(*mRegistry, *id);
    const auto& node = mRegistry->get<comp::layer_tree_node>(entity);
    return node.children.size();
  }
  else {
    return mRegistry->storage<comp::layer>().size();
  }
}

void EmitInfo::each_layer(const Maybe<LayerID> parentId, const layer_visitor& func) const
{
  if (!parentId) {
    /* Only iterate top-level layers */
    for (auto&& [entity, node] : mRegistry->view<comp::layer_tree_node>().each()) {
      const auto& parent = mRegistry->get<comp::parent>(entity);
      if (parent.entity == entt::null) {
        const auto& layer = mRegistry->get<comp::layer>(entity);
        func(layer.id);
      }
    }
  }
  else {
    /* Only visit immediate layer children, recursive visits are handled by the emitter */
    const auto parentEntity = sys::get_layer_entity(*mRegistry, *parentId);
    const auto& node = mRegistry->get<comp::layer_tree_node>(parentEntity);
    for (const auto child : node.children) {
      const auto& childLayer = mRegistry->get<comp::layer>(child);
      func(childLayer.id);
    }
  }
}

auto EmitInfo::layer_data(const LayerID id) const -> LayerData
{
  const auto entity = sys::get_layer_entity(*mRegistry, id);

  const auto& context = mRegistry->get<comp::attribute_context>(entity);
  const auto& layer = mRegistry->get<comp::layer>(entity);

  LayerData data;

  data.name = context.name;
  data.type = layer.type;
  data.opacity = layer.opacity;
  data.visible = layer.visible;

  return data;
}

void EmitInfo::each_layer_tile(const LayerID id, const layer_tile_visitor& func) const
{
  const auto entity = sys::get_tile_layer_entity(*mRegistry, id);
  const auto& layer = mRegistry->get<comp::tile_layer>(entity);
  sys::each_tile(layer, func);
}

auto EmitInfo::layer_object_count(const LayerID id) const -> usize
{
  const auto&& [entity, layer] = sys::get_object_layer(*mRegistry, id);
  return layer.objects.size();
}

void EmitInfo::each_layer_object(const LayerID id, const object_visitor& func) const
{
  const auto&& [entity, layer] = sys::get_object_layer(*mRegistry, id);
  _each_object(*mRegistry, layer.objects, func);
}

auto EmitInfo::layer_context(const LayerID id) const -> ContextID
{
  const auto entity = sys::find_layer(*mRegistry, id);
  return mRegistry->get<comp::attribute_context>(entity).id;
}

auto EmitInfo::object_context(const ObjectID id) const -> ContextID
{
  const auto entity = sys::get_object(*mRegistry, id);
  return mRegistry->get<comp::attribute_context>(entity).id;
}

auto EmitInfo::property_count(const ContextID id) const -> usize
{
  const auto& context = sys::GetContext(*mRegistry, id);
  return context.properties.size();
}

void EmitInfo::each_property(const ContextID id, const property_visitor& func) const
{
  const auto& context = sys::GetContext(*mRegistry, id);
  for (const auto propertyEntity : context.properties) {
    const auto& property = mRegistry->get<comp::property>(propertyEntity);
    func(property.name, property.value);
  }
}

auto EmitInfo::tile_width() const -> int32
{
  const auto& map = mRegistry->ctx<MapInfo>();
  return map.tile_width;
}

auto EmitInfo::tile_height() const -> int32
{
  const auto& map = mRegistry->ctx<MapInfo>();
  return map.tile_height;
}

auto EmitInfo::next_layer_id() const -> LayerID
{
  const auto& map = mRegistry->ctx<MapInfo>();
  return map.next_layer_id;
}

auto EmitInfo::next_object_id() const -> ObjectID
{
  const auto& map = mRegistry->ctx<MapInfo>();
  return map.next_object_id;
}

auto EmitInfo::row_count() const -> usize
{
  const auto& map = mRegistry->ctx<MapInfo>();
  return map.row_count;
}

auto EmitInfo::column_count() const -> usize
{
  const auto& map = mRegistry->ctx<MapInfo>();
  return map.column_count;
}

auto EmitInfo::destination_file() const -> const std::filesystem::path&
{
  return mDestinationFile;
}

auto EmitInfo::destination_dir() const -> const std::filesystem::path&
{
  return mDestinationDir;
}

}  // namespace tactile::emitter
