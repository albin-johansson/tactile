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

#include "core/event/map_events.hpp"
#include "editor/ui/common/buttons.hpp"

namespace tactile::ui {

ResizeMapDialog::ResizeMapDialog() : ADialog {"Resize Map"} {}

void ResizeMapDialog::show(usize nCurrentRows, usize nCurrentColumns)
{
  mRows = nCurrentRows;
  mColumns = nCurrentColumns;
  make_visible();
}

void ResizeMapDialog::on_update(const DocumentModel&, entt::dispatcher&)
{
  auto rows = static_cast<int>(mRows);
  ImGui::DragInt("Rows", &rows, 1.0f, 1, 10'000);
  mRows = static_cast<usize>(rows);

  auto cols = static_cast<int>(mColumns);
  ImGui::DragInt("Columns", &cols, 1.0f, 1, 10'000);
  mColumns = static_cast<usize>(cols);
}

void ResizeMapDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ResizeMapEvent>(mRows, mColumns);
}

}  // namespace tactile::ui
