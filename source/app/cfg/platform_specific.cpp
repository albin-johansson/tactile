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

#include "platform_specific.hpp"

#include "meta/build.hpp"

#if TACTILE_PLATFORM_WINDOWS

#include <SDL_syswm.h>
#include <centurion/system.hpp>
#include <dwmapi.h>

#endif  // TACTILE_PLATFORM_WINDOWS

namespace tactile {

void use_immersive_dark_mode([[maybe_unused]] cen::window& window)
{
#if TACTILE_PLATFORM_WINDOWS
  SDL_SysWMinfo wm {};
  SDL_VERSION(&wm.version);
  if (SDL_GetWindowWMInfo(window.get(), &wm)) {
    cen::shared_object dwmapi {"dwmapi.dll"};

    using Signature = HRESULT(HWND, DWORD, LPCVOID, DWORD);
    if (auto* setAttribute = dwmapi.load_function<Signature>("DwmSetWindowAttribute")) {
      HWND hwnd = wm.info.win.window;
      BOOL mode = 1;
      setAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &mode, sizeof mode);
    }
  }
#endif  // TACTILE_PLATFORM_WINDOWS
}

}  // namespace tactile