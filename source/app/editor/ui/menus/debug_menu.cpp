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

#include "debug_menu.hpp"

#include <imgui.h>

#include "editor/ui/alignment.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/scoped.hpp"
#include "io/directories.hpp"
#include "meta/build.hpp"

namespace tactile {

void update_debug_menu()
{
  static bool show_metrics = false;
  static bool show_demo = false;
  static bool show_style_editor = false;

  if (scoped::Menu menu{"Debug"}; menu.is_open()) {
    show_metrics = ImGui::MenuItem(TAC_ICON_METRICS " Show Metrics...");

    if constexpr (is_debug_build) {
      ImGui::Separator();
      show_demo = ImGui::MenuItem("Show Demo Window...");
      show_style_editor = ImGui::MenuItem("Show Style Editor...");
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Open Persistent File Directory...")) {
      io::open_directory(io::persistent_file_dir());
    }
  }

  if (show_metrics) {
    center_next_window_on_appearance();
    ImGui::ShowMetricsWindow(&show_metrics);
  }

  if (show_demo) {
    ImGui::ShowDemoWindow(&show_demo);
  }

  if (show_style_editor) {
    scoped::Window editor{"Style Editor"};
    ImGui::ShowStyleEditor();
  }
}

}  // namespace tactile
