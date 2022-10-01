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

#include <algorithm>  // find_if, iter_swap
#include <iterator>   // distance
#include <utility>    // move

#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/type/maybe.hpp"
#include "core/util/functional.hpp"
#include "core/util/numeric.hpp"
#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile {
namespace {

using LayerStorage = GroupLayer::LayerStorage;
using VisitorFunc = std::function<void(LayerStorage&, LayerStorage::iterator)>;
using ConstVisitorFunc =
    std::function<void(const LayerStorage&, LayerStorage::const_iterator)>;

/// A generic visitor for operations that may modify layers and their associated storage.
class LayerMutatorVisitor : public LayerVisitor {
 public:
  explicit LayerMutatorVisitor(const UUID& target, VisitorFunc func)
      : mTarget {target}
      , mFunc {std::move(func)}
  {
  }

  void visit(GroupLayer& layer) override
  {
    auto& storage = layer.storage();

    const auto iter =
        std::find_if(storage.begin(), storage.end(), [this](const Shared<Layer>& layer) {
          return layer->get_uuid() == mTarget;
        });

    if (iter != storage.end()) {
      mFunc(storage, iter);
    }
  }

 private:
  UUID mTarget;
  VisitorFunc mFunc;
};

/// A generic visitor for queries that don't modify any layers.
/// Note, this must be used with the accept-function!
class LayerQueryVisitor : public ConstLayerVisitor {
 public:
  explicit LayerQueryVisitor(const UUID& target, ConstVisitorFunc func)
      : mTarget {target}
      , mFunc {std::move(func)}
  {
  }

  void visit(const GroupLayer& layer) override
  {
    const auto& storage = layer.storage();

    const auto iter =
        std::find_if(storage.begin(), storage.end(), [this](const Shared<Layer>& layer) {
          return layer->get_uuid() == mTarget;
        });

    if (iter != storage.end()) {
      mFunc(storage, iter);
    }
  }

 private:
  UUID mTarget;
  ConstVisitorFunc mFunc;
};

/// A visitor that determines the global index of a layer.
class GlobalIndexCalculator final : public ConstLayerVisitor {
 public:
  explicit GlobalIndexCalculator(const UUID& target)
      : mTarget {target}
  {
  }

  void visit(const TileLayer& layer) override { check(layer); }

  void visit(const ObjectLayer& layer) override { check(layer); }

  void visit(const GroupLayer& layer) override { check(layer); }

  [[nodiscard]] auto get_index() const -> Maybe<usize>
  {
    if (mFound) {
      return mIndex;
    }
    else {
      return nothing;
    }
  }

 private:
  UUID mTarget;
  usize mIndex {};
  bool mFound {};

  void check(const Layer& layer)
  {
    if (mFound) {
      return;
    }

    mFound = layer.get_uuid() == mTarget;

    if (!mFound) {
      ++mIndex;
    }
  }
};

/// A visitor that attempts to find a layer.
class FindLayerVisitor final : public LayerVisitor {
 public:
  explicit FindLayerVisitor(const UUID& target)
      : mTarget {target}
  {
  }

  void visit(TileLayer& layer) override { check(layer); }

  void visit(ObjectLayer& layer) override { check(layer); }

  void visit(GroupLayer& layer) override { check(layer); }

  [[nodiscard]] auto found_layer() -> Layer* { return mLayer; }

 private:
  UUID mTarget;
  Layer* mLayer {};

  void check(Layer& layer)
  {
    if (layer.get_uuid() == mTarget) {
      mLayer = &layer;
    }
  }
};

class FindConstLayerVisitor final : public ConstLayerVisitor {
 public:
  explicit FindConstLayerVisitor(const UUID& target)
      : mTarget {target}
  {
  }

  void visit(const TileLayer& layer) override { check(layer); }

  void visit(const ObjectLayer& layer) override { check(layer); }

  void visit(const GroupLayer& layer) override { check(layer); }

  [[nodiscard]] auto found_layer() -> const Layer* { return mLayer; }

 private:
  UUID mTarget;
  const Layer* mLayer {};

  void check(const Layer& layer)
  {
    if (layer.get_uuid() == mTarget) {
      mLayer = &layer;
    }
  }
};

#define TACTILE_FIND_LAYER_VISITOR(Name, Type)                     \
  class Name final : public LayerVisitor {                         \
   public:                                                         \
    explicit Name(const UUID& target)                              \
        : mTarget {target}                                         \
    {                                                              \
    }                                                              \
                                                                   \
    void visit(Type& layer) override                               \
    {                                                              \
      if (mTarget == layer.get_uuid()) {                           \
        mLayer = &layer;                                           \
      }                                                            \
    }                                                              \
                                                                   \
    [[nodiscard]] auto found_layer() noexcept -> Type*             \
    {                                                              \
      return mLayer;                                               \
    }                                                              \
                                                                   \
   private:                                                        \
    UUID mTarget;                                                  \
    Type* mLayer {};                                               \
  };                                                               \
                                                                   \
  class Const##Name final : public ConstLayerVisitor {             \
   public:                                                         \
    explicit Const##Name(const UUID& target)                       \
        : mTarget {target}                                         \
    {                                                              \
    }                                                              \
                                                                   \
    void visit(const Type& layer) override                         \
    {                                                              \
      if (mTarget == layer.get_uuid()) {                           \
        mLayer = &layer;                                           \
      }                                                            \
    }                                                              \
                                                                   \
    [[nodiscard]] auto found_layer() const noexcept -> const Type* \
    {                                                              \
      return mLayer;                                               \
    }                                                              \
                                                                   \
   private:                                                        \
    UUID mTarget;                                                  \
    const Type* mLayer {};                                         \
  }

TACTILE_FIND_LAYER_VISITOR(FindTileLayerVisitor, TileLayer);
TACTILE_FIND_LAYER_VISITOR(FindObjectLayerVisitor, ObjectLayer);
TACTILE_FIND_LAYER_VISITOR(FindGroupLayerVisitor, GroupLayer);

/// Counts the amount of visited layers.
class CountingVisitor final : public ConstLayerVisitor {
 public:
  void visit(const TileLayer&) override { ++mCount; }

  void visit(const ObjectLayer&) override { ++mCount; }

  void visit(const GroupLayer&) override { ++mCount; }

  [[nodiscard]] auto count() const -> usize { return mCount; }

 private:
  usize mCount {};
};

}  // namespace

auto GroupLayer::make() -> Shared<GroupLayer>
{
  return std::make_shared<GroupLayer>();
}

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
  for (const auto& layer : mLayers) {
    layer->accept(visitor);
  }
}

void GroupLayer::each(ConstLayerVisitor& visitor) const
{
  for (const auto& layer : mLayers) {
    layer->accept(visitor);
  }
}

void GroupLayer::each(const SimpleVisitor& visitor) const
{
  struct Visitor final : ConstLayerVisitor {
    const SimpleVisitor* func {};  // Pointer to avoid copying the function object

    void visit(const TileLayer& layer) override { (*func)(&layer); }

    void visit(const ObjectLayer& layer) override { (*func)(&layer); }

    void visit(const GroupLayer& layer) override { (*func)(&layer); }
  };

  Visitor v;
  v.func = &visitor;
  each(v);
}

void GroupLayer::add_layer(const UUID& parent, const Shared<Layer>& layer)
{
  if (auto* group = find_group_layer(parent)) {
    group->add_layer(layer);
    layer->set_parent(parent);
  }
  else {
    throw TactileError {"Invalid parent layer!"};
  }
}

void GroupLayer::add_layer(Shared<Layer> layer)
{
  if (layer) {
    layer->set_parent(nothing);
    mLayers.push_back(std::move(layer));
  }
  else {
    throw TactileError {"Invalid null layer!"};
  }
}

auto GroupLayer::remove_layer(const UUID& id) -> Shared<Layer>
{
  Shared<Layer> removed;

  auto op = [&](LayerStorage& storage, LayerStorage::iterator iter) {
    removed = *iter;
    storage.erase(iter);
  };

  LayerMutatorVisitor visitor {id, op};
  accept(visitor);

  if (removed) {
    return removed;
  }
  else {
    throw TactileError {"Invalid layer identifier"};
  }
}

auto GroupLayer::duplicate_layer(const UUID& id) -> Shared<Layer>
{
  Shared<Layer> layer;

  auto op = [&](LayerStorage& storage, LayerStorage::iterator iter) {
    layer = (*iter)->clone();
    storage.insert(iter + 1, layer);
  };

  LayerMutatorVisitor visitor {id, op};
  accept(visitor);

  if (layer) {
    return layer;
  }
  else {
    throw TactileError {"Invalid layer identifier!"};
  }
}

void GroupLayer::move_layer_up(const UUID& id)
{
  bool valid = false;

  auto op = [&](LayerStorage& storage, LayerStorage::iterator iter) {
    const auto index = std::distance(storage.begin(), iter);

    if (index != 0) {
      std::iter_swap(iter, iter - 1);
      valid = true;
    }
  };

  LayerMutatorVisitor visitor {id, op};
  accept(visitor);

  if (!valid) {
    throw TactileError {"Invalid layer identifier"};
  }
}

void GroupLayer::move_layer_down(const UUID& id)
{
  bool valid = false;

  auto op = [&](LayerStorage& storage, LayerStorage::iterator iter) {
    const auto index = std::distance(storage.begin(), iter);

    if (static_cast<usize>(index) != storage.size() - 1) {
      std::iter_swap(iter, iter + 1);
      valid = true;
    }
  };

  LayerMutatorVisitor visitor {id, op};
  accept(visitor);

  if (!valid) {
    throw TactileError {"Invalid layer identifier"};
  }
}

void GroupLayer::set_opacity(const float opacity)
{
  mDelegate.set_opacity(opacity);
}

void GroupLayer::set_visible(const bool visible)
{
  mDelegate.set_visible(visible);
}

void GroupLayer::set_parent(const Maybe<UUID>& parent_id)
{
  mDelegate.set_parent(parent_id);
}

void GroupLayer::set_meta_id(const int32 id)
{
  mDelegate.set_meta_id(id);
}

void GroupLayer::set_layer_index(const UUID& id, const usize index)
{
  const auto current_index = get_local_index(id);
  const auto should_move_up = current_index > index;
  const auto n_steps = udiff(current_index, index);

  invoke_n(n_steps, [&, this] {
    if (should_move_up) {
      move_layer_up(id);
    }
    else {
      move_layer_down(id);
    }
  });
}

auto GroupLayer::layer_count() const -> usize
{
  CountingVisitor visitor;
  each(visitor);
  return visitor.count();
}

auto GroupLayer::sibling_count(const UUID& id) const -> usize
{
  Maybe<usize> count;
  auto op = [&](const LayerStorage& storage, LayerStorage::const_iterator) {
    TACTILE_ASSERT(!storage.empty());
    count = storage.size() - 1;
  };

  LayerQueryVisitor visitor {id, op};
  accept(visitor);

  if (count) {
    return *count;
  }
  else {
    throw TactileError {"Invalid layer identifier!"};
  }
}

auto GroupLayer::get_local_index(const UUID& id) const -> usize
{
  Maybe<usize> index;
  auto op = [&](const LayerStorage& storage, LayerStorage::const_iterator iter) {
    index = static_cast<usize>(std::distance(storage.begin(), iter));
  };

  LayerQueryVisitor query {id, op};
  accept(query);

  if (index) {
    return *index;
  }
  else {
    throw TactileError {"Invalid layer identifier!"};
  }
}

auto GroupLayer::get_global_index(const UUID& id) const -> usize
{
  GlobalIndexCalculator visitor {id};
  each(visitor);

  if (const auto index = visitor.get_index()) {
    return *index;
  }
  else {
    throw TactileError {"Invalid layer identifier!"};
  }
}

auto GroupLayer::can_move_layer_up(const UUID& id) const -> bool
{
  return get_local_index(id) > 0;
}

auto GroupLayer::can_move_layer_down(const UUID& id) const -> bool
{
  return get_local_index(id) < sibling_count(id);
}

auto GroupLayer::get_layer(const UUID& id) -> Shared<Layer>
{
  Shared<Layer> layer;
  auto op = [&](const LayerStorage&, LayerStorage::const_iterator iter) {
    layer = *iter;
  };

  LayerQueryVisitor visitor {id, op};
  accept(visitor);

  if (layer) {
    return layer;
  }
  else {
    throw TactileError {"Invalid layer identifier!"};
  }
}

auto GroupLayer::view_layer(const UUID& id) -> Layer&
{
  if (auto* layer = find_layer(id)) {
    return *layer;
  }
  else {
    throw TactileError {"Invalid layer identifier!"};
  }
}

auto GroupLayer::view_layer(const UUID& id) const -> const Layer&
{
  if (const auto* layer = find_layer(id)) {
    return *layer;
  }
  else {
    throw TactileError {"Invalid layer identifier!"};
  }
}

auto GroupLayer::view_tile_layer(const UUID& id) -> TileLayer&
{
  if (auto* layer = find_tile_layer(id)) {
    return *layer;
  }
  else {
    throw TactileError {"Invalid layer identifier!"};
  }
}

auto GroupLayer::view_tile_layer(const UUID& id) const -> const TileLayer&
{
  if (const auto* layer = find_tile_layer(id)) {
    return *layer;
  }
  else {
    throw TactileError {"Invalid layer identifier!"};
  }
}

auto GroupLayer::view_object_layer(const UUID& id) -> ObjectLayer&
{
  if (auto* layer = find_object_layer(id)) {
    return *layer;
  }
  else {
    throw TactileError {"Invalid layer identifier!"};
  }
}

auto GroupLayer::view_object_layer(const UUID& id) const -> const ObjectLayer&
{
  if (const auto* layer = find_object_layer(id)) {
    return *layer;
  }
  else {
    throw TactileError {"Invalid layer identifier!"};
  }
}

auto GroupLayer::view_group_layer(const UUID& id) -> GroupLayer&
{
  if (auto* layer = find_group_layer(id)) {
    return *layer;
  }
  else {
    throw TactileError {"Invalid layer identifier!"};
  }
}

auto GroupLayer::view_group_layer(const UUID& id) const -> const GroupLayer&
{
  if (const auto* layer = find_group_layer(id)) {
    return *layer;
  }
  else {
    throw TactileError {"Invalid layer identifier!"};
  }
}

auto GroupLayer::find_layer(const UUID& id) -> Layer*
{
  FindLayerVisitor visitor {id};
  each(visitor);
  return visitor.found_layer();
}

auto GroupLayer::find_layer(const UUID& id) const -> const Layer*
{
  FindConstLayerVisitor visitor {id};
  each(visitor);
  return visitor.found_layer();
}

auto GroupLayer::find_tile_layer(const UUID& id) -> TileLayer*
{
  FindTileLayerVisitor visitor {id};
  each(visitor);
  return visitor.found_layer();
}

auto GroupLayer::find_tile_layer(const UUID& id) const -> const TileLayer*
{
  ConstFindTileLayerVisitor visitor {id};
  each(visitor);
  return visitor.found_layer();
}

auto GroupLayer::find_object_layer(const UUID& id) -> ObjectLayer*
{
  FindObjectLayerVisitor visitor {id};
  each(visitor);
  return visitor.found_layer();
}

auto GroupLayer::find_object_layer(const UUID& id) const -> const ObjectLayer*
{
  ConstFindObjectLayerVisitor visitor {id};
  each(visitor);
  return visitor.found_layer();
}

auto GroupLayer::find_group_layer(const UUID& id) -> GroupLayer*
{
  FindGroupLayerVisitor visitor {id};
  each(visitor);
  return visitor.found_layer();
}

auto GroupLayer::find_group_layer(const UUID& id) const -> const GroupLayer*
{
  ConstFindGroupLayerVisitor visitor {id};
  each(visitor);
  return visitor.found_layer();
}

auto GroupLayer::get_opacity() const -> float
{
  return mDelegate.get_opacity();
}

auto GroupLayer::is_visible() const -> bool
{
  return mDelegate.is_visible();
}

auto GroupLayer::clone() const -> Shared<Layer>
{
  auto result = make();
  result->mDelegate = mDelegate.clone();

  for (const auto& layer : mLayers) {
    result->mLayers.push_back(layer->clone());
  }

  return result;
}

auto GroupLayer::ctx() -> ContextInfo&
{
  return mDelegate.ctx();
}

auto GroupLayer::ctx() const -> const ContextInfo&
{
  return mDelegate.ctx();
}

auto GroupLayer::get_uuid() const -> const UUID&
{
  return mDelegate.ctx().uuid();
}

auto GroupLayer::get_parent() const -> Maybe<UUID>
{
  return mDelegate.get_parent();
}

auto GroupLayer::get_meta_id() const -> Maybe<int32>
{
  return mDelegate.get_meta_id();
}

}  // namespace tactile
