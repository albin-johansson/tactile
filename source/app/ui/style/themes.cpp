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

#include "themes.hpp"

#include <imgui.h>

#include "colors.hpp"
#include "core/predef.hpp"
#include "core/debug/assert.hpp"
#include "core/debug/panic.hpp"

namespace tactile::ui {
namespace {

struct ThemeCfg final {
  ImVec4 accent {};
  ImVec4 accent_active {};
  ImVec4 window {};
  ImVec4 child {};
  ImVec4 text {};
};

[[nodiscard]] auto dark_theme_from_hue(const uint16 hue) -> ThemeCfg
{
  TACTILE_ASSERT(hue < 360);

  ThemeCfg cfg;

  const auto h = static_cast<float>(hue) / 360.0f;
  cfg.window = ImColor::HSV(h, 0.20f, 0.05f);
  cfg.child = cfg.window;

  cfg.accent = ImColor::HSV(h, 0.60f, 0.30f);
  cfg.accent_active = ImColor::HSV(h, 0.60f, 0.70f);

  cfg.text = ImColor::HSV(0, 0, 1.0f);

  return cfg;
}

[[nodiscard]] auto light_theme_from_hue(const uint16 hue) -> ThemeCfg
{
  TACTILE_ASSERT(hue < 360);

  ThemeCfg cfg;

  const auto h = static_cast<float>(hue) / 360.0f;
  constexpr auto s = static_cast<float>(10) / 100.0f;
  constexpr auto v = static_cast<float>(100) / 100.0f;

  cfg.window = ImColor::HSV(h, s, v);
  cfg.child = cfg.window;

  cfg.accent = ImColor::HSV(h, 0.60f, 0.60f);
  cfg.accent_active = ImColor::HSV(h, 0.60f, 0.80f);

  cfg.text = ImColor::HSV(0, 0, 0);

  return cfg;
}

void apply_theme_from_config(ImGuiStyle& style, const ThemeCfg& cfg)
{
  const auto set = [&style](const ImGuiCol_ index, const ImVec4& color) {
    style.Colors[index] = color;
  };

  const ImVec4 transparent {0, 0, 0, 0};

  const auto component = cfg.accent;
  const auto component_active = cfg.accent_active;
  const auto component_hovered = make_darker(component_active);

  set(ImGuiCol_Text, cfg.text);
  set(ImGuiCol_TextDisabled, {150, 150, 150, 0xFF});
  set(ImGuiCol_TextSelectedBg, with_alpha(cfg.accent, 0.40f));

  set(ImGuiCol_Border, make_darker(cfg.accent));
  set(ImGuiCol_BorderShadow, transparent);

  set(ImGuiCol_Separator, make_darker(component, 3));
  set(ImGuiCol_SeparatorHovered, component);
  set(ImGuiCol_SeparatorActive, component);

  set(ImGuiCol_WindowBg, cfg.window);
  set(ImGuiCol_PopupBg, cfg.window);
  set(ImGuiCol_ChildBg, cfg.child);

  set(ImGuiCol_FrameBg, component);
  set(ImGuiCol_FrameBgHovered, component_hovered);
  set(ImGuiCol_FrameBgActive, component_active);

  set(ImGuiCol_TitleBg, cfg.child);
  set(ImGuiCol_TitleBgActive, cfg.child);
  set(ImGuiCol_TitleBgCollapsed, cfg.child);

  set(ImGuiCol_Button, component);
  set(ImGuiCol_ButtonHovered, component_hovered);
  set(ImGuiCol_ButtonActive, component_active);

  set(ImGuiCol_MenuBarBg, make_darker(cfg.child));

  set(ImGuiCol_ScrollbarBg, cfg.child);
  set(ImGuiCol_ScrollbarGrab, component);
  set(ImGuiCol_ScrollbarGrabHovered, component_hovered);
  set(ImGuiCol_ScrollbarGrabActive, component_active);

  set(ImGuiCol_CheckMark, component_active);

  set(ImGuiCol_SliderGrab, cfg.window);
  set(ImGuiCol_SliderGrabActive, cfg.window);

  set(ImGuiCol_Header, component_active);
  set(ImGuiCol_HeaderHovered, component_hovered);
  set(ImGuiCol_HeaderActive, component_active);

  set(ImGuiCol_TabActive, component_active);
  set(ImGuiCol_TabUnfocusedActive, make_darker(component_active));
  set(ImGuiCol_Tab, make_darker(component));
  set(ImGuiCol_TabUnfocused, make_darker(component, 2));
  set(ImGuiCol_TabHovered, component_hovered);

  set(ImGuiCol_TableHeaderBg, cfg.child);
  set(ImGuiCol_TableRowBg, cfg.window);
  set(ImGuiCol_TableRowBgAlt, make_darker(cfg.window, 1));

  set(ImGuiCol_TableBorderStrong, cfg.accent);
  set(ImGuiCol_TableBorderLight, make_brighter(cfg.accent));

  set(ImGuiCol_DockingPreview, with_alpha(cfg.accent, 0.80f));
  set(ImGuiCol_DockingEmptyBg, with_alpha(cfg.accent, 0.80f));

  set(ImGuiCol_ResizeGrip, with_alpha(cfg.text, 0.20f));
  set(ImGuiCol_ResizeGripHovered, with_alpha(cfg.text, 0.80f));
  set(ImGuiCol_ResizeGripActive, with_alpha(cfg.text, 1.00f));

  set(ImGuiCol_PlotLines, with_alpha(cfg.text, 0.63f));
  set(ImGuiCol_PlotHistogram, with_alpha(cfg.text, 0.63f));
  set(ImGuiCol_PlotLinesHovered, cfg.accent);
  set(ImGuiCol_PlotHistogramHovered, cfg.accent);

  set(ImGuiCol_ModalWindowDimBg, {0.50f, 0.50f, 0.50f, 0.3f});
}

}  // namespace

auto human_readable_name(const EditorTheme theme) -> StringView
{
  switch (theme) {
    case EditorTheme::DearDark:
      return "Dear Dark";

    case EditorTheme::DearLight:
      return "Dear Light";

    case EditorTheme::Ruby:
      return "Ruby";

    case EditorTheme::Sapphire:
      return "Sapphire";

    case EditorTheme::Emerald:
      return "Emerald";

    case EditorTheme::Amethyst:
      return "Amethyst";

    case EditorTheme::Amber:
      return "Amber";

    case EditorTheme::Nocturnal:
      return "Nocturnal";

    case EditorTheme::Ash:
      return "Ash";

    case EditorTheme::Diamond:
      return "Diamond";

    case EditorTheme::Joker:
      return "Joker";

    case EditorTheme::Raspberry:
      return "Raspberry";

    case EditorTheme::Stealth:
      return "Stealth";

    case EditorTheme::Vanilla:
      return "Vanilla";

    case EditorTheme::Gasoline:
      return "Gasoline";

    case EditorTheme::Bumblebee:
      return "Bumblebee";

    case EditorTheme::Lavender:
      return "Lavender";

    case EditorTheme::Frost:
      return "Frost";

    case EditorTheme::Rose:
      return "Rose";

    default:
      throw TactileError {"Invalid theme enumerator!"};
  }
}

void apply_style(ImGuiStyle& style)
{
  style.WindowMenuButtonPosition = ImGuiDir_Left;
  style.WindowBorderSize = 0;

  if constexpr (on_macos) {
    style.WindowTitleAlign.x = 0.5f;
  }

  style.WindowPadding = ImVec2 {10, 10};
  style.FramePadding = ImVec2 {5, 5};
  style.CellPadding = ImVec2 {4, 4};

  style.ItemSpacing = ImVec2 {12, 8};
  style.ItemInnerSpacing = ImVec2 {4, 6};
  style.IndentSpacing = 15;

  style.GrabMinSize = 6;
  style.ScrollbarSize = 10;

  constexpr float rounding = 4;
  style.ChildRounding = 2;
  style.WindowRounding = rounding;
  style.FrameRounding = rounding;
  style.PopupRounding = rounding;
  style.ScrollbarRounding = rounding;
  style.GrabRounding = rounding;
  style.LogSliderDeadzone = rounding;
  style.TabRounding = rounding;
}

void apply_theme(ImGuiStyle& style, const EditorTheme theme)
{
  switch (theme) {
    case EditorTheme::DearDark:
      ImGui::StyleColorsDark(&style);
      break;

    case EditorTheme::DearLight:
      ImGui::StyleColorsLight(&style);
      break;

    case EditorTheme::Ruby:
      apply_theme_from_config(style, dark_theme_from_hue(0));
      break;

    case EditorTheme::Emerald:
      apply_theme_from_config(style, dark_theme_from_hue(141));
      break;

    case EditorTheme::Sapphire:
      apply_theme_from_config(style, dark_theme_from_hue(211));
      break;

    case EditorTheme::Joker:
      apply_theme_from_config(style, dark_theme_from_hue(268));
      break;

    case EditorTheme::Amethyst:
      apply_theme_from_config(style, dark_theme_from_hue(318));
      break;

    case EditorTheme::Raspberry:
      apply_theme_from_config(style, dark_theme_from_hue(346));
      break;

    case EditorTheme::Amber:
      apply_theme_from_config(style, dark_theme_from_hue(23));
      break;

    case EditorTheme::Gasoline:
      apply_theme_from_config(style, dark_theme_from_hue(82));
      break;

    case EditorTheme::Bumblebee:
      apply_theme_from_config(style, dark_theme_from_hue(56));
      break;

    case EditorTheme::Diamond:
      [[fallthrough]];
    case EditorTheme::Nocturnal:
      apply_theme_from_config(style, dark_theme_from_hue(180));
      break;

    case EditorTheme::Ash:
      apply_theme_from_config(style,
                              {.accent = {0.4f, 0.4f, 0.4f, 1.0f},
                               .accent_active = {0.5f, 0.5f, 0.5f, 1.0f},
                               .window = {0.0f, 0.0f, 0.0f, 1.0f},
                               .child = {0.0f, 0.0f, 0.0f, 1.0f},
                               .text = {1.0f, 1.0f, 1.0f, 1.0f}});
      break;

    case EditorTheme::Stealth:
      apply_theme_from_config(style,
                              ThemeCfg {
                                  .accent = {0.20f, 0.20f, 0.20f, 1.0f},
                                  .accent_active = {0.25f, 0.25f, 0.25f, 1},
                                  .window = {0.00f, 0.00f, 0.00f, 1},
                                  .child = {0.00f, 0.00f, 0.00f, 1},
                                  .text = {1.0f, 1.0f, 1.0f, 1},
                              });
      break;

    case EditorTheme::Lavender:
      apply_theme_from_config(style, light_theme_from_hue(275));
      break;

    case EditorTheme::Vanilla:
      [[fallthrough]];
    case EditorTheme::Frost:
      apply_theme_from_config(style, light_theme_from_hue(180));
      break;

    case EditorTheme::Rose:
      apply_theme_from_config(style, light_theme_from_hue(0));
      break;
  }
}

}  // namespace tactile::ui
