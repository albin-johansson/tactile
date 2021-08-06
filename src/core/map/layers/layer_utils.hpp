#pragma once

#include <memory_resource>  // memory_resource
#include <unordered_map>    // unordered_map
#include <vector_map.hpp>   // vector_map

#include "aliases/ints.hpp"
#include "aliases/layer_id.hpp"
#include "aliases/maybe.hpp"
#include "layer.hpp"
#include "utils/buffer_utils.hpp"

namespace Tactile {

class TileLayer;
class ObjectLayer;
class GroupLayer;

using layer_map = rune::vector_map<layer_id, SharedLayer>;
using group_layer_map = std::pmr::unordered_map<layer_id, GroupLayer*>;
using tile_layer_map = std::pmr::unordered_map<layer_id, TileLayer*>;
using object_layer_map = std::pmr::unordered_map<layer_id, ObjectLayer*>;

[[nodiscard]] auto AsTileLayer(const SharedLayer& layer) -> TileLayer*;

[[nodiscard]] auto AsObjectLayer(const SharedLayer& layer) -> ObjectLayer*;

[[nodiscard]] auto AsGroupLayer(const SharedLayer& layer) -> GroupLayer*;

[[nodiscard]] auto GetGroupLayers(std::pmr::memory_resource* resource,
                                  const layer_map& layers) -> group_layer_map;

[[nodiscard]] auto GetTileLayers(std::pmr::memory_resource* resource,
                                 const layer_map& layers) -> tile_layer_map;

[[nodiscard]] auto GetObjectLayers(std::pmr::memory_resource* resource,
                                   const layer_map& layers) -> object_layer_map;

auto RemoveLayer(layer_map& layers, layer_id id) -> bool;

[[nodiscard]] auto FindLayer(const layer_map& layers, layer_id id) -> SharedLayer;

[[nodiscard]] auto GetLayer(const layer_map& layers, layer_id id) -> SharedLayer;

[[nodiscard]] auto GetIndex(const layer_map& layers, layer_id id) -> Maybe<usize>;

template <typename Layer, auto QueryFunction>
class LayerQuery final
{
 public:
  using container = std::pmr::unordered_map<layer_id, Layer*>;
  using value_type = typename container::value_type;
  using iterator = typename container::iterator;
  using const_iterator = typename container::const_iterator;

  [[nodiscard]] explicit LayerQuery(const layer_map& layers)
      : mContainer{QueryFunction(&mResource.resource, layers)}
  {}

  [[nodiscard]] auto begin() noexcept -> iterator
  {
    return mContainer.begin();
  }

  [[nodiscard]] auto end() noexcept -> iterator
  {
    return mContainer.end();
  }

  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return mContainer.begin();
  }

  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return mContainer.end();
  }

 private:
  StackResource<value_type, 16> mResource;
  container mContainer;
};

using TileLayerQuery = LayerQuery<TileLayer, GetTileLayers>;
using ObjectLayerQuery = LayerQuery<ObjectLayer, GetObjectLayers>;
using GroupLayerQuery = LayerQuery<GroupLayer, GetGroupLayers>;

}  // namespace Tactile
