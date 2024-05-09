// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

/// \addtogroup Model
/// \{

/**
 * Represents editor settings that the user can control.
 */
struct Settings final
{
  float font_size;
};

/**
 * Returns the settings used by default.
 *
 * \return
 * The default settings.
 */
[[nodiscard]]
auto get_default_settings() -> Settings;

/// \}

}  // namespace tactile
