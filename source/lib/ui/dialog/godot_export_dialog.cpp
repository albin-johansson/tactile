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

#include "common/type/path.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/map_events.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/style/alignment.hpp"
#include "ui/widget/attribute_widgets.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

inline constexpr int kPolygonApproxCountDefault = 16;
inline constexpr int kPolygonApproxCountMin = 4;
inline constexpr int kPolygonApproxCountMax = 64;

struct GodotExportDialogState final {
  Path root_dir;
  Path map_dir;
  Path image_dir;
  Path tileset_dir;
  int polygon_point_count {};
  bool open_dialog {};
};

inline GodotExportDialogState gDialogState;

void show_dialog_contents(const Strings& lang)
{
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
  ui_lazy_tooltip("##RootDirTooltip", lang.tooltip.godot_project_folder.c_str());

  ImGui::SameLine(offset);
  if (auto root_path = ui_directory_path_input("##RootDir", gDialogState.root_dir)) {
    gDialogState.root_dir = std::move(*root_path);
    gDialogState.map_dir = ".";
    gDialogState.image_dir = ".";
    gDialogState.tileset_dir = ".";
  }

  ImGui::Separator();

  {
    const Disable when_root_is_unset {gDialogState.root_dir.empty()};

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(map_label.c_str());
    ui_lazy_tooltip("##MapDirTooltip", lang.tooltip.godot_map_folder.c_str());

    ImGui::SameLine(offset);
    if (const auto map_dir = ui_directory_path_input("##MapDir", gDialogState.map_dir)) {
      gDialogState.map_dir = fs::relative(*map_dir, gDialogState.root_dir);
    }

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(image_label.c_str());
    ui_lazy_tooltip("##ImageDirTooltip", lang.tooltip.godot_image_folder.c_str());

    ImGui::SameLine(offset);
    if (const auto image_dir =
            ui_directory_path_input("##ImageDir", gDialogState.image_dir)) {
      gDialogState.image_dir = fs::relative(*image_dir, gDialogState.root_dir);
    }

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(tileset_label.c_str());
    ui_lazy_tooltip("##TilesetPathTooltip", lang.tooltip.godot_tileset_folder.c_str());

    ImGui::SameLine(offset);
    if (auto tileset_dir =
            ui_directory_path_input("##TilesetPath", gDialogState.tileset_dir)) {
      gDialogState.tileset_dir = fs::relative(*tileset_dir, gDialogState.root_dir);
    }
  }

  ImGui::Separator();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(lang.misc.godot_polygon_points_label.c_str());
  ui_lazy_tooltip("##PolygonPointsTooltip", lang.tooltip.godot_polygon_points.c_str());

  ImGui::SameLine();
  ImGui::SliderInt("##PolygonPoints",
                   &gDialogState.polygon_point_count,
                   kPolygonApproxCountMin,
                   kPolygonApproxCountMax);
}

void on_dialog_accept(entt::dispatcher& dispatcher)
{
  ExportAsGodotSceneEvent event {
      .root_dir = std::move(gDialogState.root_dir),
      .map_dir = std::move(gDialogState.map_dir),
      .image_dir = std::move(gDialogState.image_dir),
      .tileset_dir = std::move(gDialogState.tileset_dir),
      .polygon_points = static_cast<usize>(gDialogState.polygon_point_count),
  };
  dispatcher.enqueue<ExportAsGodotSceneEvent>(std::move(event));
}

}  // namespace

void open_godot_export_dialog()
{
  gDialogState.root_dir.clear();
  gDialogState.map_dir.clear();
  gDialogState.image_dir.clear();
  gDialogState.tileset_dir.clear();
  gDialogState.polygon_point_count = kPolygonApproxCountDefault;
  gDialogState.open_dialog = true;
}

void update_godot_export_dialog(entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  DialogOptions options {
      .title = lang.window.export_as_godot_scene.c_str(),
      .close_label = lang.misc.close.c_str(),
      .accept_label = lang.misc.export_.c_str(),
  };

  const bool is_input_valid = !gDialogState.root_dir.empty();
  if (is_input_valid) {
    options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  if (gDialogState.open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    gDialogState.open_dialog = false;
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