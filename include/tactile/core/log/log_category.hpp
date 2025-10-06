// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include "tactile/core/primitives.hpp"

namespace tactile {

/// Represents supported log message categories.
enum class LogCategory : uint8
{
  kVerbose,
  kDebug,
  kInfo,
  kWarn,
  kError,
};

}  // namespace tactile
