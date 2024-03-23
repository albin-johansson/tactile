// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Represents the supported tile layer data encodings.
 */
enum class TileEncoding : uint8
{
  kPlainText,
  kBase64,
};

}  // namespace tactile
