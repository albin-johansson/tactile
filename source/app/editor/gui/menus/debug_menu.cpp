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

#include <centurion.hpp>
#include <imgui.h>

#include "editor/gui/alignment.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "meta/build.hpp"

namespace tactile {

void DebugMenu::update()
{
  if (scoped::Menu menu{"Debug"}; menu.is_open()) {
    mShowMetrics = ImGui::MenuItem(TAC_ICON_METRICS " Show Metrics...");

    if constexpr (is_debug_build) {
      ImGui::Separator();
      mShowDemo = ImGui::MenuItem("Show Demo Window...");
      mShowStyleEditor = ImGui::MenuItem("Show Style Editor...");
    }
  }
}

void DebugMenu::update_windows()
{
  if (mShowMetrics) {
    center_next_window_on_appearance();
    ImGui::ShowMetricsWindow(&mShowMetrics);
  }

  if constexpr (is_debug_build) {
    if (mShowDemo) {
      ImGui::ShowDemoWindow(&mShowDemo);
    }

    if (mShowStyleEditor) {
      scoped::Window editor{"Style Editor"};
      ImGui::ShowStyleEditor();
    }
  }
}

}  // namespace tactile
