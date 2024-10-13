// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

namespace tactile::core {

class Model;
class EventDispatcher;

namespace ui {

/**
 * Represents the main menu bar widget.
 */
class MenuBar final
{
 public:
  /**
   * Pushes the menu bar to the widget stack.
   *
   * \param model      The associated model.
   * \param dispatcher The event dispatcher to use.
   */
  void push(const Model& model, EventDispatcher& dispatcher);

 private:
  bool m_show_debugger {};
  bool m_show_demo {};
  bool m_show_style_editor {};

  void _push_debug_menu(const Model& model);
};

}  // namespace ui
}  // namespace tactile::core
