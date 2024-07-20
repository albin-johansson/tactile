// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Provides identifiers for common save format parse errors.
 */
enum class SaveFormatParseError : uint8
{
  kNoPropertyName,
  kNoPropertyType,
  kNoPropertyValue,
  kBadPropertyType,
  kBadPropertyValue,
};

}  // namespace tactile
