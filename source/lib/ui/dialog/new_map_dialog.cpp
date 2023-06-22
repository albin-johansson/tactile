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

#include "new_map_dialog.hpp"

#include <imgui.h>

#include "common/debug/assert.hpp"
#include "model/events/map_events.hpp"
#include "model/systems/language_system.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/style/alignment.hpp"

namespace tactile::ui {
namespace {

void _push_dialog_contents(const Strings& strings, NewMapDialogState& state)
{
  {
    const auto& rows_label = strings.misc.rows;
    const auto& columns_label = strings.misc.columns;
    const auto offset =
        minimum_offset_to_align(rows_label.c_str(), columns_label.c_str());

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(rows_label.c_str());
    ImGui::SameLine(offset);
    ImGui::InputScalar("##Rows", ImGuiDataType_U64, &state.row_count);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(columns_label.c_str());
    ImGui::SameLine(offset);
    ImGui::InputScalar("##Columns", ImGuiDataType_U64, &state.col_count);
  }

  ImGui::Separator();

  {
    const auto& tile_width_label = strings.misc.tile_width;
    const auto& tile_height_label = strings.misc.tile_height;
    const auto offset =
        minimum_offset_to_align(tile_width_label.c_str(), tile_height_label.c_str());

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(tile_width_label.c_str());
    ImGui::SameLine(offset);
    ImGui::InputInt("##TileWidth", &state.tile_size.x);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(tile_height_label.c_str());
    ImGui::SameLine(offset);
    ImGui::InputInt("##TileHeight", &state.tile_size.y);
  }
}

void _on_dialog_accept(const NewMapDialogState& state, Dispatcher& dispatcher)
{
  TACTILE_ASSERT(state.tile_size.x > 0);
  TACTILE_ASSERT(state.tile_size.y > 0);
  dispatcher.enqueue<CreateMapEvent>(state.tile_size, state.row_count, state.col_count);
}

}  // namespace

void push_new_map_dialog(const Model& model,
                         NewMapDialogState& state,
                         Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);

  DialogOptions dialog_options {
      .title = strings.window.create_new_map.c_str(),
      .close_label = strings.misc.close.c_str(),
      .accept_label = strings.misc.create.c_str(),
  };

  if (state.should_open) {
    dialog_options.flags |= UI_DIALOG_FLAG_OPEN;
    state.should_open = false;
  }

  const bool is_input_valid = (state.tile_size.x > 0) && (state.tile_size.y > 0);
  if (is_input_valid) {
    dialog_options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  auto action = DialogAction::None;
  if (const ScopedDialog dialog {dialog_options, &action}; dialog.was_opened()) {
    _push_dialog_contents(strings, state);
  }

  if (action == DialogAction::Accept) {
    _on_dialog_accept(state, dispatcher);
  }
}

}  // namespace tactile::ui
