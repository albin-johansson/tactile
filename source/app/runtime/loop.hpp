// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <centurion/event.hpp>
#include <centurion/keyboard.hpp>

#include "tactile/base/prelude.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(AppCfg)

/// Handles the core event loop logic.
class EventLoop {
 public:
  virtual ~EventLoop() noexcept = default;

  /// Starts running the event loop.
  void start();

  /// Stops the event loop.
  void stop();

 protected:
  /// Called just before the event loop begins running.
  virtual void on_startup() {}

  /// Called immediately after the event loop stops running.
  virtual void on_shutdown() {}

  /// Called once for each frame before `on_update`.
  ///
  /// This function is useful to do tasks that cannot be done between calls to the
  /// NewFrame/EndFrame functions of Dear ImGui.
  virtual void on_pre_update() {}

  /// Called once for each frame.
  virtual void on_update() = 0;

  /// Called for every pending event.
  ///
  /// Note, quit events are handled automatically, so don't check for them in the
  /// overridden version.
  ///
  /// \param handler the event handler for the pending event.
  virtual void on_event(const cen::event_handler& handler) = 0;

 private:
  cen::keyboard mKeyboard;
  bool mRunning : 1 {};

  void poll_events();
};

}  // namespace tactile
