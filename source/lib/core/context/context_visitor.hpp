// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(Map)
TACTILE_FWD_DECLARE_CLASS(TileLayer)
TACTILE_FWD_DECLARE_CLASS(ObjectLayer)
TACTILE_FWD_DECLARE_CLASS(GroupLayer)
TACTILE_FWD_DECLARE_CLASS(Object)
TACTILE_FWD_DECLARE_CLASS(Tileset)
TACTILE_FWD_DECLARE_CLASS(Tile)

/// An interface for visiting specific context implementations.
class ContextVisitor {
 public:
  virtual ~ContextVisitor() noexcept = default;

  virtual void visit([[maybe_unused]] const Map& map) {}

  virtual void visit([[maybe_unused]] const TileLayer& layer) {}

  virtual void visit([[maybe_unused]] const ObjectLayer& layer) {}

  virtual void visit([[maybe_unused]] const GroupLayer& layer) {}

  virtual void visit([[maybe_unused]] const Object& object) {}

  virtual void visit([[maybe_unused]] const Tileset& tileset) {}

  virtual void visit([[maybe_unused]] const Tile& tile) {}
};

}  // namespace tactile
