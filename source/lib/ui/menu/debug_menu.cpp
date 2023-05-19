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

#include "common/predef.hpp"
#include "io/directories.hpp"
#include "lang/language.hpp"
#include "ui/style/alignment.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

struct DebugMenuState final {
  bool show_metrics {};
  bool show_demo {};
  bool show_style_editor {};
};

inline constinit DebugMenuState gMenuState;

}  // namespace

void show_debug_menu(const Model&, Dispatcher&)
{
  const auto& lang = get_current_language();

  if (const Menu menu {lang.menu.debug.c_str()}; menu.is_open()) {
    gMenuState.show_metrics = ImGui::MenuItem(lang.action.show_metrics.c_str());

    if constexpr (kIsDebugBuild) {
      ImGui::Separator();
      gMenuState.show_demo = ImGui::MenuItem("Show Demo Window...");
      gMenuState.show_style_editor = ImGui::MenuItem("Show Style Editor...");
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.open_persistent_file_dir.c_str())) {
      open_directory(get_persistent_file_dir());
    }
  }

  if (gMenuState.show_metrics) {
    center_next_window_on_appearance();
    ImGui::ShowMetricsWindow(&gMenuState.show_metrics);
  }

  if (gMenuState.show_demo) {
    ImGui::ShowDemoWindow(&gMenuState.show_demo);
  }

  if (gMenuState.show_style_editor) {
    const Window editor {"Style Editor"};
    ImGui::ShowStyleEditor();
  }
}

}  // namespace tactile::ui
