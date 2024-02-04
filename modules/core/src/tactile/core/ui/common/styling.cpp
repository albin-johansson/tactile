// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/styling.hpp"

namespace tactile::core {

ScopedStyle::ScopedStyle(const ImGuiStyleVar style_var, const float value)
{
  ImGui::PushStyleVar(style_var, value);
}

ScopedStyle::ScopedStyle(const ImGuiStyleVar style_var, const ImVec2& value)
{
  ImGui::PushStyleVar(style_var, value);
}

ScopedStyle::~ScopedStyle() noexcept
{
  ImGui::PopStyleVar();
}

void apply_custom_style(ImGuiStyle& style)
{
  style.FrameRounding = 4.0f;

  style.WindowPadding = ImVec2 {6.0f, 6.0f};
  style.FramePadding = ImVec2 {6.0f, 4.0f};
  style.ItemSpacing = ImVec2 {6.0f, 4.0f};
  style.ItemInnerSpacing = ImVec2 {4.0f, 4.0f};
  style.ScrollbarSize = 12.0f;
  style.GrabMinSize = 8.0f;

  style.WindowBorderSize = 0.0f;
  style.ChildBorderSize = 0.0f;
  style.PopupBorderSize = 0.0f;
  style.FrameBorderSize = 0.0f;
  style.TabBorderSize = 0.0f;
  style.TabBarBorderSize = 2.0f;

  style.WindowRounding = 6.0f;
  style.ChildRounding = 2.0f;
  style.FrameRounding = 4.0f;
  style.PopupRounding = 4.0f;
  style.ScrollbarRounding = 4.0f;
  style.GrabRounding = 4.0f;
  style.TabRounding = 4.0f;

  style.CellPadding = ImVec2 {6.0f, 4.0f};
  style.TableAngledHeadersAngle = 40.0f;

  style.WindowTitleAlign = ImVec2 {0.5f, 0.5f};
  style.WindowMenuButtonPosition = ImGuiDir_None;
  style.ColorButtonPosition = ImGuiDir_Right;
  style.ButtonTextAlign = ImVec2 {0.5f, 0.5f};
  style.SelectableTextAlign = ImVec2 {0.0f, 0.0f};
  style.SeparatorTextBorderSize = 2.0f;
  style.SeparatorTextAlign = ImVec2 {0.0f, 0.5f};
  style.SeparatorTextPadding = ImVec2 {20.0f, 2.0f};

  style.DockingSeparatorSize = 0.0f;
}

}  // namespace tactile::core
