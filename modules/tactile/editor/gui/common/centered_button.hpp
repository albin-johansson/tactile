#pragma once

#include <tactile_def.hpp>

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
auto CenteredButton(NotNull<CStr> label, CStr tooltip = nullptr) -> bool;

}  // namespace tactile
