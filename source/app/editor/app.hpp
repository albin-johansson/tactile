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

#include "editor/loop.hpp"

namespace tactile {

/// The heart of the Tactile map editor.
class App final : EventLoop {
 public:
  App();

  ~App() noexcept override;

  using EventLoop::start;

 protected:
  void on_startup() override;

  void on_shutdown() override;

  void on_pre_update() override;

  void on_update() override;

  void on_event(const cen::event_handler& handler) override;

 private:
  void subscribe_to_events();

  void save_current_files_to_history();

  void on_keyboard_event(cen::keyboard_event event);

  void on_mouse_wheel_event(const cen::mouse_wheel_event& event);

  void on_quit();
};

}  // namespace tactile
