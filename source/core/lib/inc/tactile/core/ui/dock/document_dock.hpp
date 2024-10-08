// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

class Model;
class EventDispatcher;

namespace ui {

/**
 * A dock widget that contains the primary document viewports.
 */
class DocumentDock final
{
 public:
  /**
   * Pushes the document dock to the widget stack.
   *
   * \param model      The associated model.
   * \param dispatcher The event dispatcher to use.
   */
  void push(const Model& model, EventDispatcher& dispatcher);
};

}  // namespace ui
}  // namespace tactile
