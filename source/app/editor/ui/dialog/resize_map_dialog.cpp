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

#include "resize_map_dialog.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "editor/constants.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/map_events.hpp"

namespace tactile::ui {

ResizeMapDialog::ResizeMapDialog()
    : Dialog {"Resize Map"}
{
}

void ResizeMapDialog::show(usize nCurrentRows, usize nCurrentColumns)
{
  mRows = nCurrentRows;
  mColumns = nCurrentColumns;

  const auto& lang = get_current_language();
  set_title(lang.window.resize_map);

  make_visible();
}

void ResizeMapDialog::on_update(const DocumentModel&, entt::dispatcher&)
{
  const auto& lang = get_current_language();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(lang.misc.rows.c_str());

  ImGui::SameLine();

  auto rows = static_cast<int>(mRows);
  ImGui::SetNextItemWidth(-min_float);
  ImGui::DragInt("##Rows", &rows, 1.0f, 1, 10'000);
  mRows = static_cast<usize>(rows);

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(lang.misc.columns.c_str());

  ImGui::SameLine();

  auto cols = static_cast<int>(mColumns);
  ImGui::DragInt("##Columns", &cols, 1.0f, 1, 10'000);
  mColumns = static_cast<usize>(cols);
}

void ResizeMapDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ResizeMapEvent>(mRows, mColumns);
}

}  // namespace tactile::ui
