#include "layer_system.hpp"

#include <algorithm>  // sort
#include <utility>    // move, swap
#include <vector>     // erase

#include <fmt/format.h>  // format

#include "assert.hpp"
#include "core/components/attribute_context.hpp"
#include "core/components/object.hpp"
#include "core/components/parent.hpp"
#include "core/components/property.hpp"
#include "core/map.hpp"
#include "core/systems/duplicate_comp.hpp"
#include "core/systems/property_system.hpp"
#include "layer_tree_system.hpp"
#include "tactile_stdlib.hpp"
#include "tile_layer_system.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _new_layer_parent(const entt::registry& registry) -> entt::entity
{
  const auto active = registry.ctx<comp::active_layer>();
  if (active.entity != entt::null && registry.all_of<comp::group_layer>(active.entity)) {
    return active.entity;
  }
  {
    return entt::null;
  }
}

[[nodiscard]] auto _new_layer_index(const entt::registry& registry,
                                    const entt::entity layerEntity,
                                    const entt::entity parentEntity) -> usize
{
  if (parentEntity != entt::null) {
    const auto& node = registry.get<comp::layer_tree_node>(parentEntity);
    return node.children.size();
  }
  else {
    usize index = 0;
    for (auto&& [entity, layer, parent] :
         registry.view<comp::layer, comp::parent>().each()) {
      if (layerEntity != entity && parent.entity == entt::null) {
        ++index;
      }
    }

    return index;
  }
}

[[nodiscard]] auto _copy_layer(const entt::registry& registry, const entt::entity source)
    -> LayerSnapshot
{
  TACTILE_ASSERT(source != entt::null);
  TACTILE_ASSERT(registry.all_of<comp::layer>(source));
  TACTILE_ASSERT(registry.all_of<comp::layer_tree_node>(source));
  TACTILE_ASSERT(registry.all_of<comp::parent>(source));
  assert(
      (registry.any_of<comp::tile_layer, comp::object_layer, comp::group_layer>(source)));

  LayerSnapshot snapshot;
  snapshot.index = registry.get<comp::layer_tree_node>(source).index;
  snapshot.core = registry.get<comp::layer>(source);
  snapshot.context = CopyPropertyContext(registry, source);

  const auto parentEntity = registry.get<comp::parent>(source).entity;
  if (parentEntity != entt::null) {
    snapshot.parent = registry.get<comp::layer>(parentEntity).id;
  }

  switch (snapshot.core.type) {
    case layer_type::tile_layer: {
      snapshot.tiles = registry.get<comp::tile_layer>(source).matrix;
      break;
    }
    case layer_type::object_layer: {
      auto& objects = snapshot.objects.emplace();

      for (const auto objectEntity : registry.get<comp::object_layer>(source).objects) {
        auto& objectSnapshot = objects.emplace_back();
        objectSnapshot.core = registry.get<comp::object>(objectEntity);
        objectSnapshot.context = CopyPropertyContext(registry, objectEntity);
      }

      break;
    }
    case layer_type::group_layer: {
      auto& children = snapshot.children.emplace();

      for (const auto child : registry.get<comp::layer_tree_node>(source).children) {
        children.push_back(_copy_layer(registry, child));
      }

      break;
    }
  }

  return snapshot;
}

void _restore_layer_index(entt::registry& registry,
                          const entt::entity layerEntity,
                          const usize previousIndex)
{
  while (layer_local_index(registry, layerEntity) != previousIndex) {
    const auto index = layer_local_index(registry, layerEntity);
    if (index > previousIndex) {
      move_layer_up(registry, layerEntity);
    }
    else if (index < previousIndex) {
      move_layer_down(registry, layerEntity);
    }
  }
}

}  // namespace

auto make_basic_layer(entt::registry& registry,
                      const LayerID id,
                      const layer_type type,
                      std::string name,
                      const entt::entity parent) -> entt::entity
{
  const auto entity = registry.create();

  {
    auto& node = registry.emplace<comp::layer_tree_node>(entity);
    node.index = _new_layer_index(registry, entity, parent);
  }

  {
    auto& layer = registry.emplace<comp::layer>(entity);
    layer.id = id;
    layer.type = type;
    layer.visible = true;
    layer.opacity = 1.0f;
  }

  TACTILE_ASSERT(parent == entt::null || registry.all_of<comp::group_layer>(parent));
  registry.emplace<comp::parent>(entity, parent);

  if (parent != entt::null) {
    TACTILE_ASSERT(registry.all_of<comp::layer_tree_node>(parent));
    auto& parentNode = registry.get<comp::layer_tree_node>(parent);
    parentNode.children.push_back(entity);
  }

  {
    auto& context = AddPropertyContext(registry, entity);
    context.name = std::move(name);
  }

  return entity;
}

auto make_tile_layer(entt::registry& registry) -> entt::entity
{
  auto& map = registry.ctx<MapInfo>();

  const auto entity =
      make_basic_layer(registry,
                       map.next_layer_id,
                       layer_type::tile_layer,
                       fmt::format("Tile Layer {}", map.tile_layer_suffix),
                       _new_layer_parent(registry));
  ++map.next_layer_id;
  ++map.tile_layer_suffix;

  auto& tileLayer = registry.emplace<comp::tile_layer>(entity);
  tileLayer.matrix = MakeTileMatrix(map.row_count, map.column_count);

  return entity;
}

auto make_object_layer(entt::registry& registry) -> entt::entity
{
  auto& map = registry.ctx<MapInfo>();

  const auto entity =
      make_basic_layer(registry,
                       map.next_layer_id,
                       layer_type::object_layer,
                       fmt::format("Object Layer {}", map.object_layer_suffix),
                       _new_layer_parent(registry));
  ++map.next_layer_id;
  ++map.object_layer_suffix;

  registry.emplace<comp::object_layer>(entity);

  return entity;
}

auto make_group_layer(entt::registry& registry) -> entt::entity
{
  auto& map = registry.ctx<MapInfo>();

  const auto entity =
      make_basic_layer(registry,
                       map.next_layer_id,
                       layer_type::group_layer,
                       fmt::format("Group Layer {}", map.group_layer_suffix),
                       _new_layer_parent(registry));
  ++map.next_layer_id;
  ++map.group_layer_suffix;

  registry.emplace<comp::group_layer>(entity);

  return entity;
}

auto remove_layer(entt::registry& registry, const entt::entity entity) -> LayerSnapshot
{
  auto snapshot = _copy_layer(registry, entity);

  auto maybe_reset = [&](entt::entity& active, const entt::entity entity) {
    if (active != entt::null) {
      if (entity == active || is_child_layer_node(registry, entity, active)) {
        active = entt::null;
      }
    }
  };

  maybe_reset(registry.ctx<comp::active_layer>().entity, entity);
  maybe_reset(registry.ctx<comp::active_attribute_context>().entity, entity);

  destroy_layer_node(registry, entity);

  return snapshot;
}

auto restore_layer(entt::registry& registry, LayerSnapshot snapshot) -> entt::entity
{
  entt::entity parent{entt::null};
  if (snapshot.parent.has_value()) {
    parent = find_layer(registry, *snapshot.parent);
  }

  const auto entity = make_basic_layer(registry,
                                       snapshot.core.id,
                                       snapshot.core.type,
                                       snapshot.context.name,
                                       parent);

  {
    auto& layer = registry.get<comp::layer>(entity);
    layer.opacity = snapshot.core.opacity;
    layer.visible = snapshot.core.visible;
  }

  RestorePropertyContext(registry, entity, std::move(snapshot.context));

  switch (snapshot.core.type) {
    case layer_type::tile_layer: {
      auto& tileLayer = registry.emplace<comp::tile_layer>(entity);
      tileLayer.matrix = snapshot.tiles.value();
      break;
    }
    case layer_type::object_layer: {
      auto& objectLayer = registry.emplace<comp::object_layer>(entity);
      for (auto objectSnapshot : snapshot.objects.value()) {
        const auto objectEntity = registry.create();

        registry.emplace<comp::object>(objectEntity, std::move(objectSnapshot.core));
        RestorePropertyContext(registry, objectEntity, std::move(objectSnapshot.context));

        objectLayer.objects.push_back(objectEntity);
      }
      break;
    }
    case layer_type::group_layer: {
      /* We don't need to add the children to the restored group here, that is
         handled by make_basic_layer. */
      registry.emplace<comp::group_layer>(entity);
      for (auto layerSnapshot : snapshot.children.value()) {
        restore_layer(registry, std::move(layerSnapshot));
      }
      break;
    }
  }

  _restore_layer_index(registry, entity, snapshot.index);

  sort_layers(registry);
  return entity;
}

auto duplicate_layer(entt::registry& registry, const entt::entity source) -> entt::entity
{
  const auto& sourceParent = registry.get<comp::parent>(source);
  const auto copy = duplicate_layer(registry, source, sourceParent.entity, false);

  sort_layers(registry);

  return copy;
}

auto duplicate_layer(entt::registry& registry,
                     const entt::entity source,
                     const entt::entity parent,
                     const bool recursive) -> entt::entity
{
  /* The recursive flag determines whether indices need to be adjusted, since we
     do not touch indices of children of the original source layer that we want to
     duplicate. */

  if (!recursive) {
    increment_layer_indices_of_siblings_below(registry, source);
  }

  const auto copy = registry.create();

  {
    auto& node = registry.emplace<comp::layer_tree_node>(copy);
    node.index = registry.get<comp::layer_tree_node>(source).index;
  }

  registry.emplace<comp::parent>(copy, parent);

  if (parent != entt::null) {
    auto& parentNode = registry.get<comp::layer_tree_node>(parent);
    parentNode.children.push_back(copy);
  }

  {
    auto& context = DuplicateComp<comp::attribute_context>(registry, source, copy);
    if (!recursive) {
      context.name += " (Copy)";
    }
  }

  {
    auto& map = registry.ctx<MapInfo>();
    auto& layer = DuplicateComp<comp::layer>(registry, source, copy);
    layer.id = map.next_layer_id;

    if (!recursive) {
      const auto sourceNode = registry.get<comp::layer_tree_node>(source);

      auto& node = registry.get<comp::layer_tree_node>(copy);
      node.index = sourceNode.index + 1u;
    }

    ++map.next_layer_id;
  }

  if (registry.all_of<comp::tile_layer>(source)) {
    DuplicateComp<comp::tile_layer>(registry, source, copy);
  }
  else if (registry.all_of<comp::object_layer>(source)) {
    DuplicateComp<comp::object_layer>(registry, source, copy);
  }
  else if (registry.all_of<comp::group_layer>(source)) {
    registry.emplace<comp::group_layer>(copy);
    for (const auto sourceChild : registry.get<comp::layer_tree_node>(source).children) {
      /* We don't need to add the created child layer to the group layer explicitly */
      duplicate_layer(registry, sourceChild, copy, true);
    }
  }

  return copy;
}

auto find_layer(const entt::registry& registry, const LayerID id) -> entt::entity
{
  for (auto&& [entity, layer] : registry.view<comp::layer>().each()) {
    if (layer.id == id) {
      return entity;
    }
  }

  return entt::null;
}

auto get_layer_entity(const entt::registry& registry, const LayerID id) -> entt::entity
{
  const auto entity = sys::find_layer(registry, id);
  if (entity != entt::null && registry.all_of<comp::layer>(entity)) {
    return entity;
  }
  else {
    ThrowTraced(TactileError{"Invalid layer ID!"});
  }
}

auto get_layer(entt::registry& registry, const LayerID id)
    -> std::pair<entt::entity, comp::layer&>
{
  const auto entity = find_layer(registry, id);
  if (entity != entt::null && registry.all_of<comp::layer>(entity)) {
    return {entity, registry.get<comp::layer>(entity)};
  }
  else {
    ThrowTraced(TactileError{"Invalid layer identifier!"});
  }
}

auto get_layer(const entt::registry& registry, const LayerID id)
    -> std::pair<entt::entity, const comp::layer&>
{
  const auto entity = find_layer(registry, id);
  if (entity != entt::null && registry.all_of<comp::layer>(entity)) {
    return {entity, registry.get<comp::layer>(entity)};
  }
  else {
    ThrowTraced(TactileError{"Invalid layer identifier!"});
  }
}

auto get_active_layer(const entt::registry& registry) -> entt::entity
{
  const auto& active = registry.ctx<comp::active_layer>();
  return active.entity;
}

auto is_tile_layer_active(const entt::registry& registry) -> bool
{
  const auto& active = registry.ctx<comp::active_layer>();
  if (active.entity != entt::null) {
    return registry.all_of<comp::tile_layer>(active.entity);
  }
  else {
    return false;
  }
}

auto is_object_layer_active(const entt::registry& registry) -> bool
{
  const auto& active = registry.ctx<comp::active_layer>();
  if (active.entity != entt::null) {
    return registry.all_of<comp::object_layer>(active.entity);
  }
  else {
    return false;
  }
}

auto get_active_layer_id(const entt::registry& registry) -> Maybe<LayerID>
{
  const auto& active = registry.ctx<comp::active_layer>();
  if (active.entity != entt::null) {
    return registry.get<comp::layer>(active.entity).id;
  }
  else {
    return nothing;
  }
}

}  // namespace tactile::sys
