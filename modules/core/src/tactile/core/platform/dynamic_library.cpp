// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/platform/dynamic_library.hpp"

#include <utility>  // move

#include "tactile/core/debug/log/logger.hpp"

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

  [[nodiscard]] static auto load(FilePath path) -> Unique<UnixDynamicLibrary>
  {
    if (void* handle = dlopen(path.c_str(), RTLD_LAZY)) {
      // We can't use make_unique here because of the private constructor.
      return Unique<UnixDynamicLibrary> {
        new UnixDynamicLibrary {std::move(path), handle}};
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

  [[nodiscard]] auto get_path() const -> const FilePath& override
  {
    return mPath;
  }

 private:
  FilePath mPath;
  void* mHandle;

  UnixDynamicLibrary(FilePath path, void* handle)
    : mPath {std::move(path)},
      mHandle {handle}
  {}
};

#endif  // TACTILE_OS_LINUX || TACTILE_OS_APPLE

#if TACTILE_OS_WINDOWS

class Win32DynamicLibrary final : public IDynamicLibrary {
 public:
  TACTILE_DELETE_COPY(Win32DynamicLibrary);
  TACTILE_DELETE_MOVE(Win32DynamicLibrary);

  [[nodiscard]] static auto load(FilePath path) -> Unique<Win32DynamicLibrary>
  {
    if (const auto handle = LoadLibraryA(path.c_str())) {
      // We can't use make_unique here because of the private constructor.
      return Unique<Win32DynamicLibrary> {
        new Win32DynamicLibrary {std::move(path), handle}};
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

  [[nodiscard]] auto get_path() const -> const FilePath& override
  {
    return mPath;
  }

 private:
  FilePath mPath;
  HMODULE mHandle;

  explicit Win32DynamicLibrary(FilePath path, HMODULE handle)
    : mPath {std::move(path)},
      mHandle {handle}
  {}
};

#endif  // TACTILE_OS_WINDOWS

auto is_dll(const FilePath& path) -> bool
{
  const auto extension = path.extension();

  if constexpr (kIsLinux) {
    return extension == ".so";
  }
  else if constexpr (kIsApple) {
    return extension == ".so" || extension == ".dylib";
  }
  else if constexpr (kIsWindows) {
    return extension == ".dll";
  }

  TACTILE_LOG_WARN("Unknown platform, cannot determine valid DLL extensions");
  return false;
}

auto load_library(const FilePath& path) -> Unique<IDynamicLibrary>
{
  Unique<IDynamicLibrary> library;

  if (is_dll(path)) {
#if TACTILE_OS_LINUX || TACTILE_OS_APPLE
    library = UnixDynamicLibrary::load(path);
#elif TACTILE_OS_WINDOWS
    library = Win32DynamicLibrary::load(path);
#endif
  }

  return library;
}

}  // namespace tactile
