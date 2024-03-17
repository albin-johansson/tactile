// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/platform/filesystem.hpp"

#include <algorithm>  // replace

#include <SDL2/SDL.h>

#include "tactile/core/debug/generic_error.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/platform/environment.hpp"
#include "tactile/core/util/string_ops.hpp"

namespace tactile {

auto get_persistent_storage_directory() -> Result<Path>
{
  if (const auto* path = SDL_GetPrefPath("albin-johansson", "tactile")) {
    return Path {path};
  }

  TACTILE_LOG_ERROR("Could not determine persistent storage directory: {}",
                    SDL_GetError());
  return unexpected(make_error(GenericError::kInvalidState));
}

auto get_user_home_directory() -> Result<NativeString>
{
  // On Unix platforms, HOME is something like '/Users/username'.
  // On Windows, USERPROFILE is something like 'C:\Users\username'.
  const auto* var_name = kOnWindows ? "USERPROFILE" : "HOME";
  return get_env(var_name).and_then([](const String& home_dir) {  //
    return make_native_string(home_dir.c_str());
  });
}

auto normalize_path(const Path& path) -> String
{
  auto str = path.string();
  std::ranges::replace(str, '\\', '/');
  return str;
}

auto strip_home_directory_prefix(const Path& path, const NativeString& home_dir)
    -> Result<NativeString>
{
  if (NativeStringView {path.c_str()}.starts_with(home_dir)) {
    const NativeStringView path_view {path.c_str()};
    const auto path_without_home_dir = path_view.substr(home_dir.size());

    NativeString str {};
    str.reserve(1 + path_without_home_dir.size());
    str.push_back('~');
    str.insert_range(str.end(), path_without_home_dir);

    return str;
  }

  return unexpected(make_error(GenericError::kInvalidParam));
}

}  // namespace tactile
