// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

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
