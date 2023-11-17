// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/math/vector.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief Represents a rectangle with integer accuracy.
 */
struct TACTILE_CORE_API Rectangle final {
  Int2 position {};
  Int2 size {};

  [[nodiscard]] constexpr auto operator==(const Rectangle&) const -> bool = default;
};

/**
 * \brief Represents a rectangle with floating-point accuracy.
 */
struct TACTILE_CORE_API FRectangle final {
  Float2 position {};
  Float2 size {};

  [[nodiscard]] constexpr auto operator==(const FRectangle&) const -> bool = default;
};

}  // namespace tactile
