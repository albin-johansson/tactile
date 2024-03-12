// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "sdl_context.hpp"

#include <utility>  // move

#include <GL/glew.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile {
namespace {

inline constexpr uint32 kWindowFlags = cen::window::hidden | cen::window::resizable |
                                       cen::window::opengl | cen::window::allow_high_dpi;

void init_sdl_attributes()
{
  // Ensure nearest pixel sampling
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

  // Enable multi-gesture events from touchpads
  SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "1");

  // Make sure we use OpenGL
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

  if constexpr (kOnMacos) {
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

  auto& window = mWindow.emplace("Tactile", cen::window::default_size(), kWindowFlags);
  auto& gl = mGL.emplace(window);
  gl.make_current(window);

  // Enable VSync
  cen::gl::set_swap_interval(cen::gl_swap_interval::synchronized);

  if (const auto result = glewInit(); result != GLEW_OK) {
    auto message = fmt::format("Failed to initialize GLEW: {}",
                               reinterpret_cast<const char*>(glewGetString(result)));
    throw Exception {std::move(message)};
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
