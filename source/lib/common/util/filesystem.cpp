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
#include <concepts>   // same_as

#include "common/debug/assert.hpp"
#include "common/predef.hpp"
#include "common/util/env.hpp"

#if TACTILE_OS_WINDOWS

#include <windows.h>

#endif  // TACTILE_OS_WINDOWS

namespace tactile {
namespace {

[[nodiscard]] auto get_home_prefix() -> const OsString&
{
  // On Unix platforms, HOME is something like '/Users/username'
  // On Windows, USERPROFILE is something like 'C:\Users\username'
  static const auto home_env = env_var(on_windows ? "USERPROFILE" : "HOME").value();
  static const auto home_str = make_native_string(home_env.c_str()).value();
  return home_str;
}

}  // namespace

auto use_forward_slashes(const Path& path) -> String
{
  auto str = path.string();
  std::replace(str.begin(), str.end(), '\\', '/');
  return str;
}

auto has_home_prefix(const Path& path) -> bool
{
  const auto& prefix = get_home_prefix();
  const OsStringView view {path.c_str()};
  return view.starts_with(prefix);
}

auto use_short_home_prefix(const Path& path) -> Maybe<String>
{
  if (has_home_prefix(path)) {
    const auto& prefix = get_home_prefix();
    return '~' + path.string().substr(prefix.size());
  }
  else {
    return nothing;
  }
}

auto make_native_string(const char* str) -> Maybe<OsString>
{
  if (!str) {
    return nothing;
  }

#if TACTILE_OS_WINDOWS
  // Windows is the only platform that we support that uses wchar_t filesystem paths
  static_assert(std::same_as<OsStrChar, wchar_t>);

  // Figure out the required size of the converted string
  const auto wide_char_count = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);

  // Create string of appropriate size
  OsString wide_str;
  wide_str.resize(wide_char_count);

  // Finally convert the input string
  if (MultiByteToWideChar(CP_UTF8, 0, str, -1, wide_str.data(), wide_char_count) != 0) {
    // The conversion was successful, but the string features an additional
    // null-terminator which we need to get rid of.
    TACTILE_ASSERT(!wide_str.empty());
    TACTILE_ASSERT(wide_str.back() == '\0');
    wide_str.pop_back();
    return wide_str;
  }
  else {
    return nothing;
  }
#else
  return OsString {str};
#endif  // TACTILE_OS_WINDOWS
}

}  // namespace tactile