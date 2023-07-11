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

#include "new_tileset_dialog.hpp"

#include <imgui.h>

#include "model/events/tileset_events.hpp"
#include "model/i18n/language_system.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/file_dialog.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

void _select_image_file(NewTilesetDialogState& state)
{
  auto dialog = FileDialog::open_image();
  if (!dialog.is_okay()) {
    return;
  }

  state.image_path = dialog.path();
  const auto image_path_str = state.image_path.string();

  if (image_path_str.size() > state.image_path_preview_buffer.size()) {
    state.image_path_preview_buffer = state.image_path.filename().string();
  }
  else {
    state.image_path_preview_buffer = image_path_str;
  }
}

}  // namespace

void push_new_tileset_dialog(const Model& model,
                             NewTilesetDialogState& state,
                             Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);

  DialogOptions options {
      .title = strings.window.create_tileset.c_str(),
      .close_label = strings.misc.cancel.c_str(),
      .accept_label = strings.misc.create.c_str(),
  };

  if (state.should_open) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    state.should_open = false;
  }

  if (!state.image_path.empty() && state.tile_size.x > 0 && state.tile_size.y > 0) {
    options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {options, &action}; dialog.was_opened()) {
    ImGui::TextUnformatted(strings.misc.create_tileset_instruction.c_str());
    ImGui::Spacing();

    if (push_button(strings.misc.select_image.c_str())) {
      _select_image_file(state);
    }

    ImGui::SameLine();
    ImGui::InputTextWithHint("##Source",
                             strings.misc.tileset_image_input_hint.c_str(),
                             state.image_path_preview_buffer.data(),
                             sizeof state.image_path_preview_buffer,
                             ImGuiInputTextFlags_ReadOnly);

    ImGui::DragInt(strings.misc.tile_width.c_str(), &state.tile_size.x, 1.0f, 1, 10'000);
    ImGui::DragInt(strings.misc.tile_height.c_str(), &state.tile_size.y, 1.0f, 1, 10'000);
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<CreateTilesetEvent>(state.map_entity,
                                           state.image_path,
                                           state.tile_size);
  }
}

}  // namespace tactile::ui
