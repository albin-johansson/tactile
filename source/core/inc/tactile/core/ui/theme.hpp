// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>  // uint8_t

#include "tactile/base/prelude.hpp"

namespace tactile::ui {

/**
 * Represents the available editor themes.
 */
enum class Theme : std::uint8_t
{
  kDearImGuiLight,
  kDearImGuiDark
};

}  // namespace tactile::ui
