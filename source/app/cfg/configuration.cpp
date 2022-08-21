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

#include "configuration.hpp"

#include <cstdlib>    // abort
#include <exception>  // set_terminate

#include <GL/glew.h>
#include <SDL.h>
#include <boost/stacktrace/stacktrace.hpp>
#include <spdlog/spdlog.h>

#include "cfg/platform_specific.hpp"
#include "core/util/fmt.hpp"
#include "io/directories.hpp"
#include "io/persist/preferences.hpp"
#include "lang/language.hpp"
#include "meta/build.hpp"
#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile {
namespace {

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

[[nodiscard]] consteval auto get_window_flags() noexcept -> uint32
{
  auto flags = cen::window::hidden | cen::window::resizable | cen::window::opengl;

  if constexpr (on_osx) {
    flags |= cen::window::allow_high_dpi;
  }

  return flags;
}

}  // namespace

// Keep the handler out of the anonymous namespace
inline void terminate_handler()
{
  try {
    spdlog::critical("Into exile I must go. Failed I have.\n{}",
                     boost::stacktrace::stacktrace {});
  }
  catch (...) {
    /* Not much we can do */
  }

  std::abort();
}

AppCfg::AppCfg()
{
  std::set_terminate(&terminate_handler);

  init_sdl_attributes();

  mWindow.emplace("Tactile", cen::window::default_size(), get_window_flags());
  TACTILE_ASSERT(mWindow.has_value());

  use_immersive_dark_mode(*mWindow);

  // This is ugly, but it's necessary to allow macOS builds in different flavours
#ifdef TACTILE_BUILD_APP_BUNDLE
  const auto icon_path = find_resource("Tactile.icns");
#else
  const auto icon_path =
      io::find_resource(on_osx ? "assets/Tactile.icns" : "assets/icon.png");
#endif

  mWindow->set_icon(cen::surface {icon_path.string()});

  mOpenGL.emplace(*mWindow);
  mOpenGL->make_current(*mWindow);

  cen::gl::set_swap_interval(cen::gl_swap_interval::synchronized);

  if (glewInit() != GLEW_OK) {
    throw TactileError {"Failed to initialize GLEW!"};
  }

  spdlog::debug("OpenGL version... {}", (const char*) glGetString(GL_VERSION));
  spdlog::debug("OpenGL renderer... {}", (const char*) glGetString(GL_RENDERER));

  load_languages();
  io::load_preferences();

  TACTILE_ASSERT(mOpenGL.has_value());
  mImGui.emplace(*mWindow, *mOpenGL);

  mWindow->maximize();
}

auto AppCfg::window() -> cen::window&
{
  return mWindow.value();
}

}  // namespace tactile
