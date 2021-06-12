#pragma once

#include <QString>  // QString

#include "maybe.hpp"

namespace tactile {

[[nodiscard]] auto ToInteger(const QString& string) -> Maybe<int>;

}  // namespace tactile
