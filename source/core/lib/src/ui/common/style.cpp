// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/style.hpp"

namespace tactile::ui {

StyleVarScope::StyleVarScope(const ImGuiStyleVar id, const ImVec2 value)
{
  ImGui::PushStyleVar(id, value);
}

StyleVarScope::StyleVarScope(const ImGuiStyleVar id, const float value)
{
  ImGui::PushStyleVar(id, value);
}

StyleVarScope::~StyleVarScope() noexcept
{
  pop();
}

void StyleVarScope::pop() noexcept
{
  if (!mPopped) {
    ImGui::PopStyleVar();
    mPopped = true;
  }
}

StyleColorScope::StyleColorScope(const ImGuiCol id, const ImVec4& color)
{
  ImGui::PushStyleColor(id, color);
}

StyleColorScope::StyleColorScope(const ImGuiCol id, const std::uint32_t color)
{
  ImGui::PushStyleColor(id, color);
}

StyleColorScope::~StyleColorScope() noexcept
{
  pop();
}

void StyleColorScope::pop() noexcept
{
  if (!mPopped) {
    ImGui::PopStyleColor();
    mPopped = true;
  }
}

void apply_custom_style(ImGuiStyle& style)
{
  const auto rounding = 4.0f;

  // Main
  style.WindowPadding = ImVec2 {6.0f, 6.0f};
  style.FramePadding = ImVec2 {4.0f, 4.0f};
  style.ItemSpacing = ImVec2 {8.0f, 6.0f};
  style.ItemInnerSpacing = ImVec2 {4.0f, 4.0f};
  style.TouchExtraPadding = ImVec2 {0.0f, 0.0f};
  style.IndentSpacing = 21.0f;
  style.ScrollbarSize = 12.0f;
  style.GrabMinSize = 10.0f;

  // Borders
  style.WindowBorderSize = 0.0f;
  style.ChildBorderSize = 0.0f;
  style.PopupBorderSize = 0.0f;
  style.FrameBorderSize = 0.0f;
  style.TabBorderSize = 0.0f;
  style.TabBarBorderSize = 2.0f;

  // Rounding
  style.WindowRounding = rounding;
  style.PopupRounding = rounding;
  style.FrameRounding = rounding;
  style.ChildRounding = rounding;
  style.TabRounding = rounding;
  style.GrabRounding = rounding;
  style.ScrollbarRounding = rounding;

  // Tables
  style.CellPadding = ImVec2 {4.0f, 4.0f};
  // style.TableAngledHeadersAngle = 35.0f;

  // Widgets
  style.WindowTitleAlign = ImVec2 {0.5f, 0.5f};
  style.WindowMenuButtonPosition = ImGuiDir_None;
  style.ColorButtonPosition = ImGuiDir_Right;
  style.ButtonTextAlign = ImVec2 {0.5f, 0.5f};
  style.SelectableTextAlign = ImVec2 {0.0f, 0.5f};
  style.SeparatorTextBorderSize = 1.0f;
  style.SeparatorTextAlign = ImVec2 {0.5f, 0.5f};
  style.SeparatorTextPadding = ImVec2 {0.0f, 2.0f};
  // style.LogSliderDeadzone = 4.0f;

  // Docking
  style.DockingSeparatorSize = 0.0f;

  // Tooltips
  style.HoverFlagsForTooltipMouse =
      ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_Stationary;
  style.HoverFlagsForTooltipNav =
      ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay;

  // Misc
  style.DisplaySafeAreaPadding = ImVec2 {0.0f, 0.0f};
}

}  // namespace tactile::ui
