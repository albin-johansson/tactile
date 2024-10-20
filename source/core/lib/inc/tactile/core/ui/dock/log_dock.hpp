// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile::core {

class Model;
class EventDispatcher;

namespace ui {

class Language;

/**
 * Represents the dock widget that provides a view into the program log.
 */
class LogDock final
{
 public:
  /**
   * Pushes the log dock to the widget stack.
   *
   * \param model      The associated model.
   * \param dispatcher The event dispatcher to use.
   */
  void push(const Model& model, EventDispatcher& dispatcher);
};

}  // namespace ui
}  // namespace tactile::core
