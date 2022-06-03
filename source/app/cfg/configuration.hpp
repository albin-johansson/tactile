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

#include <centurion/initialization.hpp>
#include <centurion/opengl.hpp>
#include <centurion/window.hpp>

#include "cfg/imgui_context.hpp"
#include "cfg/protobuf_context.hpp"
#include "core/common/maybe.hpp"

/**
 * \defgroup cfg Configuration
 * \brief Contains utilities related to the configuration of the application.
 */

namespace tactile {

/// Handles the configuration of the application window, OpenGL context, etc.
/// \ingroup cfg
class AppConfiguration final
{
 public:
  AppConfiguration();

  /* Returns the application window */
  [[nodiscard]] auto window() -> cen::window&;

 private:
  ProtobufContext mProtobuf;
  cen::sdl mSDL;
  cen::img mIMG;

  /* Initialization of these members needs to be deferred */
  Maybe<cen::window> mWindow;
  Maybe<cen::gl_context> mOpenGL;
  Maybe<ImGuiContext> mImGui;
};

}  // namespace tactile
