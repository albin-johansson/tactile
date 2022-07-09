/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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
#include <centurion/keyboard.hpp>

#include "core/common/macros.hpp"

namespace tactile {

class AppConfiguration;

/// Handles the core event loop logic.
class AEventLoop
{
 public:
  TACTILE_DEFAULT_COPY(AEventLoop);
  TACTILE_DEFAULT_MOVE(AEventLoop);

  explicit AEventLoop(AppConfiguration* cfg);

  virtual ~AEventLoop() noexcept = default;

  /// Starts running the event loop.
  void start();

  /// Stops the event loop.
  void stop();

 protected:
  /// Called just before the event loop begins running.
  virtual void on_startup() {}

  /// Called immediately after the event loop stops running.
  virtual void on_shutdown() {}

  /**
   * Called once for each frame before `on_update()`.
   *
   * This function is useful to do tasks that cannot be done between calls to the
   * NewFrame/EndFrame functions of Dear ImGui.
   */
  virtual void on_pre_update() {}

  /// Called once for each frame.
  virtual void on_update() = 0;

  /**
   * Called for every pending event.
   *
   * Note, quit events are handled automatically, so don't check for them in the
   * overridden version.
   *
   * \param handler the event handler for the pending event.
   */
  virtual void on_event(const cen::event_handler& handler) = 0;

 private:
  AppConfiguration* mCfg{}; /* Non-owning */
  cen::keyboard     mKeyboard;
  bool              mRunning{};

  void poll_events();
};

}  // namespace tactile