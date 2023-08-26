// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/type/string.hpp"

#include <charconv>      // from_chars
#include <concepts>      // integral, floating_point
#include <sstream>       // stringstream
#include <string>        // getline, erase
#include <system_error>  // errc
#include <utility>       // move

#include <fast_float/fast_float.h>

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
