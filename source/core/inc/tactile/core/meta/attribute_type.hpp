// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/int.hpp"

namespace tactile {

/** The supported attribute value types. */
enum class AttributeType : uint8 {
  kStr,
  kInt,
  kInt2,
  kInt3,
  kInt4,
  kFloat,
  kFloat2,
  kFloat3,
  kFloat4,
  kBool,
  kPath,
  kColor,
  kObject,
};

}  // namespace tactile
