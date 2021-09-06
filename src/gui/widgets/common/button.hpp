#pragma once

#include "aliases/cstr.hpp"
#include "aliases/not_null.hpp"

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
auto Button(NotNull<CStr> text, CStr tooltip = nullptr, bool enabled = true) -> bool;

}  // namespace Tactile
