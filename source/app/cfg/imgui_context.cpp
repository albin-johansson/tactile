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

#include "imgui_context.hpp"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>
#include <spdlog/spdlog.h>

#include "editor/gui/themes.hpp"
#include "io/directories.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile {

ImGuiContext::ImGuiContext(cen::window& window, cen::gl_context& context)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  auto& io = ImGui::GetIO();
  io.ConfigFlags |= static_cast<ImGuiConfigFlags>(ImGuiConfigFlags_DockingEnable);
  io.WantCaptureKeyboard = true;

  static const auto ini = io::widget_ini_path().string();
  io.IniFilename = ini.c_str();

  ImGui_ImplSDL2_InitForOpenGL(window.get(), context.get());
  mInitializedBackend = ImGui_ImplOpenGL3_Init();

  ImGui::StyleColorsDark();

  const auto& prefs = get_preferences();
  auto& style = ImGui::GetStyle();

  apply_style(style);
  apply_theme(style, prefs.get_theme());

  style.WindowBorderSize = prefs.has_window_border() ? 1.0f : 0.0f;

  spdlog::debug("Initialized renderer backend... {}", mInitializedBackend ? "yes" : "no");
}

ImGuiContext::~ImGuiContext()
{
  if (mInitializedBackend) {
    ImGui_ImplOpenGL3_Shutdown();
  }
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

}  // namespace tactile
