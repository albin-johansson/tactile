// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "alignment.hpp"

#include <imgui_internal.h>

#include "tactile/core/debug/assert.hpp"
#include "ui/constants.hpp"

namespace tactile::ui {

void center_next_window_on_appearance()
{
  const auto* viewport = ImGui::GetMainViewport();
  const auto pos = viewport->WorkPos;
  const auto size = viewport->WorkSize;

  const ImVec2 next_pos = pos + (size / ImVec2 {2, 2});
  const ImVec2 next_pivot {0.5f, 0.5f};

  ImGui::SetNextWindowPos(next_pos, ImGuiCond_Appearing, next_pivot);
}

void center_next_item_horizontally(const float width)
{
  const auto region = ImGui::GetContentRegionAvail();
  const auto halfRegion = region / ImVec2 {2.0f, 2.0f};
  ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2 {halfRegion.x - (width / 2.0f), 0});
}

void prepare_vertical_alignment_center(const float count)
{
  const auto fontSize = ImGui::GetFontSize();

  const auto region = ImGui::GetContentRegionAvail();
  const auto halfRegion = region / ImVec2 {2.0f, 2.0f};

  const auto padding = ImGui::GetStyle().FramePadding;
  const auto height = (count * (fontSize + (2.0f * padding.y)));

  const auto cursorPos = ImGui::GetCursorPos();
  ImGui::SetCursorPos(cursorPos + ImVec2 {0, halfRegion.y - (height / 2.0f)});
}

void right_align_next_item()
{
  ImGui::SetNextItemWidth(-kMinFloat);
}

auto right_align_next_item(const float width) -> float
{
  const auto x = ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x - width);
  ImGui::SetCursorPosX(x);
  return x;
}

auto right_align_next_item(const char* text) -> float
{
  return right_align_next_item(standard_component_width(text));
}

auto standard_component_width(const char* text) -> float
{
  TACTILE_ASSERT(text);
  return ImGui::CalcTextSize(text).x + (ImGui::GetStyle().FramePadding.x * 2.0f);
}

}  // namespace tactile::ui
