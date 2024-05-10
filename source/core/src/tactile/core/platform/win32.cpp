// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/platform/win32.hpp"

#if TACTILE_OS_WINDOWS

  #include <SDL_syswm.h>
  #include <centurion/system.hpp>
  #include <dwmapi.h>

#endif  // TACTILE_OS_WINDOWS

namespace tactile {

void win32_use_immersive_dark_mode([[maybe_unused]] SDL_Window* window)
{
#if TACTILE_OS_WINDOWS
  SDL_SysWMinfo wm_info {};
  SDL_VERSION(&wm_info.version);

  if (SDL_GetWindowWMInfo(window, &wm_info)) {
    const cen::shared_object dwmapi {"dwmapi.dll"};

    using DwmSetWindowAttributeFn = HRESULT(HWND, DWORD, LPCVOID, DWORD);
    if (auto* set_attribute = dwmapi.load_function<DwmSetWindowAttributeFn>(
            "DwmSetWindowAttribute")) {
      HWND hwnd = wm_info.info.win.window;
      BOOL mode = 1;
      set_attribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &mode, sizeof mode);
    }
  }
#endif  // TACTILE_OS_WINDOWS
}

void win32_enable_virtual_terminal_processing()
{
#if TACTILE_OS_WINDOWS
  const auto enable_virtual_term_processing = [](const DWORD handle_id) {
    HANDLE handle = GetStdHandle(handle_id);
    if (handle != INVALID_HANDLE_VALUE) {  // NOLINT
      DWORD modes = 0;
      if (GetConsoleMode(handle, &modes)) {
        if (!SetConsoleMode(handle,
                            modes | ENABLE_VIRTUAL_TERMINAL_PROCESSING |
                                DISABLE_NEWLINE_AUTO_RETURN)) {
          SetConsoleMode(handle, modes | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
      }
    }
  };

  enable_virtual_term_processing(STD_OUTPUT_HANDLE);
  enable_virtual_term_processing(STD_ERROR_HANDLE);
#endif
}

}  // namespace tactile
