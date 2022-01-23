#pragma once

#include <tactile_def.hpp>

namespace tactile {

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
auto Checkbox(NotNull<CStr> label, NotNull<bool*> value, CStr tooltip = nullptr) -> bool;

}  // namespace tactile