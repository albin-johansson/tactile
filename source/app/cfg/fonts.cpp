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

#include "fonts.hpp"

#include <IconsFontAwesome5.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>

#include "io/directories.hpp"
#include "io/persistence/preferences.hpp"
#include "misc/logging.hpp"

namespace tactile {
namespace {

constexpr auto _roboto_path = "assets/fonts/roboto/Roboto-Regular.ttf";
constexpr auto _fa_path = "assets/fonts/fa/fa-solid-900.otf";
constexpr ImWchar _icon_range[] = {ICON_MIN_FA, ICON_MAX_FA, 0};

}  // namespace

void reload_fonts()
{
  static const auto roboto = find_resource(_roboto_path).string();
  static const auto fa = find_resource(_fa_path).string();

  log_debug("Reloading fonts...");

  auto& io = ImGui::GetIO();
  const auto scale = io.DisplayFramebufferScale;

  io.Fonts->Clear();

  const auto& prefs = get_preferences();
  const auto useDefaultFont = prefs.use_default_font();
  const auto size = useDefaultFont ? 13.0f : static_cast<float>(prefs.font_size());

  if (useDefaultFont) {
    ImFontConfig config{};
    config.SizePixels = size * scale.x;
    io.Fonts->AddFontDefault(&config);
  }
  else {
    io.Fonts->AddFontFromFileTTF(roboto.c_str(), size * scale.x);
  }

  /* The global scale is 1 on most platforms, and 0.5 on macOS */
  io.FontGlobalScale = 1.0f / scale.x;

  ImFontConfig config{};
  config.MergeMode = true;
  config.GlyphMinAdvanceX = size * scale.x;
  config.GlyphMaxAdvanceX = config.GlyphMinAdvanceX;
  config.GlyphOffset = {0, 2};
  io.Fonts->AddFontFromFileTTF(fa.c_str(), size * scale.x, &config, _icon_range);

  io.Fonts->Build();

  ImGui_ImplOpenGL3_DestroyFontsTexture();
  ImGui_ImplOpenGL3_CreateFontsTexture();

  ImGui::GetStyle().ScaleAllSizes(1.0f);
}

auto get_default_font_size() -> int32
{
  return 14;
}

auto get_min_font_size() -> int32
{
  return 8;
}

auto get_max_font_size() -> int32
{
  return 32;
}

}  // namespace tactile