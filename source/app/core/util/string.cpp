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

#include "string.hpp"

#include <charconv>      // from_chars
#include <concepts>      // integral
#include <sstream>       // stringstream
#include <system_error>  // errc
#include <utility>       // move

namespace tactile {
namespace {

template <std::integral T>
[[nodiscard]] auto parse(const char* begin, const char* end, const int base) -> Maybe<T>
{
  if (!begin || !end) {
    return nothing;
  }

  T value {};
  const auto [ptr, err] = std::from_chars(begin, end, value, base);

  if (err == std::errc {}) {
    return value;
  }
  else {
    return nothing;
  }
}

void foo() = delete;

}  // namespace

auto split(std::string_view str, const char sep) -> std::vector<std::string>
{
  std::stringstream stream;
  stream << str;

  std::vector<std::string> tokens;

  std::string token;
  while (std::getline(stream, token, sep)) {
    std::erase(token, '\n');
    tokens.push_back(std::move(token));
    token.clear();
  }

  return tokens;
}

auto parse_i32(std::string_view str, const int base) -> Maybe<int32>
{
  return parse<int32>(str.begin(), str.end(), base);
}

}  // namespace tactile