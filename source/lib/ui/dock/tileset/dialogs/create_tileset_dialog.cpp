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

#include "create_tileset_dialog.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "common/type/math.hpp"
#include "common/type/path.hpp"
#include "common/util/string_buffer.hpp"
#include "io/file_dialog.hpp"
#include "io/proto/preferences.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/tileset_events.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

inline Path dialog_image_path;
inline StringBuffer dialog_image_path_preview_buffer;
inline Int2 dialog_tile_size {32, 32};
inline constinit bool open_dialog = false;

void select_image_file()
{
  auto dialog = io::FileDialog::open_image();
  if (!dialog.is_okay()) {
    return;
  }

  dialog_image_path = dialog.path();
  const auto image_path_str = dialog_image_path.string();

  if (image_path_str.size() > dialog_image_path_preview_buffer.size()) {
    dialog_image_path_preview_buffer = dialog_image_path.filename().string();
  }
  else {
    dialog_image_path_preview_buffer = image_path_str;
  }
}

}  // namespace

void open_create_tileset_dialog()
{
  dialog_image_path_preview_buffer.clear();
  dialog_image_path.clear();
  dialog_tile_size = io::get_preferences().preferred_tile_size;

  open_dialog = true;
}

void update_create_tileset_dialog(entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  DialogOptions options {
      .title = lang.window.create_tileset.c_str(),
      .close_label = lang.misc.cancel.c_str(),
      .accept_label = lang.misc.create.c_str(),
  };

  if (open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    open_dialog = false;
  }

  if (!dialog_image_path.empty() && dialog_tile_size.x > 0 && dialog_tile_size.y > 0) {
    options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {options, &action}; dialog.was_opened()) {
    ImGui::TextUnformatted(lang.misc.create_tileset_instruction.c_str());
    ImGui::Spacing();

    if (ui_button(lang.misc.select_image.c_str())) {
      select_image_file();
    }

    ImGui::SameLine();
    ImGui::InputTextWithHint("##Source",
                             lang.misc.tileset_image_input_hint.c_str(),
                             dialog_image_path_preview_buffer.data(),
                             sizeof dialog_image_path_preview_buffer,
                             ImGuiInputTextFlags_ReadOnly);

    ImGui::DragInt(lang.misc.tile_width.c_str(), &dialog_tile_size.x, 1.0f, 1, 10'000);
    ImGui::DragInt(lang.misc.tile_height.c_str(), &dialog_tile_size.y, 1.0f, 1, 10'000);
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<LoadTilesetEvent>(dialog_image_path, dialog_tile_size);
  }
}

}  // namespace tactile::ui
