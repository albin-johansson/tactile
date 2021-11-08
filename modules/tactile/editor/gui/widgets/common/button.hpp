#pragma once

#include <tactile_def.hpp>

namespace Tactile {

/**
 * \brief Shows a button.
 *
 * \ingroup gui
 *
 * \param text the button text.
 * \param tooltip the tooltip text, can safely be null.
 * \param enabled `true` to show an enabled button; `false` otherwise.
 *
 * \return `true` if the button was pressed; `false` otherwise.
 */
auto Button(NotNull<CStr> text,
            CStr tooltip = nullptr,
            bool enabled = true,
            float width = 0,
            float height = 0) -> bool;

}  // namespace Tactile
