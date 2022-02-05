#pragma once

#include "tactile.hpp"

namespace tactile {

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
auto Button(c_str text,
            c_str tooltip = nullptr,
            bool enabled = true,
            float width = 0,
            float height = 0) -> bool;

}  // namespace tactile
