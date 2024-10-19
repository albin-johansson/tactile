// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <algorithm>    // find_if
#include <cctype>       // isspace
#include <locale>       // locale
#include <string>       // string
#include <string_view>  // string_view

#include "tactile/base/util/concepts.hpp"

namespace tactile {

/**
 * Removes leading and trailing spaces from a given string.
 *
 * \note
 * Only strings with at least one non-space character are modified.
 *
 * \param str The string to trim.
 *
 * \return
 * A trimmed string.
 */
[[nodiscard]]
constexpr auto trim_string(const std::string_view str) -> std::string
{
  std::string trimmed {str};

  const auto find_first_non_space = [](const auto begin, const auto end) {
    return std::find_if(begin, end, [](const char ch) {
      return !std::isspace(ch, std::locale::classic());
    });
  };

  const auto left = find_first_non_space(trimmed.begin(), trimmed.end());
  if (left != trimmed.end()) {
    trimmed.erase(trimmed.begin(), left);
  }

  const auto right = find_first_non_space(trimmed.rbegin(), trimmed.rend());
  if (right != trimmed.rend()) {
    trimmed.erase(right.base(), trimmed.end());
  }

  return trimmed;
}

/**
 * Splits a string into a collection of tokens separated by a given character.
 *
 * \tparam T A function object type.
 *
 * \param str       The full string.
 * \param separator The character to use as the separator.
 * \param callback  A callback invoked for each token in the string. The function will return
 *                  immediately if this callback returns false.
 *
 * \return
 * True if the function completed without returning early; false otherwise.
 */
template <InvocableType<bool, std::string_view> T>
constexpr auto visit_tokens(const std::string_view str,
                            const char separator,
                            const T& callback) -> bool
{
  std::size_t pos = 0;
  const auto str_length = str.size();

  while (pos < str_length) {
    const auto separator_pos = str.find_first_of(separator, pos);

    if (separator_pos == std::string_view::npos) {
      const auto token = str.substr(pos);

      if (!callback(token)) {
        return false;
      }

      break;
    }

    const auto token_length = separator_pos - pos;
    const auto token = str.substr(pos, token_length);

    if (!callback(token)) {
      return false;
    }

    pos += token.size() + 1;
  }

  return true;
}

}  // namespace tactile
