// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>  // uint8_t

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Represents the supported layer object types.
 */
enum class ObjectType : std::uint8_t
{
  kPoint,
  kRect,
  kEllipse,
};

}  // namespace tactile
