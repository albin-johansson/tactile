// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

class Model;

namespace ui {

/**
 * Represents the "Debug" menu.
 *
 * \ingroup UI
 */
class DebugMenu final
{
 public:
  /**
   * Pushes the menu to the widget stack.
   *
   * \param model The associated model.
   */
  void push(const Model& model);

 private:
  bool mShowDebugger {};
  bool mShowDemo {};
  bool mShowStyle {};
};

}  // namespace ui
}  // namespace tactile
