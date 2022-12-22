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

#include "filesystem.hpp"

#include <algorithm>  // replace

#include "core/predef.hpp"
#include "core/util/env.hpp"

#if TACTILE_OS_WINDOWS

#include <codecvt>  // codecvt_utf8
#include <locale>   // wstring_convert

#endif  // TACTILE_OS_WINDOWS

namespace tactile {
namespace {

[[nodiscard]] auto get_home_prefix() -> const OsString&
{
  // On Unix platforms, HOME is something like '/Users/username'
  // On Windows, USERPROFILE is something like 'C:\Users\username'
  static const auto home_env = env_var(on_windows ? "USERPROFILE" : "HOME").value();
  static const auto home_str = to_os_string(home_env.c_str()).value();
  return home_str;
}

}  // namespace

auto convert_to_forward_slashes(const Path& path) -> String
{
  auto str = path.string();
  std::replace(str.begin(), str.end(), '\\', '/');
  return str;
}

auto has_home_prefix(const Path& path) -> bool
{
  const auto& prefix = get_home_prefix();
  OsStringView view {path.c_str()};
  return view.starts_with(prefix);
}

auto to_path(StringView str) -> Path
{
  return {str};
}

auto to_canonical(const Path& path) -> Maybe<String>
{
  if (has_home_prefix(path)) {
    const auto& prefix = get_home_prefix();
    return '~' + path.string().substr(prefix.size());
  }
  else {
    return nothing;
  }
}

auto to_os_string(const char* str) -> Maybe<OsString>
{
  if (!str) {
    return nothing;
  }

  // Windows is the only platform that we support that uses wchar_t filesystem paths
#if TACTILE_OS_WINDOWS
  using convert_type = std::codecvt_utf8<wchar_t>;
  std::wstring_convert<convert_type, wchar_t> converter;
  return converter.from_bytes(str);
#else
  return str;
#endif  // TACTILE_OS_WINDOWS
}

}  // namespace tactile