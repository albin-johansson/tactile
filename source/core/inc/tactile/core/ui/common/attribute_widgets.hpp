// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/meta/attribute_type.hpp"

namespace tactile::ui {

class Language;

/**
 * Pushes a combo for selecting an attribute type to the widget stack.
 *
 * \param language The current language.
 * \param label    The combo label.
 * \param type     The attribute type to control.
 *
 * \return
 * True if the selected attribute type changed; false otherwise.
 */
auto push_attribute_type_combo(const Language& language,
                               const char* label,
                               AttributeType& type) -> bool;

}  // namespace tactile::ui
