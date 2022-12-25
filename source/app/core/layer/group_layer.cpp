/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "group_layer.hpp"

#include <algorithm>  // iter_swap
#include <iterator>   // distance
#include <utility>    // move

#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/type/maybe.hpp"
#include "core/util/functional.hpp"
#include "core/util/numeric.hpp"
#include "debug/panic.hpp"

namespace tactile {

void GroupLayer::accept(ContextVisitor& visitor) const
{
  visitor.visit(*this);
}

void GroupLayer::accept(LayerVisitor& visitor)
{
  visitor.visit(*this);
  each(visitor);
}

void GroupLayer::accept(ConstLayerVisitor& visitor) const
{
  visitor.visit(*this);
  each(visitor);
}

void GroupLayer::each(LayerVisitor& visitor)
{
  for (const auto& layer: mLayers) {
    layer->accept(visitor);
  }
}

void GroupLayer::each(ConstLayerVisitor& visitor) const
{
  for (const auto& layer: mLayers) {
    layer->accept(visitor);
  }
}

void GroupLayer::each(const UnaryLayerFunc& func) const
{
  struct Visitor final : ConstLayerVisitor {
    const UnaryLayerFunc* func {};  // Pointer to avoid copying the function object

    void visit(const TileLayer& layer) override { (*func)(layer); }
    void visit(const ObjectLayer& layer) override { (*func)(layer); }
    void visit(const GroupLayer& layer) override { (*func)(layer); }
  };

  Visitor v;
  v.func = &func;
  each(v);
}

auto GroupLayer::add_layer(Shared<Layer> layer) -> Result
{
  if (layer) {
    layer->set_parent(nothing);
    mLayers.push_back(std::move(layer));
    return success;
  }
  else {
    return failure;
  }
}

auto GroupLayer::add_layer(const UUID& parent_id, const Shared<Layer>& layer) -> Result
{
  if (auto* group = find_group_layer(parent_id)) {
    if (group->add_layer(layer).succeeded()) {
      layer->set_parent(parent_id);
      return success;
    }
  }

  return failure;
}

auto GroupLayer::remove_layer(const UUID& layer_id) -> Shared<Layer>
{
  Shared<Layer> removed;

  mutate_layer(layer_id, [&](LayerStorage& storage, LayerStorage::iterator iter) {
    removed = *iter;
    storage.erase(iter);
  });

  return removed;
}

auto GroupLayer::duplicate_layer(const UUID& layer_id) -> Shared<Layer>
{
  Shared<Layer> layer;
  mutate_layer(layer_id, [&](LayerStorage& storage, LayerStorage::iterator iter) {
    layer = (*iter)->clone();
    storage.insert(iter + 1, layer);
  });

  if (layer) {
    return layer;
  }
  else {
    throw TactileError {"Failed to duplicate layer in group layer!"};
  }
}

void GroupLayer::move_layer_up(const UUID& layer_id)
{
  mutate_layer(layer_id, [](LayerStorage& storage, LayerStorage::iterator iter) {
    const auto index = std::distance(storage.begin(), iter);
    if (index != 0) {
      std::iter_swap(iter, iter - 1);
    }
  });
}

void GroupLayer::move_layer_down(const UUID& layer_id)
{
  mutate_layer(layer_id, [](LayerStorage& storage, LayerStorage::iterator iter) {
    const auto index = std::distance(storage.begin(), iter);
    if (index != std::ssize(storage) - 1) {
      std::iter_swap(iter, iter + 1);
    }
  });
}

void GroupLayer::set_layer_index(const UUID& layer_id, const usize index)
{
  const auto current_index = local_layer_index(layer_id);
  const auto n_steps = udiff(current_index, index);

  if (current_index > index) {
    invoke_n(n_steps, [&, this] { move_layer_up(layer_id); });
  }
  else {
    invoke_n(n_steps, [&, this] { move_layer_down(layer_id); });
  }
}

auto GroupLayer::size() const -> usize
{
  LayerCounter counter;
  each(counter);
  return counter.layer_count();
}

auto GroupLayer::layer_sibling_count(const UUID& layer_id) const -> usize
{
  Maybe<usize> count;
  query_layer(layer_id, [&](const LayerStorage& storage, LayerStorage::const_iterator) {
    count = storage.size() - 1;
  });

  if (count) {
    return *count;
  }
  else {
    throw TactileError {"Failed to determine layer sibling count"};
  }
}

auto GroupLayer::local_layer_index(const UUID& layer_id) const -> usize
{
  Maybe<usize> index;
  query_layer(layer_id,
              [&](const LayerStorage& storage, LayerStorage::const_iterator iter) {
                index = static_cast<usize>(std::distance(storage.begin(), iter));
              });

  if (index) {
    return *index;
  }
  else {
    throw TactileError {"Failed to determine local layer index"};
  }
}

auto GroupLayer::global_layer_index(const UUID& layer_id) const -> usize
{
  LayerGlobalIndexCalculator calculator {layer_id};
  each(calculator);

  if (const auto index = calculator.get_index()) {
    return *index;
  }
  else {
    throw TactileError {"Failed to determine global layer index"};
  }
}

auto GroupLayer::can_move_layer_up(const UUID& layer_id) const -> bool
{
  return local_layer_index(layer_id) > 0;
}

auto GroupLayer::can_move_layer_down(const UUID& layer_id) const -> bool
{
  return local_layer_index(layer_id) < layer_sibling_count(layer_id);
}

auto GroupLayer::find_shared_layer(const UUID& layer_id) -> Shared<Layer>
{
  Shared<Layer> layer;

  query_layer(layer_id, [&](const LayerStorage&, LayerStorage::const_iterator iter) {
    layer = *iter;
  });

  return layer;
}

auto GroupLayer::find_layer(const UUID& layer_id) -> Layer*
{
  LayerFinder finder {layer_id};
  each(finder);
  return finder.found_layer();
}

auto GroupLayer::find_layer(const UUID& layer_id) const -> const Layer*
{
  ConstLayerFinder finder {layer_id};
  each(finder);
  return finder.found_layer();
}

auto GroupLayer::find_tile_layer(const UUID& layer_id) -> TileLayer*
{
  GenericLayerFinder<TileLayer> finder {layer_id};
  each(finder);
  return finder.get_found_layer();
}

auto GroupLayer::find_tile_layer(const UUID& layer_id) const -> const TileLayer*
{
  GenericLayerFinder<TileLayer> finder {layer_id};
  each(finder);
  return finder.get_found_layer();
}

auto GroupLayer::find_object_layer(const UUID& layer_id) -> ObjectLayer*
{
  GenericLayerFinder<ObjectLayer> finder {layer_id};
  each(finder);
  return finder.get_found_layer();
}

auto GroupLayer::find_object_layer(const UUID& layer_id) const -> const ObjectLayer*
{
  GenericLayerFinder<ObjectLayer> finder {layer_id};
  each(finder);
  return finder.get_found_layer();
}

auto GroupLayer::find_group_layer(const UUID& layer_id) -> GroupLayer*
{
  GenericLayerFinder<GroupLayer> finder {layer_id};
  each(finder);
  return finder.get_found_layer();
}

auto GroupLayer::find_group_layer(const UUID& layer_id) const -> const GroupLayer*
{
  GenericLayerFinder<GroupLayer> finder {layer_id};
  each(finder);
  return finder.get_found_layer();
}

auto GroupLayer::get_layer(const UUID& layer_id) -> Layer&
{
  if (auto* layer = find_layer(layer_id)) {
    return *layer;
  }
  else {
    throw TactileError {"Failed to find layer in group layer"};
  }
}

auto GroupLayer::get_layer(const UUID& layer_id) const -> const Layer&
{
  if (const auto* layer = find_layer(layer_id)) {
    return *layer;
  }
  else {
    throw TactileError {"Failed to find layer in group layer"};
  }
}

auto GroupLayer::get_tile_layer(const UUID& layer_id) -> TileLayer&
{
  if (auto* layer = find_tile_layer(layer_id)) {
    return *layer;
  }
  else {
    throw TactileError {"Failed to find tile layer in group layer"};
  }
}

auto GroupLayer::get_tile_layer(const UUID& layer_id) const -> const TileLayer&
{
  if (const auto* layer = find_tile_layer(layer_id)) {
    return *layer;
  }
  else {
    throw TactileError {"Failed to find tile layer in group layer"};
  }
}

auto GroupLayer::get_object_layer(const UUID& layer_id) -> ObjectLayer&
{
  if (auto* layer = find_object_layer(layer_id)) {
    return *layer;
  }
  else {
    throw TactileError {"Failed to find object layer in group layer"};
  }
}

auto GroupLayer::get_object_layer(const UUID& layer_id) const -> const ObjectLayer&
{
  if (const auto* layer = find_object_layer(layer_id)) {
    return *layer;
  }
  else {
    throw TactileError {"Failed to find object layer in group layer"};
  }
}

auto GroupLayer::get_group_layer(const UUID& layer_id) -> GroupLayer&
{
  if (auto* layer = find_group_layer(layer_id)) {
    return *layer;
  }
  else {
    throw TactileError {"Failed to find group layer in group layer"};
  }
}

auto GroupLayer::get_group_layer(const UUID& layer_id) const -> const GroupLayer&
{
  if (const auto* layer = find_group_layer(layer_id)) {
    return *layer;
  }
  else {
    throw TactileError {"Failed to find group layer in group layer"};
  }
}

auto GroupLayer::clone() const -> Shared<Layer>
{
  auto result = std::make_shared<GroupLayer>();
  result->mDelegate = mDelegate.clone();

  for (const auto& layer: mLayers) {
    result->mLayers.push_back(layer->clone());
  }

  return result;
}

void GroupLayer::mutate_layer(const UUID& layer_id, const LayerMutatorFn& op)
{
  LayerMutatorVisitor visitor {layer_id, op};
  accept(visitor);
}

void GroupLayer::query_layer(const UUID& layer_id, const LayerQueryFn& op) const
{
  LayerQueryVisitor query {layer_id, op};
  accept(query);
}

}  // namespace tactile
