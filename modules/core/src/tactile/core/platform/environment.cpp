// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/platform/environment.hpp"

#include <cstdlib>  // getenv, _dupenv_s, free

#if TACTILE_OS_WINDOWS
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
#endif

namespace tactile {

auto get_env(const char* var) -> Maybe<String>
{
  if (var) {
#if TACTILE_OS_WINDOWS
    char* temp {};
    _dupenv_s(&temp, nullptr, var);

    if (temp) {
      String result {temp};
      std::free(temp);
      return result;
    }
#else
    if (const auto* value = std::getenv(var)) {
      return String {value};
    }
#endif  // TACTILE_OS_WINDOWS
  }

  return kNone;
}

void win32_enable_virtual_terminal_processing()
{
#if TACTILE_OS_WINDOWS
  auto enable_virtual_terminal_processing_for = [](const DWORD std_handle_id) {
    HANDLE handle = GetStdHandle(std_handle_id);
    if (handle != INVALID_HANDLE_VALUE) {
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

  enable_virtual_terminal_processing_for(STD_OUTPUT_HANDLE);
  enable_virtual_terminal_processing_for(STD_ERROR_HANDLE);
#endif
}

}  // namespace tactile
