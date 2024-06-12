// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile::ui {

/**
 * Represents the available editor themes.
 */
enum class Theme : uint8
{
  kDearImGuiLight,
  kDearImGuiDark
};

}  // namespace tactile::ui
