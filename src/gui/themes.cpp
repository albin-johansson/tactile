#include "themes.hpp"

#include <algorithm>  // clamp

#include "imgui.h"

namespace tactile {
namespace {

[[nodiscard]] auto WithAlpha(const ImVec4& color, const float opacity) -> ImVec4
{
  return {color.x, color.y, color.z, opacity};
}

[[nodiscard]] auto Brighten(const ImVec4& color, const float opacity = 1)
    -> ImVec4
{
  const auto brighten = [](const float f) {
    return std::clamp(f * 1.2f, 0.0f, 1.0f);
  };

  return {brighten(color.x), brighten(color.y), brighten(color.z), opacity};
}

[[nodiscard]] auto Darken(const ImVec4& color, const float opacity = 1)
    -> ImVec4
{
  const auto darken = [](const float f) {
    return std::clamp(f * 0.8f, 0.0f, 1.0f);
  };

  return {darken(color.x), darken(color.y), darken(color.z), opacity};
}

}  // namespace

void ApplyNightTheme(ImGuiStyle& style)
{
  const auto grab = ImVec4{0.51f, 0.51f, 0.51f, 1};
  const auto gray_3 = ImVec4{0.24f, 0.24f, 0.24f, 1};
  const auto button = ImVec4{0.2f, 0.2f, 0.2f, 1};
  const auto window = ImVec4{0.1f, 0.1f, 0.1f, 1};
  const auto child = ImVec4{0.12f, 0.12f, 0.12f, 1};
  const auto black = ImVec4{0, 0, 0, 1};
  const auto white = ImVec4{1, 1, 1, 1};
  const auto lightBlue = ImVec4{0.26f, 0.59f, 0.98f, 1};
  const auto limeGreen = ImVec4{0.25f, 1, 0, 1};

  // clang-format off
  style.Colors[ImGuiCol_Text]                  = white;
  style.Colors[ImGuiCol_TextDisabled]          = ImVec4{0.6f, 0.6f, 0.6f, 1};
  style.Colors[ImGuiCol_WindowBg]              = window;
  style.Colors[ImGuiCol_ChildBg]               = child;
  style.Colors[ImGuiCol_PopupBg]               = WithAlpha(window, 0.95f);
  style.Colors[ImGuiCol_Border]                = WithAlpha(white,  0.29f);
  style.Colors[ImGuiCol_BorderShadow]          = black;

  style.Colors[ImGuiCol_FrameBg]               = button;
  style.Colors[ImGuiCol_FrameBgHovered]        = Brighten(style.Colors[ImGuiCol_FrameBg]);
  style.Colors[ImGuiCol_FrameBgActive]         = style.Colors[ImGuiCol_FrameBg];

  style.Colors[ImGuiCol_TitleBg]               = button;
  style.Colors[ImGuiCol_TitleBgActive]         = button;
  style.Colors[ImGuiCol_TitleBgCollapsed]      = button;

  style.Colors[ImGuiCol_MenuBarBg]             = button;
  style.Colors[ImGuiCol_ScrollbarBg]           = window;

  style.Colors[ImGuiCol_ScrollbarGrab]         = grab;
  style.Colors[ImGuiCol_ScrollbarGrabHovered]  = Brighten(style.Colors[ImGuiCol_ScrollbarGrab]);
  style.Colors[ImGuiCol_ScrollbarGrabActive]   = Darken(style.Colors[ImGuiCol_ScrollbarGrab]);

  style.Colors[ImGuiCol_CheckMark]             = white;
  style.Colors[ImGuiCol_SliderGrab]            = grab;
  style.Colors[ImGuiCol_SliderGrabActive]      = white;

  style.Colors[ImGuiCol_Button]                = button;
  style.Colors[ImGuiCol_ButtonHovered]         = Brighten(style.Colors[ImGuiCol_Button]);
  style.Colors[ImGuiCol_ButtonActive]          = Darken(style.Colors[ImGuiCol_Button]);

  style.Colors[ImGuiCol_Header]                = button;
  style.Colors[ImGuiCol_HeaderHovered]         = Brighten(style.Colors[ImGuiCol_Header]);
  style.Colors[ImGuiCol_HeaderActive]          = Darken(style.Colors[ImGuiCol_Header]);

  style.Colors[ImGuiCol_Separator]             = button;
  style.Colors[ImGuiCol_SeparatorHovered]      = Brighten(style.Colors[ImGuiCol_Separator]);
  style.Colors[ImGuiCol_SeparatorActive]       = Darken(style.Colors[ImGuiCol_Separator]);

  style.Colors[ImGuiCol_ResizeGrip]            = gray_3;
  style.Colors[ImGuiCol_ResizeGripHovered]     = Brighten(style.Colors[ImGuiCol_ResizeGrip]);
  style.Colors[ImGuiCol_ResizeGripActive]      = Darken(style.Colors[ImGuiCol_ResizeGrip]);

  style.Colors[ImGuiCol_Tab]                   = window;
  style.Colors[ImGuiCol_TabHovered]            = ImVec4{0.63f, 0.63f, 0.63f, 0.8f};
  style.Colors[ImGuiCol_TabActive]             = ImVec4{0.48f, 0.48f, 0.48f, 1};
  style.Colors[ImGuiCol_TabUnfocused]          = window;
  style.Colors[ImGuiCol_TabUnfocusedActive]    = window;

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

}  // namespace tactile
