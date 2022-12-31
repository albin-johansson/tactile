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

#include "core/type/maybe.hpp"
#include "core/vocabulary.hpp"

namespace tactile {

class SDLContext final {
 public:
  TACTILE_DELETE_COPY(SDLContext);
  TACTILE_DELETE_MOVE(SDLContext);

  [[nodiscard]] SDLContext();

  [[nodiscard]] auto get_window() -> cen::window&;
  [[nodiscard]] auto get_gl_context() -> cen::gl_context&;

 private:
  cen::sdl mSDL;
  cen::img mIMG;
  Maybe<cen::window> mWindow;
  Maybe<cen::gl_context> mGL;
};

}  // namespace tactile
