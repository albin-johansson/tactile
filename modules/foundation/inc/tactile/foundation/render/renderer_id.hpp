// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Provides identifiers for renderer backends.
 */
enum class RendererID : int8 {
  kUnknown,
  kOpenGL,
  kVulkan
};

}  // namespace tactile
