// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

TACTILE_FWD(class TileLayer)
TACTILE_FWD(class ObjectLayer)
TACTILE_FWD(class GroupLayer)

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

}  // namespace tactile
