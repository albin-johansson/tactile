// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/map/layer/group_layer.hpp"

#include <algorithm>  // find_if, iter_swap
#include <iterator>   // distance
#include <utility>    // move

#include "tactile/core/map/layer/layer_visitor.hpp"
#include "tactile/core/map/layer/object_layer.hpp"
#include "tactile/core/map/layer/tile_layer.hpp"
#include "tactile/foundation/misc/conversion.hpp"

namespace tactile {

using int_literals::operator""_z;

template <typename T>
class GenericLayerFinder final : public ILayerVisitor {
 public:
  explicit GenericLayerFinder(const UUID& uuid)
    : mTargetUUID {uuid}
  {}

  void visit(T& layer) override
  {
    if (layer.meta().get_uuid() == mTargetUUID) {
      mFoundLayer = &layer;
    }
  }

  [[nodiscard]] auto get_found_layer() -> T* { return mFoundLayer; }

 private:
  UUID mTargetUUID;
  T* mFoundLayer {nullptr};
};

template <typename T>
class GenericConstLayerFinder final : public IConstLayerVisitor {
 public:
  explicit GenericConstLayerFinder(const UUID& uuid)
    : mTargetUUID {uuid}
  {}

  void visit(const T& layer) override
  {
    if (layer.meta().get_uuid() == mTargetUUID) {
      mFoundLayer = &layer;
    }
  }

  [[nodiscard]] auto get_found_layer() const -> const T* { return mFoundLayer; }

 private:
  UUID mTargetUUID;
  const T* mFoundLayer {nullptr};
};

class ParentLayerFinder final : public ILayerVisitor {
 public:
  explicit ParentLayerFinder(const UUID& target_uuid)
    : mTargetUUID {target_uuid}
  {}

  void visit(GroupLayer& group) override
  {
    if (mParentLayer != nullptr) {
      return;
    }

    const auto layer_iter = std::find_if(group.mLayers.begin(),
                                         group.mLayers.end(),
                                         [this](const Shared<ILayer>& layer) {
                                           return layer->meta().get_uuid() == mTargetUUID;
                                         });

    if (layer_iter != group.mLayers.end()) {
      mParentLayer = &group;
      mTargetLayerIter = layer_iter;
    }
  }

  [[nodiscard]]
  auto get_found_parent_layer() -> GroupLayer*
  {
    return mParentLayer;
  }

  [[nodiscard]]
  auto get_found_target_layer_iter() -> GroupLayer::LayerStorage::iterator
  {
    return mTargetLayerIter;
  }

 private:
  UUID mTargetUUID;
  GroupLayer* mParentLayer {};
  GroupLayer::LayerStorage::iterator mTargetLayerIter {};
};

class ConstParentLayerFinder final : public IConstLayerVisitor {
 public:
  explicit ConstParentLayerFinder(const UUID& target_uuid)
    : mTargetUUID {target_uuid}
  {}

  void visit(const GroupLayer& group) override
  {
    if (mParentLayer != nullptr) {
      return;
    }

    const auto layer_iter = std::find_if(group.mLayers.begin(),
                                         group.mLayers.end(),
                                         [this](const Shared<ILayer>& layer) {
                                           return layer->meta().get_uuid() == mTargetUUID;
                                         });

    if (layer_iter != group.mLayers.end()) {
      mParentLayer = &group;
      mTargetLayerIter = layer_iter;
    }
  }

  [[nodiscard]]
  auto get_found_parent_layer() const -> const GroupLayer*
  {
    return mParentLayer;
  }

  [[nodiscard]]
  auto get_found_target_layer_iter() const -> GroupLayer::LayerStorage::const_iterator
  {
    return mTargetLayerIter;
  }

 private:
  UUID mTargetUUID;
  const GroupLayer* mParentLayer {};
  GroupLayer::LayerStorage::const_iterator mTargetLayerIter {};
};

class IndexBasedLayerFinder final : public ILayerVisitor {
 public:
  explicit IndexBasedLayerFinder(const ssize target_index)
    : mTargetIndex {target_index}
  {}

  void visit(TileLayer& layer) override { _check(layer); }

  void visit(ObjectLayer& layer) override { _check(layer); }

  void visit(GroupLayer& layer) override { _check(layer); }

  [[nodiscard]]
  auto found_layer() -> ILayer*
  {
    return mFoundLayer;
  }

 private:
  ssize mTargetIndex;
  ssize mCurrentIndex {0};
  ILayer* mFoundLayer {nullptr};

  void _check(ILayer& layer)
  {
    if (mTargetIndex == mCurrentIndex) {
      mFoundLayer = &layer;
    }
    ++mCurrentIndex;
  }
};

class IndexBasedConstLayerFinder final : public IConstLayerVisitor {
 public:
  explicit IndexBasedConstLayerFinder(const ssize target_index)
    : mTargetIndex {target_index}
  {}

  void visit(const TileLayer& layer) override { _check(layer); }

  void visit(const ObjectLayer& layer) override { _check(layer); }

  void visit(const GroupLayer& layer) override { _check(layer); }

  [[nodiscard]]
  auto found_layer() const -> const ILayer*
  {
    return mFoundLayer;
  }

 private:
  ssize mTargetIndex;
  ssize mCurrentIndex {0};
  const ILayer* mFoundLayer {nullptr};

  void _check(const ILayer& layer)
  {
    if (mTargetIndex == mCurrentIndex) {
      mFoundLayer = &layer;
    }
    ++mCurrentIndex;
  }
};

class LayerGlobalIndexCalculator final : public IConstLayerVisitor {
 public:
  explicit LayerGlobalIndexCalculator(const UUID& target_uuid)
    : mTargetUUID {target_uuid}
  {}

  void visit(const TileLayer& layer) override { _consider(layer); }

  void visit(const ObjectLayer& layer) override { _consider(layer); }

  void visit(const GroupLayer& layer) override { _consider(layer); }

  [[nodiscard]] auto get_global_index() const -> Maybe<ssize> { return mGlobalIndex; }

 private:
  UUID mTargetUUID;
  ssize mCurrentIndex {0};
  Maybe<ssize> mGlobalIndex;

  void _consider(const ILayer& layer)
  {
    if (!mGlobalIndex.has_value() && layer.meta().get_uuid() == mTargetUUID) {
      mGlobalIndex = mCurrentIndex;
    }

    ++mCurrentIndex;
  }
};

class LayerCounter final : public IConstLayerVisitor {
 public:
  void visit(const TileLayer&) override { ++mCount; }

  void visit(const ObjectLayer&) override { ++mCount; }

  void visit(const GroupLayer&) override { ++mCount; }

  [[nodiscard]] auto count() const -> ssize { return mCount; }

 private:
  ssize mCount {0};
};

void GroupLayer::accept(IMetaContextVisitor& visitor)
{
  visitor.visit(*this);
  each(visitor);
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

void GroupLayer::each(IMetaContextVisitor& visitor) const
{
  for (const auto& sublayer : mLayers) {
    sublayer->accept(visitor);
  }
}

void GroupLayer::each(ILayerVisitor& visitor)
{
  for (const auto& sublayer : mLayers) {
    sublayer->accept(visitor);
  }
}

void GroupLayer::each(IConstLayerVisitor& visitor) const
{
  for (const auto& sublayer : mLayers) {
    sublayer->accept(visitor);
  }
}

void GroupLayer::append_layer(Shared<ILayer> layer)
{
  const auto layer_uuid = layer->meta().get_uuid();
  mLayers.push_back(std::move(layer));
}

auto GroupLayer::append_layer_to(const UUID& parent_uuid, Shared<ILayer> layer) -> bool
{
  if (auto* group_layer = find_group_layer(parent_uuid)) {
    group_layer->append_layer(std::move(layer));
    return true;
  }

  return false;
}

auto GroupLayer::remove_layer(const UUID& uuid) -> Shared<ILayer>
{
  const auto find_result = _recursive_find(uuid);

  if (find_result.has_value()) {
    auto removed_layer = *find_result->iter;
    find_result->storage->erase(find_result->iter);

    return removed_layer;
  }

  return nullptr;
}

auto GroupLayer::duplicate_layer(const UUID& uuid) -> Shared<ILayer>
{
  const auto find_result = _recursive_find(uuid);

  if (find_result.has_value()) {
    const auto& source_layer = *find_result->iter;

    auto new_layer = source_layer->clone();
    find_result->storage->insert(find_result->iter + 1, new_layer);

    return new_layer;
  }

  return nullptr;
}

auto GroupLayer::move_layer_up(const UUID& uuid) -> bool
{
  const auto find_result = _recursive_find(uuid);

  if (find_result.has_value() &&
      _can_move_layer_up(*find_result->storage, find_result->iter)) {
    std::iter_swap(find_result->iter, find_result->iter - 1);
    return true;
  }

  return false;
}

auto GroupLayer::move_layer_down(const UUID& uuid) -> bool
{
  const auto find_result = _recursive_find(uuid);

  if (find_result.has_value() &&
      _can_move_layer_down(*find_result->storage, find_result->iter)) {
    std::iter_swap(find_result->iter, find_result->iter + 1);
    return true;
  }

  return false;
}

void GroupLayer::set_persistent_id(const Maybe<int32> id)
{
  mDelegate.set_persistent_id(id);
}

void GroupLayer::set_opacity(const float opacity)
{
  mDelegate.set_opacity(opacity);
}

void GroupLayer::set_visible(const bool visible)
{
  mDelegate.set_visible(visible);
}

auto GroupLayer::can_move_layer_up(const UUID& uuid) const -> bool
{
  const auto find_result = _recursive_find(uuid);

  if (find_result.has_value()) {
    return _can_move_layer_up(*find_result->storage, find_result->iter);
  }

  return false;
}

auto GroupLayer::can_move_layer_down(const UUID& uuid) const -> bool
{
  const auto find_result = _recursive_find(uuid);

  if (find_result.has_value()) {
    return _can_move_layer_down(*find_result->storage, find_result->iter);
  }

  return false;
}

auto GroupLayer::get_layer_local_index(const UUID& uuid) const -> Maybe<ssize>
{
  const auto find_result = _recursive_find(uuid);

  if (find_result.has_value()) {
    return std::distance(find_result->storage->begin(), find_result->iter);
  }

  return false;
}

auto GroupLayer::get_layer_global_index(const UUID& uuid) const -> Maybe<ssize>
{
  LayerGlobalIndexCalculator calculator {uuid};
  each(calculator);
  return calculator.get_global_index();
}

auto GroupLayer::layer_at_index(const ssize index) -> ILayer*
{
  IndexBasedLayerFinder finder {index};
  each(finder);
  return finder.found_layer();
}

auto GroupLayer::layer_at_index(const ssize index) const -> const ILayer*
{
  IndexBasedConstLayerFinder finder {index};
  each(finder);
  return finder.found_layer();
}

auto GroupLayer::find_layer(const UUID& uuid) -> ILayer*
{
  ParentLayerFinder finder {uuid};
  accept(finder);

  if (finder.get_found_parent_layer() != nullptr) {
    return finder.get_found_target_layer_iter()->get();
  }

  return nullptr;
}

auto GroupLayer::find_layer(const UUID& uuid) const -> const ILayer*
{
  ConstParentLayerFinder finder {uuid};
  accept(finder);

  if (finder.get_found_parent_layer() != nullptr) {
    return finder.get_found_target_layer_iter()->get();
  }

  return nullptr;
}

auto GroupLayer::find_tile_layer(const UUID& uuid) -> TileLayer*
{
  GenericLayerFinder<TileLayer> finder {uuid};
  accept(finder);
  return finder.get_found_layer();
}

auto GroupLayer::find_tile_layer(const UUID& uuid) const -> const TileLayer*
{
  GenericConstLayerFinder<TileLayer> finder {uuid};
  accept(finder);
  return finder.get_found_layer();
}

auto GroupLayer::find_object_layer(const UUID& uuid) -> ObjectLayer*
{
  GenericLayerFinder<ObjectLayer> finder {uuid};
  accept(finder);
  return finder.get_found_layer();
}

auto GroupLayer::find_object_layer(const UUID& uuid) const -> const ObjectLayer*
{
  GenericConstLayerFinder<ObjectLayer> finder {uuid};
  accept(finder);
  return finder.get_found_layer();
}

auto GroupLayer::find_group_layer(const UUID& uuid) -> GroupLayer*
{
  GenericLayerFinder<GroupLayer> finder {uuid};
  accept(finder);
  return finder.get_found_layer();
}

auto GroupLayer::find_group_layer(const UUID& uuid) const -> const GroupLayer*
{
  GenericConstLayerFinder<GroupLayer> finder {uuid};
  accept(finder);
  return finder.get_found_layer();
}

auto GroupLayer::find_parent_layer(const UUID& uuid) -> GroupLayer*
{
  ParentLayerFinder finder {uuid};
  accept(finder);
  return finder.get_found_parent_layer();
}

auto GroupLayer::layer_count() const -> ssize
{
  LayerCounter counter {};
  each(counter);
  return counter.count();
}

auto GroupLayer::get_persistent_id() const -> Maybe<int32>
{
  return mDelegate.get_persistent_id();
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
  auto other = make_shared<GroupLayer>();

  other->mDelegate = mDelegate.clone();
  other->mLayers.reserve(mLayers.size());

  for (const auto& sublayer : mLayers) {
    other->mLayers.push_back(sublayer->clone());
  }

  return other;
}

auto GroupLayer::meta() -> Metadata&
{
  return mDelegate.meta();
}

auto GroupLayer::meta() const -> const Metadata&
{
  return mDelegate.meta();
}

auto GroupLayer::begin() -> iterator
{
  return iterator {this, 0};
}

auto GroupLayer::begin() const -> const_iterator
{
  return const_iterator {this, 0};
}

auto GroupLayer::end() -> iterator
{
  return iterator {this, layer_count()};
}

auto GroupLayer::end() const -> const_iterator
{
  return const_iterator {this, layer_count()};
}

auto GroupLayer::_recursive_find(const UUID& uuid) -> Maybe<FindResult>
{
  ParentLayerFinder finder {uuid};
  accept(finder);

  if (auto* parent_layer = finder.get_found_parent_layer()) {
    return FindResult {&parent_layer->mLayers, finder.get_found_target_layer_iter()};
  }

  return kNothing;
}

auto GroupLayer::_recursive_find(const UUID& uuid) const -> Maybe<ConstFindResult>
{
  ConstParentLayerFinder finder {uuid};
  accept(finder);

  if (const auto* parent_layer = finder.get_found_parent_layer()) {
    return ConstFindResult {&parent_layer->mLayers, finder.get_found_target_layer_iter()};
  }

  return kNothing;
}

auto GroupLayer::_can_move_layer_up(const LayerStorage& storage,
                                    const LayerStorage::const_iterator iter) -> bool
{
  const auto current_index = std::distance(storage.begin(), iter);
  return current_index > 0_z;
}

auto GroupLayer::_can_move_layer_down(const LayerStorage& storage,
                                      const LayerStorage::const_iterator iter) -> bool
{
  const auto current_index = std::distance(storage.begin(), iter);
  return current_index < std::ssize(storage) - 1_z;
}

GroupLayer::Iterator::Iterator(GroupLayer* root, const ssize index)
  : mRoot {root},
    mIndex {index}
{
  if (mIndex >= 0 && mIndex < mRoot->layer_count()) {
    mCurrent = mRoot->layer_at_index(mIndex);
  }
}

GroupLayer::ConstIterator::ConstIterator(const GroupLayer* root, const ssize index)
  : mRoot {root},
    mIndex {index}
{
  if (mIndex >= 0 && mIndex < mRoot->layer_count()) {
    mCurrent = mRoot->layer_at_index(mIndex);
  }
}

auto GroupLayer::Iterator::operator*() -> reference
{
  TACTILE_ASSERT(mCurrent != nullptr);
  return *mCurrent;
}

auto GroupLayer::ConstIterator::operator*() -> reference
{
  TACTILE_ASSERT(mCurrent != nullptr);
  return *mCurrent;
}

auto GroupLayer::Iterator::operator->() -> pointer
{
  return mCurrent;
}

auto GroupLayer::ConstIterator::operator->() -> pointer
{
  return mCurrent;
}

auto GroupLayer::Iterator::operator++() -> Iterator&
{
  ++mIndex;
  mCurrent = mRoot->layer_at_index(mIndex);
  return *this;
}

auto GroupLayer::ConstIterator::operator++() -> ConstIterator&
{
  ++mIndex;
  mCurrent = mRoot->layer_at_index(mIndex);
  return *this;
}

auto GroupLayer::Iterator::operator++(int) -> Iterator
{
  auto self = *this;
  ++(*this);
  return self;
}

auto GroupLayer::ConstIterator::operator++(int) -> ConstIterator
{
  auto self = *this;
  ++(*this);
  return self;
}

}  // namespace tactile
