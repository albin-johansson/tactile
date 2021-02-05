#pragma once

#include <QString>  // QString

#include "maybe.hpp"

namespace tactile {

[[nodiscard]] auto to_integer(const QString& string) -> maybe<int>;

}  // namespace tactile
