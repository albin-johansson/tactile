// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/ui/common/icons.hpp"

namespace tactile::ui {

/// \addtogroup UI
/// \{

/**
 * Adds a button to the widget stack.
 *
 * \param label   The text label to use.
 * \param tooltip An optional tooltip.
 * \param enabled Controls whether the button is pressable.
 * \param width   The width of the button. Pass \c 0 to use automatic width.
 * \param height  The height of the button. Pass \c 0 to use automatic height.
 *
 * \return
 * True if the button was pressed; false otherwise.
 */
[[nodiscard]]
auto push_button(const char* label,
                 const char* tooltip = nullptr,
                 bool enabled = true,
                 float width = 0.0f,
                 float height = 0.0f) -> bool;

/**
 * Adds an icon button to the widget stack.
 *
 * \param icon    The icon to use.
 * \param tooltip An optional tooltip.
 * \param enabled Controls whether the button is pressable.
 *
 * \return
 * True if the button was pressed; false otherwise.
 */
[[nodiscard]]
auto push_icon_button(Icon icon,
                      const char* tooltip = nullptr,
                      bool enabled = true) -> bool;

/// \}

}  // namespace tactile::ui
