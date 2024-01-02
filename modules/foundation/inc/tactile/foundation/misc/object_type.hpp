// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Represents the supported layer object types.
 */
enum class ObjectType : uint8 {
  kPoint,
  kRect,
  kEllipse,
};

}  // namespace tactile