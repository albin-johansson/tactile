#pragma once

#include "aliases/cstr.hpp"
#include "aliases/not_null.hpp"

namespace Tactile {

/**
 * \brief Shows a `(?)` label with the specified tooltip text.
 *
 * \ingroup gui
 *
 * \param text the tooltip text.
 */
void HelpMarker(NotNull<CStr> text);

}  // namespace Tactile
