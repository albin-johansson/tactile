// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief Represents the supported layer types.
 */
enum class LayerType : uint8 {
  kTileLayer,
  kObjectLayer,
  kGroupLayer
};

}  // namespace tactile
