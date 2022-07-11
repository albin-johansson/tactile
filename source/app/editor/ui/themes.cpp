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

#include "meta/build.hpp"
#include "misc/panic.hpp"

namespace tactile::ui {
namespace {

struct ThemeCfg final
{
  ImVec4 accent {};
  ImVec4 accent_active {};
  ImVec4 window {};
  ImVec4 child {};
  ImVec4 text {};
};

[[nodiscard]] auto _theme_from_hue(const float hue) -> ThemeCfg
{
  ThemeCfg cfg;

  const auto h = hue / 255.0f;
  cfg.window = ImColor::HSV(h, 0.20f, 0.08f, 1.00f);
  cfg.child = ImColor::HSV(h, 0.60f, 0.40f, 1.00f);

  cfg.accent = ImColor::HSV(h, 0.70f, 0.60f, 1.00f);
  cfg.accent_active = ImColor::HSV(h, 0.70f, 0.80f, 1.00f);

  cfg.text = ImColor::HSV(h, 0.10f, 1.00f, 1.00f);

  return cfg;
}

[[nodiscard]] auto _with_alpha(const ImVec4& color, const float alpha) -> ImVec4
{
  return {color.x, color.y, color.z, alpha};
}

void _apply_theme_from_config(ImGuiStyle& style, const ThemeCfg& cfg)
{
  const auto set = [&style](const ImGuiCol_ index, const ImVec4& color) {
    style.Colors[index] = color;
  };

  const ImVec4 transparent {0, 0, 0, 0};

  const auto component = _with_alpha(cfg.accent, 0.60f);
  const auto componentActive = cfg.accent_active;
  const auto componentHovered = _with_alpha(componentActive, 0.70f);

  set(ImGuiCol_Text, cfg.text);
  set(ImGuiCol_TextDisabled, _with_alpha(cfg.text, 0.60f));
  set(ImGuiCol_TextSelectedBg, _with_alpha(cfg.accent, 0.40f));

  set(ImGuiCol_Border, _with_alpha(cfg.accent, 0.80f));
  set(ImGuiCol_BorderShadow, transparent);

  set(ImGuiCol_Separator, _with_alpha(component, 0.20f));
  set(ImGuiCol_SeparatorHovered, component);
  set(ImGuiCol_SeparatorActive, component);

  set(ImGuiCol_WindowBg, cfg.window);
  set(ImGuiCol_PopupBg, cfg.window);
  set(ImGuiCol_ChildBg, _with_alpha(cfg.child, 0.10f));

  set(ImGuiCol_FrameBg, _with_alpha(cfg.child, 0.70f));
  set(ImGuiCol_FrameBgHovered, _with_alpha(cfg.accent, 0.70f));
  set(ImGuiCol_FrameBgActive, _with_alpha(cfg.accent, 0.70f));

  set(ImGuiCol_TitleBg, cfg.child);
  set(ImGuiCol_TitleBgActive, cfg.child);
  set(ImGuiCol_TitleBgCollapsed, cfg.child);

  set(ImGuiCol_Button, component);
  set(ImGuiCol_ButtonHovered, componentHovered);
  set(ImGuiCol_ButtonActive, componentActive);

  set(ImGuiCol_MenuBarBg, _with_alpha(cfg.child, 0.57f));

  set(ImGuiCol_ScrollbarBg, cfg.child);
  set(ImGuiCol_ScrollbarGrab, component);
  set(ImGuiCol_ScrollbarGrabHovered, componentHovered);
  set(ImGuiCol_ScrollbarGrabActive, componentActive);

  set(ImGuiCol_CheckMark, cfg.accent);

  set(ImGuiCol_SliderGrab, component);
  set(ImGuiCol_SliderGrabActive, componentActive);

  set(ImGuiCol_Header, componentActive);
  set(ImGuiCol_HeaderHovered, componentHovered);
  set(ImGuiCol_HeaderActive, componentActive);

  set(ImGuiCol_TabActive, componentActive);
  set(ImGuiCol_TabUnfocusedActive, _with_alpha(componentActive, 0.80f));
  set(ImGuiCol_Tab, _with_alpha(component, 0.25f));
  set(ImGuiCol_TabUnfocused, _with_alpha(component, 0.25f));
  set(ImGuiCol_TabHovered, componentHovered);

  set(ImGuiCol_TableHeaderBg, cfg.child);
  set(ImGuiCol_TableRowBg, cfg.window);
  set(ImGuiCol_TableRowBgAlt, _with_alpha(cfg.child, 0.15f));
  set(ImGuiCol_TableBorderStrong, _with_alpha(cfg.text, 0.20f));
  set(ImGuiCol_TableBorderLight, _with_alpha(cfg.text, 0.10f));

  set(ImGuiCol_DockingPreview, _with_alpha(cfg.accent, 0.80f));
  set(ImGuiCol_DockingEmptyBg, _with_alpha(cfg.accent, 0.80f));

  set(ImGuiCol_ResizeGrip, _with_alpha(cfg.text, 0.20f));
  set(ImGuiCol_ResizeGripHovered, _with_alpha(cfg.text, 0.80f));
  set(ImGuiCol_ResizeGripActive, _with_alpha(cfg.text, 1.00f));

  set(ImGuiCol_PlotLines, _with_alpha(cfg.text, 0.63f));
  set(ImGuiCol_PlotHistogram, _with_alpha(cfg.text, 0.63f));
  set(ImGuiCol_PlotLinesHovered, cfg.accent);
  set(ImGuiCol_PlotHistogramHovered, cfg.accent);

  set(ImGuiCol_ModalWindowDimBg, {0.50f, 0.50f, 0.50f, 0.3f});
}

}  // namespace

auto human_readable_name(const EditorTheme theme) -> std::string_view
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

    default:
      throw TactileError("Invalid theme enumerator!");
  }
}

void apply_style(ImGuiStyle& style)
{
  style.WindowMenuButtonPosition = ImGuiDir_Left;
  style.WindowBorderSize = 0;

  if constexpr (on_osx) {
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
      _apply_theme_from_config(style, _theme_from_hue(0));
      break;

    case EditorTheme::Emerald:
      _apply_theme_from_config(style, _theme_from_hue(100));
      break;

    case EditorTheme::Diamond:
      _apply_theme_from_config(style, _theme_from_hue(120));
      break;

    case EditorTheme::Sapphire:
      _apply_theme_from_config(style, _theme_from_hue(150));
      break;

    case EditorTheme::Joker:
      _apply_theme_from_config(style, _theme_from_hue(190));
      break;

    case EditorTheme::Amethyst:
      _apply_theme_from_config(style, _theme_from_hue(225));
      break;

    case EditorTheme::Raspberry:
      _apply_theme_from_config(style, _theme_from_hue(245));
      break;

    case EditorTheme::Amber:
      _apply_theme_from_config(style, _theme_from_hue(272));
      break;

    case EditorTheme::Nocturnal:
      _apply_theme_from_config(style,
                               {.accent = {0.0f, 0.5f, 0.5f, 1.0f},
                                .accent_active = {0.0f, 0.6f, 0.6f, 1.0f},
                                .window = {0.04f, 0.04f, 0.04f, 1.0f},
                                .child = {0.1f, 0.1f, 0.1f, 1.0f},
                                .text = {1.0f, 1.0f, 1.0f, 1.0f}});
      break;

    case EditorTheme::Ash:
      _apply_theme_from_config(style,
                               {.accent = {0.4f, 0.4f, 0.4f, 1.0f},
                                .accent_active = {0.5f, 0.5f, 0.5f, 1.0f},
                                .window = {0.0f, 0.0f, 0.0f, 1.0f},
                                .child = {0.1f, 0.1f, 0.1f, 1.0f},
                                .text = {1.0f, 1.0f, 1.0f, 1.0f}});
      break;

    case EditorTheme::Stealth:
      _apply_theme_from_config(style,
                               ThemeCfg {
                                   .accent = {0.20f, 0.20f, 0.20f, 1.0f},
                                   .accent_active = {0.25f, 0.25f, 0.25f, 1},
                                   .window = {0.00f, 0.00f, 0.00f, 1},
                                   .child = {0.08f, 0.08f, 0.08f, 1},
                                   .text = {1.0f, 1.0f, 1.0f, 1},
                               });
      break;

    case EditorTheme::Vanilla:
      _apply_theme_from_config(style,
                               ThemeCfg {
                                   .accent = {0.84f, 0.82f, 0.67f, 1},
                                   .accent_active = {0.94f, 0.92f, 0.77f, 1},
                                   .window = {0.90f, 0.88f, 0.73f, 1},
                                   .child = {0.80f, 0.78f, 0.64f, 1},
                                   .text = {0.10f, 0.10f, 0.10f, 1},
                               });
      break;
  }
}

}  // namespace tactile::ui
