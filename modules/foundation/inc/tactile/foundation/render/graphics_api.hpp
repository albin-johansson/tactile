// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Represents the possible graphics APIs.
 */
enum class GraphicsAPI : int8 {
  kUnknown,
  kOpenGL,
  kVulkan
};

}  // namespace tactile
