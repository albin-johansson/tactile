// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/filesystem.hpp"

#include <algorithm>  // replace

#include <SDL2/SDL.h>

#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/foundation/platform/environment.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto _get_user_home_directory() -> const NativeString&
{
  // On Unix platforms, HOME is something like '/Users/username'
  // On Windows, USERPROFILE is something like 'C:\Users\username'
  static const auto home_env = get_env(kIsWindows ? "USERPROFILE" : "HOME").value();
  static const auto home_str = make_native_string(home_env.c_str()).value();
  return home_str;
}

}  // namespace

auto get_persistent_storage_directory() -> FilePath
{
  if (const auto* path = SDL_GetPrefPath("albin-johansson", "tactile")) {
    return FilePath {path};
  }

  TACTILE_LOG_ERROR("Could not determine persistent storage directory: {}",
                    SDL_GetError());
  throw Exception {"Could not determine persistent storage directory"};
}

auto normalize_path(const FilePath& path) -> String
{
  auto str = path.string();
  std::replace(str.begin(), str.end(), '\\', '/');
  return str;
}

auto shorten_home_directory_prefix(const FilePath& path) -> Maybe<String>
{
  if (has_home_directory_prefix(path)) {
    const auto& home_dir = _get_user_home_directory();
    return '~' + path.string().substr(home_dir.size());
  }
  else {
    return kNothing;
  }
}

auto has_home_directory_prefix(const FilePath& path) -> bool
{
  const auto& home_dir = _get_user_home_directory();
  const NativeStringView view {path.c_str()};
  return view.starts_with(home_dir);
}

}  // namespace tactile