#include "themes.hpp"

#include <imgui.h>

#include <algorithm>      // clamp
#include <cassert>        // assert
#include <centurion.hpp>  // to_underlying
#include <string_view>    // string_view

#define TACTILE_SET_COLOR_GROUP(Element, Color)     \
  style.Colors[Element] = Color;                    \
  style.Colors[Element##Hovered] = Brighten(Color); \
  style.Colors[Element##Active] = Darken(Color)

namespace Tactile {
namespace {

constexpr auto ash = "Ash";
constexpr auto dear_dark = "Dear Dark";
constexpr auto dear_light = "Dear Light";

/**
 * \brief Represents the different available themes.
 *
 * \see `ApplyTheme()`
 * \see `GetThemeFromIndex()`
 * \see `GetThemeIndex()`
 */
enum class Theme
{
  DearDark = 0,   ///< The standard Dear ImGui dark theme.
  DearLight = 1,  ///< The standard Dear ImGui light theme.
  Ash = 2
};

[[nodiscard]] auto WithAlpha(const ImVec4& color, const float opacity) -> ImVec4
{
  return {color.x, color.y, color.z, opacity};
}

[[nodiscard]] auto Brighten(const ImVec4& color, const float opacity = 1) -> ImVec4
{
  const auto brighten = [](const float f) {
    return std::clamp(f * 1.2f, 0.0f, 1.0f);
  };

  return {brighten(color.x), brighten(color.y), brighten(color.z), opacity};
}

[[nodiscard]] auto Darken(const ImVec4& color, const float opacity = 1) -> ImVec4
{
  const auto darken = [](const float f) { return std::clamp(f * 0.8f, 0.0f, 1.0f); };

  return {darken(color.x), darken(color.y), darken(color.z), opacity};
}

inline const ImVec4 white = {1, 1, 1, 1};
inline const ImVec4 black = {0, 0, 0, 1};

void ApplyAshTheme(ImGuiStyle& style)
{
  const auto accent = ImVec4{0.26f, 0.59f, 0.98f, 0.7f};
  const auto grab = ImVec4{0.51f, 0.51f, 0.51f, 1};
  const auto gray = ImVec4{0.28f, 0.28f, 0.28f, 1};
  const auto button = gray;
  const auto window = ImVec4{0.1f, 0.1f, 0.1f, 1};
  const auto child = ImVec4{0.12f, 0.12f, 0.12f, 1};
  const auto lightBlue = ImVec4{0.26f, 0.59f, 0.98f, 1};
  const auto limeGreen = ImVec4{0.25f, 1, 0, 1};

  TACTILE_SET_COLOR_GROUP(ImGuiCol_Button, button);
  TACTILE_SET_COLOR_GROUP(ImGuiCol_Separator, button);
  TACTILE_SET_COLOR_GROUP(ImGuiCol_Header, accent);
  TACTILE_SET_COLOR_GROUP(ImGuiCol_ResizeGrip, gray);
  TACTILE_SET_COLOR_GROUP(ImGuiCol_ScrollbarGrab, grab);

  // clang-format off
  style.Colors[ImGuiCol_Text]                  = white;
  style.Colors[ImGuiCol_TextDisabled]          = ImVec4{0.6f, 0.6f, 0.6f, 1};
  style.Colors[ImGuiCol_WindowBg]              = window;
  style.Colors[ImGuiCol_ChildBg]               = child;
  style.Colors[ImGuiCol_PopupBg]               = WithAlpha(window, 0.9f);
  style.Colors[ImGuiCol_Border]                = WithAlpha(white,  0.29f);
  style.Colors[ImGuiCol_BorderShadow]          = black;

  style.Colors[ImGuiCol_FrameBg]               = ImVec4{0.25f, 0.25f, 0.25f, 1.0f};
  style.Colors[ImGuiCol_FrameBgHovered]        = Brighten(style.Colors[ImGuiCol_FrameBg]);
  style.Colors[ImGuiCol_FrameBgActive]         = style.Colors[ImGuiCol_FrameBg];

  style.Colors[ImGuiCol_TitleBg]               = ImVec4{0.2f, 0.2f, 0.2f, 1.0f};
  style.Colors[ImGuiCol_TitleBgActive]         = ImVec4{0.2f, 0.2f, 0.2f, 1.0f};
  style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4{0.2f, 0.2f, 0.2f, 1.0f};

  style.Colors[ImGuiCol_MenuBarBg]             = ImVec4{0.2f, 0.2f, 0.2f, 1.0f};
  style.Colors[ImGuiCol_ScrollbarBg]           = window;

  style.Colors[ImGuiCol_CheckMark]             = white;
  style.Colors[ImGuiCol_SliderGrab]            = grab;
  style.Colors[ImGuiCol_SliderGrabActive]      = white;

  style.Colors[ImGuiCol_Tab]                   = button;
  style.Colors[ImGuiCol_TabHovered]            = Brighten(accent);
  style.Colors[ImGuiCol_TabActive]             = accent;
  style.Colors[ImGuiCol_TabUnfocused]          = child;
  style.Colors[ImGuiCol_TabUnfocusedActive]    = child;

  style.Colors[ImGuiCol_DockingPreview]        = WithAlpha(lightBlue, 0.7f);
  style.Colors[ImGuiCol_DockingEmptyBg]        = ImVec4{0.2f, 0.2f, 0.2f, 1};

  style.Colors[ImGuiCol_PlotLines]             = ImVec4{0.4f, 0.4f, 0.4f, 0.63f};
  style.Colors[ImGuiCol_PlotLinesHovered]      = limeGreen;
  style.Colors[ImGuiCol_PlotHistogram]         = ImVec4{0.4f, 0.4f, 0.4f, 0.63f};
  style.Colors[ImGuiCol_PlotHistogramHovered]  = limeGreen;

  style.Colors[ImGuiCol_TableHeaderBg]         = button;
  style.Colors[ImGuiCol_TableBorderStrong]     = black;
  style.Colors[ImGuiCol_TableBorderLight]      = black;
  style.Colors[ImGuiCol_TableRowBg]            = WithAlpha(black, 0);
  style.Colors[ImGuiCol_TableRowBgAlt]         = WithAlpha(white, 0.06f);
  style.Colors[ImGuiCol_TextSelectedBg]        = WithAlpha(limeGreen, 0.43f);
  style.Colors[ImGuiCol_DragDropTarget]        = ImVec4{1, 1, 0, 0.9f};
  style.Colors[ImGuiCol_NavHighlight]          = lightBlue;
  style.Colors[ImGuiCol_NavWindowingHighlight] = WithAlpha(white, 0.7f);
  style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4{0.8f, 0.8f, 0.8f, 0.2f};
  style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4{0.8f, 0.8f, 0.8f, 0.35f};
  // clang-format on
}

[[nodiscard]] auto GetThemeFromName(const std::string_view name) -> Maybe<Theme>
{
  if (name == dear_dark)
  {
    return Theme::DearDark;
  }
  else if (name == dear_light)
  {
    return Theme::DearLight;
  }
  else if (name == ash)
  {
    return Theme::Ash;
  }
  else
  {
    return nothing;
  }
}

}  // namespace

void ApplyTheme(ImGuiStyle& style, const std::string& name)
{
  const auto theme = GetThemeFromName(name);
  if (!theme)
  {
    return;
  }

  switch (*theme)
  {
    case Theme::Ash:
      ApplyAshTheme(style);
      break;

    case Theme::DearDark:
      ImGui::StyleColorsDark(&style);
      break;

    case Theme::DearLight:
      ImGui::StyleColorsLight(&style);
      break;

    default:
      cen::log::error("Could not apply theme \"%s\", because no such theme exists!",
                      theme);
      break;
  }
}

auto GetThemeFromIndex(const int index) -> Maybe<std::string>
{
  switch (index)
  {
    case cen::to_underlying(Theme::Ash):
      return ash;

    case cen::to_underlying(Theme::DearDark):
      return dear_dark;

    case cen::to_underlying(Theme::DearLight):
      return dear_light;

    default:
      CENTURION_LOG_WARN("Could not get theme name from invalid index: %i", index);
      return nothing;
  }
}

auto GetThemeIndex(const std::string& name) -> Maybe<int>
{
  if (const auto theme = GetThemeFromName(name))
  {
    return cen::to_underlying(*theme);
  }
  else
  {
    return nothing;
  }
}

}  // namespace Tactile
