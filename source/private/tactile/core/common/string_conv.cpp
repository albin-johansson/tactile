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

#include "tactile/core/common/string_conv.hpp"

#include <charconv>      // from_chars
#include <concepts>      // integral
#include <sstream>       // stringstream
#include <system_error>  // errc

namespace tactile {
namespace {

template <std::integral T>
[[nodiscard]] auto _str_to_int(std::string_view str, const int base) -> std::optional<T>
{
  T value {};
  const auto [ptr, err] =
      std::from_chars(str.data(), str.data() + str.size(), value, base);

  if (err == std::errc {}) {
    return value;
  }

  return {};
}

}  // namespace

auto str_to_i32(std::string_view str, const int base) -> std::optional<int32>
{
  return _str_to_int<int32>(str, base);
}

auto str_to_u32(std::string_view str, const int base) -> std::optional<uint32>
{
  return _str_to_int<uint32>(str, base);
}

auto str_to_f32(std::string_view str) -> std::optional<float32>
{
  std::stringstream stream;
  stream << str;

  float32 f {};
  stream >> f;

  if (!stream.fail()) {
    return f;
  }

  return {};
}

}  // namespace tactile
