#pragma once

#include "aliases/czstring.hpp"

namespace Tactile {

/**
 * \brief Shows a button.
 *
 * \param text the button text.
 * \param tooltip the tooltip text, can safely be null.
 * \param enabled `true` to show an enabled button; `false` otherwise.
 *
 * \return `true` if the button was pressed; `false` otherwise.
 */
auto ButtonEx(czstring text, czstring tooltip = nullptr, bool enabled = true)
    -> bool;

}  // namespace Tactile
