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

#include "godot_export_dialog.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/events/map_events.hpp"
#include "core/utils/buffers.hpp"
#include "editor/ui/alignment.hpp"
#include "editor/ui/scoped.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile::ui {

GodotExportDialog::GodotExportDialog() : ADialog {"Export As Godot Scene"}
{
  set_accept_button_label("Export");
}

void GodotExportDialog::open()
{
  // TODO need to know where the Godot project is on the filesystem

  zero_buffer(mProjectPathBuffer);
  zero_buffer(mImagePathBuffer);
  zero_buffer(mTilesetPathBuffer);
  mEmbedTilesets = io::get_preferences().embed_tilesets;
  make_visible();
}

void GodotExportDialog::on_update(const DocumentModel& model,
                                  entt::dispatcher&    dispatcher)
{
  const char* projectLabel = "Project path:";
  const char* imageLabel = "Image path:";
  const char* tilesetLabel = "Tileset path:";
  const auto  offset = minimum_offset_to_align(projectLabel, imageLabel, tilesetLabel);

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(projectLabel);
  ImGui::SameLine(offset);
  ImGui::InputTextWithHint("##ProjectPath",
                           "scenes/maps",
                           mProjectPathBuffer.data(),
                           sizeof mProjectPathBuffer);

  ImGui::SameLine();
  ImGui::TextUnformatted("(?)");
  if (ImGui::IsItemHovered()) {
    ImGui::BeginTooltip();
    ImGui::TextUnformatted("The path in the Godot project where the maps are stored.");
    ImGui::EndTooltip();
  }

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(imageLabel);
  ImGui::SameLine(offset);
  ImGui::InputTextWithHint("##ImagePath",
                           "assets/images",
                           mImagePathBuffer.data(),
                           sizeof mImagePathBuffer);

  ImGui::SameLine();
  ImGui::TextUnformatted("(?)");
  if (ImGui::IsItemHovered()) {
    ImGui::BeginTooltip();
    ImGui::TextUnformatted("The path in the Godot project where the images are stored.");
    ImGui::EndTooltip();
  }

  ImGui::Spacing();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted("Embed tilesets in the map scene:");
  ImGui::SameLine();
  ImGui::Checkbox("##EmbedTilesets", &mEmbedTilesets);

  {
    Disable whenEmbedded {mEmbedTilesets};

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(tilesetLabel);
    ImGui::SameLine(offset);
    ImGui::InputTextWithHint("##TilesetPath",
                             "scenes/tilesets",
                             mTilesetPathBuffer.data(),
                             sizeof mTilesetPathBuffer);

    ImGui::SameLine();
    ImGui::TextUnformatted("(?)");
    if (ImGui::IsItemHovered()) {
      ImGui::BeginTooltip();
      ImGui::TextUnformatted(
          "The path in the Godot project where the tilesets are stored.");
      ImGui::EndTooltip();
    }
  }
}

auto GodotExportDialog::is_current_input_valid(const DocumentModel&) const -> bool
{
  return !create_string_view_from_buffer(mProjectPathBuffer).empty() &&
         !create_string_view_from_buffer(mImagePathBuffer).empty() &&
         (mEmbedTilesets || !create_string_view_from_buffer(mTilesetPathBuffer).empty());
}

void GodotExportDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ExportAsGodotSceneEvent>(
      create_string_from_buffer(mProjectPathBuffer),
      create_string_from_buffer(mImagePathBuffer),
      create_string_from_buffer(mTilesetPathBuffer),
      mEmbedTilesets);
}

}  // namespace tactile::ui