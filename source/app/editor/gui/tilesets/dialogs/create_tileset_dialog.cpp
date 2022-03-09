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

#include <imgui.h>

#include "core/utils/buffers.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/gui/common/button.hpp"
#include "io/file_dialog.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile {

CreateTilesetDialog::CreateTilesetDialog() : dialog_base{"Create tileset"}
{
  set_accept_button_label("Create");
}

void CreateTilesetDialog::Open()
{
  zero_buffer(mPathPreviewBuffer);
  mFullImagePath.clear();

  const auto& prefs = get_preferences();
  mTileWidth = prefs.preferred_tile_width();
  mTileHeight = prefs.preferred_tile_height();

  make_visible();
}

void CreateTilesetDialog::on_update(const DocumentModel&, entt::dispatcher&)
{
  ImGui::TextUnformatted("Select an image which contains the tiles aligned in a grid.");
  ImGui::Spacing();

  if (button("Select image...")) {
    ShowImageFileDialog();
  }

  ImGui::SameLine();
  ImGui::InputTextWithHint("##Source",
                           "Source image path",
                           mPathPreviewBuffer.data(),
                           mPathPreviewBuffer.size(),
                           ImGuiInputTextFlags_ReadOnly);

  ImGui::DragInt("Tile width", &mTileWidth, 1.0f, 1, 10'000);
  ImGui::DragInt("Tile height", &mTileHeight, 1.0f, 1, 10'000);
}

void CreateTilesetDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<AddTilesetEvent>(mFullImagePath, mTileWidth, mTileHeight);
}

auto CreateTilesetDialog::is_current_input_valid(const DocumentModel&) const -> bool
{
  return mPathPreviewBuffer.front() != '\0' && mTileWidth > 0 && mTileHeight > 0;
}

void CreateTilesetDialog::ShowImageFileDialog()
{
  auto dialog = file_dialog::open_image();
  if (!dialog.is_okay()) {
    return;
  }

  mFullImagePath = dialog.path();
  const auto pathStr = mFullImagePath.string();

  if (pathStr.size() > mPathPreviewBuffer.size()) {
    const auto name = mFullImagePath.filename();
    copy_string_into_buffer(mPathPreviewBuffer, name.string());
  }
  else {
    copy_string_into_buffer(mPathPreviewBuffer, pathStr);
  }
}

}  // namespace tactile
