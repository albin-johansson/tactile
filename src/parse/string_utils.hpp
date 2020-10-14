#pragma once

#include <QString>
#include <concepts>  // constructible_from

#include "tactile_error.hpp"

namespace tactile {

template <std::constructible_from<int> T = int>
[[nodiscard]] inline auto to_int(const QString& string) -> T
{
  bool ok{};
  if (const auto result = string.toInt(&ok); ok) {
    return T{result};
  } else {
    throw tactile_error{"Failed to convert string to int!"};
  }
}

}  // namespace tactile
