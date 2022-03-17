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
namespace {

bool _show_metrics = false;
bool _show_demo = false;
bool _show_style_editor = false;

}  // namespace

void update_debug_menu()
{
  if (scoped::Menu menu{"Debug"}; menu.is_open()) {
    _show_metrics = ImGui::MenuItem(TAC_ICON_METRICS " Show Metrics...");

    if constexpr (is_debug_build) {
      ImGui::Separator();
      _show_demo = ImGui::MenuItem("Show Demo Window...");
      _show_style_editor = ImGui::MenuItem("Show Style Editor...");
    }
  }
}

void update_debug_menu_windows()
{
  if (_show_metrics) {
    center_next_window_on_appearance();
    ImGui::ShowMetricsWindow(&_show_metrics);
  }

  if constexpr (is_debug_build) {
    if (_show_demo) {
      ImGui::ShowDemoWindow(&_show_demo);
    }

    if (_show_style_editor) {
      scoped::Window editor{"Style Editor"};
      ImGui::ShowStyleEditor();
    }
  }
}

}  // namespace tactile
