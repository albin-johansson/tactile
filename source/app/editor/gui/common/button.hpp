#pragma once

#include "tactile.hpp"

namespace tactile {

/**
 * \brief Shows a button.
 *
 * \ingroup gui
 *
 * \param text the button text.
 * \param tooltip optional tooltip text.
 * \param enabled `true` to show an enabled button; `false` otherwise.
 * \param width the width of the button; `0` for automatic width.
 * \param height the height of the button; `0` for automatic height.
 *
 * \return `true` if the button was pressed; `false` otherwise.
 */
auto button(const char* text,
            const char* tooltip = nullptr,
            bool enabled = true,
            float width = 0,
            float height = 0) -> bool;

/**
 * \brief Shows a horizontally centered button.
 *
 * \ingroup gui
 *
 * \param label the label text.
 * \param tooltip optional tooltip text.
 *
 * \return `true` if the button was pressed; `false` otherwise.
 */
auto centered_button(const char* label, const char* tooltip = nullptr) -> bool;

}  // namespace tactile
