// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::ui {

/**
 * Shows a simple button.
 *
 * \param label   The button text.
 * \param tooltip The button tooltip text.
 * \param enabled Whether the button may be engaged.
 * \param width   The button width.
 * \param height  The button height.
 *
 * \return
 *    True if the button was pressed; false otherwise.
 */
TACTILE_CORE_API auto show_button(const char* label,
                                  const char* tooltip = nullptr,
                                  bool enabled = true,
                                  float width = 0,
                                  float height = 0) -> bool;

/**
 * Shows a simple button that is horizontally centered.
 *
 * \param label   The button text.
 * \param tooltip The button tooltip text.
 *
 * \return
 *    True if the button was pressed; false otherwise.
 */
TACTILE_CORE_API auto show_centered_button(const char* label,
                                           const char* tooltip = nullptr) -> bool;

}  // namespace tactile::ui
