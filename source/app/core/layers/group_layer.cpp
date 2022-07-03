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

#include "core/common/functional.hpp"
#include "core/common/maybe.hpp"
#include "core/contexts/context_visitor.hpp"
#include "core/layers/layer_visitor.hpp"
#include "core/layers/object_layer.hpp"
#include "core/layers/tile_layer.hpp"
#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile::core {
namespace {

using LayerStorage = GroupLayer::LayerStorage;
using VisitorFunc = std::function<void(LayerStorage&, LayerStorage::iterator)>;
using ConstVisitorFunc =
    std::function<void(const LayerStorage&, LayerStorage::const_iterator)>;

/// A generic visitor for operations that may modify layers and their associated storage.
class LayerMutatorVisitor : public ILayerVisitor
{
 public:
  explicit LayerMutatorVisitor(const UUID& target, VisitorFunc func)
      : mTarget{target}
      , mFunc{std::move(func)}
  {}

  void visit(GroupLayer& layer) override
  {
    auto& storage = layer.storage();

    const auto iter =
        std::find_if(storage.begin(), storage.end(), [this](const Shared<ILayer>& layer) {
          return layer->get_uuid() == mTarget;
        });

    if (iter != storage.end()) {
      mFunc(storage, iter);
    }
  }

 private:
  UUID        mTarget;
  VisitorFunc mFunc;
};

/// A generic visitor for queries that don't modify any layers.
/// Note, this must be used with the accept-function!
class LayerQueryVisitor : public IConstLayerVisitor
{
 public:
  explicit LayerQueryVisitor(const UUID& target, ConstVisitorFunc func)
      : mTarget{target}
      , mFunc{std::move(func)}
  {}

  void visit(const GroupLayer& layer) override
  {
    const auto& storage = layer.storage();

    const auto iter =
        std::find_if(storage.begin(), storage.end(), [this](const Shared<ILayer>& layer) {
          return layer->get_uuid() == mTarget;
        });

    if (iter != storage.end()) {
      mFunc(storage, iter);
    }
  }

 private:
  UUID             mTarget;
  ConstVisitorFunc mFunc;
};

/// A visitor that determines the global index of a layer.
class GlobalIndexCalculator final : public IConstLayerVisitor
{
 public:
  explicit GlobalIndexCalculator(const UUID& target) : mTarget{target} {}

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
  UUID  mTarget;
  usize mIndex{};
  bool  mFound{};

  void check(const ILayer& layer)
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
class FindLayerVisitor final : public ILayerVisitor
{
 public:
  explicit FindLayerVisitor(const UUID& target) : mTarget{target} {}

  void visit(TileLayer& layer) override { check(layer); }
  void visit(ObjectLayer& layer) override { check(layer); }
  void visit(GroupLayer& layer) override { check(layer); }

  [[nodiscard]] auto found_layer() -> ILayer* { return mLayer; }

 private:
  UUID    mTarget;
  ILayer* mLayer{};

  void check(ILayer& layer)
  {
    if (layer.get_uuid() == mTarget) {
      mLayer = &layer;
    }
  }
};

class FindConstLayerVisitor final : public IConstLayerVisitor
{
 public:
  explicit FindConstLayerVisitor(const UUID& target) : mTarget{target} {}

  void visit(const TileLayer& layer) override { check(layer); }
  void visit(const ObjectLayer& layer) override { check(layer); }
  void visit(const GroupLayer& layer) override { check(layer); }

  [[nodiscard]] auto found_layer() -> const ILayer* { return mLayer; }

 private:
  UUID          mTarget;
  const ILayer* mLayer{};

  void check(const ILayer& layer)
  {
    if (layer.get_uuid() == mTarget) {
      mLayer = &layer;
    }
  }
};

#define TACTILE_FIND_LAYER_VISITOR(Name, Type)                     \
  class Name final : public ILayerVisitor                          \
  {                                                                \
   public:                                                         \
    explicit Name(const UUID& target) : mTarget{target}            \
    {}                                                             \
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
    UUID  mTarget;                                                 \
    Type* mLayer{};                                                \
  };                                                               \
                                                                   \
  class Const##Name final : public IConstLayerVisitor              \
  {                                                                \
   public:                                                         \
    explicit Const##Name(const UUID& target) : mTarget{target}     \
    {}                                                             \
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
    UUID        mTarget;                                           \
    const Type* mLayer{};                                          \
  }

TACTILE_FIND_LAYER_VISITOR(FindTileLayerVisitor, TileLayer);
TACTILE_FIND_LAYER_VISITOR(FindObjectLayerVisitor, ObjectLayer);
TACTILE_FIND_LAYER_VISITOR(FindGroupLayerVisitor, GroupLayer);

/// Counts the amount of visited layers.
class CountingVisitor final : public IConstLayerVisitor
{
 public:
  void visit(const TileLayer&) override { ++mCount; }
  void visit(const ObjectLayer&) override { ++mCount; }
  void visit(const GroupLayer&) override { ++mCount; }

  [[nodiscard]] auto count() const -> usize { return mCount; }

 private:
  usize mCount{};
};

}  // namespace

auto GroupLayer::make() -> Shared<GroupLayer>
{
  return std::make_shared<GroupLayer>();
}

void GroupLayer::accept(IContextVisitor& visitor) const
{
  visitor.visit(*this);
}

void GroupLayer::accept(ILayerVisitor& visitor)
{
  visitor.visit(*this);
  each(visitor);
}

void GroupLayer::accept(IConstLayerVisitor& visitor) const
{
  visitor.visit(*this);
  each(visitor);
}

void GroupLayer::each(ILayerVisitor& visitor)
{
  for (const auto& layer : mLayers) {
    layer->accept(visitor);
  }
}

void GroupLayer::each(IConstLayerVisitor& visitor) const
{
  for (const auto& layer : mLayers) {
    layer->accept(visitor);
  }
}

void GroupLayer::each(const SimpleVisitor& visitor) const
{
  struct Visitor final : IConstLayerVisitor
  {
    const SimpleVisitor* func{};  // Pointer to avoid copying the function object

    void visit(const TileLayer& layer) override { (*func)(&layer); }

    void visit(const ObjectLayer& layer) override { (*func)(&layer); }

    void visit(const GroupLayer& layer) override { (*func)(&layer); }
  };

  Visitor v;
  v.func = &visitor;
  each(v);
}

void GroupLayer::add_layer(const UUID& parent, Shared<ILayer> layer)
{
  if (auto* group = find_group_layer(parent)) {
    group->add_layer(layer);
    layer->set_parent(parent);
  }
  else {
    throw TactileError{"Invalid parent layer!"};
  }
}

void GroupLayer::add_layer(Shared<ILayer> layer)
{
  if (layer) {
    layer->set_parent(nothing);
    mLayers.push_back(std::move(layer));
  }
  else {
    throw TactileError{"Invalid null layer!"};
  }
}

auto GroupLayer::remove_layer(const UUID& id) -> Shared<ILayer>
{
  Shared<ILayer> removed;

  auto op = [&](LayerStorage& storage, LayerStorage::iterator iter) {
    removed = *iter;
    storage.erase(iter);
  };

  LayerMutatorVisitor visitor{id, op};
  accept(visitor);

  if (removed) {
    return removed;
  }
  else {
    throw TactileError{"Invalid layer identifier"};
  }
}

auto GroupLayer::duplicate_layer(const UUID& id) -> Shared<ILayer>
{
  Shared<ILayer> layer;

  auto op = [&](LayerStorage& storage, LayerStorage::iterator iter) {
    layer = (*iter)->clone();
    storage.insert(iter + 1, layer);
  };

  LayerMutatorVisitor visitor{id, op};
  accept(visitor);

  if (layer) {
    return layer;
  }
  else {
    throw TactileError{"Invalid layer identifier!"};
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

  LayerMutatorVisitor visitor{id, op};
  accept(visitor);

  if (!valid) {
    throw TactileError{"Invalid layer identifier"};
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

  LayerMutatorVisitor visitor{id, op};
  accept(visitor);

  if (!valid) {
    throw TactileError{"Invalid layer identifier"};
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

void GroupLayer::set_parent(const Maybe<UUID>& parentId)
{
  mDelegate.set_parent(parentId);
}

void GroupLayer::set_meta_id(const int32 id)
{
  mDelegate.set_meta_id(id);
}

void GroupLayer::set_name(std::string name)
{
  mDelegate.set_name(std::move(name));
}

void GroupLayer::set_layer_index(const UUID& id, const usize index)
{
  const auto currentIndex = get_local_index(id);
  const auto shouldMoveUp = currentIndex > index;
  const auto nSteps = udiff(currentIndex, index);

  invoke_n(nSteps, [&] {
    if (shouldMoveUp) {
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
  auto         op = [&](const LayerStorage& storage, LayerStorage::const_iterator) {
    TACTILE_ASSERT(storage.size() >= 1);
    count = storage.size() - 1;
  };

  LayerQueryVisitor visitor{id, op};
  accept(visitor);

  if (count) {
    return *count;
  }
  else {
    throw TactileError{"Invalid layer identifier!"};
  }
}

auto GroupLayer::get_local_index(const UUID& id) const -> usize
{
  Maybe<usize> index;
  auto         op = [&](const LayerStorage& storage, LayerStorage::const_iterator iter) {
    index = static_cast<usize>(std::distance(storage.begin(), iter));
  };

  LayerQueryVisitor query{id, op};
  accept(query);

  if (index) {
    return *index;
  }
  else {
    throw TactileError{"Invalid layer identifier!"};
  }
}

auto GroupLayer::get_global_index(const UUID& id) const -> usize
{
  GlobalIndexCalculator visitor{id};
  each(visitor);

  if (const auto index = visitor.get_index()) {
    return *index;
  }
  else {
    throw TactileError{"Invalid layer identifier!"};
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

auto GroupLayer::get_layer(const UUID& id) -> Shared<ILayer>
{
  Shared<ILayer> layer;
  auto           op = [&](const LayerStorage&, LayerStorage::const_iterator iter) {
    layer = *iter;
  };

  LayerQueryVisitor visitor{id, op};
  accept(visitor);

  if (layer) {
    return layer;
  }
  else {
    throw TactileError{"Invalid layer identifier!"};
  }
}

auto GroupLayer::view_layer(const UUID& id) -> ILayer&
{
  if (auto* layer = find_layer(id)) {
    return *layer;
  }
  else {
    throw TactileError{"Invalid layer identifier!"};
  }
}

auto GroupLayer::view_layer(const UUID& id) const -> const ILayer&
{
  if (const auto* layer = find_layer(id)) {
    return *layer;
  }
  else {
    throw TactileError{"Invalid layer identifier!"};
  }
}

auto GroupLayer::view_tile_layer(const UUID& id) -> TileLayer&
{
  if (auto* layer = find_tile_layer(id)) {
    return *layer;
  }
  else {
    throw TactileError{"Invalid layer identifier!"};
  }
}

auto GroupLayer::view_tile_layer(const UUID& id) const -> const TileLayer&
{
  if (const auto* layer = find_tile_layer(id)) {
    return *layer;
  }
  else {
    throw TactileError{"Invalid layer identifier!"};
  }
}

auto GroupLayer::view_object_layer(const UUID& id) -> ObjectLayer&
{
  if (auto* layer = find_object_layer(id)) {
    return *layer;
  }
  else {
    throw TactileError{"Invalid layer identifier!"};
  }
}

auto GroupLayer::view_object_layer(const UUID& id) const -> const ObjectLayer&
{
  if (const auto* layer = find_object_layer(id)) {
    return *layer;
  }
  else {
    throw TactileError{"Invalid layer identifier!"};
  }
}

auto GroupLayer::view_group_layer(const UUID& id) -> GroupLayer&
{
  if (auto* layer = find_group_layer(id)) {
    return *layer;
  }
  else {
    throw TactileError{"Invalid layer identifier!"};
  }
}

auto GroupLayer::view_group_layer(const UUID& id) const -> const GroupLayer&
{
  if (const auto* layer = find_group_layer(id)) {
    return *layer;
  }
  else {
    throw TactileError{"Invalid layer identifier!"};
  }
}

auto GroupLayer::find_layer(const UUID& id) -> ILayer*
{
  FindLayerVisitor visitor{id};
  each(visitor);
  return visitor.found_layer();
}

auto GroupLayer::find_layer(const UUID& id) const -> const ILayer*
{
  FindConstLayerVisitor visitor{id};
  each(visitor);
  return visitor.found_layer();
}

auto GroupLayer::find_tile_layer(const UUID& id) -> TileLayer*
{
  FindTileLayerVisitor visitor{id};
  each(visitor);
  return visitor.found_layer();
}

auto GroupLayer::find_tile_layer(const UUID& id) const -> const TileLayer*
{
  ConstFindTileLayerVisitor visitor{id};
  each(visitor);
  return visitor.found_layer();
}

auto GroupLayer::find_object_layer(const UUID& id) -> ObjectLayer*
{
  FindObjectLayerVisitor visitor{id};
  each(visitor);
  return visitor.found_layer();
}

auto GroupLayer::find_object_layer(const UUID& id) const -> const ObjectLayer*
{
  ConstFindObjectLayerVisitor visitor{id};
  each(visitor);
  return visitor.found_layer();
}

auto GroupLayer::find_group_layer(const UUID& id) -> GroupLayer*
{
  FindGroupLayerVisitor visitor{id};
  each(visitor);
  return visitor.found_layer();
}

auto GroupLayer::find_group_layer(const UUID& id) const -> const GroupLayer*
{
  ConstFindGroupLayerVisitor visitor{id};
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

auto GroupLayer::clone() const -> Shared<ILayer>
{
  auto result = make();
  result->mDelegate = mDelegate.clone();

  for (const auto& layer : mLayers) {
    result->mLayers.push_back(layer->clone());
  }

  return result;
}

auto GroupLayer::get_uuid() const -> const UUID&
{
  return mDelegate.get_uuid();
}

auto GroupLayer::get_name() const -> const std::string&
{
  return mDelegate.get_name();
}

auto GroupLayer::get_props() -> PropertyBundle&
{
  return mDelegate.get_props();
}

auto GroupLayer::get_props() const -> const PropertyBundle&
{
  return mDelegate.get_props();
}

auto GroupLayer::get_comps() -> ComponentBundle&
{
  return mDelegate.get_comps();
}

auto GroupLayer::get_comps() const -> const ComponentBundle&
{
  return mDelegate.get_comps();
}

auto GroupLayer::get_parent() const -> Maybe<UUID>
{
  return mDelegate.get_parent();
}

auto GroupLayer::get_meta_id() const -> Maybe<int32>
{
  return mDelegate.get_meta_id();
}

}  // namespace tactile::core
