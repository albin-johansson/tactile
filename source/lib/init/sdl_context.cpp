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

#include "sdl_context.hpp"

#include <utility>  // move

#include <GL/glew.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "common/debug/panic.hpp"
#include "common/predef.hpp"

namespace tactile {
namespace {

inline constexpr uint32 window_flags = cen::window::hidden | cen::window::resizable |
                                       cen::window::opengl | cen::window::allow_high_dpi;

void init_sdl_attributes()
{
  // Ensure nearest pixel sampling
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

  // Enable multi-gesture events from touchpads
  SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "1");

  // Make sure we use OpenGL
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

  if constexpr (on_macos) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  }

  // OpenGL 4.1.0 Core
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

}  // namespace

SDLContext::SDLContext()
{
  init_sdl_attributes();

  auto& window = mWindow.emplace("Tactile", cen::window::default_size(), window_flags);
  auto& gl = mGL.emplace(window);
  gl.make_current(window);

  // Enable VSync
  cen::gl::set_swap_interval(cen::gl_swap_interval::synchronized);

  if (const auto result = glewInit(); result != GLEW_OK) {
    auto message = fmt::format("Failed to initialize GLEW: {}",
                               reinterpret_cast<const char*>(glewGetString(result)));
    throw TactileError {std::move(message)};
  }

  spdlog::debug("OpenGL version: {}",
                reinterpret_cast<const char*>(glGetString(GL_VERSION)));
  spdlog::debug("OpenGL renderer: {}",
                reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
}

auto SDLContext::get_window() -> cen::window&
{
  return mWindow.value();
}

auto SDLContext::get_gl_context() -> cen::gl_context&
{
  return mGL.value();
}

}  // namespace tactile