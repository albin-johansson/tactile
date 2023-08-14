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

#include "window.hpp"

#include <exception>  // exception

#include <spdlog/spdlog.h>

#include "io/directories.hpp"
#include "tactile/core/common/prelude.hpp"

#if TACTILE_OS_WINDOWS

#include <SDL_syswm.h>
#include <centurion/system/shared_object.hpp>
#include <dwmapi.h>

#endif  // TACTILE_OS_WINDOWS

namespace tactile {

void load_window_icon(cen::window& window)
{
  try {
    // This is necessary to allow macOS builds in different flavours
    const auto icon_path = find_resource(kIsAppBundle ? "Tactile.icns"  //
                                                      : "assets/icon.png");
    window.set_icon(cen::surface {icon_path.string()});
  }
  catch (const std::exception& e) {
    spdlog::error("Failed to load window icon: {}", e.what() ? e.what() : "N/A");
  }
}

void win32_use_immersive_dark_mode([[maybe_unused]] cen::window& window)
{
#if TACTILE_OS_WINDOWS
  SDL_SysWMinfo wm_info {};
  SDL_VERSION(&wm_info.version);

  if (SDL_GetWindowWMInfo(window.get(), &wm_info)) {
    const cen::shared_object dwmapi {"dwmapi.dll"};

    using DwmSetWindowAttributeFn = HRESULT(HWND, DWORD, LPCVOID, DWORD);
    if (auto* set_attribute =
            dwmapi.load_function<DwmSetWindowAttributeFn>("DwmSetWindowAttribute")) {
      HWND hwnd = wm_info.info.win.window;
      BOOL mode = 1;
      set_attribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &mode, sizeof mode);
    }
  }
#endif  // TACTILE_OS_WINDOWS
}

}  // namespace tactile