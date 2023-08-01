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

#include "debug_menu.hpp"

#include <imgui.h>

#include "core/prelude.hpp"
#include "io/directories.hpp"
#include "model/events/misc_events.hpp"
#include "ui/style/alignment.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile {

void push_debug_menu(ModelView& model, DebugMenuState& state)
{
  const auto& strings = model.get_language_strings();

  if (const ui::Menu menu {strings.menu.debug.c_str()}; menu.is_open()) {
    state.show_metrics_dialog = ImGui::MenuItem(strings.action.show_metrics.c_str());

    if constexpr (kIsDebugBuild) {
      ImGui::Separator();
      state.show_demo_dialog = ImGui::MenuItem("Show Demo Window...");
      state.show_style_editor_dialog = ImGui::MenuItem("Show Style Editor...");
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.open_persistent_file_dir.c_str())) {
      model.enqueue<OpenDirectoryInFinderEvent>(get_persistent_file_dir());
    }
  }

  if (state.show_metrics_dialog) {
    ui::center_next_window_on_appearance();
    ImGui::ShowMetricsWindow(&state.show_metrics_dialog);
  }

  if (state.show_demo_dialog) {
    ImGui::ShowDemoWindow(&state.show_demo_dialog);
  }

  if (state.show_style_editor_dialog) {
    const ui::Window editor {"Style Editor"};
    ImGui::ShowStyleEditor();
  }
}

}  // namespace tactile
