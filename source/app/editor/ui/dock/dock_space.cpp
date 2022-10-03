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

#include "dock_space.hpp"

#include <imgui_internal.h>

#include "core/type/maybe.hpp"
#include "io/directories.hpp"
#include "io/proto/preferences.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"

namespace tactile::ui {
namespace {

constinit Maybe<ImGuiID> dock_root_id;

void reset_dock_visibilities()
{
  auto& prefs = io::get_preferences();
  prefs.show_layer_dock = io::def_show_layer_dock;
  prefs.show_tileset_dock = io::def_show_tileset_dock;
  prefs.show_property_dock = io::def_show_property_dock;
  prefs.show_component_dock = io::def_show_component_dock;
  prefs.show_log_dock = io::def_show_log_dock;
}

}  // namespace

void update_dock_space()
{
  static bool initialized = false;

  dock_root_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
  if (!initialized) {
    const auto size = ImGui::GetMainViewport()->Size;
    if (size.x > 0 && size.y > 0) {
      const auto& prefs = io::get_preferences();

      if (!prefs.restore_layout || !exists(io::widget_ini_path())) {
        load_default_layout(dock_root_id.value(), false);
      }

      initialized = true;
    }
  }
}

void load_default_layout(ImGuiID id, const bool reset_visibility)
{
  ImGui::DockBuilderRemoveNodeChildNodes(id);

  const auto& lang = get_current_language();
  const auto root = id;

  auto right = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 0.33f, nullptr, &id);
  const auto right_bottom =
      ImGui::DockBuilderSplitNode(right, ImGuiDir_Down, 0.40f, nullptr, &right);

  const auto bottom = ImGui::DockBuilderSplitNode(id, ImGuiDir_Down, 0.25f, nullptr, &id);

  ImGui::DockBuilderDockWindow("Viewport", root);
  ImGui::DockBuilderDockWindow(lang.window.tileset_dock.c_str(), right);
  ImGui::DockBuilderDockWindow(lang.window.property_dock.c_str(), right);
  ImGui::DockBuilderDockWindow(lang.window.component_dock.c_str(), right);
  ImGui::DockBuilderDockWindow(lang.window.layer_dock.c_str(), right_bottom);
  ImGui::DockBuilderDockWindow(lang.window.log_dock.c_str(), bottom);

  ImGui::DockBuilderFinish(id);

  if (reset_visibility) {
    reset_dock_visibilities();
  }
}

void reset_layout()
{
  load_default_layout(dock_root_id.value(), true);
}

}  // namespace tactile::ui
