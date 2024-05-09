// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <imgui.h>

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/numeric/vec.hpp"

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

  /**
   * Returns the position of the top-left corner of the window.
   *
   * \return
   * A window position using absolute coordinates.
   */
  [[nodiscard]]
  auto get_pos() const -> Float2;

  /**
   * Returns the size of the window.
   *
   * \return
   * A window size.
   */
  [[nodiscard]]
  auto get_size() const -> Float2;

  /**
   * Returns the position of the mouse pointer in the window.
   *
   * \return
   * The relative position of the mouse if the mouse is within the window;
   * nothing otherwise.
   */
  [[nodiscard]]
  auto get_local_mouse_pos() const -> Maybe<Float2>;

 private:
  const char* mName;
  bool mIsOpen {false};
  ImVec2 mPos {};
  ImVec2 mSize {};
};

/**
 * Centers the next submitted window vertically and horizontally.
 *
 * \param condition The condition that dictates when the window is centered.
 */
void center_next_window(ImGuiCond condition);

/// \}

}  // namespace tactile::ui
