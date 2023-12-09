// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

class TileLayer;
class ObjectLayer;
class GroupLayer;

/**
 * \brief Visitor type used to inspect non-const layers.
 */
class TACTILE_CORE_API ILayerVisitor {
 public:
  TACTILE_INTERFACE_CLASS(ILayerVisitor);

  /**
   * \brief Visits a tile layer.
   *
   * \param layer the visited layer.
   */
  virtual void visit([[maybe_unused]] TileLayer& layer)
  {}

  /**
   * \brief Visits an object layer.
   *
   * \param layer the visited layer.
   */
  virtual void visit([[maybe_unused]] ObjectLayer& layer)
  {}

  /**
   * \brief Visits a group layer.
   *
   * \param layer the visited layer.
   */
  virtual void visit([[maybe_unused]] GroupLayer& layer)
  {}
};

/**
 * \brief Visitor type used to inspect const layers.
 */
class TACTILE_CORE_API IConstLayerVisitor {
 public:
  TACTILE_INTERFACE_CLASS(IConstLayerVisitor);

  /**
   * \brief Visits a tile layer.
   *
   * \param layer the visited layer.
   */
  virtual void visit([[maybe_unused]] const TileLayer& layer) {}

  /**
   * \brief Visits an object layer.
   *
   * \param layer the visited layer.
   */
  virtual void visit([[maybe_unused]] const ObjectLayer& layer) {}

  /**
   * \brief Visits a group layer.
   *
   * \param layer the visited layer.
   */
  virtual void visit([[maybe_unused]] const GroupLayer& layer) {}
};

}  // namespace tactile
