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

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/event/map_events.hpp"
#include "editor/ui/alignment.hpp"
#include "editor/ui/common/input_widgets.hpp"
#include "editor/ui/common/tooltips.hpp"
#include "editor/ui/scoped.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile::ui {

GodotExportDialog::GodotExportDialog()
    : ADialog {"Export As Godot Scene"}
{}

void GodotExportDialog::open()
{
  mRootDir.clear();
  mMapDir.clear();
  mImageDir.clear();
  mTilesetDir.clear();

  mEmbedTilesets = false;
  //  mEmbedTilesets = io::get_preferences().embed_tilesets;

  set_title("Export As Godot Scene");
  set_accept_button_label("Export");

  make_visible();
}

void GodotExportDialog::on_update(const DocumentModel&, entt::dispatcher&)
{
  const auto* root_label = "Godot project folder";
  const auto* map_label = "Map folder";
  const auto* image_label = "Image folder";
  const auto* tileset_label = "Tileset folder";

  const auto offset = minimum_offset_to_align(root_label,  //
                                              map_label,
                                              image_label,
                                              tileset_label);

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(root_label);
  lazy_tooltip("##RootDirTooltip", "Root directory of your Godot project.");

  ImGui::SameLine(offset);
  if (auto root_path = input_folder("##RootDir", mRootDir)) {
    mRootDir = std::move(*root_path);
    mMapDir = ".";
    mImageDir = ".";
    mTilesetDir = ".";
  }

  ImGui::Separator();

  {
    Disable when_root_is_unset {mRootDir.empty()};

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(map_label);
    lazy_tooltip("##MapDirTooltip",
                 "Relative path in the Godot project to where maps are stored.");

    ImGui::SameLine(offset);
    if (const auto map_dir = input_folder("##MapDir", mMapDir)) {
      mMapDir = fs::relative(*map_dir, mRootDir);
    }

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(image_label);
    lazy_tooltip("##ImageDirTooltip",
                 "Relative path in the Godot project to where images are stored.");

    ImGui::SameLine(offset);
    if (const auto image_dir = input_folder("##ImageDir", mImageDir)) {
      mImageDir = fs::relative(*image_dir, mRootDir);
    }
  }

  ImGui::Separator();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted("Embed tilesets in the map scene");

  ImGui::SameLine();
  ImGui::Checkbox("##EmbedTilesets", &mEmbedTilesets);

  {
    Disable when_embedded {mEmbedTilesets};

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(tileset_label);
    lazy_tooltip("##TilesetPathTooltip",
                 "Relative path in the Godot project to where tilesets are stored.");

    ImGui::SameLine(offset);
    if (auto tileset_dir = input_folder("##TilesetPath", mTilesetDir)) {
      mTilesetDir = fs::relative(*tileset_dir, mRootDir);
    }
  }

  ImGui::Separator();

  // TODO object types
}

auto GodotExportDialog::is_current_input_valid(const DocumentModel&) const -> bool
{
  return !mRootDir.empty() && (mEmbedTilesets || !mTilesetDir.empty());
}

void GodotExportDialog::on_accept(entt::dispatcher& dispatcher)
{
  ExportAsGodotSceneEvent event {
      .root_dir = std::move(mRootDir),
      .map_dir = std::move(mMapDir),
      .image_dir = std::move(mImageDir),
      .tileset_dir = std::move(mTilesetDir),
      .embed_tilesets = mEmbedTilesets,
  };
  dispatcher.enqueue<ExportAsGodotSceneEvent>(std::move(event));
}

}  // namespace tactile::ui