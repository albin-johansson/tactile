// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/type/string.hpp"

#include <charconv>      // from_chars
#include <concepts>      // integral, floating_point, same_as
#include <sstream>       // stringstream
#include <string>        // getline, erase
#include <system_error>  // errc
#include <utility>       // move

#include <fast_float/fast_float.h>

#include "tactile/core/debug/assert.hpp"

namespace tactile {
namespace {

template <std::integral T>
[[nodiscard]] auto _parse_integral(StringView str, const int base) -> Maybe<T>
{
  T value {};
  const auto [ptr, err] = std::from_chars(str.begin(), str.end(), value, base);

  // Require no error and fully valid string, e.g., "42 " is not accepted.
  if (err == std::errc {} && ptr == str.end()) {
    return value;
  }
  else {
    return kNone;
  }
}

template <std::floating_point T>
[[nodiscard]] auto _parse_float(StringView str) -> Maybe<T>
{
  T value {};
  const auto [ptr, err] = fast_float::from_chars(str.begin(), str.end(), value);

  // Require no error and fully valid string, e.g., "4.2 " is not accepted.
  if (err == std::errc {} && ptr == str.end()) {
    return value;
  }
  else {
    return kNone;
  }
}

}  // namespace

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
  NativeString wide_str;
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

auto str_split(StringView str, const char separator) -> Vector<String>
{
  std::stringstream stream;
  stream << str;

  Vector<String> tokens;

  String token;
  while (std::getline(stream, token, separator)) {
    std::erase(token, '\n');

    tokens.push_back(std::move(token));
    token.clear();
  }

  return tokens;
}

auto str_to_u32(StringView str, const int base) -> Maybe<uint32>
{
  return _parse_integral<uint32>(str, base);
}

auto str_to_i32(StringView str, const int base) -> Maybe<int32>
{
  return _parse_integral<int32>(str, base);
}

auto str_to_f32(StringView str) -> Maybe<float32>
{
  return _parse_float<float32>(str);
}

}  // namespace tactile
