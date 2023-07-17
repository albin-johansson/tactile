/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "common/enum/lang.hpp"
#include "model/i18n/language_components.hpp"
#include "model/registry.hpp"
#include "model/view/menu_components.hpp"

namespace tactile::sys {

/**
 * Loads all translation files and initializes the language mapping context component.
 *
 * \param model the associated registry.
 */
void load_languages(Registry& registry);

/**
 * Returns the translated strings associated with the currently active language.
 *
 * \param model the associated registry.
 *
 * \return a set of translated strings.
 */
[[nodiscard]] auto get_current_language_strings(const Registry& registry)
    -> const Strings&;

/**
 * Returns the native name of a language.
 *
 * \param lang the language to query.
 *
 * \return a language name, e.g. "Svenska" for Swedish.
 */
[[nodiscard]] auto get_language_name(Lang lang) -> const char*;

/**
 * Returns the translated string for a menu action label.
 *
 * \param strings the associated set of translated strings.
 * \param action  the menu action to translate.
 *
 * \return a string suitable for use as a menu action label.
 */
[[nodiscard]] auto get_string(const Strings& strings, MenuAction action) -> String;

}  // namespace tactile::sys
