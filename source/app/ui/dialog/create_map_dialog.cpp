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

#include "create_map_dialog.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/type/math.hpp"
#include "core/vocabulary.hpp"
#include "debug/assert.hpp"
#include "io/proto/preferences.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/map_events.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/style/alignment.hpp"

namespace tactile::ui {
namespace {

inline constinit Int2 dialog_tile_size = {0, 0};
inline constinit int dialog_row_count = 0;
inline constinit int dialog_column_count = 0;
inline constinit bool show_dialog = false;

void show_dialog_contents(const Strings& lang)
{
  {
    const auto& rows_label = lang.misc.rows;
    const auto& columns_label = lang.misc.columns;
    const auto offset =
        minimum_offset_to_align(rows_label.c_str(), columns_label.c_str());

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(rows_label.c_str());
    ImGui::SameLine(offset);
    ImGui::InputInt("##Rows", &dialog_row_count);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(columns_label.c_str());
    ImGui::SameLine(offset);
    ImGui::InputInt("##Columns", &dialog_column_count);
  }

  ImGui::Separator();

  {
    const auto& tile_width_label = lang.misc.tile_width;
    const auto& tile_height_label = lang.misc.tile_height;
    const auto offset =
        minimum_offset_to_align(tile_width_label.c_str(), tile_height_label.c_str());

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(tile_width_label.c_str());
    ImGui::SameLine(offset);
    ImGui::InputInt("##TileWidth", &dialog_tile_size.x);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(tile_height_label.c_str());
    ImGui::SameLine(offset);
    ImGui::InputInt("##TileHeight", &dialog_tile_size.y);
  }
}

void on_dialog_accept(entt::dispatcher& dispatcher)
{
  TACTILE_ASSERT(dialog_tile_size.x > 0);
  TACTILE_ASSERT(dialog_tile_size.y > 0);
  TACTILE_ASSERT(dialog_row_count > 0);
  TACTILE_ASSERT(dialog_column_count > 0);
  dispatcher.enqueue<CreateMapEvent>(dialog_tile_size,
                                     static_cast<usize>(dialog_row_count),
                                     static_cast<usize>(dialog_column_count));
}

}  // namespace

void open_create_map_dialog()
{
  const auto& prefs = io::get_preferences();

  dialog_tile_size = prefs.preferred_tile_size;
  dialog_row_count = 5;
  dialog_column_count = 5;

  show_dialog = true;
}

void update_create_map_dialog(entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  DialogOptions options {
      .title = lang.window.create_new_map.c_str(),
      .close_label = lang.misc.close.c_str(),
      .accept_label = lang.misc.create.c_str(),
  };

  if (show_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    show_dialog = false;
  }

  const bool is_input_valid = (dialog_tile_size.x > 0) && (dialog_tile_size.y > 0) &&
                              (dialog_row_count > 0) && (dialog_column_count > 0);
  if (is_input_valid) {
    options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {options, &action}; dialog.was_opened()) {
    show_dialog_contents(lang);
  }

  if (action == DialogAction::Accept) {
    on_dialog_accept(dispatcher);
  }
}

}  // namespace tactile::ui
