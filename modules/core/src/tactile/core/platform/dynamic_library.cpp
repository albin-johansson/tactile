// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/platform/dynamic_library.hpp"

#if TACTILE_OS_LINUX || TACTILE_OS_APPLE
  #include <dlfcn.h>
#endif

#if TACTILE_OS_WINDOWS
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
#endif

namespace tactile {

#if TACTILE_OS_LINUX || TACTILE_OS_APPLE

class UnixDynamicLibrary final : public IDynamicLibrary {
 public:
  TACTILE_DELETE_COPY(UnixDynamicLibrary);
  TACTILE_DELETE_MOVE(UnixDynamicLibrary);

  [[nodiscard]] static auto load(const char* path) -> Unique<UnixDynamicLibrary>
  {
    if (void* handle = dlopen(path, RTLD_LAZY)) {
      return Unique<UnixDynamicLibrary> {new UnixDynamicLibrary {handle}};
    }

    return nullptr;
  }

  ~UnixDynamicLibrary() noexcept override
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

 private:
  void* mHandle;

  explicit UnixDynamicLibrary(void* handle) noexcept
    : mHandle {handle}
  {}
};

#endif  // TACTILE_OS_LINUX || TACTILE_OS_APPLE

#if TACTILE_OS_WINDOWS

class Win32DynamicLibrary final : public IDynamicLibrary {
 public:
  TACTILE_DELETE_COPY(Win32DynamicLibrary);
  TACTILE_DELETE_MOVE(Win32DynamicLibrary);

  [[nodiscard]] static auto load(const char* path) -> Unique<Win32DynamicLibrary>
  {
    if (const auto handle = LoadLibraryA(path)) {
      return Unique<Win32DynamicLibrary> {new Win32DynamicLibrary {handle}};
    }

    return nullptr;
  }

  ~Win32DynamicLibrary() noexcept override
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

 private:
  HMODULE mHandle;

  explicit Win32DynamicLibrary(HMODULE handle) noexcept
    : mHandle {handle}
  {}
};

#endif  // TACTILE_OS_WINDOWS

auto load_library(const fs::Path& library_path) -> Unique<IDynamicLibrary>
{
  Unique<IDynamicLibrary> library;

#if TACTILE_OS_LINUX || TACTILE_OS_APPLE
  library = UnixDynamicLibrary::load(library_path.c_str());
#elif TACTILE_OS_WINDOWS
  library = Win32DynamicLibrary::load(library_path.string().c_str());
#endif

  return library;
}

}  // namespace tactile
