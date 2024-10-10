// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>  // uint8_t

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Represents known compression formats.
 */
enum class CompressionFormatId : std::uint8_t
{
  kZlib,
  kZstd
};

}  // namespace tactile
