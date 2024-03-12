// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(TileLayer)
TACTILE_FWD_DECLARE_CLASS(ObjectLayer)
TACTILE_FWD_DECLARE_CLASS(GroupLayer)

/// Visitor for all layer types.
class LayerVisitor {
 public:
  virtual ~LayerVisitor() noexcept = default;

  virtual void visit([[maybe_unused]] TileLayer& layer) {}
  virtual void visit([[maybe_unused]] ObjectLayer& layer) {}
  virtual void visit([[maybe_unused]] GroupLayer& layer) {}
};

/// Read-only visitor for all layer types.
class ConstLayerVisitor {
 public:
  virtual ~ConstLayerVisitor() noexcept = default;

  virtual void visit([[maybe_unused]] const TileLayer& layer) {}
  virtual void visit([[maybe_unused]] const ObjectLayer& layer) {}
  virtual void visit([[maybe_unused]] const GroupLayer& layer) {}
};

}  // namespace tactile
