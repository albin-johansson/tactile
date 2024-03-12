// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <centurion/event.hpp>
#include <centurion/window.hpp>

#include "runtime/loop.hpp"

namespace tactile {

/// The heart of the Tactile map editor.
class App final : EventLoop {
 public:
  explicit App(cen::window& window);

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

  void add_open_documents_to_file_history();

  void on_keyboard_event(cen::keyboard_event event);

  void on_mouse_wheel_event(const cen::mouse_wheel_event& event);

  void on_quit();
};

}  // namespace tactile
