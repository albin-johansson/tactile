#pragma once

#include "aliases/czstring.hpp"
#include "aliases/not_null.hpp"

namespace Tactile {

/**
 * \brief Shows a checkbox.
 *
 * \ingroup gui
 *
 * \param label the label associated with the checkbox.
 * \param[out] value a pointer to which the value of the checkbox will be written.
 * \param tooltip optional tooltip text.
 *
 * \return `true` if the value changed; `false` otherwise.
 */
auto Checkbox(NotNull<czstring> label,
              NotNull<bool*> value,
              czstring tooltip = nullptr) -> bool;

}  // namespace Tactile