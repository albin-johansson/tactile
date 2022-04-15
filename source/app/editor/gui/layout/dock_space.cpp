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

#include <optional>  // optional

#include <imgui.h>

#include "io/directories.hpp"
#include "io/persistence/preferences.hpp"
#include "load_default_layout.hpp"
#include "tactile.hpp"

namespace tactile {
namespace {

constinit bool _is_initialized = false;
constinit std::optional<ImGuiID> _root_id;

}  // namespace

void update_dock_space()
{
  _root_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
  if (!_is_initialized) {
    const auto size = ImGui::GetMainViewport()->Size;
    if (size.x > 0 && size.y > 0) {
      const auto& prefs = get_preferences();
      if (!prefs.will_restore_layout() || !exists(widget_ini_path())) {
        load_default_layout(_root_id.value(), false);
      }
      _is_initialized = true;
    }
  }
}

void reset_layout()
{
  load_default_layout(_root_id.value(), true);
}

}  // namespace tactile
