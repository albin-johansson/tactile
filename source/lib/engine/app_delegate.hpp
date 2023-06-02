/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <centurion/event.hpp>

#include "common/enum/backend_api.hpp"

namespace tactile {

class AppDelegate {
 public:
  virtual ~AppDelegate() noexcept = default;

  /// Called once just before the event loop begins running.
  virtual void on_startup([[maybe_unused]] BackendAPI api) {}

  /// Called once immediately after the event loop stops running.
  virtual void on_shutdown() {}

  /// Called for each frame.
  virtual void on_update() {}

  /// Called for each polled event.
  virtual void on_event([[maybe_unused]] const cen::event_handler& handler) {}

  /// Called when the engine wants the app to reload all font files.
  virtual void reload_font_files() {}

  /// Indicates whether the application wants font resources to be reloaded.
  [[nodiscard]] virtual auto want_font_reload() const -> bool { return false; }

  /// Indicates whether the application wants to exit.
  [[nodiscard]] virtual auto should_stop() const -> bool = 0;
};

}  // namespace tactile
