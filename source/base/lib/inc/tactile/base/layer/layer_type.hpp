// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>  // uint8_t

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Represents the supported layer types.
 */
enum class LayerType : std::uint8_t
{
  kTileLayer,
  kObjectLayer,
  kGroupLayer
};

}  // namespace tactile
