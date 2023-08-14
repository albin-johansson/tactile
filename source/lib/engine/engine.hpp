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

#include <centurion/input/keyboard.hpp>

#include "core/containers/smart_ptr.hpp"
#include "core/enums/backend_api.hpp"
#include "core/functional/maybe.hpp"
#include "core/prelude.hpp"
#include "engine/platform/imgui_context.hpp"
#include "engine/platform/protobuf_context.hpp"
#include "engine/platform/sdl_context.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(LoggingService)
TACTILE_FWD_DECLARE_CLASS(BackendService)
TACTILE_FWD_DECLARE_CLASS(AppDelegate)

class Engine final {
 public:
  explicit Engine(BackendAPI api);

  ~Engine() noexcept;

  void start();

  void set_app_delegate(Unique<AppDelegate> app);

  [[nodiscard]] auto get_window() -> cen::window&;

 private:
  BackendAPI mAPI;
  Unique<LoggingService> mLoggingService;
  Maybe<ProtobufContext> mProtobuf;
  Maybe<SDLContext> mSDL;
  Maybe<ImGuiContext> mImGui;
  Unique<BackendService> mBackendService;
  Unique<AppDelegate> mApp;
  cen::keyboard mKeyboard;
  bool mRunning : 1 {};

  void _poll_events();
};

}  // namespace tactile
