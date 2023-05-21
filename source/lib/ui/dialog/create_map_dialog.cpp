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

#include "create_map_dialog.hpp"

#include <imgui.h>

#include "common/debug/assert.hpp"
#include "common/numeric.hpp"
#include "common/type/math.hpp"
#include "model/context.hpp"
#include "model/event/map_events.hpp"
#include "systems/language_system.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/style/alignment.hpp"

namespace tactile::ui {
namespace {

struct CreateMapDialogState final {
  Int2 tile_size {};
  uint64 row_count {};
  uint64 column_count {};
  bool open_dialog {};
};

inline constinit CreateMapDialogState gDialogState;

void _show_dialog_contents(const Strings& strings)
{
  {
    const auto& rows_label = strings.misc.rows;
    const auto& columns_label = strings.misc.columns;
    const auto offset =
        minimum_offset_to_align(rows_label.c_str(), columns_label.c_str());

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(rows_label.c_str());
    ImGui::SameLine(offset);
    ImGui::InputScalar("##Rows", ImGuiDataType_U64, &gDialogState.row_count);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(columns_label.c_str());
    ImGui::SameLine(offset);
    ImGui::InputScalar("##Columns", ImGuiDataType_U64, &gDialogState.column_count);
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
    ImGui::InputInt("##TileWidth", &gDialogState.tile_size.x);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(tile_height_label.c_str());
    ImGui::SameLine(offset);
    ImGui::InputInt("##TileHeight", &gDialogState.tile_size.y);
  }
}

void _on_dialog_accept(Dispatcher& dispatcher)
{
  TACTILE_ASSERT(gDialogState.tile_size.x > 0);
  TACTILE_ASSERT(gDialogState.tile_size.y > 0);
  dispatcher.enqueue<CreateMapEvent>(gDialogState.tile_size,
                                     gDialogState.row_count,
                                     gDialogState.column_count);
}

}  // namespace

void open_create_map_dialog()
{
  const auto& settings = get_global_settings();

  gDialogState.tile_size = settings.get_preferred_tile_size();
  gDialogState.row_count = 5;
  gDialogState.column_count = 5;
  gDialogState.open_dialog = true;
}

void show_create_map_dialog(const Model& model, Entity, Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);

  DialogOptions options {
      .title = strings.window.create_new_map.c_str(),
      .close_label = strings.misc.close.c_str(),
      .accept_label = strings.misc.create.c_str(),
  };

  if (gDialogState.open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    gDialogState.open_dialog = false;
  }

  const bool is_input_valid =
      (gDialogState.tile_size.x > 0) && (gDialogState.tile_size.y > 0);
  if (is_input_valid) {
    options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {options, &action}; dialog.was_opened()) {
    _show_dialog_contents(strings);
  }

  if (action == DialogAction::Accept) {
    _on_dialog_accept(dispatcher);
  }
}

}  // namespace tactile::ui
