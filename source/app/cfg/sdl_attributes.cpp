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

#include "sdl_attributes.hpp"

#include <SDL.h>
#include <centurion.hpp>

#include "meta/build.hpp"

namespace tactile {

void init_sdl_attributes()
{
  /* Ensure nearest pixel sampling */
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

  /* Enable multi-gesture events from touchpads */
  SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "1");

  /* Make sure that we use OpenGL */
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

  if constexpr (on_osx) {
    cen::gl::set(cen::gl_attribute::context_flags,
                 SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  }

  cen::gl::set(cen::gl_attribute::context_profile_mask, SDL_GL_CONTEXT_PROFILE_CORE);
  cen::gl::set(cen::gl_attribute::context_major_version, 3);
  cen::gl::set(cen::gl_attribute::context_minor_version, 2);

  cen::gl::set(cen::gl_attribute::double_buffer, 1);
}

}  // namespace tactile
