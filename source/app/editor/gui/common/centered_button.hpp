#pragma once

#include "tactile.hpp"

namespace tactile {

/**
 * \brief Shows a button that is centered in the parent region.
 *
 * \ingroup gui
 *
 * \param label the button label.
 * \param tooltip optional tooltip for the button.
 *
 * \return `true` if the button was pressed; `false` otherwise.
 */
auto CenteredButton(c_str label, c_str tooltip = nullptr) -> bool;

}  // namespace tactile
