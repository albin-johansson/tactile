// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief The supported tile layer data encoding strategies.
 */
enum class TileEncoding : uint8 {
  kPlainText,  ///< Encode tile layer data in ordinary plain text.
  kBase64      ///< Encode tile layer data using Base64.
};

}  // namespace tactile
