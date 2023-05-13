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
#include <centurion/keyboard.hpp>

#include "common/predef.hpp"
#include "common/type/maybe.hpp"
#include "common/type/ptr.hpp"
#include "engine/app_delegate.hpp"
#include "engine/backend/backend.hpp"
#include "engine/backend/backend_api.hpp"
#include "platform/imgui_context.hpp"
#include "platform/protobuf_context.hpp"
#include "platform/sdl_context.hpp"

namespace tactile {

class Engine final {
 public:
  explicit Engine(BackendAPI api);

  void start();

  void stop();

  void set_app_delegate(Unique<AppDelegate> app);

  [[nodiscard]] auto get_window() -> cen::window&;

 private:
  Maybe<ProtobufContext> mProtobuf;
  Maybe<SDLContext> mSDL;
  Maybe<ImGuiContext> mImGui;
  Unique<Backend> mBackend;
  Unique<AppDelegate> mApp;
  cen::keyboard mKeyboard;
  bool mRunning : 1 {};

  void poll_events();

  void reload_fonts();
};

[[noreturn]] TACTILE_NOINLINE void on_terminate();

}  // namespace tactile
