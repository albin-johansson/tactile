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

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/map_events.hpp"
#include "ui/constants.hpp"
#include "ui/dialog/dialog.hpp"

namespace tactile::ui {
namespace {

inline constinit uint64 dialog_row_count = 0;
inline constinit uint64 dialog_column_count = 0;
inline constinit bool show_dialog = false;

}  // namespace

void open_resize_map_dialog(const TileExtent current_extent)
{
  dialog_row_count = current_extent.rows;
  dialog_column_count = current_extent.cols;
  show_dialog = true;
}

void update_resize_map_dialog(entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  DialogOptions options {
      .title = lang.window.resize_map.c_str(),
      .close_label = lang.misc.close.c_str(),
      .accept_label = lang.misc.ok.c_str(),
      .flags = UI_DIALOG_FLAG_INPUT_IS_VALID,
  };

  if (show_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    show_dialog = false;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {options, &action}; dialog.was_opened()) {
    const uint64 min_value = 1;
    const uint64 max_value = 10'000;

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.misc.rows.c_str());
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-min_float);
    ImGui::DragScalar("##Rows",
                      ImGuiDataType_U64,
                      &dialog_row_count,
                      1.0f,
                      &min_value,
                      &max_value);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.misc.columns.c_str());
    ImGui::SameLine();
    ImGui::DragScalar("##Columns",
                      ImGuiDataType_U64,
                      &dialog_column_count,
                      1.0f,
                      &min_value,
                      &max_value);
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<ResizeMapEvent>(dialog_row_count, dialog_column_count);
  }
}

}  // namespace tactile::ui
