// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/util/string_ops.hpp"

#include <algorithm>  // find_if
#include <cctype>     // isspace
#include <locale>     // locale

namespace tactile::core {

auto trim_string(const std::string_view str) -> std::string
{
  std::string copy {str};

  const auto find_first_non_space = [](const auto begin, const auto end) {
    return std::find_if(begin, end, [](const char ch) {
      return !std::isspace(ch, std::locale::classic());
    });
  };

  const auto left_iter = find_first_non_space(copy.begin(), copy.end());
  if (left_iter != copy.end()) {
    copy.erase(copy.begin(), left_iter);
  }

  const auto right_iter = find_first_non_space(copy.rbegin(), copy.rend());
  if (right_iter != copy.rend()) {
    copy.erase(right_iter.base(), copy.end());
  }

  return copy;
}

}  // namespace tactile::core
