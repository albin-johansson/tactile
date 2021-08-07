#pragma once

#include "aliases/czstring.hpp"
#include "aliases/not_null.hpp"

namespace Tactile {

/**
 * \brief Shows a button that is centered in the parent region.
 *
 * \ingroup gui
 *
 * \param label the button label.
 *
 * \return `true` if the button was pressed; `false` otherwise.
 */
auto CenteredButton(NotNull<czstring> label) -> bool;

}  // namespace Tactile
