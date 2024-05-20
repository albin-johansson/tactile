// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/concepts.hpp"

namespace tactile {

/**
 * Removes leading and trailing spaces from a given string.
 *
 * \note
 * This function will only modify strings that have at least one non-space
 * character.
 *
 * \param str The string to trim.
 *
 * \return
 * A trimmed string.
 */
[[nodiscard]]
auto trim_string(StringView str) -> String;

/**
 * Splits a string into a collection of tokens separated by a given character.
 *
 * \tparam T A function object type.
 *
 * \param str       The full string.
 * \param separator The character to use as the separator.
 * \param callback  A callback invoked for each token in the string. The
 *                  function will return immediately if the callback returns
 *                  false.
 *
 * \return
 * True if the function completed without returning early; false otherwise.
 */
template <InvocableType<bool, StringView> T>
auto split_string(const StringView str,
                  const char separator,
                  const T& callback) -> bool
{
  usize pos = 0;
  const auto str_length = str.size();

  while (pos < str_length) {
    const auto separator_pos = str.find_first_of(separator, pos);

    if (separator_pos == StringView::npos) {
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
