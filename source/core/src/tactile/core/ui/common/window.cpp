// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/window.hpp"

#include "tactile/base/container/hash_map.hpp"
#include "tactile/core/debug/validation.hpp"
#include "tactile/core/util/lookup.hpp"

namespace tactile::ui {
inline namespace window {

/**
 * Provides window information that needs to persist between frames.
 */
struct WindowData final
{
  /** Was the mouse within the window in the previous frame? */
  bool had_mouse_then : 1;

  /** Is the mouse within the window in this frame? */
  bool has_mouse_now : 1;
};

// TODO language changes will cause issues here, force single language per run?
inline HashMap<const char*, WindowData> gWindowData {};

}  // namespace window

Window::Window(const char* name, const ImGuiWindowFlags flags, bool* is_open)
  : mName {require_not_null(name, "null name")},
    mIsOpen {ImGui::Begin(name, is_open, flags)}
{
  auto& window_data = gWindowData[name];
  window_data.had_mouse_then = window_data.has_mouse_now;
  window_data.has_mouse_now = has_mouse();
}

Window::~Window() noexcept
{
  ImGui::End();
}

auto Window::has_mouse() const -> bool
{
  const auto window_pos = ImGui::GetWindowPos();
  const auto window_size = ImGui::GetWindowSize();
  return mIsOpen &&
         ImGui::IsMouseHoveringRect(window_pos, window_pos + window_size);
}

auto Window::did_mouse_enter() const -> bool
{
  const auto& window_data = lookup_in(gWindowData, mName);
  return window_data.has_mouse_now && !window_data.had_mouse_then;
}

auto Window::did_mouse_exit() const -> bool
{
  const auto& window_data = lookup_in(gWindowData, mName);
  return !window_data.has_mouse_now && window_data.had_mouse_then;
}

auto Window::is_focused(const ImGuiFocusedFlags flags) const -> bool
{
  return mIsOpen && ImGui::IsWindowFocused(flags);
}

auto Window::is_docked() const -> bool
{
  return mIsOpen && ImGui::IsWindowDocked();
}

auto Window::is_open() const -> bool
{
  return mIsOpen;
}

}  // namespace tactile::ui
