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

#include "tactile/core/platform/dynamic_library.hpp"

#if TACTILE_OS_LINUX || TACTILE_OS_MACOS
  #include <dlfcn.h>
#endif  // TACTILE_OS_LINUX || TACTILE_OS_MACOS

#if TACTILE_OS_WINDOWS
  #include <windows.h>
#endif  // TACTILE_OS_WINDOWS

namespace tactile {
namespace {

#if TACTILE_OS_LINUX || TACTILE_OS_MACOS

class UnixLib final : public IDynamicLibrary {
 public:
  TACTILE_DELETE_COPY(UnixLib);
  TACTILE_DELETE_MOVE(UnixLib);

  explicit UnixLib(const char* path)
      : mHandle {dlopen(path, RTLD_LAZY)}
  {
  }

  ~UnixLib() noexcept override
  {
    if (mHandle) {
      dlclose(mHandle);
      mHandle = nullptr;
    }
  }

  [[nodiscard]] auto get_symbol(const char* name) const -> void* override
  {
    return dlsym(mHandle, name);
  }

  [[nodiscard]] auto is_valid() const -> bool { return mHandle != nullptr; }

 private:
  void* mHandle {};
};

#endif  // TACTILE_OS_LINUX || TACTILE_OS_MACOS

#if TACTILE_OS_WINDOWS

class WindowsLib final : public IDynamicLibrary {
 public:
  TACTILE_DELETE_COPY(WindowsLib);
  TACTILE_DELETE_MOVE(WindowsLib);

  explicit WindowsLib(const char* path)
      : mHandle {LoadLibraryA(path)}
  {
  }

  ~WindowsLib() noexcept override
  {
    if (mHandle) {
      FreeLibrary(mHandle);
      mHandle = nullptr;
    }
  }

  [[nodiscard]] auto get_symbol(const char* name) const -> void* override
  {
    return static_cast<void*>(GetProcAddress(mHandle, name));
  }

  [[nodiscard]] auto is_valid() const -> bool { return mHandle != NULL; }

 private:
  HMODULE mHandle;
};

#endif  // TACTILE_OS_WINDOWS

}  // namespace

auto load_dynamic_library(const char* path) -> UniquePtr<IDynamicLibrary>
{
#if TACTILE_OS_LINUX || TACTILE_OS_MACOS
  auto library = make_unique<UnixLib>(path);
  if (library->is_valid()) {
    return library;
  }
#elif TACTILE_OS_WINDOWS
  auto library = make_unique<WindowsLib>(path);
  if (library->is_valid()) {
    return library;
  }
#endif  // TACTILE_OS_LINUX || TACTILE_OS_MACOS

  return nullptr;
}

}  // namespace tactile