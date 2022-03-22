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

#include "load_default_layout.hpp"

#include <imgui_internal.h>

#include "io/persistence/preferences.hpp"

namespace tactile {

void load_default_layout(ImGuiID id, const bool resetVisibility)
{
  ImGui::DockBuilderRemoveNodeChildNodes(id);

  const auto root = id;

  auto right = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 0.20f, nullptr, &id);
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
    get_preferences().reset_dock_visibilities();
  }
}

}  // namespace tactile
