// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile::core {

class Model;
class EventDispatcher;

namespace ui {

/**
 * Represents the "Edit" menu.
 */
class EditMenu final
{
 public:
  /**
   * Pushes the menu to the widget stack.
   *
   * \param model      The associated model.
   * \param dispatcher The event dispatcher to use.
   */
  void push(const Model& model, EventDispatcher& dispatcher);
};

}  // namespace ui
}  // namespace tactile::core
