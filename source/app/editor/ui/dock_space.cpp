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

#include "core/common/maybe.hpp"
#include "io/directories.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile::ui {
namespace {

constinit Maybe<ImGuiID> _root_id;

}  // namespace

void update_dock_space()
{
  static bool initialized = false;

  _root_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
  if (!initialized) {
    const auto size = ImGui::GetMainViewport()->Size;
    if (size.x > 0 && size.y > 0) {
      const auto& prefs = io::get_preferences();

      if (!prefs.will_restore_layout() || !exists(io::widget_ini_path())) {
        load_default_layout(_root_id.value(), false);
      }

      initialized = true;
    }
  }
}

void load_default_layout(ImGuiID id, const bool resetVisibility)
{
  ImGui::DockBuilderRemoveNodeChildNodes(id);

  const auto root = id;

  auto       right = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 0.20f, nullptr, &id);
  const auto rightBottom =
      ImGui::DockBuilderSplitNode(right, ImGuiDir_Down, 0.25f, nullptr, &right);

  const auto bottom = ImGui::DockBuilderSplitNode(id, ImGuiDir_Down, 0.25f, nullptr, &id);

  ImGui::DockBuilderDockWindow("Viewport", root);
  ImGui::DockBuilderDockWindow("Tilesets", right);
  ImGui::DockBuilderDockWindow("Properties", right);
  ImGui::DockBuilderDockWindow("Components", right);
  ImGui::DockBuilderDockWindow("Layers", rightBottom);
  ImGui::DockBuilderDockWindow("Log", bottom);

  ImGui::DockBuilderFinish(id);

  if (resetVisibility) {
    io::get_preferences().reset_dock_visibilities();
  }
}

void reset_layout()
{
  load_default_layout(_root_id.value(), true);
}

}  // namespace tactile::ui
