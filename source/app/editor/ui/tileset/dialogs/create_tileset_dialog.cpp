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

#include "core/event/tileset_events.hpp"
#include "core/util/buffers.hpp"
#include "editor/lang/language.hpp"
#include "editor/lang/strings.hpp"
#include "editor/ui/common/buttons.hpp"
#include "io/file_dialog.hpp"
#include "io/persist/preferences.hpp"

namespace tactile::ui {

CreateTilesetDialog::CreateTilesetDialog()
    : ADialog {"Create Tileset"}
{}

void CreateTilesetDialog::open()
{
  zero_buffer(mPathPreviewBuffer);
  mFullImagePath.clear();

  const auto& prefs = io::get_preferences();
  mTileSize = prefs.preferred_tile_size;

  const auto& lang = get_current_language();
  set_title(lang.window.create_tileset);
  set_accept_button_label(lang.misc.create);

  make_visible();
}

void CreateTilesetDialog::on_update(const DocumentModel&, entt::dispatcher&)
{
  const auto& lang = get_current_language();

  ImGui::TextUnformatted(lang.misc.create_tileset_instruction.c_str());
  ImGui::Spacing();

  if (button(lang.misc.select_image.c_str())) {
    show_image_file_dialog();
  }

  ImGui::SameLine();
  ImGui::InputTextWithHint("##Source",
                           lang.misc.tileset_image_input_hint.c_str(),
                           mPathPreviewBuffer.data(),
                           mPathPreviewBuffer.size(),
                           ImGuiInputTextFlags_ReadOnly);

  ImGui::DragInt(lang.misc.tile_width.c_str(), &mTileSize.x, 1.0f, 1, 10'000);
  ImGui::DragInt(lang.misc.tile_height.c_str(), &mTileSize.y, 1.0f, 1, 10'000);
}

void CreateTilesetDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<LoadTilesetEvent>(mFullImagePath, mTileSize);
}

auto CreateTilesetDialog::is_current_input_valid(const DocumentModel&) const -> bool
{
  return mPathPreviewBuffer.front() != '\0' && mTileSize.x > 0 && mTileSize.y > 0;
}

void CreateTilesetDialog::show_image_file_dialog()
{
  auto dialog = io::FileDialog::open_image();
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

}  // namespace tactile::ui
