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

#include "tactile/core/platform/window.hpp"

#include <SDL2/SDL.h>

#include "tactile/core/common/error.hpp"

namespace tactile {
namespace {

void _set_sdl_opengl_attributes()
{
  if constexpr (TACTILE_OS_MACOS) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  }

  // OpenGL 4.1.0 Core
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
}

}  // namespace

void WindowDeleter::operator()(SDL_Window* window) noexcept
{
  SDL_DestroyWindow(window);
}

Window::Window(const GraphicsAPI api)
    : mAPI {api}
{
  uint32 window_flags =
      SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;

  if (api == GraphicsAPI::OpenGL) {
    window_flags |= SDL_WINDOW_OPENGL;
    _set_sdl_opengl_attributes();
  }
  else if (api == GraphicsAPI::Vulkan) {
    window_flags |= SDL_WINDOW_VULKAN;
  }

  mWindow.reset(SDL_CreateWindow("Tactile",
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 800,
                                 600,
                                 window_flags));
  if (!mWindow) {
    throw Error {"Could not create window"};
  }
}

void Window::show()
{
  SDL_ShowWindow(mWindow.get());
}

void Window::hide()
{
  SDL_HideWindow(mWindow.get());
}

void Window::maximize()
{
  SDL_MaximizeWindow(mWindow.get());
}

auto Window::get_width() const -> int
{
  int width = 0;
  SDL_GetWindowSizeInPixels(mWindow.get(), &width, nullptr);
  return width;
}

auto Window::get_height() const -> int
{
  int height = 0;
  SDL_GetWindowSizeInPixels(mWindow.get(), nullptr, &height);
  return height;
}

auto Window::get_graphics_api() const -> GraphicsAPI
{
  return mAPI;
}

auto Window::get_handle() -> SDL_Window*
{
  return mWindow.get();
}

}  // namespace tactile
