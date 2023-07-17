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

#include "resize_map_dialog.hpp"

#include <imgui.h>

#include "model/events/map_events.hpp"
#include "model/i18n/language_system.hpp"
#include "ui/constants.hpp"
#include "ui/dialog/dialog.hpp"

namespace tactile::ui {

void push_resize_map_dialog(const Registry& registry,
                            ResizeMapDialogState& state,
                            Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(registry);

  DialogOptions options {
      .title = strings.window.resize_map.c_str(),
      .close_label = strings.misc.close.c_str(),
      .accept_label = strings.misc.ok.c_str(),
      .flags = UI_DIALOG_FLAG_INPUT_IS_VALID,
  };

  if (state.should_open) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    state.should_open = false;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {options, &action}; dialog.was_opened()) {
    const uint64 min_value = 1;
    const uint64 max_value = 10'000;

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(strings.misc.rows.c_str());
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-kMinFloat);
    ImGui::DragScalar("##Rows",
                      ImGuiDataType_U64,
                      &state.row_count,
                      1.0f,
                      &min_value,
                      &max_value);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(strings.misc.columns.c_str());
    ImGui::SameLine();
    ImGui::DragScalar("##Columns",
                      ImGuiDataType_U64,
                      &state.col_count,
                      1.0f,
                      &min_value,
                      &max_value);
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<ResizeMapEvent>(state.row_count, state.col_count);
  }
}

}  // namespace tactile::ui
