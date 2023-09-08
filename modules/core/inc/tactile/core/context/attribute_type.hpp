// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/prelude.hpp"

namespace tactile {

/** The supported property value types. */
enum class AttributeType : uint8 {
  Str,
  Int,
  Int2,
  Int3,
  Int4,
  Float,
  Float2,
  Float3,
  Float4,
  Bool,
  Path,
  Color,
  Object,
};

}  // namespace tactile
