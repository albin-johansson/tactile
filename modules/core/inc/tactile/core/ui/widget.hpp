// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::ui {

/**
 * Represents user interface widgets.
 */
class IWidget {
 public:
  TACTILE_INTERFACE_CLASS(IWidget);

  /**
   * Updates the widget state.
   *
   * \param model      The associated model.
   * \param dispatcher The associated event dispatcher.
   */
  virtual void update(const Model& model, EventDispatcher& dispatcher) = 0;
};

}  // namespace tactile::ui
