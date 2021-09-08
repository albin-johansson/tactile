#include "layer_system.hpp"

#include <algorithm>  // sort
#include <cassert>    // assert
#include <format>     // format
#include <utility>    // move, swap
#include <vector>     // erase

#include "build.hpp"
#include "copy.hpp"
#include "core/components/group_layer.hpp"
#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/components/object_layer.hpp"
#include "core/components/parent.hpp"
#include "core/components/property.hpp"
#include "core/components/property_context.hpp"
#include "core/components/tile_layer.hpp"
#include "core/map.hpp"
#include "property_system.hpp"
#include "tile_layer_system.hpp"

namespace Tactile::Sys {
namespace {

[[nodiscard]] auto GetNewLayerParent(const entt::registry& registry) -> entt::entity
{
  const auto active = registry.ctx<ActiveLayer>();
  if (active.entity != entt::null && registry.all_of<GroupLayer>(active.entity))
  {
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
  if (parentEntity != entt::null)
  {
    const auto& group = registry.get<GroupLayer>(parentEntity);
    return group.layers.size();
  }
  else
  {
    usize index = 0;
    for (auto&& [entity, layer, parent] : registry.view<Layer, Parent>().each())
    {
      if (layerEntity != entity && parent.entity == entt::null)
      {
        ++index;
      }
    }

    return index;
  }
}

[[nodiscard]] auto GetLayerSibling(const entt::registry& registry,
                                   const entt::entity entity,
                                   const usize targetIndex) -> entt::entity
{
  const auto& parent = registry.get<Parent>(entity);
  if (parent.entity != entt::null)
  {
    const auto& parentLayer = registry.get<GroupLayer>(parent.entity);
    for (const auto child : parentLayer.layers)
    {
      const auto& childLayer = registry.get<Layer>(child);
      if (childLayer.index == targetIndex)
      {
        return child;
      }
    }
  }
  else
  {
    for (auto&& [otherEntity, otherLayer, otherParent] :
         registry.view<Layer, Parent>().each())
    {
      if (otherParent.entity == entt::null && otherLayer.index == targetIndex)
      {
        return otherEntity;
      }
    }
  }

  return entt::null;
}

[[nodiscard]] auto CountAllLayersAbove(const entt::registry& registry,
                                       const entt::entity entity) -> usize
{
  usize count = 0;

  auto sibling = GetLayerSiblingAbove(registry, entity);
  while (sibling != entt::null)
  {
    count += GetLayerChildrenCount(registry, sibling);
    sibling = GetLayerSiblingAbove(registry, sibling);
  }

  return count;
}

void SwapLayerIndices(entt::registry& registry,
                      const entt::entity entity,
                      const entt::entity targetEntity)
{
  assert(entity != entt::null);
  assert(targetEntity != entt::null);

  auto& layer = registry.get<Layer>(entity);
  auto& targetLayer = registry.get<Layer>(targetEntity);

  std::swap(layer.index, targetLayer.index);

  SortLayers(registry);
}

void OffsetLayerIndicesOfSiblingsBelow(entt::registry& registry,
                                       const entt::entity entity,
                                       const int offset)
{
  auto sibling = GetLayerSiblingBelow(registry, entity);
  while (sibling != entt::null)
  {
    auto& siblingLayer = registry.get<Layer>(sibling);
    const auto newIndex = siblingLayer.index + offset;
    sibling = GetLayerSiblingBelow(registry, sibling);
    siblingLayer.index = newIndex;
  }
}

void DestroyChildLayers(entt::registry& registry, const entt::entity entity)
{
  auto& group = registry.get<GroupLayer>(entity);
  for (const auto child : group.layers)
  {
    if (registry.all_of<GroupLayer>(child))
    {
      DestroyChildLayers(registry, child);
      registry.destroy(child);
    }
    else
    {
      registry.destroy(child);
    }
  }
}

}  // namespace

auto AddBasicLayer(entt::registry& registry,
                   const LayerID id,
                   const LayerType type,
                   std::string name,
                   const entt::entity parent) -> entt::entity
{
  const auto entity = registry.create();

  {
    auto& layer = registry.emplace<Layer>(entity);
    layer.id = id;
    layer.index = GetNewLayerIndex(registry, entity, parent);
    layer.type = type;
    layer.visible = true;
    layer.opacity = 1.0f;
  }

  assert(parent == entt::null || registry.all_of<GroupLayer>(parent));
  registry.emplace<Parent>(entity, parent);

  if (parent != entt::null)
  {
    auto& parentLayer = registry.get<GroupLayer>(parent);
    parentLayer.layers.push_back(entity);
  }

  {
    auto& context = registry.emplace<PropertyContext>(entity);
    context.name = std::move(name);
  }

  return entity;
}

auto AddTileLayer(entt::registry& registry) -> entt::entity
{
  auto& map = registry.ctx<Map>();

  const auto entity =
      AddBasicLayer(registry,
                    map.next_layer_id,
                    LayerType::TileLayer,
                    std::format("Tile Layer {}", map.tile_layer_suffix),
                    GetNewLayerParent(registry));
  ++map.next_layer_id;
  ++map.tile_layer_suffix;

  auto& tileLayer = registry.emplace<TileLayer>(entity);
  tileLayer.matrix = MakeTileMatrix(map.row_count, map.column_count);

  return entity;
}

auto AddObjectLayer(entt::registry& registry) -> entt::entity
{
  auto& map = registry.ctx<Map>();

  const auto entity =
      AddBasicLayer(registry,
                    map.next_layer_id,
                    LayerType::ObjectLayer,
                    std::format("Object Layer {}", map.object_layer_suffix),
                    GetNewLayerParent(registry));
  ++map.next_layer_id;
  ++map.object_layer_suffix;

  registry.emplace<ObjectLayer>(entity);

  return entity;
}

auto AddGroupLayer(entt::registry& registry) -> entt::entity
{
  auto& map = registry.ctx<Map>();

  const auto entity =
      AddBasicLayer(registry,
                    map.next_layer_id,
                    LayerType::GroupLayer,
                    std::format("Group Layer {}", map.group_layer_suffix),
                    GetNewLayerParent(registry));
  ++map.next_layer_id;
  ++map.group_layer_suffix;

  registry.emplace<GroupLayer>(entity);

  return entity;
}

auto RestoreLayer(entt::registry& registry, LayerSnapshot snapshot) -> entt::entity
{
  entt::entity parent{entt::null};
  if (snapshot.parent.has_value())
  {
    parent = FindLayer(registry, *snapshot.parent);
  }

  const auto entity = AddBasicLayer(registry,
                                    snapshot.core.id,
                                    snapshot.core.type,
                                    snapshot.context.name,
                                    parent);

  RestorePropertyContext(registry, entity, std::move(snapshot.context));

  switch (snapshot.core.type)
  {
    case LayerType::TileLayer:
    {
      auto& tileLayer = registry.emplace<TileLayer>(entity);
      tileLayer.matrix = snapshot.tiles.value();
      break;
    }
    case LayerType::ObjectLayer:
    {
      auto& objectLayer = registry.emplace<ObjectLayer>(entity);
      for (auto objectSnapshot : snapshot.objects.value())
      {
        const auto objectEntity = registry.create();

        registry.emplace<Object>(objectEntity, std::move(objectSnapshot.core));
        RestorePropertyContext(registry,
                               objectEntity,
                               std::move(objectSnapshot.context));

        objectLayer.objects.push_back(objectEntity);
      }
      break;
    }
    case LayerType::GroupLayer:
    {
      /* We don't need to add the children to the restored group here, that is
         handled by AddBasicLayer. */
      registry.emplace<GroupLayer>(entity);
      for (auto layerSnapshot : snapshot.children.value())
      {
        RestoreLayer(registry, std::move(layerSnapshot));
      }
      break;
    }
  }

  /* Restore the previous layer index */
  while (GetLayerIndex(registry, entity) != snapshot.core.index)
  {
    const auto index = GetLayerIndex(registry, entity);
    if (index > snapshot.core.index)
    {
      MoveLayerUp(registry, entity);
    }
    else if (index < snapshot.core.index)
    {
      MoveLayerDown(registry, entity);
    }
  }

  SortLayers(registry);
  return entity;
}

void SortLayers(entt::registry& registry)
{
  auto sorter = [&](const entt::entity a, const entt::entity b) {
    const auto fst = GetLayerGlobalIndex(registry, a);
    const auto snd = GetLayerGlobalIndex(registry, b);
    return fst < snd;
  };
  registry.sort<Layer>(sorter, entt::insertion_sort{});

  /* Ensure that group layers store sorted child layers */
  for (auto&& [entity, group] : registry.view<GroupLayer>().each())
  {
    std::ranges::sort(group.layers, [&](const entt::entity a, const entt::entity b) {
      const auto& fst = registry.get<Layer>(a);
      const auto& snd = registry.get<Layer>(b);
      return fst.index < snd.index;
    });
  }
}

void RemoveLayer(entt::registry& registry, const entt::entity entity)
{
  /* Reset the active layer if we're removing the active layer. */
  auto& active = registry.ctx<ActiveLayer>();
  if (entity == active.entity)
  {
    active.entity = entt::null;
  }

  /* Fix indices of siblings that are below the removed layer */
  OffsetLayerIndicesOfSiblingsBelow(registry, entity, -1);

  /* Remove the layer from the parent group layer, if there is one. */
  const auto& parent = registry.get<Parent>(entity);
  if (parent.entity != entt::null)
  {
    auto& group = registry.get<GroupLayer>(parent.entity);
    std::erase(group.layers, entity);
  }

  if (registry.all_of<GroupLayer>(entity))
  {
    DestroyChildLayers(registry, entity);
  }

  registry.destroy(entity);
  SortLayers(registry);
}

void SelectLayer(entt::registry& registry, const entt::entity entity)
{
  assert(entity != entt::null);

  auto& active = registry.ctx<ActiveLayer>();
  active.entity = entity;
}

auto CopyLayer(const entt::registry& registry, const entt::entity source)
    -> LayerSnapshot
{
  assert(source != entt::null);

  LayerSnapshot snapshot;
  snapshot.core = registry.get<Layer>(source);
  snapshot.context = CopyPropertyContext(registry, source);

  const auto parentEntity = registry.get<Parent>(source).entity;
  if (parentEntity != entt::null)
  {
    snapshot.parent = registry.get<Layer>(parentEntity).id;
  }

  switch (snapshot.core.type)
  {
    case LayerType::TileLayer:
    {
      snapshot.tiles = registry.get<TileLayer>(source).matrix;
      break;
    }
    case LayerType::ObjectLayer:
    {
      auto& objects = snapshot.objects.emplace();

      for (const auto objectEntity : registry.get<ObjectLayer>(source).objects)
      {
        auto& objectSnapshot = objects.emplace_back();
        objectSnapshot.core = registry.get<Object>(objectEntity);
        objectSnapshot.context = CopyPropertyContext(registry, objectEntity);
      }

      break;
    }
    case LayerType::GroupLayer:
    {
      auto& children = snapshot.children.emplace();

      for (const auto child : registry.get<GroupLayer>(source).layers)
      {
        children.push_back(CopyLayer(registry, child));
      }

      break;
    }
  }

  return snapshot;
}

auto DuplicateLayer(entt::registry& registry, const entt::entity source)
    -> entt::entity
{
  const auto& sourceParent = registry.get<Parent>(source);
  const auto copy = DuplicateLayer(registry, source, sourceParent.entity, false);

  SortLayers(registry);

  return copy;
}

auto DuplicateLayer(entt::registry& registry,
                    const entt::entity source,
                    const entt::entity parent,
                    const bool recursive) -> entt::entity
{
  /* The recursive flag determines whether indices need to be adjusted, since we
     do not touch indices of children of the original source layer that we want to
     duplicate. */

  if (!recursive)
  {
    OffsetLayerIndicesOfSiblingsBelow(registry, source, 1);
  }

  const auto copy = registry.create();

  registry.emplace<Parent>(copy, parent);
  if (parent != entt::null)
  {
    auto& parentLayer = registry.get<GroupLayer>(parent);
    parentLayer.layers.push_back(copy);
  }

  {
    auto& context = Copy<PropertyContext>(registry, source, copy);
    if (!recursive)
    {
      context.name += " (Copy)";
    }
  }

  {
    auto& map = registry.ctx<Map>();
    auto& layer = Copy<Layer>(registry, source, copy);
    layer.id = map.next_layer_id;

    if (!recursive)
    {
      const auto sourceLayer = registry.get<Layer>(source);
      layer.index = sourceLayer.index + 1;
    }

    ++map.next_layer_id;
  }

  if (registry.all_of<TileLayer>(source))
  {
    Copy<TileLayer>(registry, source, copy);
  }
  else if (registry.all_of<ObjectLayer>(source))
  {
    Copy<ObjectLayer>(registry, source, copy);
  }
  else if (registry.all_of<GroupLayer>(source))
  {
    Copy<GroupLayer>(registry, source, copy);
  }

#if TACTILE_DEBUG
  const auto& layer = registry.get<Layer>(copy);
  cen::log::debug(
      "Created duplicated layer: {entity: [%u], parent: [%u], id: %i, index: %i }",
      copy,
      parent,
      layer.id,
      layer.index);
#endif  // TACTILE_DEBUG

  return copy;
}

void MoveLayerUp(entt::registry& registry, const entt::entity entity)
{
  assert(CanMoveLayerUp(registry, entity));

  const auto targetEntity = GetLayerSiblingAbove(registry, entity);
  SwapLayerIndices(registry, entity, targetEntity);
}

void MoveLayerDown(entt::registry& registry, const entt::entity entity)
{
  assert(CanMoveLayerDown(registry, entity));

  const auto targetEntity = GetLayerSiblingBelow(registry, entity);
  SwapLayerIndices(registry, entity, targetEntity);
}

void SetLayerOpacity(entt::registry& registry,
                     const entt::entity entity,
                     const float opacity)
{
  assert(entity != entt::null);

  auto& layer = registry.get<Layer>(entity);
  layer.opacity = opacity;
}

void SetLayerVisible(entt::registry& registry,
                     const entt::entity entity,
                     const bool visible)
{
  assert(entity != entt::null);

  auto& layer = registry.get<Layer>(entity);
  layer.visible = visible;
}

auto FindLayer(const entt::registry& registry, const LayerID id) -> entt::entity
{
  for (auto&& [entity, layer] : registry.view<Layer>().each())
  {
    if (layer.id == id)
    {
      return entity;
    }
  }

  return entt::null;
}

auto GetActiveLayer(const entt::registry& registry) -> entt::entity
{
  const auto& active = registry.ctx<ActiveLayer>();
  return active.entity;
}

auto GetLayerIndex(const entt::registry& registry, const entt::entity entity)
    -> usize
{
  assert(entity != entt::null);
  const auto& layer = registry.get<Layer>(entity);
  return layer.index;
}

auto GetLayerGlobalIndex(const entt::registry& registry,
                         const entt::entity sourceEntity) -> usize
{
  /* SortLayers makes use of this function, so do not assume sorted layers! */
  assert(sourceEntity != entt::null);
  const auto& sourceLayer = registry.get<Layer>(sourceEntity);
  const auto& sourceParent = registry.get<Parent>(sourceEntity);

  const auto base = CountAllLayersAbove(registry, sourceEntity) + sourceLayer.index;
  if (sourceParent.entity == entt::null)
  {
    return base;
  }
  else
  {
    return base + GetLayerGlobalIndex(registry, sourceParent.entity) + 1;
  }
}

auto GetLayerSiblingAbove(const entt::registry& registry, const entt::entity entity)
    -> entt::entity
{
  assert(entity != entt::null);
  const auto index = registry.get<Layer>(entity).index;
  return GetLayerSibling(registry, entity, index - 1);
}

auto GetLayerSiblingBelow(const entt::registry& registry, const entt::entity entity)
    -> entt::entity
{
  assert(entity != entt::null);
  const auto index = registry.get<Layer>(entity).index;
  return GetLayerSibling(registry, entity, index + 1);
}

auto GetSiblingCount(const entt::registry& registry, const entt::entity entity)
    -> usize
{
  assert(entity != entt::null);
  const auto& parent = registry.get<Parent>(entity);
  if (parent.entity == entt::null)
  {
    // The layer is root-level, so count layers without parents
    usize count = 0;

    for (auto&& [otherEntity, otherLayer, otherParent] :
         registry.view<Layer, Parent>().each())
    {
      if (otherEntity != entity && otherParent.entity == entt::null)
      {
        ++count;
      }
    }

    return count;
  }
  else
  {
    const auto& parentLayer = registry.get<GroupLayer>(parent.entity);
    return parentLayer.layers.size() - 1;  // Exclude the queried layer
  }
}

auto GetLayerChildrenCount(const entt::registry& registry, const entt::entity entity)
    -> usize
{
  assert(entity != entt::null);
  if (const auto* group = registry.try_get<GroupLayer>(entity))
  {
    usize count = group->layers.size();

    for (const auto child : group->layers)
    {
      count += GetLayerChildrenCount(registry, child);
    }

    return count;
  }
  else
  {
    return 0;
  }
}

auto GetLayerOpacity(const entt::registry& registry, const entt::entity entity)
    -> float
{
  assert(entity != entt::null);
  const auto& layer = registry.get<Layer>(entity);
  return layer.opacity;
}

auto IsLayerVisible(const entt::registry& registry, const entt::entity entity)
    -> bool
{
  assert(entity != entt::null);
  const auto& layer = registry.get<Layer>(entity);
  return layer.visible;
}

auto CanMoveLayerUp(const entt::registry& registry, const entt::entity entity)
    -> bool
{
  assert(entity != entt::null);
  return registry.get<Layer>(entity).index > 0;
}

auto CanMoveLayerDown(const entt::registry& registry, const entt::entity entity)
    -> bool
{
  assert(entity != entt::null);
  const auto index = registry.get<Layer>(entity).index;
  const auto nSiblings = GetSiblingCount(registry, entity);
  return index < nSiblings;
}

auto IsTileLayerActive(const entt::registry& registry) -> bool
{
  const auto& active = registry.ctx<ActiveLayer>();
  if (active.entity != entt::null)
  {
    return registry.all_of<TileLayer>(active.entity);
  }
  else
  {
    return false;
  }
}

auto GetActiveLayerID(const entt::registry& registry) -> Maybe<LayerID>
{
  const auto& active = registry.ctx<ActiveLayer>();
  if (active.entity != entt::null)
  {
    return registry.get<Layer>(active.entity).id;
  }
  else
  {
    return nothing;
  }
}

}  // namespace Tactile::Sys
