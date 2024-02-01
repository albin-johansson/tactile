// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/ui/widget.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

class Model;

/**
 * Represents the central dock widget that hosts the main editor viewports.
 */
class TACTILE_CORE_API EditorDockWidget final {
 public:
  /**
   * Updates the editor dock widget.
   *
   * \param model      The associated model.
   * \param dispatcher The event dispatcher to use.
   */
  void update(const Model& model, EventDispatcher& dispatcher);
};

static_assert(WidgetType<EditorDockWidget>);

}  // namespace tactile::core
