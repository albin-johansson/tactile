#pragma once

#include "tactile.hpp"

namespace tactile {

/**
 * \brief Shows a checkbox.
 *
 * \ingroup gui
 *
 * \param label the checkbox label.
 * \param[out] value a pointer to which the value of the checkbox will be written.
 * \param tooltip optional tooltip text.
 *
 * \return `true` if the value changed; `false` otherwise.
 */
auto checkbox(const char* label, bool* value, const char* tooltip = nullptr) -> bool;

}  // namespace tactile