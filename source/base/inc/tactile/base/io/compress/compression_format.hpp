// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Represents known compression formats.
 */
enum class CompressionFormat : uint8
{
  kZlib,
  kZstd
};

}  // namespace tactile
