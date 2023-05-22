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

#include "imgui_context.hpp"

#include <imgui.h>
#include <spdlog/spdlog.h>

#include "io/directories.hpp"
#include "model/context.hpp"
#include "ui/style/themes.hpp"

namespace tactile {

ImGuiContext::ImGuiContext()
{
  spdlog::debug("Initializing ImGui context");

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  static const auto layout_file_path = get_widget_ini_path().string();

  auto& io = ImGui::GetIO();
  io.ConfigFlags |= static_cast<ImGuiConfigFlags>(ImGuiConfigFlags_DockingEnable);
  io.IniFilename = layout_file_path.c_str();

  ImGui::StyleColorsDark();
  ui::apply_style(ImGui::GetStyle());
}

ImGuiContext::~ImGuiContext() noexcept
{
  ImGui::DestroyContext();
}

}  // namespace tactile
