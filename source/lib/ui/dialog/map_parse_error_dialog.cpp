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

#include "map_parse_error_dialog.hpp"

#include <imgui.h>

#include "ui/dialog/dialog.hpp"

namespace tactile {

void push_map_parse_error_dialog(ModelView model, MapParseErrorDialogState& state)
{
  const auto& strings = model.get_language_strings();

  ui::DialogOptions dialog_options {
      .title = strings.window.map_parse_error.c_str(),
      .close_label = strings.misc.close.c_str(),
      .flags = ui::UI_DIALOG_FLAG_INPUT_IS_VALID,
  };

  if (state.should_open) {
    dialog_options.flags |= ui::UI_DIALOG_FLAG_OPEN;
    state.should_open = false;
  }

  if (const ui::ScopedDialog dialog {dialog_options}; dialog.was_opened()) {
    ImGui::TextUnformatted(strings.misc.map_parse_error.c_str());
    ImGui::TextUnformatted(state.cause.c_str());
  }
}

}  // namespace tactile
