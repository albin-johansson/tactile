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

#include <centurion/window.hpp>

#include "common/macros.hpp"
#include "common/type/maybe.hpp"
#include "init/protobuf_context.hpp"
#include "init/sdl_context.hpp"
#include "ui/imgui_context.hpp"

namespace tactile {

class AppInitializer final {
 public:
  TACTILE_DELETE_COPY(AppInitializer);
  TACTILE_DELETE_MOVE(AppInitializer);

  [[nodiscard]] AppInitializer();

  [[nodiscard]] auto get_window() -> cen::window&;

 private:
  Maybe<ProtobufContext> mProtobuf;
  Maybe<SDLContext> mSDL;
  Maybe<ImGuiContext> mImGui;
};

void on_terminate();

}  // namespace tactile
