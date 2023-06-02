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

#include "fonts.hpp"

#include <IconsFontAwesome6.h>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "io/directories.hpp"

namespace tactile::ui {
namespace {

constexpr auto kFontRobotoPath = "assets/fonts/roboto/Roboto-Regular.ttf";
constexpr auto kFontFaPath = "assets/fonts/fa/fa-solid-900.otf";
constexpr ImWchar kFontIconRange[] = {ICON_MIN_FA, ICON_MAX_FA, 0};

}  // namespace

void reload_imgui_fonts(const Settings& settings)
{
  spdlog::debug("[UI] Reloading fonts");

  const auto using_default_font = settings.test_flag(SETTINGS_USE_DEFAULT_FONT_BIT);
  const auto font_size =
      using_default_font ? 13.0f : static_cast<float>(settings.get_font_size());

  static const auto roboto_path = find_resource(kFontRobotoPath).string();
  static const auto fa_path = find_resource(kFontFaPath).string();

  auto& io = ImGui::GetIO();
  const auto fb_scale = io.DisplayFramebufferScale;

  io.Fonts->Clear();

  if (using_default_font) {
    ImFontConfig default_config {};
    default_config.SizePixels = font_size * fb_scale.x;
    io.Fonts->AddFontDefault(&default_config);
  }
  else {
    io.Fonts->AddFontFromFileTTF(roboto_path.c_str(), font_size * fb_scale.x);
  }

  // The global scale is 1 on most platforms, and 0.5 on macOS
  io.FontGlobalScale = 1.0f / fb_scale.x;

  ImFontConfig fa_config {};
  fa_config.MergeMode = true;
  fa_config.GlyphMinAdvanceX = font_size * fb_scale.x;
  fa_config.GlyphMaxAdvanceX = fa_config.GlyphMinAdvanceX;
  fa_config.GlyphOffset = {0, 2};
  io.Fonts->AddFontFromFileTTF(fa_path.c_str(),
                               font_size * fb_scale.x,
                               &fa_config,
                               kFontIconRange);

  io.Fonts->Build();

  ImGui::GetStyle().ScaleAllSizes(1.0f);
}

}  // namespace tactile::ui
