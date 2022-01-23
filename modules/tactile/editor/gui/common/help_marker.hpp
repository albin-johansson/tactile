#pragma once

#include <tactile_def.hpp>

namespace tactile {

/**
 * \brief Shows a `(?)` label with the specified tooltip text.
 *
 * \ingroup gui
 *
 * \param text the tooltip text.
 */
void HelpMarker(NotNull<CStr> text);

}  // namespace tactile
