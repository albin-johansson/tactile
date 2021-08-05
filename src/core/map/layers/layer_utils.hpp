#pragma once

#include <memory_resource>  // memory_resource
#include <vector>           // vector
#include <vector_map.hpp>   // vector_map

#include "aliases/ints.hpp"
#include "aliases/layer_id.hpp"
#include "aliases/layer_stack_resource.hpp"
#include "aliases/maybe.hpp"
#include "aliases/shared.hpp"
#include "layer.hpp"

namespace Tactile {

class TileLayer;
class ObjectLayer;
class GroupLayer;

using layer_map = rune::vector_map<layer_id, SharedLayer>;

[[nodiscard]] auto AsTileLayer(const SharedLayer& layer) -> TileLayer*;

[[nodiscard]] auto AsObjectLayer(const SharedLayer& layer) -> ObjectLayer*;

[[nodiscard]] auto AsGroupLayer(const SharedLayer& layer) -> GroupLayer*;

[[nodiscard]] auto GetTileLayers(std::pmr::memory_resource* resource,
                                 const layer_map& layers)
    -> std::pmr::vector<layer_id>;

[[nodiscard]] auto GetGroups(std::pmr::memory_resource* resource,
                             const layer_map& layers) -> std::pmr::vector<layer_id>;

auto RemoveLayer(layer_map& layers, layer_id id) -> bool;

[[nodiscard]] auto FindLayer(const layer_map& layers, layer_id id) -> SharedLayer;

[[nodiscard]] auto GetLayer(const layer_map& layers, layer_id id) -> SharedLayer;

[[nodiscard]] auto GetIndex(const layer_map& layers, layer_id id) -> Maybe<usize>;

}  // namespace Tactile
