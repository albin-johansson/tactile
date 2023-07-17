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

#include "godot_export_dialog.hpp"

#include <utility>  // move

#include <imgui.h>

#include "common/type/path.hpp"
#include "model/events/map_events.hpp"
#include "model/i18n/language_system.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/style/alignment.hpp"
#include "ui/widget/attribute_widgets.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

constexpr int kPolygonApproxCountMin = 4;
constexpr int kPolygonApproxCountMax = 64;

void _push_dialog_contents(const Strings& strings, GodotExportDialogState& state)
{
  const auto& root_label = strings.misc.godot_project_folder_label;
  const auto& map_label = strings.misc.godot_map_folder_label;
  const auto& image_label = strings.misc.godot_image_folder_label;
  const auto& tileset_label = strings.misc.godot_tileset_folder_label;

  const auto offset = minimum_offset_to_align(root_label.c_str(),  //
                                              map_label.c_str(),
                                              image_label.c_str(),
                                              tileset_label.c_str());

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(root_label.c_str());
  push_lazy_tooltip("##RootDirTooltip", strings.tooltip.godot_project_folder.c_str());

  ImGui::SameLine(offset);
  if (auto root_path = push_directory_path_input(strings, "##RootDir", state.root_dir)) {
    state.root_dir = std::move(*root_path);
    state.map_dir = ".";
    state.image_dir = ".";
    state.tileset_dir = ".";
  }

  ImGui::Separator();

  {
    const Disable when_root_is_unset {state.root_dir.empty()};

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(map_label.c_str());
    push_lazy_tooltip("##MapDirTooltip", strings.tooltip.godot_map_folder.c_str());

    ImGui::SameLine(offset);
    if (const auto map_dir =
            push_directory_path_input(strings, "##MapDir", state.map_dir)) {
      state.map_dir = fs::relative(*map_dir, state.root_dir);
    }

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(image_label.c_str());
    push_lazy_tooltip("##ImageDirTooltip", strings.tooltip.godot_image_folder.c_str());

    ImGui::SameLine(offset);
    if (const auto image_dir =
            push_directory_path_input(strings, "##ImageDir", state.image_dir)) {
      state.image_dir = fs::relative(*image_dir, state.root_dir);
    }

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(tileset_label.c_str());
    push_lazy_tooltip("##TilesetPathTooltip",
                      strings.tooltip.godot_tileset_folder.c_str());

    ImGui::SameLine(offset);
    if (auto tileset_dir =
            push_directory_path_input(strings, "##TilesetPath", state.tileset_dir)) {
      state.tileset_dir = fs::relative(*tileset_dir, state.root_dir);
    }
  }

  ImGui::Separator();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(strings.misc.godot_polygon_points_label.c_str());
  push_lazy_tooltip("##PolygonPointsTooltip",
                    strings.tooltip.godot_polygon_points.c_str());

  ImGui::SameLine();
  ImGui::SliderInt("##PolygonPoints",
                   &state.polygon_point_count,
                   kPolygonApproxCountMin,
                   kPolygonApproxCountMax);
}

void _on_dialog_accept(const GodotExportDialogState& state, Dispatcher& dispatcher)
{
  ExportAsGodotSceneEvent event {
      .root_dir = state.root_dir,
      .map_dir = state.map_dir,
      .image_dir = state.image_dir,
      .tileset_dir = state.tileset_dir,
      .polygon_points = static_cast<usize>(state.polygon_point_count),
  };

  dispatcher.enqueue<ExportAsGodotSceneEvent>(std::move(event));
}

}  // namespace

void push_godot_export_dialog(const Registry& registry,
                              GodotExportDialogState& state,
                              Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(registry);

  DialogOptions options {
      .title = strings.window.export_as_godot_scene.c_str(),
      .close_label = strings.misc.close.c_str(),
      .accept_label = strings.misc.export_.c_str(),
  };

  const bool is_input_valid = !state.root_dir.empty();
  if (is_input_valid) {
    options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  if (state.should_open) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    state.should_open = false;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {options, &action}; dialog.was_opened()) {
    _push_dialog_contents(strings, state);
  }

  if (action == DialogAction::Accept) {
    _on_dialog_accept(state, dispatcher);
  }
}

}  // namespace tactile::ui