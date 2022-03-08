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

#include <centurion.hpp>

#include "imgui_context.hpp"
#include "protobuf_context.hpp"
#include "tactile.hpp"

namespace tactile {

/**
 * \brief Handles the configuration of the application window, OpenGL context, etc.
 *
 * \ingroup cfg
 */
class app_configuration final
{
 public:
  app_configuration();

  /* Returns the application window */
  [[nodiscard]] auto window() -> cen::window&;

 private:
  protobuf_context mProtobuf;
  cen::sdl mSDL;
  cen::img mIMG;

  /* Initialization of these members needs to be deferred */
  maybe<cen::window> mWindow;
  maybe<cen::gl_context> mOpenGL;
  maybe<im_gui_context> mImGui;
};

}  // namespace tactile
