/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "tactile/core/common/string_util.hpp"

#include <charconv>      // from_chars
#include <concepts>      // integral
#include <sstream>       // stringstream
#include <string>        // getline, erase
#include <system_error>  // errc
#include <utility>       // move

namespace tactile {
namespace {

template <std::integral T>
[[nodiscard]] auto _str_to_int(StringView str, const int base) -> Maybe<T>
{
  T value {};
  const auto [ptr, err] =
      std::from_chars(str.data(), str.data() + str.size(), value, base);

  if (err == std::errc {}) {
    return value;
  }

  return kNone;
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

auto str_to_i32(StringView str, const int base) -> Maybe<int32>
{
  return _str_to_int<int32>(str, base);
}

auto str_to_u32(StringView str, const int base) -> Maybe<uint32>
{
  return _str_to_int<uint32>(str, base);
}

auto str_to_f32(StringView str) -> Maybe<float32>
{
  std::stringstream stream;
  stream << str;

  float32 f {};
  stream >> f;

  if (!stream.fail()) {
    return f;
  }

  return kNone;
}

}  // namespace tactile
