// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Represents different compression modes.
 */
enum class CompressionMode : uint8 {
  kNone,  ///< Use no compression.
  kZlib,  ///< Use Zlib compression.
  kZstd,  ///< Use Zstd compression.
};

using CompressionKind = CompressionMode;

}  // namespace tactile
