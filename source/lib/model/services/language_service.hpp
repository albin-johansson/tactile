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

#include "core/containers/hash_map.hpp"
#include "core/containers/string.hpp"
#include "core/enums/lang.hpp"
#include "core/enums/menu_action.hpp"
#include "model/services/i18n/strings.hpp"

namespace tactile {

class LanguageService final {
 public:
  /// Loads the translation JSON files.
  void load_languages();

  [[nodiscard]] auto get_strings(Lang language) const -> const Strings&;

  [[nodiscard]] auto get_current_language_strings() const -> const Strings&;

  [[nodiscard]] static auto get_name(Lang language) -> const char*;

 private:
  HashMap<Lang, Strings> mLanguages;
};

}  // namespace tactile