// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/concepts.hpp"

namespace tactile {

/**
 * Creates a string using the native filesystem character type.
 *
 * \param str The source string, may be null.
 *
 * \return
 *    A corresponding native string; or an error code if something went wrong.
 */
[[nodiscard]]
auto make_native_string(const char* str) -> Result<NativeString>;

/**
 * Removes leading and trailing spaces from a given string.
 *
 * \note
 *    This function will only modify strings that have at least
 *    one non-space character.
 *
 * \param str The string to trim.
 *
 * \return
 *    A trimmed string.
 */
[[nodiscard]]
auto trim_string(String str) -> String;

/**
 * Splits a string into a collection of tokens separated by a given character.
 *
 * \tparam T A function object type.
 *
 * \param str       The full string.
 * \param separator The character to use as the separator.
 * \param callback  A callback invoked for each token in the string. The function will
 *                  return immediately if the callback returns false.
 *
 * \return True if the function completed without returning early; false otherwise.
 */
template <InvocableType<bool, StringView> T>
auto split_string(const StringView str, const char separator, const T& callback) -> bool
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
