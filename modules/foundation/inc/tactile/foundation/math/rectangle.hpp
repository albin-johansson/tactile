// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/math/vector.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Represents a rectangle with integer accuracy.
 */
struct Rectangle final {
  Int2 position {};
  Int2 size {};

  [[nodiscard]] constexpr auto operator==(const Rectangle&) const -> bool = default;
};

/**
 * \brief Represents a rectangle with floating-point accuracy.
 */
struct FRectangle final {
  Float2 position {};
  Float2 size {};

  [[nodiscard]] constexpr auto operator==(const FRectangle&) const -> bool = default;
};

}  // namespace tactile
