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

#pragma once

#include <algorithm>     // min
#include <charconv>      // from_chars
#include <concepts>      // integral, floating_point
#include <cstring>       // strlen
#include <string>        // string, stof
#include <system_error>  // errc
#include <vector>        // vector

#include "core/common/ints.hpp"
#include "core/common/maybe.hpp"

namespace tactile {

[[nodiscard]] auto split(const char* str, char sep) -> std::vector<std::string>;

/**
 * \brief Converts a string into an integer.
 *
 * \details This function is particularly useful for converting substrings into integers.
 *
 * \param str the string that will be converted to an integer, can safely be null.
 * \param length the length of the section of the string that will be processed.
 * \param base the base used by the parsed integer.
 *
 * \return an integer using the specified base;
 *         an empty optional is returned upon failure.
 */
template <std::integral T>
[[nodiscard]] auto from_string(const char* str, const usize length, const int base)
    -> Maybe<T>
{
  if (!str) {
    return nothing;
  }

  T value{};

  const auto* begin = str;
  const auto* end = str + (std::min)(length, std::strlen(str));
  const auto [ptr, err] = std::from_chars(begin, end, value, base);

  if (err == std::errc{}) {
    return value;
  }
  else {
    return nothing;
  }
}

/**
 * \brief Converts a full string into an integer.
 *
 * \param str the string that will be converted to an integer, can safely be null.
 * \param base the base used by the parsed integer.
 *
 * \return an integer using the specified base;
 *         an empty optional is returned upon failure.
 */
template <std::integral T>
[[nodiscard]] auto from_string(const char* str, const int base = 10) -> Maybe<T>
{
  if (!str) {
    return nothing;
  }
  else {
    return from_string<T>(str, std::strlen(str), base);
  }
}

/**
 * \brief Converts a string into a floating-point value.
 *
 * \param str the string that will be converted to a float, can safely be null.
 *
 * \return the corresponding floating-point value;
 *         an empty optional is returned upon failure.
 */
template <std::floating_point T>
[[nodiscard]] auto from_string(const char* str) -> Maybe<T>
{
  if (!str) {
    return nothing;
  }

  /* We only do this check in the floating-point overload, because it's the only version
     that isn't widely supported (at the time of writing, only MSVC provides it) */
#if __cpp_lib_to_chars >= 201611L
  T value{};

  const auto [ptr, err] = std::from_chars(str, str + std::strlen(str), value);
  if (err == std::errc{}) {
    return value;
  }
  else {
    return nothing;
  }
#else
  try {
    return static_cast<T>(std::stof(str));
  }
  catch (...) {
    return nothing;
  }
#endif  // __cpp_lib_to_chars >= 201611L
}

}  // namespace tactile
