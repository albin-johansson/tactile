// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/int.hpp"

namespace tactile {

/**
 * Represents different compression modes.
 */
enum class CompressionType : uint8 {
  kNone,
  kZlib,
  kZstd
};

}  // namespace tactile
