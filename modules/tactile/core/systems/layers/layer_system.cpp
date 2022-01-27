#include "layer_system.hpp"

#include <algorithm>  // sort
#include <utility>    // move, swap
#include <vector>     // erase

#include <fmt/format.h>  // format
#include <tactile_stdlib.hpp>

#include "assert.hpp"
#include "core/components/attribute_context.hpp"
#include "core/components/object.hpp"
#include "core/components/parent.hpp"
#include "core/components/property.hpp"
#include "core/map.hpp"
#include "core/systems/duplicate_comp.hpp"
#include "core/systems/property_system.hpp"
#include "layer_tree_system.hpp"
#include "tile_layer_system.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto GetNewLayerParent(const entt::registry& registry) -> entt::entity
{
  const auto active = registry.ctx<ActiveLayer>();
  if (active.entity != entt::null && registry.all_of<GroupLayer>(active.entity)) {
    return active.entity;
  }
  {
    return entt::null;
  }
}

[[nodiscard]] auto GetNewLayerIndex(const entt::registry& registry,
                                    const entt::entity layerEntity,
                                    const entt::entity parentEntity) -> usize
{
  if (parentEntity != entt::null) {
    const auto& node = registry.get<LayerTreeNode>(parentEntity);
    return node.children.size();
  }
  else {
    usize index = 0;
    for (auto&& [entity, layer, parent] : registry.view<Layer, comp::parent>().each()) {
      if (layerEntity != entity && parent.entity == entt::null) {
        ++index;
      }
    }

    return index;
  }
}

}  // namespace

auto make_basic_layer(entt::registry& registry,
                      const LayerID id,
                      const LayerType type,
                      std::string name,
                      const entt::entity parent) -> entt::entity
{
  const auto entity = registry.create();

  {
    auto& node = registry.emplace<LayerTreeNode>(entity);
    node.index = GetNewLayerIndex(registry, entity, parent);
  }

  {
    auto& layer = registry.emplace<Layer>(entity);
    layer.id = id;
    layer.type = type;
    layer.visible = true;
    layer.opacity = 1.0f;
  }

  TACTILE_ASSERT(parent == entt::null || registry.all_of<GroupLayer>(parent));
  registry.emplace<comp::parent>(entity, parent);

  if (parent != entt::null) {
    TACTILE_ASSERT(registry.all_of<LayerTreeNode>(parent));
    auto& parentNode = registry.get<LayerTreeNode>(parent);
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
                       LayerType::TileLayer,
                       fmt::format("Tile Layer {}", map.tile_layer_suffix),
                       GetNewLayerParent(registry));
  ++map.next_layer_id;
  ++map.tile_layer_suffix;

  auto& tileLayer = registry.emplace<TileLayer>(entity);
  tileLayer.matrix = MakeTileMatrix(map.row_count, map.column_count);

  return entity;
}

auto make_object_layer(entt::registry& registry) -> entt::entity
{
  auto& map = registry.ctx<MapInfo>();

  const auto entity =
      make_basic_layer(registry,
                       map.next_layer_id,
                       LayerType::ObjectLayer,
                       fmt::format("Object Layer {}", map.object_layer_suffix),
                       GetNewLayerParent(registry));
  ++map.next_layer_id;
  ++map.object_layer_suffix;

  registry.emplace<ObjectLayer>(entity);

  return entity;
}

auto make_group_layer(entt::registry& registry) -> entt::entity
{
  auto& map = registry.ctx<MapInfo>();

  const auto entity =
      make_basic_layer(registry,
                       map.next_layer_id,
                       LayerType::GroupLayer,
                       fmt::format("Group Layer {}", map.group_layer_suffix),
                       GetNewLayerParent(registry));
  ++map.next_layer_id;
  ++map.group_layer_suffix;

  registry.emplace<GroupLayer>(entity);

  return entity;
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
    auto& layer = registry.get<Layer>(entity);
    layer.opacity = snapshot.core.opacity;
    layer.visible = snapshot.core.visible;
  }

  RestorePropertyContext(registry, entity, std::move(snapshot.context));

  switch (snapshot.core.type) {
    case LayerType::TileLayer: {
      auto& tileLayer = registry.emplace<TileLayer>(entity);
      tileLayer.matrix = snapshot.tiles.value();
      break;
    }
    case LayerType::ObjectLayer: {
      auto& objectLayer = registry.emplace<ObjectLayer>(entity);
      for (auto objectSnapshot : snapshot.objects.value()) {
        const auto objectEntity = registry.create();

        registry.emplace<comp::object>(objectEntity, std::move(objectSnapshot.core));
        RestorePropertyContext(registry, objectEntity, std::move(objectSnapshot.context));

        objectLayer.objects.push_back(objectEntity);
      }
      break;
    }
    case LayerType::GroupLayer: {
      /* We don't need to add the children to the restored group here, that is
         handled by AddBasicLayer. */
      registry.emplace<GroupLayer>(entity);
      for (auto layerSnapshot : snapshot.children.value()) {
        restore_layer(registry, std::move(layerSnapshot));
      }
      break;
    }
  }

  /* Restore the previous layer index */
  while (get_layer_index(registry, entity) != snapshot.index) {
    const auto index = get_layer_index(registry, entity);
    if (index > snapshot.index) {
      layer_tree::MoveNodeUp(registry, entity);
    }
    else if (index < snapshot.index) {
      layer_tree::MoveNodeDown(registry, entity);
    }
  }

  layer_tree::SortNodes(registry);
  return entity;
}

void sort_layers(entt::registry& registry)
{
  layer_tree::SortNodes(registry);
}

void remove_layer(entt::registry& registry, const entt::entity entity)
{
  auto maybe_reset = [&](entt::entity& active, const entt::entity entity) {
    if (active != entt::null) {
      if (entity == active || layer_tree::IsChildNode(registry, entity, active)) {
        active = entt::null;
      }
    }
  };

  maybe_reset(registry.ctx<ActiveLayer>().entity, entity);
  maybe_reset(registry.ctx<comp::active_attribute_context>().entity, entity);

  layer_tree::DestroyNode(registry, entity);
}

auto copy_layer(const entt::registry& registry, const entt::entity source)
    -> LayerSnapshot
{
  TACTILE_ASSERT(source != entt::null);
  TACTILE_ASSERT(registry.all_of<Layer>(source));
  TACTILE_ASSERT(registry.all_of<LayerTreeNode>(source));
  TACTILE_ASSERT(registry.all_of<comp::parent>(source));
  assert((registry.any_of<TileLayer, ObjectLayer, GroupLayer>(source)));

  LayerSnapshot snapshot;
  snapshot.index = registry.get<LayerTreeNode>(source).index;
  snapshot.core = registry.get<Layer>(source);
  snapshot.context = CopyPropertyContext(registry, source);

  const auto parentEntity = registry.get<comp::parent>(source).entity;
  if (parentEntity != entt::null) {
    snapshot.parent = registry.get<Layer>(parentEntity).id;
  }

  switch (snapshot.core.type) {
    case LayerType::TileLayer: {
      snapshot.tiles = registry.get<TileLayer>(source).matrix;
      break;
    }
    case LayerType::ObjectLayer: {
      auto& objects = snapshot.objects.emplace();

      for (const auto objectEntity : registry.get<ObjectLayer>(source).objects) {
        auto& objectSnapshot = objects.emplace_back();
        objectSnapshot.core = registry.get<comp::object>(objectEntity);
        objectSnapshot.context = CopyPropertyContext(registry, objectEntity);
      }

      break;
    }
    case LayerType::GroupLayer: {
      auto& children = snapshot.children.emplace();

      for (const auto child : registry.get<LayerTreeNode>(source).children) {
        children.push_back(copy_layer(registry, child));
      }

      break;
    }
  }

  return snapshot;
}

auto duplicate_layer(entt::registry& registry, const entt::entity source) -> entt::entity
{
  const auto& sourceParent = registry.get<comp::parent>(source);
  const auto copy = duplicate_layer(registry, source, sourceParent.entity, false);

  layer_tree::SortNodes(registry);

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
    layer_tree::IncrementIndicesOfSiblingsBelow(registry, source);
  }

  const auto copy = registry.create();

  {
    auto& node = registry.emplace<LayerTreeNode>(copy);
    node.index = registry.get<LayerTreeNode>(source).index;
  }

  registry.emplace<comp::parent>(copy, parent);

  if (parent != entt::null) {
    auto& parentNode = registry.get<LayerTreeNode>(parent);
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
    auto& layer = DuplicateComp<Layer>(registry, source, copy);
    layer.id = map.next_layer_id;

    if (!recursive) {
      const auto sourceNode = registry.get<LayerTreeNode>(source);

      auto& node = registry.get<LayerTreeNode>(copy);
      node.index = sourceNode.index + 1u;
    }

    ++map.next_layer_id;
  }

  if (registry.all_of<TileLayer>(source)) {
    DuplicateComp<TileLayer>(registry, source, copy);
  }
  else if (registry.all_of<ObjectLayer>(source)) {
    DuplicateComp<ObjectLayer>(registry, source, copy);
  }
  else if (registry.all_of<GroupLayer>(source)) {
    registry.emplace<GroupLayer>(copy);
    for (const auto sourceChild : registry.get<LayerTreeNode>(source).children) {
      /* We don't need to add the created child layer to the group layer explicitly */
      duplicate_layer(registry, sourceChild, copy, true);
    }
  }

  return copy;
}

void move_layer_up(entt::registry& registry, const entt::entity entity)
{
  layer_tree::MoveNodeUp(registry, entity);
}

void move_layer_down(entt::registry& registry, const entt::entity entity)
{
  layer_tree::MoveNodeDown(registry, entity);
}

void set_layer_opacity(entt::registry& registry,
                       const entt::entity entity,
                       const float opacity)
{
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<Layer>(entity));

  auto& layer = registry.get<Layer>(entity);
  layer.opacity = opacity;
}

void set_layer_visible(entt::registry& registry,
                       const entt::entity entity,
                       const bool visible)
{
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<Layer>(entity));

  auto& layer = registry.get<Layer>(entity);
  layer.visible = visible;
}

auto find_layer(const entt::registry& registry, const LayerID id) -> entt::entity
{
  for (auto&& [entity, layer] : registry.view<Layer>().each()) {
    if (layer.id == id) {
      return entity;
    }
  }

  return entt::null;
}

auto get_layer(entt::registry& registry, const LayerID id)
    -> std::pair<entt::entity, Layer&>
{
  const auto entity = find_layer(registry, id);
  if (entity != entt::null && registry.all_of<Layer>(entity)) {
    return {entity, registry.get<Layer>(entity)};
  }
  else {
    ThrowTraced(TactileError{"Invalid layer identifier!"});
  }
}

auto get_layer(const entt::registry& registry, const LayerID id)
    -> std::pair<entt::entity, const Layer&>
{
  const auto entity = find_layer(registry, id);
  if (entity != entt::null && registry.all_of<Layer>(entity)) {
    return {entity, registry.get<Layer>(entity)};
  }
  else {
    ThrowTraced(TactileError{"Invalid layer identifier!"});
  }
}

auto get_active_layer(const entt::registry& registry) -> entt::entity
{
  const auto& active = registry.ctx<ActiveLayer>();
  return active.entity;
}

auto get_layer_index(const entt::registry& registry, const entt::entity entity) -> usize
{
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<LayerTreeNode>(entity));
  return registry.get<LayerTreeNode>(entity).index;
}

auto can_move_layer_up(const entt::registry& registry, const entt::entity entity) -> bool
{
  return layer_tree::CanMoveNodeUp(registry, entity);
}

auto can_move_layer_down(const entt::registry& registry, const entt::entity entity)
    -> bool
{
  return layer_tree::CanMoveNodeDown(registry, entity);
}

auto is_tile_layer_active(const entt::registry& registry) -> bool
{
  const auto& active = registry.ctx<ActiveLayer>();
  if (active.entity != entt::null) {
    return registry.all_of<TileLayer>(active.entity);
  }
  else {
    return false;
  }
}

auto is_object_layer_active(const entt::registry& registry) -> bool
{
  const auto& active = registry.ctx<ActiveLayer>();
  if (active.entity != entt::null) {
    return registry.all_of<ObjectLayer>(active.entity);
  }
  else {
    return false;
  }
}

auto get_active_layer_id(const entt::registry& registry) -> Maybe<LayerID>
{
  const auto& active = registry.ctx<ActiveLayer>();
  if (active.entity != entt::null) {
    return registry.get<Layer>(active.entity).id;
  }
  else {
    return nothing;
  }
}

}  // namespace tactile::sys
