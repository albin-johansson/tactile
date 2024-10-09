// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile::core {

class Model;
class EventDispatcher;

namespace ui {

class Language;

/**
 * Represents the "File" menu.
 */
class FileMenu final
{
 public:
  /**
   * Pushes the menu to the widget stack.
   *
   * \param model      The associated model.
   * \param dispatcher The event dispatcher to use.
   */
  void push(const Model& model, EventDispatcher& dispatcher);

 private:
  static void _push_recent_files_menu(const Language& language, EventDispatcher& dispatcher);
};

}  // namespace ui
}  // namespace tactile::core
