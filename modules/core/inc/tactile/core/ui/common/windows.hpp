// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <imgui.h>

#include "tactile/core/api.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::ui {

/**
 * Provides window configuration options.
 */
struct ScopedWindowOptions final {
  ImGuiWindowFlags flags;
  bool* is_open;
};

/**
 * RAII utility for showing windows.
 */
class TACTILE_CORE_API ScopedWindow final {
 public:
  TACTILE_DELETE_COPY(ScopedWindow);
  TACTILE_DELETE_MOVE(ScopedWindow);

  /**
   * Begins recording a new window.
   *
   * \param title   The window title, cannot be null.
   * \param options The window options.
   */
  explicit ScopedWindow(const char* title, const ScopedWindowOptions& options = {});

  /**
   * Ends the window recording.
   */
  ~ScopedWindow() noexcept;

  /**
   * Indicates whether the window is open.
   *
   * \return
   *    True if the window is open; false otherwise.
   */
  [[nodiscard]]
  auto is_open() const -> bool;

 private:
  bool mIsOpen;
};

}  // namespace tactile::ui
