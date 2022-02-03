#include "themes.hpp"

#include <imgui.h>

#include "throw.hpp"

namespace tactile {
namespace {

constexpr float gAccentOpacity = 0.65f;
constexpr float gBgOpacity = 0.15f;
constexpr float gAreaOpacity = 0.40f;

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
  cfg.accent  = ImColor::HSV(hue / 255.0f, 0.70f, gAccentOpacity);
  cfg.bg      = ImColor::HSV(hue / 255.0f, 0.25f, gBgOpacity);
  cfg.area    = ImColor::HSV(hue / 255.0f, 0.50f, gAreaOpacity);
  cfg.text    = ImColor::HSV(hue / 255.0f, 0.10f, 1.00f);
  // clang-format on

  return cfg;
}

void ApplyThemeFromConfig(ImGuiStyle& style, const ThemeCfg& cfg)
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

  style.Colors[ImGuiCol_ModalWindowDimBg] = {0.50f, 0.50f, 0.50f, 0.3f};

  // style.Colors[ImGuiCol_DragDropTarget]        = {area.x, area.y, area.z, 0.80f};
  // style.Colors[ImGuiCol_NavHighlight]          = {area.x, area.y, area.z, 0.80f};
  // style.Colors[ImGuiCol_NavWindowingHighlight] = {area.x, area.y, area.z, 0.80f};
  // style.Colors[ImGuiCol_NavWindowingDimBg]     = {area.x, area.y, area.z, 0.80f};
  // clang-format on
}

}  // namespace

void ApplyTheme(ImGuiStyle& style, const Theme theme)
{
  switch (theme) {
    case Theme::dear_dark:
      ImGui::StyleColorsDark(&style);
      break;

    case Theme::dear_light:
      ImGui::StyleColorsLight(&style);
      break;

    case Theme::ruby:
      ApplyThemeFromConfig(style, MakeThemeFromHue(0));
      break;

    case Theme::sapphire:
      ApplyThemeFromConfig(style, MakeThemeFromHue(150));
      break;

    case Theme::emerald:
      ApplyThemeFromConfig(style, MakeThemeFromHue(100));
      break;

    case Theme::amethyst:
      ApplyThemeFromConfig(style, MakeThemeFromHue(225));
      break;

    case Theme::amber:
      ApplyThemeFromConfig(style, MakeThemeFromHue(272));
      break;

    case Theme::nocturnal:
      ApplyThemeFromConfig(style,
                           {.accent = {0.0f, 0.5f, 0.5f, gAccentOpacity},
                            .bg = {0.04f, 0.04f, 0.04f, gBgOpacity},
                            .area = {0.15f, 0.15f, 0.15f, gAreaOpacity},
                            .text = {1.0f, 1.0f, 1.0f, 1.0f}});
      break;

    case Theme::ash:
      ApplyThemeFromConfig(style,
                           {.accent = {0.4f, 0.4f, 0.4f, gAccentOpacity},
                            .bg = {0.04f, 0.04f, 0.04f, gBgOpacity},
                            .area = {0.15f, 0.15f, 0.15f, gAreaOpacity},
                            .text = {1.0f, 1.0f, 1.0f, 1.0f}});
      break;
  }
}

auto human_readable_name(const Theme theme) -> std::string_view
{
  switch (theme) {
    case Theme::dear_dark:
      return "Dear Dark";

    case Theme::dear_light:
      return "Dear Light";

    case Theme::ruby:
      return "Ruby";

    case Theme::sapphire:
      return "Sapphire";

    case Theme::emerald:
      return "Emerald";

    case Theme::amethyst:
      return "Amethyst";

    case Theme::amber:
      return "Amber";

    case Theme::nocturnal:
      return "Nocturnal";

    case Theme::ash:
      return "Ash";

    default:
      throw_traced(tactile_error{"Invalid theme enumerator!"});
  }
}

}  // namespace tactile
