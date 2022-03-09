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

#include "map_parse_error_dialog.hpp"

#include <imgui.h>

#include "editor/gui/icons.hpp"

namespace tactile {

map_parse_error_dialog::map_parse_error_dialog() : dialog_base{"Map Parse Error"}
{
  set_close_button_label(nullptr);
}

void map_parse_error_dialog::show(const parsing::parse_error error)
{
  mError = error;
  make_visible();
}

void map_parse_error_dialog::on_update(const document_model&, entt::dispatcher&)
{
  ImGui::TextUnformatted(
      "Oops, something went wrong when parsing the map! " ICON_FA_SAD_TEAR);
  ImGui::Text("Cause: %s", parsing::to_cause(mError.value()).data());
}

}  // namespace tactile