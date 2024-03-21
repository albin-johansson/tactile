// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Represents the supported layer types.
 */
enum class LayerType : uint8 {
  kTileLayer,
  kObjectLayer,
  kGroupLayer
};

}  // namespace tactile
