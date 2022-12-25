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

#include "visitors.hpp"

#include <algorithm>  // find_if
#include <utility>    // move

#include "core/layer/group_layer.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto find_layer_in_storage(LayerVector& storage, const UUID& layer_id)
    -> LayerVector::iterator
{
  return std::find_if(storage.begin(), storage.end(), [&](const Shared<Layer>& layer) {
    return layer_id == layer->get_uuid();
  });
}

[[nodiscard]] auto find_layer_in_storage(const LayerVector& storage, const UUID& layer_id)
    -> LayerVector::const_iterator
{
  return std::find_if(storage.begin(), storage.end(), [&](const Shared<Layer>& layer) {
    return layer_id == layer->get_uuid();
  });
}

}  // namespace

LayerMutatorVisitor::LayerMutatorVisitor(const UUID& layer_id, LayerMutatorFn fun)
    : mLayerId {layer_id},
      mFun {std::move(fun)}
{
}

void LayerMutatorVisitor::visit(GroupLayer& layer)
{
  auto& storage = layer.get_storage();
  if (auto iter = find_layer_in_storage(storage, mLayerId); iter != storage.end()) {
    mFun(storage, iter);
  }
}

LayerQueryVisitor::LayerQueryVisitor(const UUID& layer_id, LayerQueryFn fun)
    : mLayerId {layer_id},
      mFun {std::move(fun)}
{
}

void LayerQueryVisitor::visit(const GroupLayer& layer)
{
  const auto& storage = layer.get_storage();
  if (auto iter = find_layer_in_storage(storage, mLayerId); iter != storage.end()) {
    mFun(storage, iter);
  }
}

LayerGlobalIndexCalculator::LayerGlobalIndexCalculator(const UUID& layer_id)
    : mLayerId {layer_id}
{
}

void LayerGlobalIndexCalculator::visit(const TileLayer& layer)
{
  check(layer);
}

void LayerGlobalIndexCalculator::visit(const ObjectLayer& layer)
{
  check(layer);
}

void LayerGlobalIndexCalculator::visit(const GroupLayer& layer)
{
  check(layer);
}

void LayerGlobalIndexCalculator::check(const Layer& layer)
{
  if (!mIndex.has_value()) {
    if (layer.get_uuid() == mLayerId) {
      mIndex = mCount;
    }

    ++mCount;
  }
}

LayerFinder::LayerFinder(const UUID& layer_id)
    : mLayerId {layer_id}
{
}

void LayerFinder::visit(TileLayer& layer)
{
  check(layer);
}

void LayerFinder::visit(ObjectLayer& layer)
{
  check(layer);
}

void LayerFinder::visit(GroupLayer& layer)
{
  check(layer);
}

void LayerFinder::check(Layer& layer)
{
  if (layer.get_uuid() == mLayerId) {
    mLayer = &layer;
  }
}

ConstLayerFinder::ConstLayerFinder(const UUID& layer_id)
    : mLayerId {layer_id}
{
}

void ConstLayerFinder::visit(const TileLayer& layer)
{
  check(layer);
}

void ConstLayerFinder::visit(const ObjectLayer& layer)
{
  check(layer);
}

void ConstLayerFinder::visit(const GroupLayer& layer)
{
  check(layer);
}

void ConstLayerFinder::check(const Layer& layer)
{
  if (layer.get_uuid() == mLayerId) {
    mLayer = &layer;
  }
}

}  // namespace tactile