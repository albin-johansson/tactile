#include "themes.hpp"

#include <algorithm>  // clamp

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
  const auto gray_4 = ImVec4{0.51f, 0.51f, 0.51f, 1};
  const auto gray_3 = ImVec4{0.24f, 0.24f, 0.24f, 1};
  const auto gray_2 = ImVec4{0.16f, 0.16f, 0.16f, 1};
  const auto gray_1 = ImVec4{0.1f, 0.1f, 0.1f, 1};
  const auto black = ImVec4{0, 0, 0, 1};
  const auto white = ImVec4{1, 1, 1, 1};
  const auto lightBlue = ImVec4{0.26f, 0.59f, 0.98f, 1};
  const auto limeGreen = ImVec4{0.25f, 1, 0, 1};

  // clang-format off
  style.Colors[ImGuiCol_Text]                  = white;
  style.Colors[ImGuiCol_TextDisabled]          = ImVec4{0.6f, 0.6f, 0.6f, 1};
  style.Colors[ImGuiCol_WindowBg]              = gray_1;
  style.Colors[ImGuiCol_ChildBg]               = gray_1;
  style.Colors[ImGuiCol_PopupBg]               = gray_1;
  style.Colors[ImGuiCol_Border]                = WithAlpha(white,  0.29f);
  style.Colors[ImGuiCol_BorderShadow]          = black;

  style.Colors[ImGuiCol_FrameBg]               = gray_2;
  style.Colors[ImGuiCol_FrameBgHovered]        = Brighten(style.Colors[ImGuiCol_FrameBg]);
  style.Colors[ImGuiCol_FrameBgActive]         = style.Colors[ImGuiCol_FrameBg];

  style.Colors[ImGuiCol_TitleBg]               = gray_2;
  style.Colors[ImGuiCol_TitleBgActive]         = gray_2;
  style.Colors[ImGuiCol_TitleBgCollapsed]      = gray_2;

  style.Colors[ImGuiCol_MenuBarBg]             = gray_2;
  style.Colors[ImGuiCol_ScrollbarBg]           = gray_1;

  style.Colors[ImGuiCol_ScrollbarGrab]         = gray_4;
  style.Colors[ImGuiCol_ScrollbarGrabHovered]  = Brighten(style.Colors[ImGuiCol_ScrollbarGrab]);
  style.Colors[ImGuiCol_ScrollbarGrabActive]   = Darken(style.Colors[ImGuiCol_ScrollbarGrab]);

  style.Colors[ImGuiCol_CheckMark]             = white;
  style.Colors[ImGuiCol_SliderGrab]            = gray_4;
  style.Colors[ImGuiCol_SliderGrabActive]      = white;

  style.Colors[ImGuiCol_Button]                = gray_2;
  style.Colors[ImGuiCol_ButtonHovered]         = Brighten(style.Colors[ImGuiCol_Button]);
  style.Colors[ImGuiCol_ButtonActive]          = Darken(style.Colors[ImGuiCol_Button]);

  style.Colors[ImGuiCol_Header]                = gray_2;
  style.Colors[ImGuiCol_HeaderHovered]         = Brighten(style.Colors[ImGuiCol_Header]);
  style.Colors[ImGuiCol_HeaderActive]          = Darken(style.Colors[ImGuiCol_Header]);

  style.Colors[ImGuiCol_Separator]             = gray_2;
  style.Colors[ImGuiCol_SeparatorHovered]      = Brighten(style.Colors[ImGuiCol_Separator]);
  style.Colors[ImGuiCol_SeparatorActive]       = Darken(style.Colors[ImGuiCol_Separator]);

  style.Colors[ImGuiCol_ResizeGrip]            = gray_3;
  style.Colors[ImGuiCol_ResizeGripHovered]     = Brighten(style.Colors[ImGuiCol_ResizeGrip]);
  style.Colors[ImGuiCol_ResizeGripActive]      = Darken(style.Colors[ImGuiCol_ResizeGrip]);

  style.Colors[ImGuiCol_Tab]                   = gray_1;
  style.Colors[ImGuiCol_TabHovered]            = ImVec4{0.63f, 0.63f, 0.63f, 0.8f};
  style.Colors[ImGuiCol_TabActive]             = ImVec4{0.48f, 0.48f, 0.48f, 1};
  style.Colors[ImGuiCol_TabUnfocused]          = gray_1;
  style.Colors[ImGuiCol_TabUnfocusedActive]    = gray_1;

  style.Colors[ImGuiCol_DockingPreview]        = WithAlpha(lightBlue, 0.7f);
  style.Colors[ImGuiCol_DockingEmptyBg]        = ImVec4{0.2f, 0.2f, 0.2f, 1};

  style.Colors[ImGuiCol_PlotLines]             = ImVec4{0.4f, 0.4f, 0.4f, 0.63f};
  style.Colors[ImGuiCol_PlotLinesHovered]      = limeGreen;
  style.Colors[ImGuiCol_PlotHistogram]         = ImVec4{0.4f, 0.4f, 0.4f, 0.63f};
  style.Colors[ImGuiCol_PlotHistogramHovered]  = limeGreen;

  style.Colors[ImGuiCol_TableHeaderBg]         = gray_2;
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
