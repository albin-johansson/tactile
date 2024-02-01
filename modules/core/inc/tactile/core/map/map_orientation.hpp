// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

/**
 * Represents different map presentation styles.
 */
enum class MapOrientation : int8 {
  kOrthogonal,
  kIsometric,
  kHexagonal
};

}  // namespace tactile::core
