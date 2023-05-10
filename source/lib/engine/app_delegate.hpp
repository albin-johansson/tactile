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

namespace tactile {

class AppDelegate {
 public:
  virtual ~AppDelegate() noexcept = default;

  /// Called once just before the event loop begins running.
  virtual void on_startup() {}

  /// Called once immediately after the event loop stops running.
  virtual void on_shutdown() {}

  /// Called before each frame is started (regardless of the frame start success).
  virtual void on_pre_update() {}

  /// Called for each frame.
  virtual void on_update() {}

  /// Called for each polled event.
  virtual void on_event([[maybe_unused]] const cen::event_handler& handler) {}

  /// Called whenever the backend font resources have been reloaded.
  virtual void on_font_reload() {}

  /// Indicates whether the application wants to exit.
  [[nodiscard]] virtual auto should_stop() const -> bool = 0;
};

}  // namespace tactile
