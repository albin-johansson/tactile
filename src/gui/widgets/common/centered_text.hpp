#pragma once

#include "aliases/czstring.hpp"
#include "aliases/not_null.hpp"

namespace Tactile {

/**
 * \brief Shows a label that is centered in the parent region.
 *
 * \ingroup gui
 *
 * \param text the text that will be displayed.
 */
void CenteredText(NotNull<czstring> text);

}  // namespace Tactile
