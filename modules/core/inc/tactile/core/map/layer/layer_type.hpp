// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/prelude.hpp"

namespace tactile {

/** Represents all of the supported layer types. */
enum class LayerType : uint32 {
  TileLayer,
  ObjectLayer,
  GroupLayer
};

}  // namespace tactile
