// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/util/string_ops.hpp"

#include <algorithm>  // find_if
#include <cctype>     // isspace
#include <locale>     // locale

namespace tactile {

auto trim_string(String str) -> String
{
  const auto find_first_non_space = [](const auto begin, const auto end) {
    return std::find_if(begin, end, [](const char ch) {
      return !std::isspace(ch, std::locale::classic());
    });
  };

  const auto left_iter = find_first_non_space(str.begin(), str.end());
  if (left_iter != str.end()) {
    str.erase(str.begin(), left_iter);
  }

  const auto right_iter = find_first_non_space(str.rbegin(), str.rend());
  if (right_iter != str.rend()) {
    str.erase(right_iter.base(), str.end());
  }

  return str;
}

}  // namespace tactile
