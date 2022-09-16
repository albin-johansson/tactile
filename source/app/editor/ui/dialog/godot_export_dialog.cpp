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
#include "lang/language.hpp"
#include "lang/strings.hpp"

namespace tactile::ui {

GodotExportDialog::GodotExportDialog()
    : Dialog {"Export As Godot Scene"}
{}

void GodotExportDialog::open()
{
  mRootDir.clear();
  mMapDir.clear();
  mImageDir.clear();
  mTilesetDir.clear();
  mPolygonPointCount = 16;

  const auto& lang = get_current_language();
  set_title(lang.window.export_as_godot_scene);
  set_accept_button_label(lang.misc.export_);
  set_close_button_label(lang.misc.close);

  make_visible();
}

void GodotExportDialog::on_update(const DocumentModel&, entt::dispatcher&)
{
  const auto& lang = get_current_language();

  const auto& root_label = lang.misc.godot_project_folder_label;
  const auto& map_label = lang.misc.godot_map_folder_label;
  const auto& image_label = lang.misc.godot_image_folder_label;
  const auto& tileset_label = lang.misc.godot_tileset_folder_label;

  const auto offset = minimum_offset_to_align(root_label.c_str(),  //
                                              map_label.c_str(),
                                              image_label.c_str(),
                                              tileset_label.c_str());

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(root_label.c_str());
  lazy_tooltip("##RootDirTooltip", lang.tooltip.godot_project_folder.c_str());

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
    ImGui::TextUnformatted(map_label.c_str());
    lazy_tooltip("##MapDirTooltip", lang.tooltip.godot_map_folder.c_str());

    ImGui::SameLine(offset);
    if (const auto map_dir = input_folder("##MapDir", mMapDir)) {
      mMapDir = fs::relative(*map_dir, mRootDir);
    }

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(image_label.c_str());
    lazy_tooltip("##ImageDirTooltip", lang.tooltip.godot_image_folder.c_str());

    ImGui::SameLine(offset);
    if (const auto image_dir = input_folder("##ImageDir", mImageDir)) {
      mImageDir = fs::relative(*image_dir, mRootDir);
    }

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(tileset_label.c_str());
    lazy_tooltip("##TilesetPathTooltip", lang.tooltip.godot_tileset_folder.c_str());

    ImGui::SameLine(offset);
    if (auto tileset_dir = input_folder("##TilesetPath", mTilesetDir)) {
      mTilesetDir = fs::relative(*tileset_dir, mRootDir);
    }
  }

  ImGui::Separator();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(lang.misc.godot_polygon_points_label.c_str());
  lazy_tooltip("##PolygonPointsTooltip", lang.tooltip.godot_polygon_points.c_str());

  ImGui::SameLine();
  ImGui::SliderInt("##PolygonPoints", &mPolygonPointCount, 4, 64);
}

auto GodotExportDialog::is_current_input_valid(const DocumentModel&) const -> bool
{
  return !mRootDir.empty();
}

void GodotExportDialog::on_accept(entt::dispatcher& dispatcher)
{
  ExportAsGodotSceneEvent event {
      .root_dir = std::move(mRootDir),
      .map_dir = std::move(mMapDir),
      .image_dir = std::move(mImageDir),
      .tileset_dir = std::move(mTilesetDir),
      .polygon_points = static_cast<usize>(mPolygonPointCount),
  };
  dispatcher.enqueue<ExportAsGodotSceneEvent>(std::move(event));
}

}  // namespace tactile::ui