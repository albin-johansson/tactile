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

#include <IconsFontAwesome5.h>
#include <imgui.h>

#include "editor/gui/themes.hpp"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "io/directories.hpp"
#include "io/persistence/preferences.hpp"
#include "meta/build.hpp"
#include "misc/logging.hpp"

namespace tactile {
namespace {

void _load_fonts()
{
  auto& io = ImGui::GetIO();
  io.Fonts->Clear();

  constexpr float size = on_osx ? 13.0f : 11.0f;
  constexpr float scale = on_osx ? 2.0f : 1.0f;  // io.DisplayFramebufferScale

  io.FontGlobalScale = 1.0f / scale;

  if constexpr (on_osx) {
    const auto path = find_resource("resources/fonts/roboto/Roboto-Regular.ttf");
    io.Fonts->AddFontFromFileTTF(path.string().c_str(), size * scale);
  }
  else {
    io.Fonts->AddFontDefault();
  }

  ImFontConfig config{};
  config.MergeMode = true;
  config.PixelSnapH = true;
  config.GlyphOffset = {0, 1.5f};
  config.GlyphMinAdvanceX = size * scale;
  config.GlyphMaxAdvanceX = config.GlyphMinAdvanceX;

  static constexpr ImWchar icon_range[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
  const auto path = find_resource("resources/fonts/fa/fa-solid-900.otf");
  io.Fonts->AddFontFromFileTTF(path.string().c_str(), 13.0f * scale, &config, icon_range);

  io.Fonts->Build();
}

}  // namespace

ImGuiContext::ImGuiContext(cen::window& window, cen::gl_context& context)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  auto& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // NOLINT
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // NOLINT
  io.WantCaptureKeyboard = true;

  ImGui_ImplSDL2_InitForOpenGL(window.get(), context.get());
  if constexpr (on_osx) {
    mInitializedBackend = ImGui_ImplOpenGL3_Init("#version 150");
  }
  else {
    mInitializedBackend = ImGui_ImplOpenGL3_Init("#version 130");
  }

  _load_fonts();

  ImGui::StyleColorsDark();

  const auto& prefs = get_preferences();
  auto& style = ImGui::GetStyle();

  apply_style(style);
  apply_theme(style, prefs.get_theme());

  style.WindowBorderSize = prefs.has_window_border() ? 1.0f : 0.0f;

  log_debug("Initialized renderer backend... {}", mInitializedBackend ? "yes" : "no");
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
