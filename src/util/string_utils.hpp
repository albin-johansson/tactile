#pragma once

#include <QString>  // QString

#include "maybe.hpp"

namespace tactile {

[[nodiscard]] auto ToInteger(const QString& string) -> maybe<int>;

}  // namespace tactile
