/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include "filename_filter.hpp"

#include <locale>  // locale, isalpha, isdigit, isspace

#include <imgui.h>

namespace tactile::ui {

auto filename_filter(ImGuiInputTextCallbackData* data) -> int
{
  const auto& locale = std::locale::classic();
  const auto  ch = static_cast<wchar_t>(data->EventChar);

  if (std::isalpha(ch, locale) || std::isdigit(ch, locale) || std::isspace(ch, locale) ||
      ch == '-' || ch == '_') {
    return 0;  // Accept the character
  }
  else {
    return 1;  // Reject the character
  }
}

}  // namespace tactile::ui
