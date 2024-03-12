// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <ostream>

namespace tactile {

/// Represents the available map object types.
enum class ObjectType {
  Point,
  Rect,
  Ellipse
};

/// Outputs an object type to a stream for debugging purposes.
auto operator<<(std::ostream& stream, ObjectType type) -> std::ostream&;

}  // namespace tactile
