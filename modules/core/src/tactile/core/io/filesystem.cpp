// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/io/filesystem.hpp"

#include <algorithm>  // replace

#include "tactile/core/platform/environment.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile::fs {
namespace {

[[nodiscard]] auto _get_home_prefix() -> const NativeString&
{
  // On Unix platforms, HOME is something like '/Users/username'
  // On Windows, USERPROFILE is something like 'C:\Users\username'
  static const auto home_env = get_env(kIsWindows ? "USERPROFILE" : "HOME").value();
  static const auto home_str = make_native_string(home_env.c_str()).value();
  return home_str;
}

}  // namespace

auto use_forward_slashes(const Path& path) -> String
{
  auto str = path.string();
  std::ranges::replace(str, '\\', '/');
  return str;
}

auto use_short_home_prefix(const Path& path) -> Maybe<String>
{
  if (has_home_prefix(path)) {
    const auto& prefix = _get_home_prefix();
    return '~' + path.string().substr(prefix.size());
  }
  else {
    return kNone;
  }
}

auto has_home_prefix(const Path& path) -> bool
{
  const auto& prefix = _get_home_prefix();
  const NativeStringView view {path.c_str()};
  return view.starts_with(prefix);
}

}  // namespace tactile::fs