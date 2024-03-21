// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

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
