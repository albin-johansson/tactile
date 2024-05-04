// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <imgui.h>

#include "tactile/base/prelude.hpp"

namespace tactile::ui {

/// \addtogroup UI
/// \{

/**
 * Wrapper around an ImGui window.
 */
class Window final
{
 public:
  TACTILE_DELETE_COPY(Window);
  TACTILE_DELETE_MOVE(Window);

  /**
   * Pushes a window to the widget stack.
   *
   * \param      name    The window name.
   * \param      flags   The window flags to use.
   * \param[out] is_open If set, a close button will be shown. The value will be
   *                     set to false if the window was closed using this
   *                     button.
   */
  [[nodiscard]]
  explicit Window(const char* name,
                  ImGuiWindowFlags flags = 0,
                  bool* is_open = nullptr);

  /**
   * Pops the window from the widget stack.
   */
  ~Window() noexcept;

  /**
   * Indicates whether the window contains the mouse pointer.
   *
   * \return
   * True if the window contains the mouse; false otherwise.
   */
  [[nodiscard]]
  auto has_mouse() const -> bool;

  /**
   * Indicates whether the mouse pointer entered the window during this frame.
   *
   * \return
   * True if the mouse entered the window; false otherwise.
   */
  [[nodiscard]]
  auto did_mouse_enter() const -> bool;

  /**
   * Indicates whether the mouse pointer exited the window during this frame.
   *
   * \return
   * True if the mouse exited the window; false otherwise.
   */
  [[nodiscard]]
  auto did_mouse_exit() const -> bool;

  /**
   * Indicates whether the window has input focus.
   *
   * \param flags The focus flags to use.
   *
   * \return
   * True if the window has input focus; false otherwise.
   */
  [[nodiscard]]
  auto is_focused(ImGuiFocusedFlags flags = 0) const -> bool;

  /**
   * Indicates whether the window is currently docked.
   *
   * \return
   * True if the window is docked; false otherwise.
   */
  [[nodiscard]]
  auto is_docked() const -> bool;

  /**
   * Indicates whether the window is open.
   *
   * \return
   * True if the window is open; false otherwise.
   */
  [[nodiscard]]
  auto is_open() const -> bool;

 private:
  const char* mName;
  bool mIsOpen {false};
};

/// \}

}  // namespace tactile::ui
