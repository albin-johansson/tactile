// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "themes.hpp"

#include <algorithm>  // clamp

#include <imgui.h>

#include "tactile/base/prelude.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/exception.hpp"
#include "ui/style/colors.hpp"

namespace tactile::ui {
namespace {

struct ThemeCfg final {
  ImVec4 accent {};
  ImVec4 accent_active {};
  ImVec4 window {};
  ImVec4 child {};
  ImVec4 text {};
};

[[nodiscard]] auto create_dark_theme(const uint16 hue, const int32 saturation) -> ThemeCfg
{
  TACTILE_ASSERT(hue < 360);
  TACTILE_ASSERT(saturation <= 100);

  ThemeCfg cfg;

  const auto h = static_cast<float>(hue) / 360.0f;
  const auto s = static_cast<float>(saturation) / 100.0f;

  cfg.window = ImColor::HSV(h, 0.20f, 0.05f);
  cfg.child = cfg.window;

  cfg.accent = ImColor::HSV(h, s, 0.30f);
  cfg.accent_active = ImColor::HSV(h, s, 0.70f);

  cfg.text = ImColor::HSV(0, 0, 1.0f);

  return cfg;
}

[[nodiscard]] auto create_light_theme(const uint16 hue, const int32 saturation)
    -> ThemeCfg
{
  TACTILE_ASSERT(hue < 360);
  TACTILE_ASSERT(saturation <= 100);

  ThemeCfg cfg;

  const auto h = static_cast<float>(hue) / 360.0f;
  const auto s = static_cast<float>(saturation) / 100.0f;

  cfg.window = ImColor::HSV(h, 0.1f, 1.0f);
  cfg.child = cfg.window;

  cfg.accent = ImColor::HSV(h, s, 0.60f);
  cfg.accent_active = ImColor::HSV(h, s, 0.80f);

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
  set(ImGuiCol_TextSelectedBg, with_alpha(cfg.accent, 0.40f));

  // This causes weird issues, so skip for now
  // set(ImGuiCol_TextDisabled, ImVec4 {0xCC, 0xCC, 0xCC, 0xFF});

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
      throw Exception {"Invalid theme enumerator!"};
  }
}

void apply_style(ImGuiStyle& style)
{
  // Main
  style.WindowPadding = ImVec2 {8, 8};
  style.FramePadding = ImVec2 {4, 4};
  style.CellPadding = ImVec2 {4, 4};
  style.ItemSpacing = ImVec2 {8, 8};
  style.ItemInnerSpacing = ImVec2 {8, 4};
  style.IndentSpacing = 12;
  style.ScrollbarSize = 12;
  style.GrabMinSize = 12;

  // Borders
  style.WindowBorderSize = 0;
  style.ChildBorderSize = 0;
  style.PopupBorderSize = 0;
  style.FrameBorderSize = 0;
  style.TabBorderSize = 0;

  // Rounding
  style.ChildRounding = 6;
  style.WindowRounding = 6;
  style.FrameRounding = 6;
  style.PopupRounding = 6;
  style.ScrollbarRounding = 6;
  style.GrabRounding = 6;
  style.TabRounding = 6;

  // Widgets
  style.WindowTitleAlign = ImVec2 {0.5f, 0.5f};
  style.WindowMenuButtonPosition = ImGuiDir_None;
  style.ColorButtonPosition = ImGuiDir_Right;
  style.ButtonTextAlign = ImVec2 {0.5f, 0.5f};
  style.SelectableTextAlign = ImVec2 {0.0f, 0.5f};
  style.SeparatorTextBorderSize = 1.0f;
  style.SeparatorTextAlign = ImVec2 {0.5f, 0.5f};
  style.SeparatorTextPadding = ImVec2 {20, 3};

  // Rendering
  style.DisabledAlpha = 0.4f;
}

void apply_theme(ImGuiStyle& style, const EditorTheme theme, int32 saturation)
{
  saturation = std::clamp(saturation, 0, 100);

  switch (theme) {
    case EditorTheme::DearDark:
      ImGui::StyleColorsDark(&style);
      break;

    case EditorTheme::DearLight:
      ImGui::StyleColorsLight(&style);
      break;

    case EditorTheme::Ruby:
      apply_theme_from_config(style, create_dark_theme(0, saturation));
      break;

    case EditorTheme::Emerald:
      apply_theme_from_config(style, create_dark_theme(141, saturation));
      break;

    case EditorTheme::Sapphire:
      apply_theme_from_config(style, create_dark_theme(211, saturation));
      break;

    case EditorTheme::Joker:
      apply_theme_from_config(style, create_dark_theme(268, saturation));
      break;

    case EditorTheme::Amethyst:
      apply_theme_from_config(style, create_dark_theme(318, saturation));
      break;

    case EditorTheme::Raspberry:
      apply_theme_from_config(style, create_dark_theme(346, saturation));
      break;

    case EditorTheme::Amber:
      apply_theme_from_config(style, create_dark_theme(23, saturation));
      break;

    case EditorTheme::Gasoline:
      apply_theme_from_config(style, create_dark_theme(82, saturation));
      break;

    case EditorTheme::Bumblebee:
      apply_theme_from_config(style, create_dark_theme(56, saturation));
      break;

    case EditorTheme::Diamond:
      [[fallthrough]];
    case EditorTheme::Nocturnal:
      apply_theme_from_config(style, create_dark_theme(180, saturation));
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
      apply_theme_from_config(style, create_light_theme(275, saturation));
      break;

    case EditorTheme::Vanilla:
      [[fallthrough]];
    case EditorTheme::Frost:
      apply_theme_from_config(style, create_light_theme(180, saturation));
      break;

    case EditorTheme::Rose:
      apply_theme_from_config(style, create_light_theme(0, saturation));
      break;
  }
}

}  // namespace tactile::ui
