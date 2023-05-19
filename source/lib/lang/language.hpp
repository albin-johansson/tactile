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

#include "common/macros.hpp"
#include "common/type/string.hpp"
#include "core/menu.hpp"
#include "lang/strings.hpp"

namespace tactile {

/// All of the supported languages.
enum class Lang {
  EN = 0,     ///< American English
  EN_GB = 1,  ///< British English
  SV = 2      ///< Swedish
};

/// Loads all supported translation files.
void load_languages();

[[nodiscard]] auto get_language(Lang lang) -> const Strings&;

[[nodiscard]] auto get_current_language() -> const Strings&;

[[nodiscard]] auto get_language_name(Lang lang) -> const char*;

[[nodiscard]] auto get_label(const Strings& lang, MenuAction action) -> String;

}  // namespace tactile
