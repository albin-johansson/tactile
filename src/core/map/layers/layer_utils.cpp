#include "layer_utils.hpp"

#include "core/tactile_error.hpp"
#include "group_layer.hpp"
#include "object_layer.hpp"
#include "tile_layer.hpp"

namespace Tactile {

auto AsTileLayer(const SharedLayer& layer) -> TileLayer*
{
  if (layer->GetType() != LayerType::TileLayer)
  {
    return nullptr;
  }
  else
  {
    return dynamic_cast<TileLayer*>(layer.get());
  }
}

auto AsObjectLayer(const SharedLayer& layer) -> ObjectLayer*
{
  if (layer->GetType() != LayerType::ObjectLayer)
  {
    return nullptr;
  }
  else
  {
    return dynamic_cast<ObjectLayer*>(layer.get());
  }
}

auto AsGroupLayer(const SharedLayer& layer) -> GroupLayer*
{
  if (layer->GetType() != LayerType::GroupLayer)
  {
    return nullptr;
  }
  else
  {
    return dynamic_cast<GroupLayer*>(layer.get());
  }
}

auto GetGroupLayers(std::pmr::memory_resource* resource, const layer_map& layers)
    -> group_layer_map
{
  group_layer_map groups{resource};

  for (const auto& [id, layer] : layers)
  {
    if (auto* groupLayer = AsGroupLayer(layer))
    {
      groups.try_emplace(id, groupLayer);

      StackResource<group_layer_map::value_type, 16> res;
      for (const auto& [child, childLayer] :
           GetGroupLayers(&res.resource, groupLayer->GetLayers()))
      {
        groups.try_emplace(child, childLayer);
      }
    }
  }

  return groups;
}

auto GetTileLayers(std::pmr::memory_resource* resource, const layer_map& layers)
    -> tile_layer_map
{
  tile_layer_map tileLayers{resource};

  for (const auto& [id, layer] : layers)
  {
    if (auto* tileLayer = AsTileLayer(layer))
    {
      tileLayers.try_emplace(id, tileLayer);
    }
    else if (const auto* groupLayer = AsGroupLayer(layer))
    {
      StackResource<tile_layer_map::value_type, 16> res;
      for (const auto& [childId, childLayer] :
           GetTileLayers(&res.resource, groupLayer->GetLayers()))
      {
        tileLayers.try_emplace(childId, childLayer);
      }
    }
  }

  return tileLayers;
}

auto GetObjectLayers(std::pmr::memory_resource* resource, const layer_map& layers)
    -> object_layer_map
{
  object_layer_map objectLayers{resource};

  for (const auto& [id, layer] : layers)
  {
    if (auto* objectLayer = AsObjectLayer(layer))
    {
      objectLayers.try_emplace(id, objectLayer);
    }
    else if (const auto* groupLayer = AsGroupLayer(layer))
    {
      StackResource<object_layer_map::value_type, 16> res;
      for (const auto& [childId, childLayer] :
           GetObjectLayers(&res.resource, groupLayer->GetLayers()))
      {
        objectLayers.try_emplace(childId, childLayer);
      }
    }
  }

  return objectLayers;
}

auto RemoveLayer(layer_map& layers, const layer_id id) -> bool
{
  if (layers.contains(id))
  {
    layers.erase(id);
    return true;
  }
  else
  {
    GroupLayerQuery query{layers};
    for (auto [group, groupLayer] : query)
    {
      return RemoveLayer(groupLayer->GetLayers(), id);
    }
  }

  return false;
}

auto FindLayer(const layer_map& layers, const layer_id id) -> SharedLayer
{
  if (const auto it = layers.find(id); it != layers.end())
  {
    return it->second;
  }
  else
  {
    GroupLayerQuery query{layers};
    for (auto [group, groupLayer] : query)
    {
      return FindLayer(groupLayer->GetLayers(), id);
    }
  }

  return nullptr;
}

auto GetLayer(const layer_map& layers, const layer_id id) -> SharedLayer
{
  if (auto layer = FindLayer(layers, id))
  {
    return layer;
  }
  else
  {
    throw TactileError{"Failed to find the specified layer!"};
  }
}

auto GetIndex(const layer_map& layers, const layer_id id) -> Maybe<usize>
{
  if (layers.contains(id))
  {
    return layers.index_of(id);
  }
  else
  {
    GroupLayerQuery query{layers};
    for (auto [group, groupLayer] : query)
    {
      return GetIndex(groupLayer->GetLayers(), id);
    }
  }

  return nothing;
}

}  // namespace Tactile
