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

#include "dock_space.hpp"

#include <imgui_internal.h>
#include <spdlog/spdlog.h>

#include "common/type/path.hpp"
#include "io/directories.hpp"
#include "model/event/setting_events.hpp"
#include "model/persistence/settings.hpp"
#include "model/systems/language_system.hpp"

namespace tactile::ui {

void update_dock_space(const Model& model, DockSpaceState& state)
{
  state.root_dock_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

  if (!state.did_initialize) {
    const auto size = ImGui::GetMainViewport()->Size;
    if (size.x > 0 && size.y > 0) {
      const auto& settings = model.get<Settings>();

      if (!settings.test_flag(SETTINGS_RESTORE_LAYOUT_BIT) ||
          !fs::exists(get_widget_ini_path())) {
        load_default_layout(model, state.root_dock_id.value());
      }

      state.did_initialize = true;
    }
  }
}

void load_default_layout(const Model& model, ImGuiID id)
{
  const auto& lang = sys::get_current_language_strings(model);

  ImGui::DockBuilderRemoveNodeChildNodes(id);

  const auto root = id;

  auto right = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 0.33f, nullptr, &id);
  const auto right_bottom =
      ImGui::DockBuilderSplitNode(right, ImGuiDir_Down, 0.40f, nullptr, &right);

  const auto bottom = ImGui::DockBuilderSplitNode(id, ImGuiDir_Down, 0.25f, nullptr, &id);

  ImGui::DockBuilderDockWindow("Viewport", root);
  ImGui::DockBuilderDockWindow(lang.window.tileset_dock.c_str(), right);
  ImGui::DockBuilderDockWindow(lang.window.property_dock.c_str(), right);
  ImGui::DockBuilderDockWindow(lang.window.component_dock.c_str(), right);
  ImGui::DockBuilderDockWindow(lang.window.animation_dock.c_str(), right_bottom);
  ImGui::DockBuilderDockWindow(lang.window.layer_dock.c_str(), right_bottom);
  ImGui::DockBuilderDockWindow(lang.window.log_dock.c_str(), bottom);

  ImGui::DockBuilderFinish(id);
}

void reset_layout(const Model& model, const ImGuiID root_id, Dispatcher& dispatcher)
{
  load_default_layout(model, root_id);
  dispatcher.enqueue<ResetDockVisibilitiesEvent>();
}

void check_for_missing_layout_file(const Model& model,
                                   const ImGuiID root_id,
                                   Dispatcher& dispatcher)
{
  const auto& ini_path = get_widget_ini_path();

  if (!fs::exists(ini_path)) {
    spdlog::warn("[UI] Resetting layout because 'imgui.ini' file was missing");
    ui::reset_layout(model, root_id, dispatcher);

    const auto str = ini_path.string();
    ImGui::SaveIniSettingsToDisk(str.c_str());
  }
}

}  // namespace tactile::ui
