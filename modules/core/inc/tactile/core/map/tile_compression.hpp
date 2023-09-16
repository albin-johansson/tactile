// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief The supported tile layer data compression strategies.
 */
enum class TileCompression : uint8 {
  kNone,  ///< Apply no compression to tile layer data.
  kZlib,  ///< Use Zlib compression.
  kZstd   ///< Use Zstd compression.
};

}  // namespace tactile
