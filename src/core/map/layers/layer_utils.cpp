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

auto GetTileLayers(std::pmr::memory_resource* resource, const layer_map& layers)
    -> std::pmr::vector<layer_id>
{
  std::pmr::vector<layer_id> tileLayers{resource};

  for (const auto& [id, layer] : layers)
  {
    if (const auto* groupLayer = AsGroupLayer(layer))
    {
      LayerStackResource res;
      for (const auto layerId :
           GetTileLayers(&res.resource, groupLayer->GetLayers()))
      {
        tileLayers.push_back(layerId);
      }
    }
    else if (AsTileLayer(layer))
    {
      tileLayers.push_back(id);
    }
  }

  return tileLayers;
}

auto GetGroups(std::pmr::memory_resource* resource, const layer_map& layers)
    -> std::pmr::vector<layer_id>
{
  std::pmr::vector<layer_id> groups{resource};

  for (const auto& [id, layer] : layers)
  {
    if (const auto* groupLayer = AsGroupLayer(layer))
    {
      groups.push_back(id);

      LayerStackResource res;
      for (const auto layerId : GetGroups(&res.resource, groupLayer->GetLayers()))
      {
        groups.push_back(layerId);
      }
    }
  }

  return groups;
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
    LayerStackResource res;
    for (const auto group : GetGroups(&res.resource, layers))
    {
      if (auto* groupLayer = AsGroupLayer(layers.at(group)))
      {
        return RemoveLayer(groupLayer->GetLayers(), id);
      }
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
    LayerStackResource res;
    for (const auto group : GetGroups(&res.resource, layers))
    {
      if (auto* groupLayer = AsGroupLayer(layers.at(group)))
      {
        return FindLayer(groupLayer->GetLayers(), id);
      }
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
    LayerStackResource res;
    for (const auto group : GetGroups(&res.resource, layers))
    {
      if (auto* groupLayer = AsGroupLayer(layers.at(group)))
      {
        return GetIndex(groupLayer->GetLayers(), id);
      }
    }
  }

  return nothing;
}

}  // namespace Tactile
