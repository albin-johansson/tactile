// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/map/map_spec.hpp"

namespace tactile::core {

class Model;
class EventDispatcher;

namespace ui {

/**
 * A dialog for creating new map documents.
 */
class NewMapDialog final
{
 public:
  /**
   * Pushes the dialog to the widget stack.
   *
   * \param model      The associated model.
   * \param dispatcher The event dispatcher that will be used.
   */
  void push(const Model& model, EventDispatcher& dispatcher);

  /**
   * Schedules the dialog to be opened.
   */
  void open();

 private:
  MapSpec mSpec {};
  bool mShouldOpen {false};
};

}  // namespace ui
}  // namespace tactile::core
