// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::ui {

/**
 * \interface IWidget
 * \brief Represents user interface widgets.
 */
class IWidget {
 public:
  TACTILE_INTERFACE_CLASS(IWidget);

  /**
   * \brief Updates the widget state.
   *
   * \param      model the associated model.
   * \param dispatcher the associated event dispatcher.
   */
  virtual void update(const Model& model, EventDispatcher& dispatcher) = 0;
};

}  // namespace tactile::ui
