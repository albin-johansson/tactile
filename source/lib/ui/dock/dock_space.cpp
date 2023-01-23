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

#include "common/type/maybe.hpp"
#include "common/type/path.hpp"
#include "io/directories.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/settings.hpp"

namespace tactile::ui {
namespace {

inline constinit Maybe<ImGuiID> gDockRootId;

}  // namespace

void update_dock_space()
{
  static bool initialized = false;

  gDockRootId = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
  if (!initialized) {
    const auto size = ImGui::GetMainViewport()->Size;
    if (size.x > 0 && size.y > 0) {
      const auto& settings = get_settings();

      if (!settings.test_flag(SETTINGS_RESTORE_LAYOUT_BIT) ||
          !fs::exists(get_widget_ini_path())) {
        load_default_layout(gDockRootId.value(), false);
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
  ImGui::DockBuilderDockWindow(lang.window.animation_dock.c_str(), right_bottom);
  ImGui::DockBuilderDockWindow(lang.window.layer_dock.c_str(), right_bottom);
  ImGui::DockBuilderDockWindow(lang.window.log_dock.c_str(), bottom);

  ImGui::DockBuilderFinish(id);

  if (reset_visibility) {
    get_settings().reset_dock_visibilities();
  }
}

void reset_layout()
{
  load_default_layout(gDockRootId.value(), true);
}

}  // namespace tactile::ui
