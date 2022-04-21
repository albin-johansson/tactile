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

#include <centurion/system.hpp>

#include "cfg/sdl_attributes.hpp"
#include "io/directories.hpp"
#include "io/persistence/preferences.hpp"
#include "meta/build.hpp"
#include "misc/assert.hpp"
#include "misc/logging.hpp"
#include "misc/throw.hpp"

#if TACTILE_PLATFORM_WINDOWS

#include <SDL_syswm.h>
#include <dwmapi.h>

#endif  // TACTILE_PLATFORM_WINDOWS

namespace tactile {
namespace {

[[nodiscard]] consteval auto _get_window_flags() noexcept -> uint32
{
  auto flags = cen::window::hidden | cen::window::resizable | cen::window::opengl;

  if constexpr (on_osx) {
    flags |= cen::window::allow_high_dpi;
  }

  return flags;
}

void _win32_use_immersive_dark_mode([[maybe_unused]] cen::window& window)
{
#if TACTILE_PLATFORM_WINDOWS
  SDL_SysWMinfo wm{};
  SDL_VERSION(&wm.version);
  if (SDL_GetWindowWMInfo(window.get(), &wm)) {
    HWND hwnd = wm.info.win.window;

    cen::shared_object dwmapi{"dwmapi.dll"};

    auto* setAttribute = dwmapi.load_function<HRESULT(HWND, DWORD, LPCVOID, DWORD)>(
        "DwmSetWindowAttribute");

    if (!setAttribute) {
      return;
    }

    BOOL mode = 1;
    setAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &mode, sizeof mode);
  }
#endif  // TACTILE_PLATFORM_WINDOWS
}

}  // namespace

AppConfiguration::AppConfiguration()
{
  /* Use terminate handler that doesn't do anything fancy, e.g. no logging */
  std::set_terminate([] { std::abort(); });

  if constexpr (is_debug_build) {
    set_log_level(LogLevel::verbose);
  }
  else {
    set_log_level(LogLevel::info);
  }

  init_sdl_attributes();

  mWindow.emplace("Tactile", cen::window::default_size(), _get_window_flags());
  TACTILE_ASSERT(mWindow.has_value());

  _win32_use_immersive_dark_mode(*mWindow);

  /* This is ugly, but it's necessary to allow macOS builds in different flavours */
#ifdef TACTILE_BUILD_APP_BUNDLE
  const auto iconPath = find_resource("Tactile.icns");
#else
  const auto iconPath = find_resource(on_osx ? "assets/Tactile.icns" : "assets/icon.png");
#endif

  mWindow->set_icon(cen::surface{iconPath.string()});

  mOpenGL.emplace(*mWindow);
  mOpenGL->make_current(*mWindow);

  cen::gl::set_swap_interval(cen::gl_swap_interval::synchronized);

  if (glewInit() != GLEW_OK) {
    log_error("Failed to initialize GLEW!");
    panic("Failed to initialize GLEW!");
  }

  log_debug("OpenGL version... {}", glGetString(GL_VERSION));
  log_debug("OpenGL renderer... {}", glGetString(GL_RENDERER));

  load_preferences();

  TACTILE_ASSERT(mOpenGL.has_value());
  mImGui.emplace(*mWindow, *mOpenGL);

  mWindow->maximize();
}

auto AppConfiguration::window() -> cen::window&
{
  return mWindow.value();
}

}  // namespace tactile
