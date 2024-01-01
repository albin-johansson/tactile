// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/container/string.hpp"

#include <concepts>  // same_as
#include <sstream>   // stringstream
#include <string>    // getline, erase
#include <utility>   // move

#if TACTILE_OS_WINDOWS
  #include <windows.h>
#endif

#include "tactile/foundation/debug/assert.hpp"

namespace tactile {

auto make_native_string(const char* str) -> Maybe<NativeString>
{
  if (!str) {
    return kNone;
  }

#if TACTILE_OS_WINDOWS
  // Windows is the only platform that we support that uses wchar_t filesystem paths
  static_assert(std::same_as<NativeChar, wchar_t>);

  // Figure out the required size of the converted string
  const auto wide_char_count = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);

  // Create string of appropriate size
  NativeString wide_str {};
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
    return kNone;
  }
#else
  return NativeString {str};
#endif  // TACTILE_OS_WINDOWS
}

auto str_split(const StringView str, const char separator) -> Vector<String>
{
  std::stringstream stream {};
  stream << str;

  Vector<String> tokens {};

  String token {};
  while (std::getline(stream, token, separator)) {
    std::erase(token, '\n');

    tokens.push_back(std::move(token));
    token.clear();
  }

  return tokens;
}

}  // namespace tactile
