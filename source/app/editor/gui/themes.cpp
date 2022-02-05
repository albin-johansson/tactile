#include "themes.hpp"

#include <imgui.h>

#include "misc/throw.hpp"

namespace tactile {
namespace {

constexpr float _accent_opacity = 0.65f;
constexpr float _area_opacity = 0.40f;

struct theme_cfg final
{
  ImVec4 accent{};
  ImVec4 bg{};
  ImVec4 area{};
  ImVec4 text{};
};

[[nodiscard]] auto _theme_from_hue(const float hue) -> theme_cfg
{
  theme_cfg cfg;

  const auto h = hue / 255.0f;
  cfg.bg = ImColor::HSV(h, 0.20f, 0.08f, 1.00f);
  cfg.area = ImColor::HSV(h, 0.60f, 0.40f, _area_opacity);
  cfg.accent = ImColor::HSV(h, 0.70f, 0.60f, _accent_opacity);
  cfg.text = ImColor::HSV(h, 0.10f, 1.00f, 1.00f);

  return cfg;
}

[[nodiscard]] auto _with_alpha(const ImVec4& color, const float alpha) -> ImVec4
{
  return {color.x, color.y, color.z, alpha};
}

void _apply_theme_from_config(ImGuiStyle& style, const theme_cfg& cfg)
{
  const auto set = [&style](const ImGuiCol_ index, const ImVec4& color) {
    style.Colors[index] = color;
  };

  const ImVec4 transparent{0, 0, 0, 0};
  //  const ImVec4 magenta{1.0f, 0.2f, 1.0f, 1.0f};
  //  const ImVec4 cyan{0.2f, 0.8f, 0.8f, 1.0f};
  //  const ImVec4 red{1.0f, 0.0f, 0.0f, 1.0f};
  //  const ImVec4 green{0.0f, 1.0f, 0.0f, 1.0f};
  //  const ImVec4 blue{0.0f, 0.0f, 1.0f, 1.0f};

  const auto component = _with_alpha(cfg.accent, 0.60f);
  const auto componentActive = _with_alpha(component, 1.00f);
  const auto componentHover = _with_alpha(component, 0.80f);

  set(ImGuiCol_Text, cfg.text);
  set(ImGuiCol_TextDisabled, _with_alpha(cfg.text, 0.60f));
  set(ImGuiCol_TextSelectedBg, _with_alpha(cfg.accent, 0.40f));

  set(ImGuiCol_Border, _with_alpha(cfg.accent, 0.50f));
  set(ImGuiCol_BorderShadow, transparent);

  set(ImGuiCol_Separator, _with_alpha(cfg.accent, 0.15f));
  set(ImGuiCol_SeparatorHovered, _with_alpha(cfg.accent, 0.15f));
  set(ImGuiCol_SeparatorActive, _with_alpha(cfg.accent, 0.15f));

  set(ImGuiCol_WindowBg, cfg.bg);
  set(ImGuiCol_PopupBg, _with_alpha(cfg.bg, 0.95f));
  set(ImGuiCol_ChildBg, _with_alpha(cfg.area, 0.10f));

  set(ImGuiCol_FrameBg, _with_alpha(cfg.area, 0.70f));
  set(ImGuiCol_FrameBgHovered, _with_alpha(cfg.accent, 0.70f));
  set(ImGuiCol_FrameBgActive, _with_alpha(cfg.accent, 0.70f));

  set(ImGuiCol_TitleBg, _with_alpha(cfg.area, 1.00f));
  set(ImGuiCol_TitleBgActive, _with_alpha(cfg.area, 1.00f));
  set(ImGuiCol_TitleBgCollapsed, _with_alpha(cfg.area, 1.00f));

  set(ImGuiCol_Button, component);
  set(ImGuiCol_ButtonHovered, componentHover);
  set(ImGuiCol_ButtonActive, componentActive);

  set(ImGuiCol_MenuBarBg, _with_alpha(cfg.area, 0.57f));
  set(ImGuiCol_ScrollbarBg, _with_alpha(cfg.area, 1.00f));
  set(ImGuiCol_ScrollbarGrab, _with_alpha(cfg.accent, 0.31f));
  set(ImGuiCol_ScrollbarGrabHovered, _with_alpha(cfg.accent, 0.78f));
  set(ImGuiCol_ScrollbarGrabActive, _with_alpha(cfg.accent, 1.00f));

  set(ImGuiCol_CheckMark, _with_alpha(cfg.accent, 1.00f));
  set(ImGuiCol_SliderGrab, _with_alpha(cfg.accent, 0.24f));
  set(ImGuiCol_SliderGrabActive, _with_alpha(cfg.accent, 1.00f));

  set(ImGuiCol_Header, _with_alpha(cfg.accent, 0.70f));
  set(ImGuiCol_HeaderHovered, _with_alpha(cfg.accent, 0.80f));
  set(ImGuiCol_HeaderActive, _with_alpha(cfg.accent, 1.00f));

  set(ImGuiCol_TabActive, component);
  set(ImGuiCol_TabUnfocusedActive, _with_alpha(component, 0.80f));
  set(ImGuiCol_Tab, _with_alpha(component, 0.25f));
  set(ImGuiCol_TabUnfocused, _with_alpha(component, 0.25f));
  set(ImGuiCol_TabHovered, componentHover);

  set(ImGuiCol_TableHeaderBg, _with_alpha(cfg.area, 1.00f));
  set(ImGuiCol_TableRowBg, cfg.bg);
  set(ImGuiCol_TableRowBgAlt, _with_alpha(cfg.area, 0.15f));
  set(ImGuiCol_TableBorderStrong, _with_alpha(cfg.text, 0.10f));
  set(ImGuiCol_TableBorderLight, _with_alpha(cfg.text, 0.10f));

  set(ImGuiCol_DockingPreview, _with_alpha(cfg.accent, 0.80f));
  set(ImGuiCol_DockingEmptyBg, _with_alpha(cfg.accent, 0.80f));

  set(ImGuiCol_ResizeGrip, _with_alpha(cfg.accent, 0.20f));
  set(ImGuiCol_ResizeGripHovered, _with_alpha(cfg.accent, 0.78f));
  set(ImGuiCol_ResizeGripActive, _with_alpha(cfg.accent, 1.00f));

  set(ImGuiCol_PlotLines, _with_alpha(cfg.text, 0.63f));
  set(ImGuiCol_PlotHistogram, _with_alpha(cfg.text, 0.63f));
  set(ImGuiCol_PlotLinesHovered, _with_alpha(cfg.accent, 1.00f));
  set(ImGuiCol_PlotHistogramHovered, _with_alpha(cfg.accent, 1.00f));

  set(ImGuiCol_ModalWindowDimBg, {0.50f, 0.50f, 0.50f, 0.3f});
}

}  // namespace

auto human_readable_name(const editor_theme theme) -> std::string_view
{
  switch (theme) {
    case editor_theme::dear_dark:
      return "Dear Dark";

    case editor_theme::dear_light:
      return "Dear Light";

    case editor_theme::ruby:
      return "Ruby";

    case editor_theme::sapphire:
      return "Sapphire";

    case editor_theme::emerald:
      return "Emerald";

    case editor_theme::amethyst:
      return "Amethyst";

    case editor_theme::amber:
      return "Amber";

    case editor_theme::nocturnal:
      return "Nocturnal";

    case editor_theme::ash:
      return "Ash";

    case editor_theme::diamond:
      return "Diamond";

    case editor_theme::joker:
      return "Joker";

    case editor_theme::raspberry:
      return "Raspberry";

    default:
      throw_traced(tactile_error{"Invalid theme enumerator!"});
  }
}

void apply_style(ImGuiStyle& style)
{
  style.WindowMenuButtonPosition = ImGuiDir_Right;
  style.WindowBorderSize = 0;

  style.WindowPadding = ImVec2{10, 10};
  style.FramePadding = ImVec2{5, 5};
  style.CellPadding = ImVec2{4, 4};

  style.ItemSpacing = ImVec2{12, 8};
  style.ItemInnerSpacing = ImVec2{4, 6};
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

void apply_theme(ImGuiStyle& style, editor_theme theme)
{
  switch (theme) {
    case editor_theme::dear_dark:
      ImGui::StyleColorsDark(&style);
      break;

    case editor_theme::dear_light:
      ImGui::StyleColorsLight(&style);
      break;

    case editor_theme::ruby:
      _apply_theme_from_config(style, _theme_from_hue(0));
      break;

    case editor_theme::emerald:
      _apply_theme_from_config(style, _theme_from_hue(100));
      break;

    case editor_theme::diamond:
      _apply_theme_from_config(style, _theme_from_hue(120));
      break;

    case editor_theme::sapphire:
      _apply_theme_from_config(style, _theme_from_hue(150));
      break;

    case editor_theme::joker:
      _apply_theme_from_config(style, _theme_from_hue(190));
      break;

    case editor_theme::amethyst:
      _apply_theme_from_config(style, _theme_from_hue(225));
      break;

    case editor_theme::raspberry:
      _apply_theme_from_config(style, _theme_from_hue(245));
      break;

    case editor_theme::amber:
      _apply_theme_from_config(style, _theme_from_hue(272));
      break;

    case editor_theme::nocturnal:
      _apply_theme_from_config(style,
                               {.accent = {0.0f, 0.5f, 0.5f, _accent_opacity},
                                .bg = {0.04f, 0.04f, 0.04f, 1.00f},
                                .area = {0.15f, 0.15f, 0.15f, _area_opacity},
                                .text = {1.0f, 1.0f, 1.0f, 1.0f}});
      break;

    case editor_theme::ash:
      _apply_theme_from_config(style,
                               {.accent = {0.4f, 0.4f, 0.4f, _accent_opacity},
                                .bg = {0.04f, 0.04f, 0.04f, 1.00f},
                                .area = {0.15f, 0.15f, 0.15f, _area_opacity},
                                .text = {1.0f, 1.0f, 1.0f, 1.0f}});
      break;
  }
}

}  // namespace tactile
