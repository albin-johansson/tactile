#include "themes.hpp"

#include <imgui.h>

#include "core/tactile_error.hpp"

namespace Tactile {
namespace {

struct ThemeCfg final
{
  ImVec4 accent{};
  ImVec4 bg{};
  ImVec4 area{};
  ImVec4 text{};
};

[[nodiscard]] auto MakeThemeFromHue(const float hue) -> ThemeCfg
{
  ThemeCfg cfg;

  // clang-format off
  cfg.accent  = ImColor::HSV(hue / 255.0f, 0.70f, 0.65f);
  cfg.bg      = ImColor::HSV(hue / 255.0f, 0.25f, 0.15f);
  cfg.area    = ImColor::HSV(hue / 255.0f, 0.50f, 0.40f);
  cfg.text    = ImColor::HSV(hue / 255.0f, 0.10f, 1.00f);
  // clang-format on

  return cfg;
}

void SetupStyleFromHue(ImGuiStyle& style, const ThemeCfg& cfg)
{
  // clang-format off
  style.Colors[ImGuiCol_Text]           = {cfg.text.x, cfg.text.y, cfg.text.z, 1.00f};
  style.Colors[ImGuiCol_TextDisabled]   = {cfg.text.x, cfg.text.y, cfg.text.z, 0.60f};
  style.Colors[ImGuiCol_TextSelectedBg] = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.43f};

  style.Colors[ImGuiCol_WindowBg] = {cfg.bg.x, cfg.bg.y, cfg.bg.z, 1.00f};
  style.Colors[ImGuiCol_PopupBg]  = {cfg.bg.x, cfg.bg.y, cfg.bg.z, 0.95f};
  style.Colors[ImGuiCol_ChildBg]  = {cfg.area.x, cfg.area.y, cfg.area.z, 0.10f};

  style.Colors[ImGuiCol_Border]       = {cfg.text.x, cfg.text.y, cfg.text.z, 0.30f};
  style.Colors[ImGuiCol_BorderShadow] = {0.00f, 0.00f, 0.00f, 0.00f};

  style.Colors[ImGuiCol_FrameBg]        = {cfg.area.x, cfg.area.y, cfg.area.z, 0.70f};
  style.Colors[ImGuiCol_FrameBgHovered] = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.70f};
  style.Colors[ImGuiCol_FrameBgActive]  = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.70f};

  style.Colors[ImGuiCol_TitleBg]          = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.45f};
  style.Colors[ImGuiCol_TitleBgActive]    = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.78f};
  style.Colors[ImGuiCol_TitleBgCollapsed] = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.35f};

  style.Colors[ImGuiCol_MenuBarBg]            = {cfg.area.x, cfg.area.y, cfg.area.z, 0.57f};
  style.Colors[ImGuiCol_ScrollbarBg]          = {cfg.area.x, cfg.area.y, cfg.area.z, 1.00f};
  style.Colors[ImGuiCol_ScrollbarGrab]        = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.31f};
  style.Colors[ImGuiCol_ScrollbarGrabHovered] = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.78f};
  style.Colors[ImGuiCol_ScrollbarGrabActive]  = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 1.00f};

  style.Colors[ImGuiCol_CheckMark]        = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 1.00f};
  style.Colors[ImGuiCol_SliderGrab]       = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.24f};
  style.Colors[ImGuiCol_SliderGrabActive] = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 1.00f};

  style.Colors[ImGuiCol_Button]        = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.44f};
  style.Colors[ImGuiCol_ButtonHovered] = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.86f};
  style.Colors[ImGuiCol_ButtonActive]  = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 1.00f};

  style.Colors[ImGuiCol_Header]        = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.76f};
  style.Colors[ImGuiCol_HeaderHovered] = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.86f};
  style.Colors[ImGuiCol_HeaderActive]  = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 1.00f};

  style.Colors[ImGuiCol_Separator]        = {cfg.text.x, cfg.text.y, cfg.text.z, 0.10f};
  style.Colors[ImGuiCol_SeparatorHovered] = {cfg.text.x, cfg.text.y, cfg.text.z, 0.10f};
  style.Colors[ImGuiCol_SeparatorActive]  = {cfg.text.x, cfg.text.y, cfg.text.z, 0.10f};

  style.Colors[ImGuiCol_ResizeGrip]        = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.20f};
  style.Colors[ImGuiCol_ResizeGripHovered] = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.78f};
  style.Colors[ImGuiCol_ResizeGripActive]  = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 1.00f};

  style.Colors[ImGuiCol_Tab]                = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.80f};
  style.Colors[ImGuiCol_TabHovered]         = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 1.00f};
  style.Colors[ImGuiCol_TabActive]          = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.95f};
  style.Colors[ImGuiCol_TabUnfocused]       = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.60f};
  style.Colors[ImGuiCol_TabUnfocusedActive] = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.80f};

  style.Colors[ImGuiCol_DockingPreview] = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.80f};
  style.Colors[ImGuiCol_DockingEmptyBg] = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 0.80f};

  style.Colors[ImGuiCol_PlotLines]            = {cfg.text.x, cfg.text.y, cfg.text.z, 0.63f};
  style.Colors[ImGuiCol_PlotLinesHovered]     = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 1.00f};
  style.Colors[ImGuiCol_PlotHistogram]        = {cfg.text.x, cfg.text.y, cfg.text.z, 0.63f};
  style.Colors[ImGuiCol_PlotHistogramHovered] = {cfg.accent.x, cfg.accent.y, cfg.accent.z, 1.00f};

  style.Colors[ImGuiCol_TableHeaderBg]     = {cfg.area.x, cfg.area.y, cfg.area.z, 1.00f};
  style.Colors[ImGuiCol_TableRowBg]        = {cfg.area.x, cfg.area.y, cfg.area.z, 0.10f};
  style.Colors[ImGuiCol_TableRowBgAlt]     = {cfg.area.x, cfg.area.y, cfg.area.z, 0.30f};
  style.Colors[ImGuiCol_TableBorderStrong] = {cfg.text.x, cfg.text.y, cfg.text.z, 0.10f};
  style.Colors[ImGuiCol_TableBorderLight]  = {cfg.text.x, cfg.text.y, cfg.text.z, 0.10f};
  style.Colors[ImGuiCol_ModalWindowDimBg] = {0.80f, 0.80f, 0.80f, 0.3f};
  // clang-format on
}

}  // namespace

void ApplyTheme(ImGuiStyle& style, const Theme theme)
{
  switch (theme) {
    case Theme::DearDark:
      ImGui::StyleColorsDark(&style);
      break;

    case Theme::DearLight:
      ImGui::StyleColorsLight(&style);
      break;

    case Theme::Ruby:
      SetupStyleFromHue(style, MakeThemeFromHue(0));
      break;

    case Theme::Sapphire:
      SetupStyleFromHue(style, MakeThemeFromHue(150));
      break;

    case Theme::Emerald:
      SetupStyleFromHue(style, MakeThemeFromHue(100));
      break;

    case Theme::Amethyst:
      SetupStyleFromHue(style, MakeThemeFromHue(225));
      break;

    case Theme::Amber:
      SetupStyleFromHue(style, MakeThemeFromHue(272));
      break;
  }
}

auto ToString(const Theme theme) -> std::string_view
{
  switch (theme) {
    case Theme::DearDark:
      return "DearDark";

    case Theme::DearLight:
      return "DearLight";

    case Theme::Ruby:
      return "Ruby";

    case Theme::Sapphire:
      return "Sapphire";

    case Theme::Emerald:
      return "Emerald";

    case Theme::Amethyst:
      return "Amethyst";

    case Theme::Amber:
      return "Amber";

    default:
      throw TactileError{"Did not recognize theme enumerator!"};
  }
}

// const float hue = 150;
// const ImVec4 accent = ImColor::HSV(hue / 255.0f, 0.70f, 0.63f);
// const ImVec4 bg = ImColor::HSV(hue / 255.0f, 0.23f, 0.16f);
// const ImVec4 area = ImColor::HSV(hue / 255.0f, 0.49f, 0.39f);
// const ImVec4 text = ImColor::HSV(hue / 255.0f, 0.08f, 0.92f);

//      ThemeCfg cfg;
//
//      cfg.accent = {0.19f, 0.40f, 0.63f, 0.63f};
//      cfg.bg = {0.12f, 0.14f, 0.16f, 0.16f};
//      cfg.area = {0.20f, 0.29f, 0.39f, 0.39f};
//      cfg.text = {0.85f, 0.88f, 0.92f, 0.92f};
//
//      SetupStyleFromHue(cfg);

}  // namespace Tactile
