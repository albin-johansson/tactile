// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#include "tactile/editor/plugin/dynamic_library.hpp"

#include <SDL3/SDL.h>

#include "tactile/core/misc/defer.hpp"

namespace tactile::editor {

class DynamicLibrary::Impl final
{
 public:
  TACTILE_DELETE_COPY(Impl);
  TACTILE_DELETE_MOVE(Impl);

  explicit Impl(SDL_SharedObject* so_handle) noexcept
    : m_so_handle {so_handle},
      m_unload_so {[so_handle]() noexcept { SDL_UnloadObject(so_handle); }}
  {}

  ~Impl() noexcept = default;

  [[nodiscard]]
  auto find_function(const char* name) const -> void*
  {
    // NOLINTNEXTLINE(*-reinterpret-cast)
    return reinterpret_cast<void*>(SDL_LoadFunction(m_so_handle, name));
  }

 private:
  SDL_SharedObject* m_so_handle;
  Defer m_unload_so;
};

TACTILE_DEFINE_PIMPL_CLASS(DynamicLibrary);

DynamicLibrary::DynamicLibrary() = default;

auto DynamicLibrary::open(const char* path) -> Result<DynamicLibrary>
{
  auto* so_handle = SDL_LoadObject(path);
  if (so_handle == nullptr) {
    return error(Errc::kBadFile);
  }

  DynamicLibrary dylib {};
  dylib.m_impl = make_unique<Impl>(so_handle);

  return dylib;
}

auto DynamicLibrary::find_function(const char* name) const -> void*
{
  return m_impl->find_function(name);
}

}  // namespace tactile::editor
