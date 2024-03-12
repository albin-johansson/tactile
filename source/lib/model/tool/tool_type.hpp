// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

namespace tactile {

/// Represents the various available mouse tool types.
enum class ToolType {
  None,  /// Used when no tool is enabled.
  Stamp,
  Eraser,
  Bucket,
  ObjectSelection,
  Rectangle,
  Ellipse,
  Point
};

}  // namespace tactile
