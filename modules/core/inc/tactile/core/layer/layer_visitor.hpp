// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

class TileLayer;
class ObjectLayer;
class GroupLayer;

/**
 * Visitor type used to inspect non-const layers.
 */
class ILayerVisitor {
 public:
  TACTILE_INTERFACE_CLASS(ILayerVisitor);

  /**
   * Visits a tile layer.
   *
   * \param layer The visited layer.
   */
  virtual void visit([[maybe_unused]] TileLayer& layer) {}

  /**
   * Visits an object layer.
   *
   * \param layer The visited layer.
   */
  virtual void visit([[maybe_unused]] ObjectLayer& layer) {}

  /**
   * Visits a group layer.
   *
   * \param layer The visited layer.
   */
  virtual void visit([[maybe_unused]] GroupLayer& layer) {}
};

/**
 * Visitor type used to inspect const layers.
 */
class IConstLayerVisitor {
 public:
  TACTILE_INTERFACE_CLASS(IConstLayerVisitor);

  /**
   * Visits a tile layer.
   *
   * \param layer The visited layer.
   */
  virtual void visit([[maybe_unused]] const TileLayer& layer) {}

  /**
   * Visits an object layer.
   *
   * \param layer The visited layer.
   */
  virtual void visit([[maybe_unused]] const ObjectLayer& layer) {}

  /**
   * Visits a group layer.
   *
   * \param layer The visited layer.
   */
  virtual void visit([[maybe_unused]] const GroupLayer& layer) {}
};

}  // namespace tactile::core
