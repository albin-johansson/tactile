// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile::ui {

/// \addtogroup UI
/// \{

/**
 * RAII helper for the BeginMenu/EndMenu ImGui functions.
 */
class MenuScope final
{
 public:
  TACTILE_DELETE_COPY(MenuScope);
  TACTILE_DELETE_MOVE(MenuScope);

  /**
   * Pushes a menu to the widget stack.
   *
   * \param name The menu name.
   */
  [[nodiscard]]
  explicit MenuScope(const char* name);

  /**
   * Pops the menu from the widget stack.
   */
  ~MenuScope() noexcept;

  /**
   * Indicates whether the menu is open.
   *
   * \return
   * True if the menu is open; false otherwise.
   */
  [[nodiscard]]
  auto is_open() const -> bool;

 private:
  bool mIsOpen {};
};

/// \}

}  // namespace tactile::ui
