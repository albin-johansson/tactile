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

#include "create_tileset_dialog.hpp"

#include <imgui.h>

#include "common/type/math.hpp"
#include "common/type/path.hpp"
#include "common/util/string_buffer.hpp"
#include "io/file_dialog.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/tileset_events.hpp"
#include "model/settings.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

struct CreateTilesetDialogState final {
  Path image_path;
  StringBuffer image_path_preview_buffer {};
  Int2 tile_size {32, 32};
  bool open_dialog {};
};

inline CreateTilesetDialogState gDialogState;

void _select_image_file()
{
  auto dialog = FileDialog::open_image();
  if (!dialog.is_okay()) {
    return;
  }

  gDialogState.image_path = dialog.path();
  const auto image_path_str = gDialogState.image_path.string();

  if (image_path_str.size() > gDialogState.image_path_preview_buffer.size()) {
    gDialogState.image_path_preview_buffer = gDialogState.image_path.filename().string();
  }
  else {
    gDialogState.image_path_preview_buffer = image_path_str;
  }
}

}  // namespace

void open_create_tileset_dialog()
{
  gDialogState.image_path_preview_buffer.clear();
  gDialogState.image_path.clear();
  gDialogState.tile_size = get_settings().get_preferred_tile_size();
  gDialogState.open_dialog = true;
}

void show_create_tileset_dialog(const Model& model, Entity, Dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  DialogOptions options {
      .title = lang.window.create_tileset.c_str(),
      .close_label = lang.misc.cancel.c_str(),
      .accept_label = lang.misc.create.c_str(),
  };

  if (gDialogState.open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    gDialogState.open_dialog = false;
  }

  if (!gDialogState.image_path.empty() &&  //
      gDialogState.tile_size.x > 0 && gDialogState.tile_size.y > 0) {
    options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {options, &action}; dialog.was_opened()) {
    ImGui::TextUnformatted(lang.misc.create_tileset_instruction.c_str());
    ImGui::Spacing();

    if (ui_button(lang.misc.select_image.c_str())) {
      _select_image_file();
    }

    ImGui::SameLine();
    ImGui::InputTextWithHint("##Source",
                             lang.misc.tileset_image_input_hint.c_str(),
                             gDialogState.image_path_preview_buffer.data(),
                             sizeof gDialogState.image_path_preview_buffer,
                             ImGuiInputTextFlags_ReadOnly);

    ImGui::DragInt(lang.misc.tile_width.c_str(),
                   &gDialogState.tile_size.x,
                   1.0f,
                   1,
                   10'000);
    ImGui::DragInt(lang.misc.tile_height.c_str(),
                   &gDialogState.tile_size.y,
                   1.0f,
                   1,
                   10'000);
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<LoadTilesetEvent>(gDialogState.image_path, gDialogState.tile_size);
  }
}

}  // namespace tactile::ui
