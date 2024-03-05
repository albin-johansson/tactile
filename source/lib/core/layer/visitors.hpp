/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#pragma once

#include "common/type/uuid.hpp"
#include "core/layer/layer.hpp"
#include "core/layer/layer_visitor.hpp"
#include "tactile/base/container/function.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/base/int.hpp"

namespace tactile {

using LayerVector = Vector<Shared<Layer>>;
using LayerMutatorFn = Function<void(LayerVector&, LayerVector::iterator)>;
using LayerQueryFn = Function<void(const LayerVector&, LayerVector::const_iterator)>;

/// A generic visitor for operations that may modify layers and their associated storage.
class LayerMutatorVisitor : public LayerVisitor {
 public:
  explicit LayerMutatorVisitor(const UUID& layer_id, LayerMutatorFn fun);

  void visit(GroupLayer& layer) override;

 private:
  UUID mLayerId;
  LayerMutatorFn mFun;
};

/// A generic visitor for queries that doesn't modify layers or their storage.
/// Note, this must be used with the accept-function!
class LayerQueryVisitor : public ConstLayerVisitor {
 public:
  explicit LayerQueryVisitor(const UUID& layer_id, LayerQueryFn fun);

  void visit(const GroupLayer& layer) override;

 private:
  UUID mLayerId;
  LayerQueryFn mFun;
};

/// Counts the amount of visited layers.
class LayerCounter final : public ConstLayerVisitor {
 public:
  void visit(const TileLayer&) override { ++mCount; }
  void visit(const ObjectLayer&) override { ++mCount; }
  void visit(const GroupLayer&) override { ++mCount; }

  [[nodiscard]] auto layer_count() const -> usize { return mCount; }

 private:
  usize mCount {};
};

/// A visitor that determines the global index of a layer.
class LayerGlobalIndexCalculator final : public ConstLayerVisitor {
 public:
  explicit LayerGlobalIndexCalculator(const UUID& layer_id);

  void visit(const TileLayer& layer) override;
  void visit(const ObjectLayer& layer) override;
  void visit(const GroupLayer& layer) override;

  [[nodiscard]] auto get_index() const -> Maybe<usize> { return mIndex; }

 private:
  UUID mLayerId;
  usize mCount {};
  Maybe<usize> mIndex {};

  void check(const Layer& layer);
};

/// A visitor that attempts to find a layer.
class LayerFinder final : public LayerVisitor {
 public:
  explicit LayerFinder(const UUID& layer_id);

  void visit(TileLayer& layer) override;
  void visit(ObjectLayer& layer) override;
  void visit(GroupLayer& layer) override;

  [[nodiscard]] auto found_layer() -> Layer* { return mLayer; }

 private:
  UUID mLayerId;
  Layer* mLayer {};

  void check(Layer& layer);
};

class ConstLayerFinder final : public ConstLayerVisitor {
 public:
  explicit ConstLayerFinder(const UUID& layer_id);

  void visit(const TileLayer& layer) override;
  void visit(const ObjectLayer& layer) override;
  void visit(const GroupLayer& layer) override;

  [[nodiscard]] auto found_layer() -> const Layer* { return mLayer; }

 private:
  UUID mLayerId;
  const Layer* mLayer {};

  void check(const Layer& layer);
};

template <typename T>
class GenericLayerFinder final : public LayerVisitor {
 public:
  explicit GenericLayerFinder(const UUID& layer_id)
      : mLayerId {layer_id}
  {
  }

  void visit(T& layer) override
  {
    if (mLayerId == layer.get_uuid()) {
      mLayer = &layer;
    }
  }

  [[nodiscard]] auto get_found_layer() -> T* { return mLayer; }

 private:
  UUID mLayerId;
  T* mLayer {};
};

template <typename T>
class GenericConstLayerFinder final : public ConstLayerVisitor {
 public:
  explicit GenericConstLayerFinder(const UUID& layer_id)
      : mLayerId {layer_id}
  {
  }

  void visit(const T& layer) override
  {
    if (mLayerId == layer.get_uuid()) {
      mLayer = &layer;
    }
  }

  [[nodiscard]] auto get_found_layer() const -> const T* { return mLayer; }

 private:
  UUID mLayerId;
  const T* mLayer {};
};

}  // namespace tactile
