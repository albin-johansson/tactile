// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/ui/widget.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

class Model;

/**
 * Represents the dock widget for managing the layer stack of a map document.
 */
class TACTILE_CORE_API LayerDockWidget final {
 public:
  /**
   * Updates the layer dock widget.
   *
   * \param model      The associated model.
   * \param dispatcher The event dispatcher to use.
   */
  void update(const Model& model, EventDispatcher& dispatcher);
};

static_assert(Widget<LayerDockWidget>);

}  // namespace tactile
